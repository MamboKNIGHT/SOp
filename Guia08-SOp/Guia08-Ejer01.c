/*
*Nombre		   :	copiar.c
*Proposito	   :	copia un archivo utilizando archivos proyectados
*Autor		   :	Juan Joel Suta Cusihuaman
*FCreacion	   :	24/11/2023
*FModificacion :    
*/

#include <sys/types.h> 
#include <sys/stat.h> 
#include <sys/mman.h> 
#include <fcntl.h> 
#include <stdio.h> 
#include <unistd.h>
#include <string.h>

int main(int argc,char **argv)
{
    /* Variables */ 
    int i;
    int fdo; 
    int fdd; 
    char *org; 
    char *dst; 
    char *p; 
    char *q;
    struct stat bstat;
    /* Verificar los argumentos */ 
    if(argc != 3 )
    {
        fprintf(stderr, "Uso: %s orig dest \n", argv[0]); 
        return(1);
    }

    /* Abre el archivo origen para lectura */ 
    if( (fdo = open(argv[1], O_RDONLY))<0 )
    {
        perror("No puede abrise el archivo origen"); 
        return(1);
    }
     
    /* Crea el archivo destino */
    if( (fdd = open(argv[2], O_CREAT|O_TRUNC|O_RDWR)) < 0 )
    {
        perror("No puede crearse el archivo destino"); 
        close(fdo);
        return(1);
    }
    
    /* Determinar la longitud del archivo origen */ 
    if( fstat(fdo, &bstat)<0 )
    {
        perror("Error en fstat del archivo origen"); 
        close(fdo); 
        return(1);
    }
    
    /* Establecer que la longitud del archivo destino es igual a la del origen */
    if( ftruncate(fdd,bstat.st_size) < 0 )
    {
        perror("Error en ftruncate del archivo destino"); 
        close(fdo);
        close(fdd); 
        return(1);
    }
    
    /*Se proyecta el archivo origen*/
    if(( org=mmap(NULL, bstat.st_size, PROT_READ, MAP_PRIVATE, fdo,0)) == MAP_FAILED)
    {
        perror("Error en la proyeccion del archivo origen"); 
        close(fdo);
        close(fdd); 
        return(1);
    }
    
    /*Se proyecta el archivo destino*/
    if((dst=mmap(NULL, bstat.st_size, PROT_WRITE, MAP_SHARED, fdd,0)) == MAP_FAILED)
    {
        perror("Error en la proyeccion del archivo destino"); 
        close(fdo);
        close(fdd);
        return(1);
    }
    
    memcpy(dst, org, bstat.st_size);

    /*Se eliminan las proyecciones*/ 
    munmap(org,bstat.st_size); 
    munmap(dst,bstat.st_size);

    /*Se cierran los archivos*/ 
    close(fdo);
    close(fdd);
    
    return 0;
}