//This program i.e Program2.c runs in an infinite loop
//reading C strings as processed by Program1.c,
//one line at a time from the shared memory.
//After reading one line from the Shared Memory,
//this program counts the number of vowels, consonants, digits and words
//present in the string and prints the output on the terminal screen.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MEMORY_SIZE_BYTE 1024
sem_t *semaphore_id;

void count(char *line)
{
	int vowels, consonant, digit, space;

	vowels = consonant = digit = space = 0;

	for (int i = 0; line[i] != '\0'; ++i)
	{
		if (line[i] == 'a' || line[i] == 'e' || line[i] == 'i' ||
			line[i] == 'o' || line[i] == 'u' || line[i] == 'A' ||
			line[i] == 'E' || line[i] == 'I' || line[i] == 'O' ||
			line[i] == 'U')
		{
			++vowels;
		}
		else if ((line[i] >= 'a' && line[i] <= 'z') || (line[i] >= 'A' && line[i] <= 'Z'))
		{
			++consonant;
		}
		else if (line[i] >= '0' && line[i] <= '9')
		{
			++digit;
		}
		else if (line[i] == ' ')
		{
			++space;
		}
	}

	printf("\nNumber of Vowels    : %d", vowels);
	printf("\nNumber of Consonants: %d", consonant);
	printf("\nNumber of Digits    : %d", digit);
	printf("\nNumber of words     : %d\n", space + 1);
}

int main()
{
    //create shared memory
    //id: the id for shared memory
    int key_shm = 9999;
    int shm_id;
     
    //located the shared memory
    shm_id = shmget(key_shm, MEMORY_SIZE_BYTE, IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("Failed to create shared memory segment!");
        exit(1);
    }
    
    //open a semaphore
    //if not exist, create one
    semaphore_id = sem_open("/program_sem", O_CREAT, S_IRUSR | S_IWUSR, 1);
    sem_post(semaphore_id);
    
    //attach the segment to our data space.
    char* ShrMemTrigger;
    if ((ShrMemTrigger = shmat(shm_id, NULL, 0)) == (char *) -1) {
        perror("Error in Attaching The Shared Memory!");
        exit(1);
    }
    
    while(1)
    {
        sleep(2);
        
        sem_wait(semaphore_id);
        
        int length = strlen(ShrMemTrigger);
        
        
        if(length > 0 && *ShrMemTrigger != '@')
        //there is a line in the shared memory && this line has not read
        {
            
            count(ShrMemTrigger);
            //count() function processes the string stored on shared memory
            //Finds: # of Vowels, Consonants, Digits and Words
            //And prints the same statistic in other terminal.

            *ShrMemTrigger = '@';
            //change the first character of the line to '@',
            //means it has been read by the second process
        }
        sem_post(semaphore_id);
        //releasing semaphore
        sleep(2);
    }
    // Detaching the shared memory segment.
    shmdt(ShrMemTrigger);
    //Deallocating the shared memory segment.
    shmctl (shm_id, IPC_RMID, 0);
    
    //Closing the Semaphore
    if ( sem_close(semaphore_id) < 0 )
    {
        perror("sem_close");
    }
    
    //Removing Program's semaphore from the system.
    if (sem_unlink("/program_sem") < 0 )
    {
        perror("sem_unlink_error");
    }
}