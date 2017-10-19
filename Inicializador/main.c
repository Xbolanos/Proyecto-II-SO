#include "../functions.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
extern int getIdOfSharedMemory(key_t key, int size);

int main(int argc, char *argv[])
{
    key_t key = 2000;
    key_t request = 2001; 
    int shmidReq;
    int *requestSize;
    int mode;


    /*  create the segment: */
    shmidReq = getIdOfSharedMemory(request, sizeof(int));

    /* attach to the segment to get a pointer to it: */
    requestSize = shmat(shmidReq, (void *)0, 0);

    
    requestSize[0] = 25;

    printf("%d\n", sizeof(int));

    int size = atoi(argv[2]); 
    int shmid = getIdOfSharedMemory(key, requestSize[0]*sizeof(int)); 
    int * r = shmat(shmid, (void *)0, 0);
    int i = 0;
    while(i < requestSize[0]*sizeof(int)){
        r[i] = 0;
        i = i + 4; 
    }
    r[0] = 4; 
    r[16] = 76; 

    return 0;
}


