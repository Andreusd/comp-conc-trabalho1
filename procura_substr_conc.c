// Andre Uziel Daniel Hashimoto

// comando para compilar -> gcc procura_substr_conc.c -o procura_substr_conc -lpthread

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int nthreads; //numero de threads

char textoArquivo[100000];
char stringProcurada[50];

int procuraSubstr(int ini,int fim) {
  int tamStringProcurada = strlen(stringProcurada);
  //printf("procurando %s\ntamanho da string: %d\n",stringProcurada,fim-ini); //debug
  for(int i=ini;i<fim;i++) {
      if(textoArquivo[i]==stringProcurada[0]) {
          for(int j=0;j<tamStringProcurada;j++) {
              if(textoArquivo[i+j]!=stringProcurada[j]) {
                  break;
              }
              if(j==tamStringProcurada-1) {
                  printf("\nachei! %d\n",i);
                  return i;
              }
          }
      }
  }
  printf("\nnao achei :( \n");
  return -1;
}

void * tarefa (void * arg) {
  int tamTextoArquivo = strlen(textoArquivo);
  long int id = (long int) arg; //identificador da thread
  int * indexLocal; //indice da substring
  indexLocal = (int *) malloc(sizeof(int));
  if(indexLocal==NULL) {exit(1);}
  long int tamBloco = tamTextoArquivo/nthreads;  //tamanho do bloco de cada thread 
  long int ini = id * tamBloco; //elemento inicial do bloco da thread
  long int fim; //elemento final(nao processado) do bloco da thread
  if(id == nthreads-1) fim = tamTextoArquivo;
  else fim = ini + tamBloco; //trata o resto se houver

  *indexLocal = procuraSubstr(ini,fim);

  pthread_exit((void *) indexLocal); 
}

int main(int argc, char * argv[]) {
  if(argc<3) {
    fprintf(stderr,"digite %s <substring> <nthreads>\n",argv[0]);
    return 1;
  }
  double inicio,fim; // variaveis para medir o tempo
  pthread_t *tid; //identificadores das threads no sistema
  double *retorno; //valor de retorno das threads
  int index; //indice da substring

  strcpy(stringProcurada,argv[1]);
  nthreads = atoll(argv[2]);


  FILE *fptr;
  if((fptr = fopen("arquivo_alvo.txt","r")) == NULL) {
      printf("Erro na abertura!");   
      exit(1);             
  }
  fscanf(fptr,"%s", textoArquivo);

//cria os identificadores
  tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
  if(tid==NULL) {
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
  }
  //criar as threads
  for(long int i=0; i<nthreads; i++) {
      if(pthread_create(tid+i, NULL, tarefa, (void*) i)){
          fprintf(stderr, "ERRO--pthread_create\n");
          return 3;
      }
  }
  //aguardar o termino das threads
  for(long int i=0; i<nthreads; i++) {
      if(pthread_join(*(tid+i), (void**) &retorno)){
          fprintf(stderr, "ERRO--pthread_create\n");
          return 3;
      }
    if(*retorno != -1) {
      index=*retorno;
    }
  }

  printf("%d",index);
}