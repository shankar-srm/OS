#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>


#define MAX_BUFFERS 10

#define LOGFILE "/tmp/exam.log"

#define SEM_MUTEX_NAME "/sem-mutex1"
#define SEM_BUFFER_COUNT_NAME "/sem-buffer-count1"
#define SEM_SPOOL_SIGNAL_NAME "/sem-spool-signal1"
#define SHARED_MEM_NAME "/posix-shared-mem-example1"

struct shared_memory {
    char buf [MAX_BUFFERS] [256];
    int buffer_index;
    int buffer_print_index;
};

void error (char *msg);

int main (int argc, char **argv)
{
    struct shared_memory *shared_mem_ptr;
    sem_t *mutex_sem, *buffer_count_sem, *spool_signal_sem;
    int fd_shm;
    char mybuf [256];
    
 
    if ((mutex_sem = sem_open (SEM_MUTEX_NAME, 0, 0, 0)) == SEM_FAILED)
        error ("sem_open");
    

    if ((fd_shm = shm_open (SHARED_MEM_NAME, O_RDWR, 0)) == -1)
        error ("shm_open");

    if ((shared_mem_ptr = mmap (NULL, sizeof (struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED,
            fd_shm, 0)) == MAP_FAILED)
       error ("mmap");

    
	
    if ((buffer_count_sem = sem_open (SEM_BUFFER_COUNT_NAME, 0, 0, 0)) == SEM_FAILED)
        error ("sem_open");

   
    if ((spool_signal_sem = sem_open (SEM_SPOOL_SIGNAL_NAME, 0, 0, 0)) == SEM_FAILED)
        error ("sem_open");

    char buf [200], *cp;

    printf ("Please type a message: ");

    while (fgets (buf, 198, stdin)) {
     
        int length = strlen (buf);
        if (buf [length - 1] == '\n')
           buf [length - 1] = '\0';

       
        if (sem_wait (buffer_count_sem) == -1)
            error ("sem_wait: buffer_count_sem");
    
        /* There might be multiple producers. We must ensure that 
 *             only one producer uses buffer_index at a time.  */
      
        if (sem_wait (mutex_sem) == -1)
            error ("sem_wait: mutex_sem");

	  
            time_t now = time (NULL);
            cp = ctime (&now);
            int len = strlen (cp);
            if (*(cp + len -1) == '\n')
                *(cp + len -1) = '\0';
            sprintf (shared_mem_ptr -> buf [shared_mem_ptr -> buffer_index], "%d: %s %s\n", getpid (), 
                     cp, buf);
            (shared_mem_ptr -> buffer_index)++;
            if (shared_mem_ptr -> buffer_index == MAX_BUFFERS)
                shared_mem_ptr -> buffer_index = 0;

  
        if (sem_post (mutex_sem) == -1)
            error ("sem_post: mutex_sem");
    
	
        if (sem_post (spool_signal_sem) == -1)
            error ("sem_post: (spool_signal_sem");

        printf ("Please type a message: ");
    }
 
    if (munmap (shared_mem_ptr, sizeof (struct shared_memory)) == -1)
        error ("munmap");
    exit (0);
}


void error (char *msg)
{
    perror (msg);
    exit (1);
}
