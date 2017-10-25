#include "../functions.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <semaphore.h>

extern int getIdOfSharedMemory(key_t key, int size);
extern void print_shared_memory();
static sem_t * semP;
static sem_t * semaphore;  
int main(int argc, char *argv[])
{
	if(argc < 2 || argc > 2){
		printf("Ingrese como unico parametro 1 o 2, \n->1 para ver la memoria\n->2 para ver el estado de los procesos\n (:");
	}
	else if(atoi(argv[1])==1){
		spyMemory();
	}
	else if(atoi(argv[1])==0){
		spyProcesses();
	}
	else{
		printf("Ingrese como unico parametro 1 o 2, \n->1 para ver la memoria\n->2 para ver el estado de los procesos\n (:");
	}
   printf("okay (:\n");
   return 0;
}

void spyMemory(){
	int shmid = getIdOfSharedMemory(request, sizeof(int));
	requestSize = shmat(shmid, (void *)0, 0);

	int shmI = getIdOfSharedMemory(key, requestSize[0]*sizeof(int)); 
	r = shmat(shmI, (void *)0, 0);
	
	semaphore = sem_open(SMAIN, 0); /* Open a preexisting semaphore. */
	int value;
	sem_getvalue(semaphore, &value);
 
    while(value == 0){
    	sem_getvalue(semaphore, &value);
    	sleep(1); 
    }
	sem_wait(semaphore); 
	

	print_shared_memory(); 
	sem_post(semaphore); 

}

void spyProcesses(){
	/*AQUI PIDE MEM COMPARTIDA DE LOS PROCESOS EN GENERAL*/
    int *process_shm; 
    int sizeProcess = (int) sizeof(int) * 20000;
	int process_shm_id = getIdOfSharedMemory(processes_key, sizeProcess); 
	process_shm = shmat(process_shm_id, NULL, 0); 
  
    /*parte de semaforos*/
    semP = sem_open(SNAME, 0); /* Open a preexisting semaphore. */

    int value;
	sem_getvalue(semP, &value);
    while(value == 0){
    	sem_getvalue(semP, &value);
    	sleep(1); 
    }
    sem_wait(semP);
    for(int i = 8; i < 20000; i += 8) {
    	if (process_shm[i] != 0){
    		switch(process_shm[i + 4]) {
	            case 0:
	                printf("PID: %i, State: %s\n", process_shm[i], "ACTIVE");
	                break;
	            case 1:
	                printf("PID: %i, State: %s\n", process_shm[i], "DEAD OLD");
	                break;
	            case 2:
	                printf("PID: %i, State: %s\n", process_shm[i], "DEAD NO SPACE");
	                break;
	            case 3:
	                printf("PID: %i, State: %s\n", process_shm[i], "SEARCHING SPACE");
	                break;
	            default:
	                break;
	        }
    	}
     }
     sem_post(semP); 
}

    
 