typedef struct _linkedArray {
    int data[50];
    int pos;
    struct _linkedArray * next;
} linkedArray;

// Alloca um novo linkedArray
linkedArray* novo_linkedArray() {
    linkedArray *la;
    la = (linkedArray *) malloc( sizeof( linkedArray ) );
    if ( !la ) {
        fprintf(stderr,"ERRO MALLOC LINKEDARRAY\n");
        exit(40);
    }
    la->pos = 0;
    la->next = NULL;
    return la;
}

// Desalloca um linkedArray e seus `próximos`
void destroy_linkedArray(linkedArray *la) {
    while ( la ) {
        linkedArray *next = la->next;
        free( la );
        la = next;
    }
}

// Insere um valor num linkedArray
int push_linkedArray(linkedArray *la, int n) {
    // Testando se linkedArray existe
    if ( !la ) {
        return -1; 
    }
    // Se não cabe...
    if ( la->pos >= 50 ) {
        // Cria um próximo, se ele não existe
        if ( la->next == NULL ) {
            linkedArray * next = novo_linkedArray();
            la->next = next;
        }
        // Pede para o próximo guardar
        push_linkedArray( la->next, n );
    } else {
        // Aqui cabe, só precisa inserir
        (la->data)[la->pos++] = n;
    }
    return 0;
}

// Pega um valor, similar a array[pos]
int get_linkedArray(linkedArray * la, int pos) {
    // Testando se linkedArray existe
    if ( !la ) {
        return -1; 
    }
    // Se está no próximo...
    if ( pos >= 50 ) {
        // Testando se existe próximo
        if ( la->next == NULL ) {
            return -1;
        }
        // Pede para o próximo
        return get_linkedArray( la->next, pos-50 );
    } else {
        // Está aqui, só retorna
        return (la->data)[pos];
    }
}
