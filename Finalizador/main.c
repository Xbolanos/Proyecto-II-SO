#include "../functions.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <fcntl.h>
extern int getIdOfSharedMemory(key_t key, int size);
extern void print_shared_memory();
static sem_t * smain;
static sem_t * semP;
int main(int argc, char *argv[])
{
    printf("Inicio de Finalizador.\n");
    int shmidReq = getIdOfSharedMemory(request, sizeof(int));  
    /* attach to the segment to get a pointer to it: */
    requestSize = shmat(shmidReq, (void *)0, 0);
    requestSize[1]=1;
    int size = requestSize[0]*sizeof(float); 
    int shmid = getIdOfSharedMemory(key, size); 
    int sizeProcess = sizeof(int) * 2000;
    int proccess_shm_id = getIdOfSharedMemory(processes_key, sizeProcess);
    r = shmat(shmid, (void *)0, 0);
    if(requestSize[2]==1){
        /*  create the segment: */
        printf("Esperando a cerrar procesos...\n");
        while( requestSize[1]==1);
    }
    shmctl(shmidReq, IPC_RMID, NULL);
    shmctl(shmid, IPC_RMID, NULL);
    shmctl(proccess_shm_id, IPC_RMID, NULL);
    smain = sem_open(SMAIN, O_CREAT, 0644, 1); 
    semP = sem_open(SNAME, O_CREAT, 0644, 1); 
    sem_destroy(smain); 
    sem_destroy(semP); 

    printf("Fin del Finalizador.\n");
 

    return 0;
}
