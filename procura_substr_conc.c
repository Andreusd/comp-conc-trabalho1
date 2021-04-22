// Andre Uziel Daniel Hashimoto

// comando para compilar -> gcc procura_substr_conc.c -o procura_substr_conc -lpthread

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "timer.h"
#include "linked_array.c"

#define TAM_BUF             5000000
#define DEFAULT_FILENAME    "arquivo_alvo.txt"

typedef struct {
    long int id;
    long int offset;
    long int lenTexto;
    char *texto;
} arg_t;

char *stringProcurada;
int tamStringProcurada;

linkedArray * procuraSubstr(char *texto, int fim, long int offset, long int id) {
    linkedArray * la = novo_linkedArray();
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
                // printf("id=%ld: achei! posicao local: %d\n", id, i);
                if(push_linkedArray(la,i+offset)<0) {
                    fprintf(stderr,"ERRO GRAVE PUSH LINKEDARRAY %d THREAD %ld\n",i,id);
                }
            }
        }
    }
    return la;
}

void* tarefa (void *arg) {
    arg_t *a = (arg_t *)arg;
    long int id = a->id;
    long int lenTexto = a->lenTexto;
    char *texto = a->texto;

    linkedArray * la = procuraSubstr(texto, lenTexto, a->offset, id);

    pthread_exit((void*)la);
}

int main(int argc, char **argv) {

    int nthreads; //numero de threads

    char *nomeArquivo = DEFAULT_FILENAME;

    char textoArquivo[TAM_BUF];
    long int tamTextoArquivo;

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
    long int tam = tamTextoArquivo / nthreads;
    long int rem = tamTextoArquivo % nthreads;

    // criar as threads
    for ( long int i = 0; i < nthreads; i++) {
        args[i].id = i;
        args[i].offset = offset;
        args[i].lenTexto = tam;
        args[i].texto = textoArquivo + offset;

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
        if ( pthread_join(*(tid+i), (void**) (retorno+i) ) ) {
            fprintf(stderr, "ERRO--pthread_join\n");
            return 3;
        }
    }
    free(args);
    free(tid);
    GET_TIME(fim);
    //IMPRIME OS VALORES
    int total = 0;
    for ( int i = 0; i < nthreads; i++ ) {
        int pos = 0;
        linkedArray * la = retorno[i];
        if(la->pos<=0) continue; // nao achou nada
        while(la) {
            printf("%d: %d\n",i,get_linkedArray(la,pos%50));
            pos++;
            if(pos%50 >= la->pos){
                break;
            }
            if(pos%50==0) {
                la = la->next;
            }
        }
        destroy_linkedArray(retorno[i]);
        total+=pos;
    }
    free(retorno);
    printf("houve %d posicoes encontradas\n",total);
    printf("tempo da implementacao concorrente: %lf\n",fim-inicio);
    return 0;
}
