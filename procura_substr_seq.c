#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int procuraSubstr(char * string, char * substring) {
    int tamString = strlen(string);
    int tamSubstring = strlen(substring);
    printf("procurando %s\ntamanho da string: %d\n",substring,tamString); //debug
    for(int i=0;i<tamString;i++) {
        if(string[i]==substring[0]) {
            for(int j=0;j<tamSubstring;j++) {
                if(string[i+j]!=substring[j]) {
                    break;
                }
                if(j==tamSubstring-1) {
                    printf("achei!");
                    return i;
                }
            }
        }
    }
    return -1;
}


int main(int argc, char * argv[]) {
  char textoArquivo[100000];
  char stringProcurada[50];

  if(argc<2) {
    fprintf(stderr,"digite %s <substring>\n",argv[0]);
    return 1;
  }

  strcpy(stringProcurada,argv[1]);

  FILE *fptr;
  if((fptr = fopen("arquivo_alvo.txt","r")) == NULL) {
      printf("Erro na abertura!");   
      exit(1);             
  }
  fscanf(fptr,"%s", textoArquivo);

  int index = procuraSubstr(textoArquivo,stringProcurada);
  printf("%d",index);
}