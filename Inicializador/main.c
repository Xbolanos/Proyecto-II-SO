#include "../functions.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
extern int getIdOfSharedMemory(key_t key, int size);
key_t key = 2000;
key_t request = 2001; 
int shmidReq;
int *requestSize;
int mode;
float * r;
void print_shared_memory();
int main(int argc, char *argv[])
{
   


    /*  create the segment: */
    shmidReq = getIdOfSharedMemory(request, sizeof(int));

    /* attach to the segment to get a pointer to it: */
    requestSize = shmat(shmidReq, (void *)0, 0);
    requestSize[0] = atoi(argv[1]);

   

    int size = requestSize[0]*sizeof(float); 
    int shmid = getIdOfSharedMemory(key, size); 
    r = shmat(shmid, (void *)0, 0);
    int i = 0;
   
   
    while(i < requestSize[0]*sizeof(float)){
        r[i] = 0.0;
        i = i + 4; 
    }
    print_shared_memory();
    
 

    return 0;
}
void replace_Element(int i, int j, int* array, int size){
    for(int x=0;x<size;x++){
        printf("%f\n",array[x] );
        if(array[x]==i){
            array[x]=j;
        }
    }


}


void print_shared_memory(){
    int i = 0;
    while(i < requestSize[0]*sizeof(float)){
        printf("Posicion [%d]: %f\n", i/4,r[i]);
        i = i + 4; 
    }
}