#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>



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

