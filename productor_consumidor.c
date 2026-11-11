#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define TAM_BUFFER 5
#define NUM_PRODUCTORES 2
#define NUM_CONSUMIDORES 2
#define ITEMS_POR_PRODUCTOR 10

int buffer[TAM_BUFFER];
int in = 0;
int out = 0;

sem_t sem_vacios;
sem_t sem_llenos;
pthread_mutex_t mutex;

void* productor(void* arg) {
    long id = (long)arg;
    for (int i = 1; i <= ITEMS_POR_PRODUCTOR; i++) {
        int item = (id * 100) + i; // Elemento producido identificable

        sem_wait(&sem_vacios);           // Esperar a que haya espacio libre
        pthread_mutex_lock(&mutex);       // Entrar a la sección crítica

        buffer[in] = item;
        printf("[Productor %ld] Producido: %d en posición %d\n", id, item, in);
        in = (in + 1) % TAM_BUFFER;

        pthread_mutex_unlock(&mutex);     // Salir de la sección crítica
        sem_post(&sem_llenos);            // Notificar que hay un nuevo elemento

        usleep(50000); // Pequeña pausa simulada
    }
    pthread_exit(NULL);
}

void* consumidor(void* arg) {
    long id = (long)arg;
    int total_items_a_consumir = (NUM_PRODUCTORES * ITEMS_POR_PRODUCTOR) / NUM_CONSUMIDORES;

    for (int i = 0; i < total_items_a_consumir; i++) {
        sem_wait(&sem_llenos);            // Esperar a que haya elementos disponibles
        pthread_mutex_lock(&mutex);       // Entrar a la sección crítica

        int item = buffer[out];
        printf("  [Consumidor %ld] Consumido: %d de posición %d\n", id, item, out);
        out = (out + 1) % TAM_BUFFER;

        pthread_mutex_unlock(&mutex);     // Salir de la sección crítica
        sem_post(&sem_vacios);            // Notificar que hay un espacio libre

        usleep(80000); // Pequeña pausa simulada
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t prod[NUM_PRODUCTORES], cons[NUM_CONSUMIDORES];

    // Inicialización de sincronización
    sem_init(&sem_vacios, 0, TAM_BUFFER);
    sem_init(&sem_llenos, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    // Crear los 2 hilos productores
    for (long i = 0; i < NUM_PRODUCTORES; i++) {
        pthread_create(&prod[i], NULL, productor, (void*)(i + 1));
    }

    // Crear los 2 hilos consumidores
    for (long i = 0; i < NUM_CONSUMIDORES; i++) {
        pthread_create(&cons[i], NULL, consumidor, (void*)(i + 1));
    }

    // Esperar a que terminen todos los hilos
    for (int i = 0; i < NUM_PRODUCTORES; i++) pthread_join(prod[i], NULL);
    for (int i = 0; i < NUM_CONSUMIDORES; i++) pthread_join(cons[i], NULL);

    // Destrucción de recursos
    sem_destroy(&sem_vacios);
    sem_destroy(&sem_llenos);
    pthread_mutex_destroy(&mutex);

    printf("\nEjecución finalizada con éxito. Todos los datos fueron producidos y consumidos.\n");
    return 0;
}
