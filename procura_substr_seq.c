#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "timer.h"

#define TAM_BUF             5000000
#define DEFAULT_FILENAME    "arquivo_alvo.txt"

int procuraSubstr(char *string, char *substring) {
    int tamString = strlen(string); // debug
    printf("procurando: %s\ntamanho da string: %d\n", substring, tamString); //debug

    // Até o final da string
    for ( int i = 0; string[i] != '\0'; i++ ) {

        // Se é um possível começo
        if ( string[i] == substring[0] ) {
            int j;

            // Até o final da substring
            for ( j = 0; substring[j] != '\0'; j++ ) {

                // Se está diferente, j = 0 e para
                if ( string[i+j] != substring[j] ) {
                    j = 0;
                    break;
                }
            }

            // Se chegamos ao final da substring, achamos uma posição!
            if ( substring[j] == '\0' ) {
                printf("achei!\n");
                return i;
            }
        }

    }
    return -1;
}

int main(int argc, char **argv) {
    char textoArquivo[TAM_BUF];
    char *nomeArquivo = DEFAULT_FILENAME;
    char *stringProcurada;
    double inicio,fim; // variaveis para medir o tempo

    if ( argc < 2 ) {
        fprintf(stderr, "digite %s <substring> [arquivoEntrada]\n", argv[0]);
        printf("argc: %d\n", argc);
        return 1;
    } else if ( argc == 3 ) {
      nomeArquivo = argv[2];
    }

    stringProcurada = argv[1];

    FILE *fptr = fopen(nomeArquivo, "r");
    if ( fptr == NULL ) {
        printf("Erro na abertura!\n");
        exit(1);
    }
    fgets(textoArquivo, TAM_BUF, fptr);

    fclose(fptr);

    GET_TIME(inicio);
    int index = procuraSubstr(textoArquivo, stringProcurada);
    GET_TIME(fim);
    printf("indice: %d\n", index);
    printf("tempo da implementacao sequencial: %lf\n",fim-inicio);

    return 0;
}
