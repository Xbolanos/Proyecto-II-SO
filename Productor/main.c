#include "../functions.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

static sem_t * smain;
static sem_t * semP;
int type=0;
int *process_shm; 


extern int * search(int *r, int number);
extern int * search_seg(int *r, int number);
int * finding(int * r, int number, int type);
extern int getIdOfSharedMemory(key_t key, int size);
extern void writeLog (int PID, int action, int line);

void threadfunc(int *arguments[3]){
        int proc=arguments[1][0];
        int * space=arguments[0];
        int number=arguments[2];
        for(int i=0;i<number;i++){
            int j=space[i];
            r[j]=proc;
          
            if(arguments[3] != 0){
               writeLogS(proc, 0,  arguments[3], space[i]/4); 
            }
            else if (type != 1){
                writeLog(proc, 0, space[i]/4);    
            }
            
        }
        print_shared_memory();
        sem_post(smain);
        printf("\n\nSale Semaforo: Proceso #%d \n\n",proc);

        int i= 30 + rand() % (60+1 - 30);
        sleep(i);
        printf("\n\nPide Semaforo: Proceso #%d \n\n",proc);
        sem_wait(smain);
        if(type==0){
            sem_wait(semP);
            process_shm[(proc*8)+4] = 1; // murio de viejito
            sem_post(semP);
            replace_Element(proc, 0, r,requestSize[0]*sizeof(int));
            //en replace element es donde escribe en bitacora (: 
            for(int i=0;i<number;i++){
                
                    writeLog(proc, 1, space[i]/4);    
            }
            
        }
        else{
            for(int i=0;i<number;i++){
                int j=space[i];
                r[j]=0;
                writeLogS(proc, 1,  arguments[3], space[i]/4); 
          }
        }
        print_shared_memory();
        sem_post(smain);
        printf("\n\nSale Semaforo: Proceso #%d \n\n",arguments[1][0]);   
}



int main(int argc, char *argv[])
{
    if(argc==2){
        int shmid = getIdOfSharedMemory(request, sizeof(int));
        requestSize = shmat(shmid, (void *)0, 0);
        requestSize[2]=1;
       
        int shmI = getIdOfSharedMemory(key, requestSize[0]*sizeof(int)); 
        r = shmat(shmI, (void *)0, 0);

        smain = sem_open(SMAIN, O_CREAT, 0644, 1); 
        sem_init(smain, 0, 1);
        semP = sem_open(SNAME, O_CREAT, 0644, 1); 
        sem_init(semP, 0, 1);
        
        int sizeProcess = (int) sizeof(int) * 20000;
        int process_shm_id = getIdOfSharedMemory(processes_key, sizeProcess); 
        process_shm = shmat(process_shm_id, NULL, 0);  


        if(atoi(argv[1])==0){
            type=0;
            printf("Paginacion\n");
            pagination();
            
        }else{
            type=1;
            printf("Segmentacion\n");
            segmentation();
        }
        requestSize[1]=2;
        printf("ERROR: Espacio sin ser Iniciado o Recientemente Finalizado.\n");
    }else{
        printf("ERROR: Debe de ingresar 0 en caso de ser Paginación y 1 en caso de ser Segmentación. \n");
    }
    return 0;   

}

