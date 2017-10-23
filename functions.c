#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

key_t key = 2000;
key_t request = 2001; 
int *requestSize;
int mode;
int * r;


void print_shared_memory(){
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

int * search(int *r, int number){
    int * list=(int *) malloc(sizeof(int)*number);
    //printf("Number%d\n", number);
    int pos=0;
    for( int i = 0;i < requestSize[0]*sizeof(int);i=i+4){
        if(pos==number){
            return list;
        }else{
            if (r[i]==0){  
                list[pos]=i;
                printf("I:%d\n", i);
                pos++;
            }
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



int getIdOfSharedMemory(key_t key, int size){
    int  id; 
    id = shmget(key, size, 0644 | IPC_CREAT); //crea el espacio pero
    // si ya esta asignado entonces le da el id de lo que ya se creo
    //siempre y cuando tenga la misma llave y tamanno 
    if (id == -1){
        perror("shmat");
        return -1; 
    } 
    else{
        return id; 
    }
}

