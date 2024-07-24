#include "header.h"
#include <stdio.h>

//Funzioni di Test
void testMinHeapIngrediente() {
    MinHeapIngrediente heap = nuovoHeapIngredienti(10);
    inserisciIngrediente(&heap, 5, 10);
    inserisciIngrediente(&heap, 3, 20);
    inserisciIngrediente(&heap, 8, 15);
    printf("Stato MinHeapIngrediente dopo inserimenti:\n");
    for (int i = 0; i < heap.dimensione; i++) {
        printf("Ingrediente %d: scadenza = %d, quantita = %d\n", i, heap.lotto[i].scadenza, heap.lotto[i].quantita);
    }
    IngredienteMinHeap ing = rimuoviIngrediente(&heap);
    printf("Rimosso ingrediente: scadenza = %d, quantita = %d\n", ing.scadenza, ing.quantita);
    printf("Stato MinHeapIngrediente dopo rimozione:\n");
    for (int i = 0; i < heap.dimensione; i++) {
        printf("Ingrediente %d: scadenza = %d, quantita = %d\n", i, heap.lotto[i].scadenza, heap.lotto[i].quantita);
    }
    liberaLotto(&heap);
}

void testNodoAVL() {
    NodoAVL* avl = NULL;
    avl = inserisciAVL(avl, "Pomodoro", 5, 10, 10);
    avl = inserisciAVL(avl, "Pomodoro", 3, 20, 10);
    avl = inserisciAVL(avl, "Carota", 8, 15, 10);
    avl = inserisciAVL(avl, "Zucchina", 2, 5, 10);
    printf("Stato AVL dopo inserimenti:\n");
    printf("Root: %s, Altezza: %d\n", avl->nome, avl->altezza);
    NodoAVL* nodo = avl;
    while (nodo != NULL) {
        printf("%s: altezza = %d\n", nodo->nome, nodo->altezza);
        nodo = nodo->sinistro;  // esplora i nodi a sinistra
    }
    avl = eliminaAVL(avl, "Pomodoro");
    printf("Stato AVL dopo eliminazione di Pomodoro:\n");
    if (avl != NULL)
        printf("Root: %s, Altezza: %d\n", avl->nome, avl->altezza);
    liberaAVL(avl);
}

void testNodoBST() {
    NodoBST* bst = NULL;
    Ricetta ricetta1 = {"Pasta", NULL};
    Ricetta ricetta2 = {"Pizza", NULL};
    Ricetta ricetta3 = {"Insalata", NULL};
    bst = inserisciBST(bst, ricetta1);
    bst = inserisciBST(bst, ricetta2);
    bst = inserisciBST(bst, ricetta3);
    printf("Stato BST dopo inserimenti:\n");
    NodoBST* nodo = bst;
    while (nodo != NULL) {
        printf("Ricetta: %s\n", nodo->ricetta.nome);
        nodo = nodo->sinistro;  // esplora i nodi a sinistra
    }
    nodo = cercaBST(bst, "Pasta");
    if (nodo != NULL)
        printf("Trovata ricetta: %s\n", nodo->ricetta.nome);
    bst = eliminaBST(bst, "Pasta");
    printf("Stato BST dopo eliminazione di Pasta:\n");
    if (bst != NULL)
        printf("Root: %s\n", bst->ricetta.nome);

    liberaBST(bst);
}

void testCodaOrdini() {
    CodaOrdini* coda = creaCoda();
    aggiungiCoda(coda, "Pizza", 2, 1);
    aggiungiCoda(coda, "Pasta", 3, 2);
    aggiungiCoda(coda, "Insalata", 1, 3);
    printf("Stato CodaOrdini dopo inserimenti:\n");
    Ordine* temp = coda->testa;
    while (temp != NULL) {
        printf("Ordine: ricetta = %s, quantita = %d, tempo_arrivo = %d\n", temp->nome_ricetta, temp->quantita, temp->tempo_arrivo);
        temp = temp->next;
    }
    Ordine* ordine = rimuoviCoda(coda);
    printf("Rimosso ordine: ricetta = %s, quantita = %d\n", ordine->nome_ricetta, ordine->quantita);
    printf("Stato CodaOrdini dopo rimozione:\n");
    temp = coda->testa;
    while (temp != NULL) {
        printf("Ordine: ricetta = %s, quantita = %d, tempo_arrivo = %d\n", temp->nome_ricetta, temp->quantita, temp->tempo_arrivo);
        temp = temp->next;
    }
    liberaCoda(coda);
}

void testMinHeapOrdini() {
        MinHeap* heap = creaMinHeap(10);
        inserisciOrdineHeap(heap, 1, "Pizza", 2);
        inserisciOrdineHeap(heap, 3, "Pasta", 3);
        inserisciOrdineHeap(heap, 2, "Insalata", 1);
        printf("Stato MinHeap dopo inserimenti:\n");
        for (int i = 0; i < heap->dimensione; i++) {
            printf("Ordine %d: tempo_arrivo = %d, ricetta = %s, quantita = %d\n", i, heap->ordini[i].tempo_arrivo, heap->ordini[i].ricetta, heap->ordini[i].quantita);
        }
        OrdineHeap ordine = rimuoviMin(heap);
        printf("Rimosso ordine: tempo_arrivo = %d, ricetta = %s, quantita = %d\n", ordine.tempo_arrivo, ordine.ricetta, ordine.quantita);
        printf("Stato MinHeap dopo rimozione:\n");
        for (int i = 0; i < heap->dimensione; i++) {
            printf("Ordine %d: tempo_arrivo = %d, ricetta = %s, quantita = %d\n", i, heap->ordini[i].tempo_arrivo, heap->ordini[i].ricetta, heap->ordini[i].quantita);
        }
        liberaMinHeapOrdini(heap);
}

void testMaxHeapSpedizioni() {
    MaxHeapSpedizioni heap = creaMaxHeap(10);
    inserisciSpedizione(&heap, "Pizza", 1, 2, 5);
    inserisciSpedizione(&heap, "Pasta", 3, 3, 3);
    inserisciSpedizione(&heap, "Insalata", 2, 1, 7);
    printf("Stato MaxHeapSpedizioni dopo inserimenti:\n");
    for (int i = 0; i < heap.dimensione; i++) {
        printf("Spedizione %d: nome = %s, peso = %d\n", i, heap.spedizioni[i].nome, heap.spedizioni[i].peso);
    }
    Spedizione spedizione = rimuoviMax(&heap);
    printf("Rimossa spedizione: nome = %s, peso = %d\n", spedizione.nome, spedizione.peso);
    printf("Stato MaxHeapSpedizioni dopo rimozione:\n");
    for (int i = 0; i < heap.dimensione; i++) {
        printf("Spedizione %d: nome = %s, peso = %d\n", i, heap.spedizioni[i].nome, heap.spedizioni[i].peso);
    }
    liberaMaxHeap(&heap);
}

void testGenerale() {
    printf("Test MinHeapIngrediente:\n");
    testMinHeapIngrediente();
    printf("\nTest NodoAVL:\n");
    testNodoAVL();
    printf("\nTest NodoBST:\n");
    testNodoBST();
    printf("\nTest CodaOrdini:\n");
    testCodaOrdini();
    printf("\nTest MinHeapOrdini:\n");
    testMinHeapOrdini();
    printf("\nTest MaxHeapSpedizioni:\n");
    testMaxHeapSpedizioni();
}
