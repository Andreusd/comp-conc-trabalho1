// Andre Uziel Daniel Hashimoto

// comando para compilar -> gcc procura_substr_conc.c -o procura_substr_conc -lpthread

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "timer.h"
#include "linked_array.c"

#define TAM_BUF             8000000
#define DEFAULT_FILENAME    "setup/entrada.txt"
#define F_STD_OUT           "setup/saida_conc.txt"

typedef struct {
    long int id;
    long int offset;
    long int lenTexto;
} arg_t;

char *texto;
char *substring;

linkedArray * procuraSubstr(long int offset, long int fim, long int id) {
    linkedArray * la = novo_linkedArray();
    for ( int i = offset; i < fim ; i++ ) {

        // Ter certeza que não está errado
        if ( texto[i] == '\0' ) {
            printf("ERRO-- texto[i] == '\\0'\n");
        }

        // Se é um possível começo
        if ( texto[i] == substring[0] ) {
            int j;

            // Até o final da substring
            for ( j = 0; substring[j] != '\0' && texto[i+j] != '\0'; j++ ) {
                // Se está diferente, j = 0 e para
                if( texto[i+j] != substring[j] ) {
                    break;
                }
            }

            // Se chegamos ao final da substring, achamos uma posição!
            if ( substring[j] == '\0' ) {
                // printf("id=%ld: achei! posicao local: %d\n", id, i); //DEBUG
                if ( push_linkedArray(la, i) < 0 ) {
                    fprintf(stderr, "ERRO GRAVE PUSH LINKEDARRAY %d THREAD %ld\n", i, id);
                }
            }
        }
    }
    return la;
}

void* tarefa (void *arg) {
    arg_t *a = (arg_t *)arg;
    long int id = a->id;
    long int offset = a->offset;
    long int lenTexto = a->lenTexto;

    linkedArray * la = procuraSubstr(offset, offset + lenTexto, id);

    pthread_exit( (void*) la );
}

int main(int argc, char **argv) {

    int nthreads; //numero de threads

    char *nomeArquivo = DEFAULT_FILENAME;

    char textoArquivo[TAM_BUF];
    long int tamTextoArquivo;

    long int tamSubstring;

    double inicio,fim; // variaveis para medir o tempo

    pthread_t *tid; //identificadores das threads no sistema
    arg_t *args; //identificadores das threads no sistema
    linkedArray **retorno; //retorno da funcao

    if ( argc < 3 ) {
        fprintf(stderr, "digite %s <nthreads> <substring> [arquivoEntrada]\n", argv[0]);
        printf("argc: %d\n", argc);
        return 1;
    } else if ( argc == 4 ) {
      nomeArquivo = argv[3];
    }

    nthreads = atoll(argv[1]);

    substring = argv[2];
    tamSubstring = strlen(substring);

    FILE *fptr = fopen(nomeArquivo, "r");
    if ( fptr == NULL ) {
        printf("Erro na abertura! renomeie o arquivo desejado em setup para entrada.txt ou passe-o como argumento!\n");
        exit(1);
    }
    fgets(textoArquivo, TAM_BUF, fptr);
    texto = textoArquivo;
    tamTextoArquivo = strlen(textoArquivo);

    fclose(fptr);

    GET_TIME(inicio);

    // cria os identificadores
    tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if ( tid == NULL ) {
        fprintf(stderr, "ERRO--malloc tid\n");
        return 2;
    }

    // cria os argumentos
    args = (arg_t *) malloc(sizeof(arg_t) * nthreads);
    if ( args == NULL ) {
        fprintf(stderr, "ERRO--malloc args\n");
        return 2;
    }

    //cria os retornos
    retorno = (linkedArray **) malloc(sizeof(linkedArray *) * nthreads);
    if ( retorno == NULL ) {
        fprintf(stderr, "ERRO--malloc retorno\n");
        return 2;
    }

    long int offset = 0;
    long int tam = (tamTextoArquivo - tamSubstring) / nthreads;
    long int rem = (tamTextoArquivo - tamSubstring) % nthreads;

    // criar as threads
    for ( long int i = 0; i < nthreads; i++ ) {
        args[i].id = i;
        args[i].offset = offset;
        args[i].lenTexto = tam;

        if ( rem > 0 ) {
            args[i].lenTexto += 1;
            rem -= 1;
        }
        offset += args[i].lenTexto;

        // printf("id=%ld, offset=%ld, lenTexto=%ld, rem=%ld\n",
        //         args[i].id, args[i].offset, args[i].lenTexto, rem);
        if ( pthread_create(tid+i, NULL, tarefa, (void *) (args+i)) ) {
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
    }

    // aguardar o termino das threads
    for ( int i = 0; i < nthreads; i++ ) {
        if ( pthread_join(*(tid+i), (void**) (retorno+i) ) ) {
            fprintf(stderr, "ERRO--pthread_join\n");
            return 3;
        }
    }
    free(args);
    free(tid);

    GET_TIME(fim);

    // Abrindo arquivo de saída
    FILE *fout = fopen(F_STD_OUT, "w");
    if ( !fout ) {
        fprintf(stderr, "ERRO--Não foi possível abrir %s\n", F_STD_OUT);
        return 4;
    }

    //IMPRIME OS VALORES
    fprintf(fout, "indices:\n");

    int total = 0;
    for ( int i = 0; i < nthreads; i++ ) {
        int pos = 0;
        linkedArray * la = retorno[i];
        while ( la ) {
            if ( pos%50 >= la->pos ) {
                break;
            }
            fprintf(fout, "%d: %d\n", i, get_linkedArray(la, pos%50) );
            pos++;
            if ( pos%50 == 0 ) {
                la = la->next;
            }
        }
        destroy_linkedArray(retorno[i]);
        total += pos;
    }

    free(retorno);
    fprintf(fout, "houve %d posicoes encontradas\n", total);

    fclose(fout);

    printf("tempo da implementacao concorrente: %lf\n", fim-inicio);
    return 0;
}
