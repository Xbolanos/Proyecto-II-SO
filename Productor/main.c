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
    int shmid;
    int *requestSize;
    int mode;



    /*  create the segment: */
    shmid = getIdOfSharedMemory(request, sizeof(int));
    requestSize = shmat(shmid, (void *)0, 0);



    /* attach to the segment to get a pointer to it: */
    
    printf("%d\n", requestSize[0]);
    int shmI = getIdOfSharedMemory(key, requestSize[0]*sizeof(int)); 
    int * r = shmat(shmI, (void *)0, 0);
     
    int i = 0;
    while(i < requestSize[0]*sizeof(int)){
        printf("%d\n", r[i]);
        i = i + 4; 
    }
    
    return 0;
}