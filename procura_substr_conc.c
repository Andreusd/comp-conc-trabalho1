// Andre Uziel Daniel Hashimoto

// comando para compilar -> gcc procura_substr_conc.c -o procura_substr_conc -lpthread

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "timer.h"

#define TAM_BUF             5000000
#define DEFAULT_FILENAME    "arquivo_alvo.txt"

typedef struct {
    long int id;
    long int offset;
    long int lenTexto;
    char *texto;
    long int indexLocal;
} arg_t;

char *stringProcurada;
int tamStringProcurada;

int procuraSubstr(char *texto, int fim, long int id) {
    // int tamStringProcurada = strlen(stringProcurada);
    // printf("procurando %s\ntamanho da string: %d\n",stringProcurada,fim-ini); //debug
    for ( int i = 0; i < fim ; i++ ) {

        // Ter certeza que não está errado
        if ( texto[i] == '\0' ) {
            printf("ERRO-- texto[i] == '\\0'\n");
        }

        // Se é um possível começo
        if ( texto[i] == stringProcurada[0] ) {
            int j;

            // Até o final da substring
            for ( j = 0; stringProcurada[j] != '\0' && texto[i+j] != '\0'; j++ ) {
                // Se está diferente, j = 0 e para
                if( texto[i+j] != stringProcurada[j] ) {
                    break;
                }
            }

            // Se chegamos ao final da substring, achamos uma posição!
            if ( stringProcurada[j] == '\0' ) {
                printf("id=%ld: achei! posicao local: %d\n", id, i);
                return i;
            }
        }
    }
    printf("id=%ld: nao achei :(\n", id);
    return -1;
}

void* tarefa (void *arg) {
    arg_t *a = (arg_t *)arg;
    long int id = a->id;
    long int lenTexto = a->lenTexto;
    char *texto = a->texto;

    /* Movido para a main
     * int tamTextoArquivo = strlen(textoArquivo);
     * long int id = (long int) arg; //identificador da thread
     * int *indexLocal; //indice da substring

     * indexLocal = (int *) malloc(sizeof(int));
     * if ( indexLocal == NULL ) {
     *     exit(1);
     * }
     * long int tamBloco = tamTextoArquivo/nthreads;  //tamanho do bloco de cada thread
     * long int ini = id * tamBloco; //elemento inicial do bloco da thread
     * long int fim; //elemento final(nao processado) do bloco da thread

     * if ( id == nthreads-1 )
     *     fim = tamTextoArquivo;
     * else
     *     fim = ini + tamBloco; //trata o resto se houver
     */

    a->indexLocal = procuraSubstr(texto, lenTexto, id);

    pthread_exit(NULL);
}

int main(int argc, char **argv) {

    int nthreads; //numero de threads

    char *nomeArquivo = DEFAULT_FILENAME;

    char textoArquivo[TAM_BUF];
    long int tamTextoArquivo;

    double inicio,fim; // variaveis para medir o tempo

    pthread_t *tid; //identificadores das threads no sistema
    arg_t *args; //identificadores das threads no sistema
    long int index = -1; //indice da substring

    if ( argc < 3 ) {
        fprintf(stderr, "digite %s <nthreads> <substring> [arquivoEntrada]\n", argv[0]);
        printf("argc: %d\n", argc);
        return 1;
    } else if ( argc == 4 ) {
      nomeArquivo = argv[3];
    }

    nthreads = atoll(argv[1]);

    stringProcurada = argv[2];
    tamStringProcurada = strlen(stringProcurada);

    FILE *fptr = fopen(nomeArquivo, "r");
    if ( fptr == NULL ) {
        printf("Erro na abertura!\n");
        exit(1);
    }
    fgets(textoArquivo, TAM_BUF, fptr);
    tamTextoArquivo = strlen(textoArquivo);

    fclose(fptr);

    GET_TIME(inicio);
    // cria os identificadores
    tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if ( tid == NULL ) {
        fprintf(stderr, "ERRO--malloc\n");
        return 2;
    }

    // cria os argumentos
    args = (arg_t *) malloc(sizeof(arg_t) * nthreads);
    if ( args == NULL ) {
        fprintf(stderr, "ERRO--malloc\n");
        return 2;
    }

    /* Divisão antiga
     * long int tamBloco = tamTextoArquivo/nthreads;  //tamanho do bloco de cada thread
     * long int ini = id * tamBloco; //elemento inicial do bloco da thread
     * long int fim; //elemento final(nao processado) do bloco da thread
     *
     * if ( id == nthreads-1 ) fim = tamTextoArquivo;
     * else fim = ini + tamBloco; //trata o resto se houver
     */

    long int offset = 0;
    long int tam = tamTextoArquivo / nthreads;
    long int rem = tamTextoArquivo % nthreads;

    // criar as threads
    for ( long int i = 0; i < nthreads; i++) {
        args[i].id = i;
        args[i].offset = offset;
        args[i].lenTexto = tam;
        args[i].texto = textoArquivo + offset;
        args[i].indexLocal = -1;

        if ( rem > 0 ) {
            args[i].lenTexto += 1;
            rem -= 1;
        }
        offset += args[i].lenTexto;

        // printf("id=%ld, lenTexto=%ld, texto=%ld, rem=%ld\n",
        //         args[i].id, args[i].lenTexto, args[i].texto, rem);
        if ( pthread_create(tid+i, NULL, tarefa, (void *) (args+i)) ) {
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
    }

    // aguardar o termino das threads
    for ( int i = 0; i < nthreads; i++ ) {
        if ( pthread_join(*(tid+i), NULL) ) {
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }

        long int indexLocal = args[i].indexLocal;
        if ( indexLocal != -1 ) {
            index = indexLocal + args[i].offset;
            break; // se uma thread retornar uma posicao valida, posso sair, pois elas estao em ordem
        }
    }
    GET_TIME(fim);
    if(index!=-1)
        printf("%ld\n", index);
    else {
        printf("nenhuma ocorrencia encontrada :(\n");
    }
    printf("tempo da implementacao concorrente: %lf\n",fim-inicio);

    return 0;
}
