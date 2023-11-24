#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    // Variables
    int fd;
    int offset;
    char *data;
    struct stat sbuf;

    // Verificar que el programa se haya invocado correctamente
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <nombre_archivo> <desplazamiento>\n", argv[0]);
        exit(1);
    }

    // Procesar
    if ((fd = open(argv[1], O_RDONLY)) == -1) {
        perror("open");
        exit(1);
    }

    if (stat(argv[1], &sbuf) == -1) {
        perror("stat");
        exit(1);
    }

    offset = atoi(argv[2]);

    if (offset < 0 || offset > sbuf.st_size - 1) {
        fprintf(stderr, "El desplazamiento debe estar en el rango 0-%ld\n", sbuf.st_size - 1);
        exit(1);
    }

    data = mmap((caddr_t)0, sbuf.st_size, PROT_READ, MAP_SHARED, fd, 0);

    if (data == (caddr_t)(-1)) {
        perror("mmap");
        exit(1);
    }

    // Mostrar resultados
    printf("El byte en la posición %d es %c\n", offset, data[offset]);

    // Salir
    munmap(data, sbuf.st_size);
    close(fd);

    return 0;
}
/*
*Ejemplo:
*Consola -> gcc -o G8EP2 Guia08-EjerPro01.c
*Consola -> G8EP2 Guia08-Ejer01.c 0
*Resultado: El byte en la posición 0 es /
*/
