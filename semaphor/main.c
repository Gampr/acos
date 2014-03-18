#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int a = 0;
int num[2] = {0, 1};
sem_t sem[2];
const char * s[] = {" ", "0"};

void * f(void * arg) {
    int i;
    int k = *(int *)arg;
    for (i = 0; i < 3; ++ i) {
        sem_wait(&sem[k]);
        printf("%s", (char *)s[k]);
        sem_post(&sem[k ^ 1]);
    }
    return NULL;
}

int main(int argc, char ** argv) {
    pthread_t thread1, thread2;

    sem_init(sem, 0, 1);
    sem_init(sem + 1, 0, 0);
    pthread_create(&thread1, NULL, f, num);
    pthread_create(&thread2, NULL, f, num + 1);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    return 0;
}
