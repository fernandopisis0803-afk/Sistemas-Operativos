#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_FILOSOFOS 5
#define CICLOS_COMIDA 3

pthread_mutex_t tenedores[NUM_FILOSOFOS];
pthread_t filosofos[NUM_FILOSOFOS];

void* filosofo_rutina(void* arg) {
    long id = (long)arg;
    int izq = id;
    int der = (id + 1) % NUM_FILOSOFOS;

    for (int i = 1; i <= CICLOS_COMIDA; i++) {
        // El filósofo piensa
        printf("[Filósofo %ld] Está pensando (Turno %d)...\n", id, i);
        usleep(100000);

        // Prevenir Deadlock: El último filósofo invierte el orden al tomar los tenedores
        if (id == NUM_FILOSOFOS - 1) {
            pthread_mutex_lock(&tenedores[der]);
            pthread_mutex_lock(&tenedores[izq]);
        } else {
            pthread_mutex_lock(&tenedores[izq]);
            pthread_mutex_lock(&tenedores[der]);
        }

        // El filósofo come
        printf("  ==> [Filósofo %ld] Tomó tenedores y está COMIENDO (Turno %d)\n", id, i);
        usleep(150000);

        // Soltar tenedores
        pthread_mutex_unlock(&tenedores[izq]);
        pthread_mutex_unlock(&tenedores[der]);

        printf("  <-- [Filósofo %ld] Soltó los tenedores y terminó de comer (Turno %d)\n", id, i);
    }

    pthread_exit(NULL);
}

int main() {
    // Inicializar los mutex para cada tenedor
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_mutex_init(&tenedores[i], NULL);
    }

    // Crear hilos para cada filósofo
    for (long i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_create(&filosofos[i], NULL, filosofo_rutina, (void*)i);
    }

    // Esperar a que todos los filósofos terminen de comer
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_join(filosofos[i], NULL);
    }

    // Destruir mutexes
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_mutex_destroy(&tenedores[i]);
    }

    printf("\nEjecución finalizada con éxito. Todos los filósofos comieron sin interbloqueos.\n");
    return 0;
}
