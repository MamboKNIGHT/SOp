#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void countAlphabet(char *filename);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <nombre_archivo>\n", argv[0]);
        exit(1);
    }

    countAlphabet(argv[1]);

    return 0;
}

void countAlphabet(char *filename) {
    int fd;
    char *data;
    int i;
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

    int alphabetCount[26] = {0};  // Inicializar el arreglo de frecuencias a cero
    for (i = 0; i < sbuf.st_size; i++) {
        char c = data[i];
        if (isalpha(c)) {
            c = tolower(c);
            alphabetCount[c - 'a']++;
        }
    }

    printf("Frecuencia de caracteres del alfabeto inglés:\n");
    for (i = 0; i < 26; i++) {
        printf("%c: %d\n", 'a' + i, alphabetCount[i]);
    }

    munmap(data, sbuf.st_size);
    close(fd);
}
/*
*Ejemplo:
*Consola -> gcc -o G8EP4 Guia08-EjerPro04.c
*Consola -> G8EP4 Guia08-Ejer01.c 
*Resultado: Frecuencia de caracteres del alfabeto ingles:
            a: 92
            b: 15
            c: 71
            d: 72
            e: 124
            f: 38
            g: 23
            h: 29
            i: 97
            j: 0
            k: 0
            l: 56
            m: 22
            n: 67
            o: 104
            p: 36
            q: 2
            r: 117
            s: 81
            t: 86
            u: 30
            v: 22
            w: 3
            x: 7
            y: 12
            z: 7
*/
