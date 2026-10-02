#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int i = 0;

    switch (fork()) {

        case -1:
            perror("Error al crear procesos");
            exit(-1);
            break;

        case 0:
            /* Código para el hijo */
            i = 10000;

            while (i >= 1) {
                printf("\t\tSoy el proceso hijo: %d\n", i--);
            }
            break;

        default:
            /* Código para el padre */
            i = 1;

            while (i <= 10000) {
                printf("Soy el proceso padre: %d\n", i++);
            }
            break;
    }

    exit(0);
}
