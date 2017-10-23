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

sem_t semaphore;
int type=0;

extern int * search(int *r, int number);
extern int * search_seg(int *r, int number);
int * finding(int * r, int number, int type);
extern int getIdOfSharedMemory(key_t key, int size);

void threadfunc(int *arguments[3]){
        int proc=arguments[1][0];
        int * space=arguments[0];
        int number=arguments[2];
        for(int i=0;i<number;i++){
                int j=space[i];
                r[j]=proc;
        }
        //**AQUI VA A ESCRIBIR BITACORA
        print_shared_memory();
        sem_post(&semaphore);
        printf("\n\nSale Semaforo: %d \n\n",proc);

        int i= 2 + rand() % (6+1 - 2);
        sleep(i);
        printf("\n\nPide Semaforo: %d \n\n",proc);
        sem_wait(&semaphore);
        if(type==0){
            replace_Element(proc, 0, r,requestSize[0]*sizeof(int));
        }
        else{
         
            for(int i=0;i<number;i++){
                int j=space[i];
                r[j]=0;
          }
        }
        //**AQUI VA A ESCRIBIR BITACORA
        print_shared_memory();
        sem_post(&semaphore);
        printf("\n\nSale Semaforo: %d \n\n",arguments[1][0]);
        
        

       
        
    
}



int main(int argc, char *argv[])
{
    

    /*  create the segment: */
    int shmid = getIdOfSharedMemory(request, sizeof(int));
    requestSize = shmat(shmid, (void *)0, 0);



    /* attach to the segment to get a pointer to it: */
    printf("NO HAN MUERTO%d\n",requestSize[1] );
    int shmI = getIdOfSharedMemory(key, requestSize[0]*sizeof(int)); 
    r = shmat(shmI, (void *)0, 0);
    sem_init(&semaphore, 0, 1);

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
    printf("Espacio sin ser Iniciado o Recientemente Finalizado.\n");
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
            printf("Number:%d\n", number);
            //int *space=(int *) malloc(sizeof(int)*number);
            printf("\n\nPide Semaforo: %d\n\n",idprocess);
            sem_wait(&semaphore);
            int * space=finding(r,number,type);
            int * arguments[3];
            arguments[0]=space;
            arguments[1]=nprocess;
            arguments[2]=number;
            if(space!=NULL)
                pthread_create(mythread, NULL,threadfunc, arguments);
            else{ 
                //AQUI VA BITACORA DE QUE MURIO
                sem_post(&semaphore);
                printf("\n\nSale Semaforo: %d \n\n",idprocess);
            }
            int waitb= 3 + rand() % (6+1 - 3);
           
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
                printf("\n\nSIZE: %d\n\n",number );         
                int *space=(int *) malloc(sizeof(int)*number);
                printf("\n\nPide Semaforo: %d\n\n",idprocess);
                sem_wait(&semaphore);
                space=finding(r,number,type);
                int * arguments[3];
                arguments[0]=space;
                arguments[1]=nprocess;
                arguments[2]=number;
                if(space!=NULL){
               
                    pthread_create(mythread, NULL,threadfunc, arguments);
                    
                }
                else{ 
                    sem_post(&semaphore);
                    printf("\n\nSale Semaforo: %d \n\n",idprocess);
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
            printf("tamaño: %d\n", sizeof(n));
            return n; 
        }
    }
    return NULL;
}

void print_list(int* list,int number){
    for(int i=0; i<number;i++){
        printf("ELemento %d: %d\n",i, list[i]);
    }
}

