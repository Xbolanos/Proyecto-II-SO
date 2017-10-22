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

key_t key = 2000;
key_t request = 2001; 
int shmid;
int *requestSize;
int mode;
int * r;
int * search(int *r, int number);
int * finding(int * r, int number, int type);
int * search_seg(int *r, int number);
int type=0;
void threadfunc(int *arguments[2]) {
        int proc=arguments[1][0];
        int * space=arguments[0];
        for(int i=0;i<sizeof(space[0]);i++){
                int j=space[i];
                r[j]=proc;
        }
        //**AQUI VA A ESCRIBIR BITACORA
        print_shared_memory();
        sem_post(&semaphore);
        printf("\n\nSale Semaforo: %d \n\n",proc);

        int i= 1 + rand() % (3+1 - 1);
        print_shared_memory();
        printf("Estoy por aqui1\n");
        sleep(i);
        printf("\n\nPide Semaforo: %d \n\n",proc);
        sem_wait(&semaphore);
        printf("Estoy por aqui\n");
        if(type==0){
            replace_Element(proc, 0, r,requestSize[0]*sizeof(int));
        }
        else{
         
            for(int i=0;i<sizeof(space[0]);i++){
                int j=space[i];
                r[j]=0;
          }
        }
        //**AQUI VA A ESCRIBIR BITACORA
        print_shared_memory();
        sem_post(&semaphore);
        printf("\n\nSale Semaforo: %d \n\n",arguments[1][0]);
        
        

       
        
    
}

extern int getIdOfSharedMemory(key_t key, int size);

int main(int argc, char *argv[])
{
    

    /*  create the segment: */
    shmid = getIdOfSharedMemory(request, sizeof(int));
    requestSize = shmat(shmid, (void *)0, 0);



    /* attach to the segment to get a pointer to it: */
    
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

    
    return 0;

}

void pagination(){
    int idprocess = 0;        
        while(1){
            pthread_t *mythread;
            mythread = (pthread_t *)malloc(sizeof(*mythread));
            idprocess++;
            int *nprocess[1];
            nprocess[0]=idprocess;
            int number= 1 + rand() % (10+1 - 1);
            
            int *space=(int *) malloc(sizeof(int)*number);
            printf("\n\nPide Semaforo: %d\n\n",idprocess);
            sem_wait(&semaphore);
            space=finding(r,number,type);
            printf("sale\n");
            int * arguments[2];
            arguments[0]=space;
            arguments[1]=nprocess;
            if(space!=NULL)
                pthread_create(mythread, NULL,threadfunc, arguments);
            else{ 
                sem_post(&semaphore);
                printf("\n\nSale Semaforo: %d \n\n",idprocess);
            }
            int waitb= 3 + rand() % (6+1 - 3);
           
            sleep(waitb);
        }
}

void segmentation(){
    int idprocess = 0;

        while(1){
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
                printf("sale\n");
                int * arguments[2];
                arguments[0]=space;
                arguments[1]=nprocess;
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
int * search(int *r, int number){
    int * list=(int *) malloc(sizeof(int)*number);
    int pos=0;
    for( int i = 0;i < requestSize[0]*sizeof(int);i=i+4){

   
        if(pos==number){
            return list;
        }else{
            if (r[i]==0){    
                list[pos]=i;
                pos++;
            }
        }
    }
    return NULL;
}

int * finding(int * r, int number, int type){
    int i=0;
    int* n;
    while(i<31){
       if(type==0){
           n=search(r, number);
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


int * search_seg(int *r, int number){
    int * list=(int *) malloc(sizeof(int)*number);
    int pos=0;
    for( int i = 0;i < requestSize[0]*sizeof(int);i=i+4){
        // printf("ENTRA\n\n", r[i]);
   
        if(pos==number){
            return list;
        }else{
            if(pos!=0 && i!=0){
                if( r[i-4]==0 ){
                    if (r[i]==0){
                        list[pos]=i;
                        pos++;

                    }
            }else{
                memset(list,0,sizeof(list));
                pos=0;
            }

        }else{
            if (r[i]==0){
                        list[pos]=i;
                        pos++;

                    }

        }
        }
    }
    return NULL;
}

int print_shared_memory(){
    int i = 0;
    while(i < requestSize[0]*sizeof(int)){
        printf("Posicion [%d]: %d\n", i/4,r[i]);
        i = i + 4; 
    }
}
void replace_Element(int i, int j, int* array, int size){
    for(int x=0;x<size;x++){
        if(array[x]==i){
            array[x]=j;
        }
    }


}
void replace_Element_Between(int i,int* array, int init, int end){
    printf("init:%d\n",init);
    printf("end:%d\n",end );
    for(int x=init;x<end+4;x+=4){
            printf("array[x] %d\n", array[x]);
            array[x]=0;
        
    }


}