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
    printf("No es aqui 1\n");
    /* attach to the segment to get a pointer to it: */
    requestSize = shmat(shmidReq, (void *)0, 0);
    printf("No es aqui 1.1\n");
    printf("No es aqui 1.2\n");
    requestSize[1]=1;
   
    printf("No es aqui 2\n");
    int size = requestSize[0]*sizeof(float); 
    int shmid = getIdOfSharedMemory(key, size); 
    r = shmat(shmid, (void *)0, 0);
    printf("Cerrando\n");
    while( requestSize[1]==1);
    shmctl(shmidReq, IPC_RMID, NULL);
    shmctl(shmid, IPC_RMID, NULL);
    
 

    return 0;
}
