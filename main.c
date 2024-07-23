/* 5 Strutture dati:  Ingredienti - AVL con min-Heap
*                     Ricette - BST con liste
*                     Ordini da Fare - Coda FIFO
*                     Ordini fatti - min-Heap
*                     Camioncino - max-Heap
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Ingredienti - Heap
typedef struct IngredienteMinHeap {
    int scadenza;
    int quantita;
}  IngredienteMinHeap;

typedef struct {
    IngredienteMinHeap* lotto;
    int dimensione;
    int capacita;
} MinHeapIngrediente;

void heapify(MinHeapIngrediente* heap, int i);
void inserisciIngredienteHeap(MinHeapIngrediente* heap, int scadenza, int quantita);
void liberaMinHeap(MinHeapIngrediente* heap);
IngredienteMinHeap rimuoviIngredienteHeap(MinHeapIngrediente* heap);
MinHeapIngrediente creaHeapIngredienti(int capacita);

//Ingredienti - AVL
typedef struct NodoAVL {
    char nome[256];
    MinHeapIngrediente heap;
    struct NodoAVL *sinistro;
    struct NodoAVL *destro;
    int altezza;
} NodoAVL;



int main(void) {
    return 0;
}
