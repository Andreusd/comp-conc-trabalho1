#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_BUF 100000

int procuraSubstr(char *string, char *substring) {
    int tamString = strlen(string);
    int tamSubstring = strlen(substring);
    printf("procurando: %s\ntamanho da string: %d\n", substring, tamString); //debug
    for ( int i = 0; i < tamString; i++ ) {

        if ( string[i] == substring[0] ) {
            for ( int j = 0; j < tamSubstring; j++ ) {
                if ( string[i+j] != substring[j] ) {
                    break;
                }
                if ( j == tamSubstring-1 ) {
                    printf("achei!\n");
                    return i;
                }
            }
        }

    }
    return -1;
}

int main(int argc, char **argv) {
  char textoArquivo[TAM_BUF];
  char *stringProcurada;

  if ( argc < 2 ) {
    fprintf(stderr, "digite %s <substring>\n", argv[0]);
    return 1;
  }

  stringProcurada = argv[1];

  FILE *fptr = fopen("arquivo_alvo.txt", "r");
  if ( fptr == NULL ) {
      printf("Erro na abertura!\n");   
      exit(1);             
  }
  fgets(textoArquivo, TAM_BUF, fptr);

  int index = procuraSubstr(textoArquivo, stringProcurada);
  printf("%d\n", index);
}
