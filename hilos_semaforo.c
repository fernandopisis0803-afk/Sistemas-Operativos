#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semaforo;

void* contar(void* arg) {
    long id = (long)arg;

    for (int i = 1; i <= 10000; i++) {
        if (i == 4000) {
            sem_wait(&semaforo);
        }

        printf("[Hilo %ld] Cuenta: %d\n", id, i);

        if (i == 6000) {
            sem_post(&semaforo);
        }
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t hilo1, hilo2;

    if (sem_init(&semaforo, 0, 1) != 0) {
        perror("Error al inicializar el semáforo");
        return 1;
    }

    if (pthread_create(&hilo1, NULL, contar, (void*)1) != 0) {
        perror("Error al crear el Hilo 1");
        return 1;
    }

    if (pthread_create(&hilo2, NULL, contar, (void*)2) != 0) {
        perror("Error al crear el Hilo 2");
        return 1;
    }

    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);

    sem_destroy(&semaforo);

    printf("\nEjecución finalizada con éxito.\n");
    return 0;
}
