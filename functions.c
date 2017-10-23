#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

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
            printf("libera\n");
            writeLog(i, 1, x/4);
        }
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


void writeLog (int PID, int action, int line) {
    /*
    param action: 
    0 : asignacion 
    1 : desasignacion 
    2 : proceso que murio buscando 
    3 : proceso que murio por viejito
    */

    FILE *log;

    char filename[] = "bitacora.txt";
    
    log = fopen(filename, "a");
    
    if (log == NULL){
     printf("ERROR: no se puede abrir el archivo - bitacora - \n"); 
    }
    
    time_t timeP;
    time ( &timeP );
    struct tm * timeinfo = localtime ( &timeP );
    if(action == 0){
        fprintf(log, "Productor\n PID: %i; Tipo accion: %s; Hora: %i:%i:%i; Linea: %i\n", PID, "asignacion", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, line);      
    }    
    else if (action == 1){
        fprintf(log, "Productor\n PID: %i; Tipo accion: %s; Hora: %i:%i:%i; Linea: %i\n", PID, "desasignacion", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, line);      
    } 
    else if (action == 2){
        fprintf(log, "Productor\n PID: %i; Causa de muerte: %s; Hora: %i:%i:%i\n", PID, "no encontro memoria", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);      
    } 
    
    fclose(log);
}


