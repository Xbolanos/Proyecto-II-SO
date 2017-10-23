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
    int *process_shm; 
    int sizeProcess = (int) sizeof(int) * 20000;
    int process_shm_id = getIdOfSharedMemory(processes_key, sizeProcess); 
    process_shm = shmat(process_shm_id, NULL, 0);   

    printf("jist in case\n");

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
    
    printf("okay (:\n");

    return 0;
}