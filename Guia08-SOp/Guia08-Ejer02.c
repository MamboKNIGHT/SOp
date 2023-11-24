/*
*Nombre		    :	demoMapeo
*Proposito 	    :	muestra el enésimo caracter de un archivo
*Autor	    	:	Juan Joel Suta Cusihuaman
*FCreacion 	    :	24/11/2023
*FModif.	    :	---
*/
#include <stdio.h> 
#include <stdlib.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/mman.h> 
#include <sys/stat.h> 
#include <errno.h>

int main(int argc, char *argv[])
{
    //Variables 
    int fd;
    int offset; 
    char *data;
    struct stat sbuf;


    //Verificar que el programa se haya invocado correctamente 
    if (argc != 2)
    {
        fprintf(stderr, "Uso: demoMapeo <desplazamiento>\n"); 
        exit(1);
    }
     
    
    //Indicar proposito del programa
    printf("ESTE PROGRAMA MUESTRA EL ENESIMO CARACTER DE UN ARCHIVO PROYECTADO EN MEMORIA\n");
    
    //Procesar
    if ((fd = open("Guia08-Ejer01.c", O_RDONLY)) == -1)
    {
        perror("open");
        exit(1);
    }
    
    if (stat("Guia08-Ejer01.c", &sbuf) == -1)
    {
        perror("stat"); 
        exit(1);
    }
    
    offset = atoi(argv[1]);
    
    if (offset < 0 || offset > sbuf.st_size-1)
    {
        fprintf(stderr, "El desplazamiento debe estar en el rango 0-%ld\n", sbuf.st_size-1);
        exit(1);
    }
    
    data = mmap((caddr_t)0, sbuf.st_size, PROT_READ, MAP_SHARED, fd, 0);
    
    if (data == (caddr_t)(-1))
    {
        perror("mmap"); 
        exit(1);
    }
    
    //Mostrar resultados
    printf("El byte en la posicion %d es %c\n", offset, data[offset]);
    
    //Salir 
    return 0;
}
/*
*Ejemplo:
*Consola -> gcc -o G8E2 Guia08-Ejer02.c 
*Consola -> G8E1 0
*Resultado: ESTE PROGRAMA MUESTRA EL ENESIMO CARACTER DE UNA ARCHIVO PROYECTADO EN MEMORIA
*           El byte en la posicion 0 es /
*/
