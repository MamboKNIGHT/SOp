#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int countCharacter(char *filename, char target);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <nombre_archivo> <caracter>\n", argv[0]);
        exit(1);
    }

    char target = argv[2][0];
    int count = countCharacter(argv[1], target);

    printf("El caracter '%c' aparece %d veces en el archivo.\n", target, count);

    return 0;
}

int countCharacter(char *filename, char target) {
    int fd;
    char *data;
    struct stat sbuf;

    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error al abrir el archivo");
        exit(1);
    }

    if (stat(filename, &sbuf) == -1) {
        perror("Error al obtener información del archivo");
        close(fd);
        exit(1);
    }

    data = mmap(NULL, sbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

    if (data == MAP_FAILED) {
        perror("Error al proyectar el archivo en memoria");
        close(fd);
        exit(1);
    }

    int count = 0;
    int i;
    for (i = 0; i < sbuf.st_size; i++) {
        if (data[i] == target) {
            count++;
        }
    }

    munmap(data, sbuf.st_size);
    close(fd);

    return count;
}