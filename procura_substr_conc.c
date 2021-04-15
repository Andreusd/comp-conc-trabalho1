// Andre Uziel Daniel Hashimoto

// comando para compilar -> gcc procura_substr_conc.c -o procura_substr_conc -lpthread

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define TAM_BUF             100000
#define DEFAULT_FILENAME    "arquivo_alvo.txt"

typedef struct {
    long int id;
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
                printf("id=%ld: achei! %d\n", id, i);
                return i;
            }
        }
    }
    printf("id=%ld: nao achei :(\n", id);
    return -1;
}

void* tarefa (void *arg) {
    arg_t a = * (arg_t *)arg;
    long int id = a.id;
    long int lenTexto = a.lenTexto;
    char *texto = a.texto;

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

    a.indexLocal = procuraSubstr(texto, lenTexto, id);

    pthread_exit(NULL);
}

int main(int argc, char **argv) {

    int nthreads; //numero de threads

    char textoArquivo[TAM_BUF];
    long int tamTextoArquivo;

    // double inicio,fim; // variaveis para medir o tempo
    pthread_t *tid; //identificadores das threads no sistema
    arg_t *args; //identificadores das threads no sistema
    long int index; //indice da substring

    if ( argc < 3 ) {
        fprintf(stderr, "digite %s <substring> <nthreads>\n", argv[0]);
        return 1;
    }
    printf("argc: %d\n", argc); //debug

    stringProcurada = argv[1];
    tamStringProcurada = strlen(stringProcurada);
    nthreads = atoll(argv[2]);

    FILE *fptr = fopen(DEFAULT_FILENAME, "r");
    if ( fptr == NULL ) {
        printf("Erro na abertura!\n");
        exit(1);
    }
    fscanf(fptr,"%s", textoArquivo);
    tamTextoArquivo = strlen(textoArquivo);

    fclose(fptr);

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
    index = 0xFFFFFFFFFFFFFFFF;
    for ( int i = 0; i < nthreads; i++ ) {
        if ( pthread_join(*(tid+i), NULL) ) {
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }

        long int result = args[i].indexLocal;
        if ( result != -1 && index < result ) {
            index = result;
        }
    }

    printf("%ld\n", index);

    return 0;
}
