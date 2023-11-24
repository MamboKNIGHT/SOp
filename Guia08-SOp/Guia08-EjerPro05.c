#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void interleaveFiles(char *file1, char *file2, char *outputFile);

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Uso: %s <archivo1> <archivo2> <archivo_salida>\n", argv[0]);
        exit(1);
    }

    interleaveFiles(argv[1], argv[2], argv[3]);

    return 0;
}

void interleaveFiles(char *file1, char *file2, char *outputFile) {
    FILE *fp1, *fp2, *output;
    char line1[256], line2[256];

    fp1 = fopen(file1, "r");
    if (fp1 == NULL) {
        perror("Error al abrir el archivo 1");
        exit(1);
    }

    fp2 = fopen(file2, "r");
    if (fp2 == NULL) {
        perror("Error al abrir el archivo 2");
        fclose(fp1);
        exit(1);
    }

    output = fopen(outputFile, "w");
    if (output == NULL) {
        perror("Error al crear el archivo de salida");
        fclose(fp1);
        fclose(fp2);
        exit(1);
    }

    while (fgets(line1, sizeof(line1), fp1) != NULL || fgets(line2, sizeof(line2), fp2) != NULL) {
        if (line1[0] != '\0') {
            fprintf(output, "%s", line1);
            line1[0] = '\0';  // Limpiar la línea después de escribir
        }

        if (line2[0] != '\0') {
            fprintf(output, "%s", line2);
            line2[0] = '\0';  // Limpiar la línea después de escribir
        }
    }

    fclose(fp1);
    fclose(fp2);
    fclose(output);

    printf("Archivos intercalados con éxito en %s.\n", outputFile);
}