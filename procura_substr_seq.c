#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "timer.h"
#include "linked_array.c"

#define TAM_BUF             5000000
#define DEFAULT_FILENAME    "setup/entrada.txt"
#define F_STD_OUT           "setup/saida.txt"

linkedArray * procuraSubstr(char *string, char *substring) {
    linkedArray * la = novo_linkedArray();

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
                if ( push_linkedArray(la, i) < 0 ) {
                    fprintf(stderr,"ERRO GRAVE PUSH LINKEDARRAY %d\n",i);
                }
            }
        }

    }
    return la;
}

int main(int argc, char **argv) {
    char textoArquivo[TAM_BUF];
    char *nomeArquivo = DEFAULT_FILENAME;
    char *substring;
    double inicio,fim; // variaveis para medir o tempo

    // Lendo entrada
    if ( argc < 2 ) {
        fprintf(stderr, "digite %s <substring> [arquivoEntrada]\n", argv[0]);
        printf("argc: %d\n", argc);
        return 1;
    } else if ( argc == 3 ) {
        nomeArquivo = argv[2];
    }

    // Inicializando string e substring
    substring = argv[1];

    FILE *fptr = fopen(nomeArquivo, "r");
    if ( fptr == NULL ) {
        printf("Erro na abertura!\n");
        exit(1);
    }
    fgets(textoArquivo, TAM_BUF, fptr);

    fclose(fptr);

    // AQUI começa o trabalho

    GET_TIME(inicio);
    linkedArray * la = procuraSubstr(textoArquivo, substring);
    GET_TIME(fim);

    // AQUI termina o trabalho

    // Abrindo arquivo de saída
    FILE *fout = fopen(F_STD_OUT, "w");
    if ( !fout ) {
        fprintf(stderr, "ERRO--Não foi possível abrir %s\n", F_STD_OUT);
        return 4;
    }

    // Imprime os valores
    fprintf(fout, "indices:\n");

    int pos = 0;
    linkedArray * primeiro = la;
    while ( la ) {
        fprintf(fout, "%d\n", get_linkedArray(la, pos%50));
        pos++;
        if ( pos%50 >= la->pos ) {
            break;
        }
        if ( pos%50 == 0 ) {
            la = la->next;
        }
    }
    destroy_linkedArray(primeiro);
    fprintf(fout, "houve %d posicoes encontradas\n", pos);

    fclose(fout);

    printf("tempo da implementacao sequencial: %lf\n", fim-inicio);

    return 0;
}
