typedef struct _linkedArray {
    int data[50];
    int pos;
    struct _linkedArray * next;
} linkedArray;

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

void destroy_linkedArray(linkedArray *la) {
    while ( la ) {
        linkedArray *next = la->next;
        free( la );
        la = next;
    }
}

int push_linkedArray(linkedArray *la, int n) {
    if ( !la ) {
        return -1; 
    }
    if ( la->pos >= 50 ) {
        if ( la->next == NULL ) {
            linkedArray * next = novo_linkedArray();
            la->next = next;
        }
        push_linkedArray( la->next, n );
    } else {
        (la->data)[la->pos++] = n;
    }
    return 0;
}

int get_linkedArray(linkedArray * la, int pos) {
    if ( !la ) {
        return -1; 
    }
    if ( pos >= 50 ) {
        if ( la->next == NULL ) {
            return -1;
        }
        return get_linkedArray( la->next, pos-50 );
    } else {
        return (la->data)[pos];
    }
}
