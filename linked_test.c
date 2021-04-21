#include <stdio.h>
#include <stdlib.h>
#include "linked_array.c"

#define TAM 50000

int main() {
    linkedArray * la = novo_linkedArray();
    for(int i=0;i<TAM;i++) {
        push_linkedArray(la,i);
    }
    for(int i=0;i<TAM;i++) {
        if(i!=get_linkedArray(la,i))
            printf("%d - %d\n",i,get_linkedArray(la,i));
    }
}