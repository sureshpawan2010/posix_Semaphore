#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
//innstnce for seamaphore
sem_t s;
void *T(void *arg){
    sem_wait(&s);
    printf("Welcome! \n");
    sleep(4);
    printf("Bye ! \n");
    sem_post(&s);
}

int main(void){
    sem_init(&s, 0, 1);
    //thread instances 
    pthread_t o1,o2;
    printf("In a 1st thread now...\n");
    pthread_create(&o1,NULL,T,NULL);
    sleep(4);
    printf("In a 2nd thread now..\n");
    pthread_create(&o2,NULL,T,NULL);
    pthread_join(o1,NULL);
    pthread_join(o2,NULL);
    sem_destroy(&s);
}