#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define NITER 1000000

//Semaphore instances
sem_t mutex;
int cnt = 0;

void * Count(void * a)
{
    sem_wait(&mutex);
    int i, tmp;
    for(i = 0; i < NITER; i++)
    {
        tmp = cnt;      /* copy the global cnt locally */
        tmp = tmp+1;    /* increment the local copy */
        cnt = tmp;      /* store the local value into the global cnt */ 
    }
    sem_post(&mutex);
}

int main(int argc, char * argv[])
{   
    int32_t sem_val = 1;
    pthread_t tid1, tid2;
    //initialize semaphore
    sem_init(&mutex,0,sem_val);
    if(pthread_create(&tid1, NULL, Count, NULL))
    {
      printf("\n ERROR creating thread 1");
      exit(1);
    }

    if(pthread_create(&tid2, NULL, Count, NULL))
    {
      printf("\n ERROR creating thread 2");
      exit(1);
    }

    if(pthread_join(tid1, NULL))	/* wait for the thread 1 to finish */
    {
      printf("\n ERROR joining thread");
      exit(1);
    }

    if(pthread_join(tid2, NULL))        /* wait for the thread 2 to finish */
    {
      printf("\n ERROR joining thread");
      exit(1);
    }

    if (cnt < 2 * NITER) 
        printf("\n BOOM! cnt is [%d], should be %d\n", cnt, 2*NITER);
    else
        printf("\n OK! cnt is [%d]\n", cnt);
  
    pthread_exit(NULL);
}

/*
NOTE:
Resource Link : http://www.csc.villanova.edu/~mdamian/threads/posixsem.html

Threads can greatly simplify writing elegant and efficient programs. However, there are problems when multiple threads share a common address space, like the variable cnt in our earlier example.
To understand what might happen, let us analyze this simple piece of code:

      THREAD 1                THREAD 2
      a = data;               b = data;
      a++;                    b--;
      data = a;               data = b;

Now if this code is executed serially (for instance, THREAD 1 first and then THREAD 2), there are no problems. However threads execute in an arbitrary order, so consider the following situation:
So data could end up +1, 0, -1, and there is NO WAY to know which value! It is completely non-deterministic!

The solution to this is to provide functions that will block a thread if another thread is accessing data that it is using.

Pthreads may use semaphores to achieve this. 
*/