void pagination(){
    int idprocess = 0;        
        while(requestSize[1]==0){
            pthread_t *mythread;
            mythread = (pthread_t *)malloc(sizeof(*mythread));
            idprocess++;
            int *nprocess[1];
            nprocess[0]=idprocess; 
            int number= 1 + rand() % (10+1 - 1);
            printf("\n\nSe deben asignar %d páginas.\n\n", number);
            //int *space=(int *) malloc(sizeof(int)*number);
            printf("\n\nPide Semaforo: Proceso # %d\n\n",idprocess);
            printf("OKA\n");
            sem_wait(smain);
            printf("OKA2\n");
            sem_wait(semP); 
            process_shm[idprocess*8] = idprocess;
            process_shm[(idprocess*8)+4] = 3; // buscando (: 
            printf("\n\nEn espera(BLOQUEADO): Proceso # %d\n\n",idprocess);
            printf("OKA3\n");
            sem_post(semP); 
            printf("OKA4\n");
            int * space=finding(r,number,type);
            printf("OKA5\n");
            printf("\n\nSale de Espera(DESBLOQUEADO): Proceso # %d\n\n",idprocess);
            int * arguments[4];
            arguments[0]=space;
            arguments[1]=nprocess;
            arguments[2]=number;
            arguments[4]=0; 
            if(space!=NULL){
                sem_wait(semP); 
                process_shm[(idprocess*8)+4] = 0; // vivo (: 
                sem_post(semP); 
                pthread_create(mythread, NULL,threadfunc, arguments);
            }
            else{
                printf("\n\nMuere: Proceso # %d\n\n",idprocess); 
                sem_wait(semP);
                process_shm[(idprocess*8)+4] = 2; // murio porq no encontro :( 
                sem_post(semP); 
                writeLog(idprocess, 2, 0); 
                sem_post(smain);
                printf("\n\nSale Semaforo: Proceso # %d \n\n",idprocess);
            }
            int waitb= 3 + rand() % (6+1 - 3);
           //int waitb= 30 + rand() % (60+1 - 30);
            
            sleep(waitb);
        }

}

void segmentation(){
    int idprocess = 0;

        while(requestSize[1]==0){
            pthread_t *mythread;
            mythread = (pthread_t *)malloc(sizeof(*mythread));
            idprocess++;
            int *nprocess[1];
            
            int segs= 1 + rand() % (5+1 - 1);
            nprocess[0]=idprocess;
       
            for(int i=0;i<segs;i++){
                int number= 1 + rand() % (3+1 - 1);
                printf("\n\nSe deben asignar %d líneas.\n\n", number);        
                int *space=(int *) malloc(sizeof(int)*number);
                printf("\n\nPide Semaforo: Proceso # %d\n\n",idprocess);
                sem_wait(smain);
                sem_wait(semP);
                process_shm[idprocess*8] = idprocess;
                process_shm[(idprocess*8)+4] = 3; // buscando (: 
                printf("\n\nEn espera(BLOQUEADO): Proceso # %d\n\n",idprocess);
                sem_post(semP);
                space=finding(r,number,type);
                printf("\n\nSale de Espera(DESBLOQUEADO): Proceso # %d\n\n",idprocess);
                int * arguments[4];
                arguments[0]=space;               
                arguments[1]=nprocess;
                arguments[2]=number;
                arguments[3]=i; 
                if(space!=NULL){
                    sem_wait(semP);
                    process_shm[(idprocess*8)+4] = 0; // vivo (: 
                    sem_post(semP);
                    pthread_create(mythread, NULL,threadfunc, arguments);
                    
                }
                else{ 
                     printf("\n\nMuere: Proceso # %d\n\n",idprocess);
                    sem_wait(semP);
                    process_shm[(idprocess*8)+4] = 2; // murio porq no encontro :( 
                    sem_post(semP);
                    writeLogS(idprocess, 2, i, 0); 
                    sem_post(smain);
                    printf("\n\nSale Semaforo: Proceso # %d \n\n",idprocess);
                }
                
            }
            int waitb= 3 + rand() % (6+1 - 3);
            sleep(waitb);
        }

}

int * finding(int * r, int number, int type){
    int i=0;
    int* n;
    while(i<31){
       //AQUI VA BITACORA DE ESPERA
       if(type==0){
           n=search(r, number);
           //print_list(n,number);
        }else{
            n=search_seg(r,number);
        }
       if(n==NULL){
           sleep(1);
           i++;
        }
        else{
         
            return n; 
        }
    }
    return NULL;
}

void print_list(int* list,int number){
    for(int i=0; i<number;i++){
        printf("Elemento %d: %d\n",i, list[i]);
    }
}

