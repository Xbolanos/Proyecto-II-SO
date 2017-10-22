#include "../functions.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
extern int getIdOfSharedMemory(key_t key, int size);
extern void print_shared_memory();

int main(int argc, char *argv[])
{
   


    /*  create the segment: */
    int shmidReq = getIdOfSharedMemory(request, sizeof(int));
    printf("shmidReq: %d\n", shmidReq);
    /* attach to the segment to get a pointer to it: */
    requestSize = shmat(shmidReq, (void *)0, 0);
    requestSize[0] = atoi(argv[1]);
    requestSize[1]=0;
    

    int size = requestSize[0]*sizeof(float); 
    int shmid = getIdOfSharedMemory(key, size); 
    r = shmat(shmid, (void *)0, 0);
    int i = 0;
    printf("shmid: %d\n", shmidReq);
   
   
    while(i < requestSize[0]*sizeof(float)){
        r[i] = 0.0;
        i = i + 4; 
    }
    print_shared_memory();
    
 

    return 0;
}
