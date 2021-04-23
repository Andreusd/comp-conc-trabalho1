#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_SEED    1
#define FILENAME        "wordlist.txt"
#define OUT_FILE        "shuffled"

// Usa os indexes embaralhados para listar as palavras (não as separa com \n)
void escreveSaida(FILE *fin, FILE *fout, unsigned long *v, unsigned long tam) {
    for ( unsigned long i = 0; i < tam; i++ ) {
        fseek(fin, v[i], SEEK_SET);

        int c;
        while ( (c = fgetc(fin)) != '\n' ) {
            if ( c == EOF ) break;

            fputc(c, fout);
        }
    }
}

// Gera um numero entre 0 e max
int randNum( int max ) {
    return rand() % max;
}

// Embaralha um vetor de números usando uma seed
void shuffle(unsigned int seed, unsigned long *v, unsigned long len) {
    srand(seed);

    for (int i = len-1; i > 0; i--) {
        int j = randNum( i );

        unsigned long temp = v[j];
        v[j] = v[i];
        v[i] = temp;
    }
}

// Faz um preprocessamento para saber onde (no arquivo) está a linha x
void indexaLinhas(FILE *fin, unsigned long *v, unsigned long tam) {
    fseek(fin, 0, SEEK_SET);

    v[0] = ftell(fin);

    for ( unsigned long i = 1; i < tam && !feof(fin); i++ ) {
        int c;
        while ( (c = fgetc(fin)) != '\n' ) {
            if ( c == EOF ) break;
        }
        v[i] = ftell(fin);

        if ( v[i] == -1L ) {
            printf("ERRO indexaLinhas: ftell = -1, i = %ld\n", i);
        }
    }
}

// Conta quantas linhas o arquivo tem (quantas palavras)
unsigned long contaLinhas(FILE *fin) {
    unsigned long count = 0;

    fseek(fin, 0, SEEK_SET);

    while ( !feof(fin) ) {
        int c = fgetc(fin);
        if ( c == '\n' ) {
            count += 1;
        }
    }
    return count;
}

int pot(int b, int n) {
  int acc = b;
  for(int i=0;i<n;i++) {
    acc = acc*b;
  }
  return acc;
}

int main(int argc, char **argv) {

    unsigned int seed = DEFAULT_SEED;

    // Ler seeds do usuário (se não tiver, usa o padrão)
    if ( argc > 1 ) {
        seed = atoll(argv[1]);
    }


    FILE *fin = fopen(FILENAME, "r");
    if ( !fin ) {
        printf("Não foi possível abrir para ler: %s\n", FILENAME);
        return 1;
    }

    unsigned long numLinhas = contaLinhas(fin);

    unsigned long *sequencia = malloc(numLinhas*sizeof(*sequencia));
    if ( !sequencia ) {
        printf("Sem memória: %s possui muitas entradas\n", FILENAME);
        fclose(fin);
        return 2;
    }

    indexaLinhas(fin, sequencia, numLinhas);

    shuffle(seed, sequencia, numLinhas);

    char fileName[20] = OUT_FILE;
    int tamFileName = strlen(fileName);

    printf("Gerando arquivo para teste...\n");
    fflush(stdout);
    for(int i=1;i<=7;i++) {
        fileName[tamFileName]=('0'+i);
        fileName[tamFileName+1]='.';
        fileName[tamFileName+2]='t';
        fileName[tamFileName+3]='x';
        fileName[tamFileName+4]='t';
        FILE *fout = fopen(fileName, "a+");
        if ( !fout ) {
            printf("Não foi possível abrir para escrever: %s\n", OUT_FILE);
            free(sequencia);
            fclose(fin);
            return 3;
        }
        for(int j=0;j<=pot(2,i);j++)
            escreveSaida(fin, fout, sequencia, numLinhas);
        fclose(fout);
    }


    free(sequencia);

    fclose(fin);
    return 0;
}
