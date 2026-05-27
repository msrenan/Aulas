#include <stdio.h>
#include "estruturas/hashtable.c"

int main() {
    
    init(10);
    printf("HashTable initialized.\n");
    char* k = malloc(10 * sizeof(char));
    k = "teste";
    printf("Inserting key teste.\n");
    insert(k, 10);
    printf("Inserted value 10.\n");
    k = "bro";
    printf("Inserted key bro.\n");
    insert(k, 0);
    printf("Inserted value 0.\n");

    printf("Insertion done!\n");

    printf("teste -> %d\n", search("teste"));
    printf("bro -> %d\n", search("bro"));

    
}