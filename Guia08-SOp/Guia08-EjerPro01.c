#include <sys/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void compareFiles(char *file1, char *file2);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s archivo1 archivo2\n", argv[0]);
        exit(1);
    }

    compareFiles(argv[1], argv[2]);

    return 0;
}

void compareFiles(char *file1, char *file2) {
    int fd1, fd2;
    char *data1, *data2;
    struct stat sbuf1, sbuf2;

    fd1 = open(file1, O_RDONLY);
    if (fd1 == -1) {
        perror("Error al abrir el archivo 1");
        exit(1);
    }

    fd2 = open(file2, O_RDONLY);
    if (fd2 == -1) {
        perror("Error al abrir el archivo 2");
        close(fd1);
        exit(1);
    }

    if (fstat(fd1, &sbuf1) == -1 || fstat(fd2, &sbuf2) == -1) {
        perror("Error al obtener información del archivo");
        close(fd1);
        close(fd2);
        exit(1);
    }

    if (sbuf1.st_size != sbuf2.st_size) {
        printf("Los archivos son de diferentes tamaños.\n");
        close(fd1);
        close(fd2);
        exit(0);
    }

    data1 = mmap(NULL, sbuf1.st_size, PROT_READ, MAP_PRIVATE, fd1, 0);
    data2 = mmap(NULL, sbuf2.st_size, PROT_READ, MAP_PRIVATE, fd2, 0);

    if (data1 == MAP_FAILED || data2 == MAP_FAILED) {
        perror("Error al proyectar archivos en memoria");
        close(fd1);
        close(fd2);
        exit(1);
    }

    int result = memcmp(data1, data2, sbuf1.st_size);

    if (result == 0) {
        printf("Los archivos son idénticos.\n");
    } else {
        printf("Los archivos son diferentes.\n");
    }

    munmap(data1, sbuf1.st_size);
    munmap(data2, sbuf2.st_size);

    close(fd1);
    close(fd2);
}
