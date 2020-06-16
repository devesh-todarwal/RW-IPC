//This program i.e Program1.c runs in an infinite loop
//receiving strings as input from the user, one line at a time.
//After reading one line from the standard input,
//this program sends this information to the other program

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MEMORY_SIZE_BYTE 1024
sem_t *semaphore_id;

int main(void)
{
    int shm_id;
    int key_shm = 9999;
    
    //creating shared memory segment
    
    shm_id = shmget(key_shm, MEMORY_SIZE_BYTE, IPC_CREAT | 0666);
    //shm_id: the id for recognising the shared memory

    if (shm_id == -1) {
        perror("Shared Memory Segmentation Failed!");
        exit(1);
    }
    
    //attach the segment to our data space.
    char* ShrMemTrigger;
    if ((ShrMemTrigger = shmat(shm_id, NULL, 0)) == (char *) -1) {
        perror("Error in Attaching Shared Memory!");
        exit(1);
    }
    
    //open a semaphore
    //if not exist, create one.
    semaphore_id = sem_open("/program_sem", O_CREAT, S_IRUSR | S_IWUSR, 1);
    sem_post(semaphore_id);
    
    //if the line contains the secret code "C00L".
    //sends this information to the the shared memory
    while(1)
    {
        //get alpha numeric strings as input from the user
        //wiht
        sleep(2);
        
        //read command line input 
        char inp_text[128];
        
        {
            
            while(gets(inp_text) != NULL)
            {
                
                sleep(1);
                {
                    
                    sem_wait(semaphore_id);//waiting for semaphore
                    sprintf (ShrMemTrigger, "%s", inp_text);
                    sem_post(semaphore_id);//release semaphore
                    
                    //wait until the line has read.
                    //The '@' means that the line has been read
                    while(*ShrMemTrigger != '@')
                    {
                        sleep(1);
                    }
                }
                
            }
        }
    }
    
    //Detach the shared memory segment.
    shmdt(ShrMemTrigger);
    
    //Deallocate the shared memory segment.
    shmctl (shm_id, IPC_RMID, 0);
    
    //Closing the Semaphore
    if (sem_close(semaphore_id) < 0 )
    {
        perror("semaphore_close");
    }
    
    //Removing Program1 semaphore  from the system.
    if (sem_unlink("/program_sem") < 0 )
    {
        perror("semaphore_unlink_error!");
    }
    
}