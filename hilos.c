#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* funcion_hilo(void* arg) {
    for (int i = 10000; i >= 1; i--) {
        printf("[Hilo Secundario]: %d\n", i);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t hilo_id;
    int resultado;

    resultado = pthread_create(&hilo_id, NULL, funcion_hilo, NULL);
    if (resultado != 0) {
        perror("Error al crear el hilo");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i <= 10000; i++) {
        printf("[Hilo Principal]:  %d\n", i);
    }

    resultado = pthread_join(hilo_id, NULL);
    if (resultado != 0) {
        perror("Error al esperar al hilo");
        exit(EXIT_FAILURE);
    }

    return 0;
}
