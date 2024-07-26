#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Funzione per verificare la correttezza del MinHeap
bool verificaMinHeap(MinHeapIngrediente* heap) {
    for (int i = 0; i <= (heap->dimensione - 2) / 2; i++) {
        if (heap->lotto[i].scadenza > heap->lotto[2 * i + 1].scadenza) {
            return false;
        }
        if (2 * i + 2 < heap->dimensione && heap->lotto[i].scadenza > heap->lotto[2 * i + 2].scadenza) {
            return false;
        }
    }
    return true;
}

// Funzione per stampare lo stato del MinHeap
void testHeapOrder(MinHeapIngrediente* heap) {
    if (verificaMinHeap(heap)) {
        printf("L'heap e un min-heap valido.\n");
    } else {
        printf("L'heap non e un min-heap valido.\n");
    }
    for (int i = 0; i < heap->dimensione; i++) {
        printf("Scadenza: %d, Quantita: %d\n", heap->lotto[i].scadenza, heap->lotto[i].quantita);
    }
}

// Funzioni di Test Migliorate
void testMinHeapInserimento() {
    printf("\n--- Test Inserimento ---\n");
    MinHeapIngrediente heap = nuovoHeapIngredienti(2);
    NodoAVL* nodo = NULL;
    printf("Inserimento elemento con scadenza 100, quantita 10\n");
    inserisciIngrediente(&heap, 100, 10, nodo);
    printf("Inserimento elemento con scadenza 99, quantita 11\n");
    inserisciIngrediente(&heap, 99, 11, nodo);
    printf("Inserimento elemento con scadenza 4, quantita 12\n");
    inserisciIngrediente(&heap, 4, 12, nodo);
    printf("Stato MinHeapIngrediente dopo inserimenti:\n");
    testHeapOrder(&heap);
    liberaLotto(&heap);
}

void testMinHeapRimozione() {
    printf("\n--- Test Rimozione ---\n");
    MinHeapIngrediente heap = nuovoHeapIngredienti(2);
    NodoAVL* nodo = NULL;
    printf("Inserimento elemento con scadenza 100, quantita 10\n");
    inserisciIngrediente(&heap, 100, 10, nodo);
    printf("Inserimento elemento con scadenza 99, quantita 11\n");
    inserisciIngrediente(&heap, 99, 11, nodo);
    printf("Inserimento elemento con scadenza 4, quantita 12\n");
    inserisciIngrediente(&heap, 4, 12, nodo);
    printf("Stato MinHeapIngrediente prima della rimozione:\n");
    testHeapOrder(&heap);
    IngredienteMinHeap rimosso = rimuoviIngrediente(&heap);
    printf("Rimosso ingrediente: scadenza = %d, quantita = %d\n", rimosso.scadenza, rimosso.quantita);
    printf("Stato MinHeapIngrediente dopo rimozione:\n");
    testHeapOrder(&heap);
    liberaLotto(&heap);
}

void testMinHeapOverflow() {
    printf("\n--- Test Overflow ---\n");
    MinHeapIngrediente heap = nuovoHeapIngredienti(2);
    NodoAVL* nodo = NULL;
    for (int i = 0; i < 10; i++) {
        printf("Inserimento elemento con scadenza %d, quantita 10\n", i + 1);
        inserisciIngrediente(&heap, i + 1, 10, nodo);
    }
    printf("Stato MinHeapIngrediente dopo multipli inserimenti (overflow):\n");
    testHeapOrder(&heap);
    liberaLotto(&heap);
}

void testMinHeapInit() {
    printf("\n--- Test Inizializzazione ---\n");
    MinHeapIngrediente heap = nuovoHeapIngredienti(10);
    if (heap.dimensione == 0 && heap.capacita == 10) {
        printf("Inizializzazione corretta.\n");
    } else {
        printf("Errore di inizializzazione.\n");
    }
    liberaLotto(&heap);
}

void testUsoParzialeIngrediente() {
    printf("\n--- Test Uso Parziale Ingrediente ---\n");
    MinHeapIngrediente heap = nuovoHeapIngredienti(3);
    NodoAVL* nodo = NULL;
    printf("Inserimento elemento con scadenza 100, quantita 10\n");
    inserisciIngrediente(&heap, 100, 10, nodo);
    printf("Stato dopo inserimento:\n");
    testHeapOrder(&heap);
    printf("Inserimento elemento con scadenza 99, quantita 11\n");
    inserisciIngrediente(&heap, 99, 11, nodo);
    printf("Stato dopo inserimento:\n");
    testHeapOrder(&heap);
    printf("Inserimento elemento con scadenza 4, quantita 12\n");
    inserisciIngrediente(&heap, 4, 12, nodo);
    printf("Stato dopo inserimento:\n");
    testHeapOrder(&heap);
    printf("Stato MinHeapIngrediente dopo inserimenti:\n");
    testHeapOrder(&heap);
    printf("Uso di parte dell'ingrediente con scadenza 4\n");
    IngredienteMinHeap rimosso = rimuoviIngrediente(&heap);
    printf("Rimosso ingrediente: scadenza = %d, quantita = %d\n", rimosso.scadenza, rimosso.quantita);
    int quantita_usata = 12; // Usare tutto l'ingrediente con scadenza 4
    printf("Utilizzata quantità %d\n", quantita_usata);
    printf("Stato MinHeapIngrediente dopo rimozione dell'ingrediente con scadenza 4:\n");
    testHeapOrder(&heap);
    quantita_usata = 6;
    printf("Uso di parte dell'ingrediente con scadenza 99\n");
    rimosso = rimuoviIngrediente(&heap);
    printf("Rimosso ingrediente: scadenza = %d, quantita = %d\n", rimosso.scadenza, rimosso.quantita);
    rimosso.quantita -= quantita_usata;
    printf("Rimanente ingrediente con scadenza %d: quantita %d\n", rimosso.scadenza, rimosso.quantita);
    if (rimosso.quantita > 0) {
        inserisciIngrediente(&heap, rimosso.scadenza, rimosso.quantita, nodo);
    }
    printf("Stato MinHeapIngrediente dopo reinserimento dell'ingrediente con scadenza 99:\n");
    testHeapOrder(&heap);
    printf("Stato MinHeapIngrediente dopo uso parziale:\n");
    testHeapOrder(&heap);
    liberaLotto(&heap);
}

void testRifornimentoRicetta() {
    printf("\n--- Test Rifornimento e Preparazione Ricetta ---\n");
    NodoAVL* nodo = nuovoAVL("IngredienteTest", 10);
    avl = nodo;
    printf("Rifornimento con scadenza 100, quantita 10\n");
    inserisciIngrediente(&nodo->heap, 100, 10, nodo);
    printf("Stato dopo rifornimento:\n");
    testHeapOrder(&nodo->heap);
    printf("Rifornimento con scadenza 99, quantita 10\n");
    inserisciIngrediente(&nodo->heap, 99, 10, nodo);
    printf("Stato dopo rifornimento:\n");
    testHeapOrder(&nodo->heap);
    printf("Rifornimento con scadenza 4, quantita 10\n");
    inserisciIngrediente(&nodo->heap, 4, 10, nodo);
    printf("Stato dopo rifornimento:\n");
    testHeapOrder(&nodo->heap);
    printf("Aggiunta ricetta 'RicettaTest' con 15 di 'IngredienteTest'\n");
    Ricetta nuova_ricetta;
    strcpy(nuova_ricetta.nome, "RicettaTest");
    IngredienteRicetta* ingrediente_ricetta = (IngredienteRicetta*)malloc(sizeof(IngredienteRicetta));
    strcpy(ingrediente_ricetta->nome, "IngredienteTest");
    ingrediente_ricetta->quantita = 15;
    ingrediente_ricetta->next = NULL;
    nuova_ricetta.ingredienti = ingrediente_ricetta;
    bst = inserisciBST(bst, nuova_ricetta);
    printf("Ricetta aggiunta\n");
    printf("Preparazione della ricetta 'RicettaTest'\n");
    printf("Stato MinHeapIngrediente prima della preparazione:\n");
    testHeapOrder(&nodo->heap);
    preparazione("RicettaTest", 1);
    printf("Stato MinHeapIngrediente dopo la preparazione:\n");
    testHeapOrder(&nodo->heap);
    liberaAVL(avl);
    liberaBST(bst);
    free(ingrediente_ricetta);
}

void testMinHeapIngrediente() {
    testMinHeapInserimento();
    testMinHeapRimozione();
    testMinHeapOverflow();
    testMinHeapInit();
}

// Funzione di Stampa
void stampaHeapIngredienti(MinHeapIngrediente* heap) {
    printf("Heap degli ingredienti:\n");
    for (int i = 0; i < heap->dimensione; i++) {
        printf("  Ingrediente scadenza: %d, Quantita: %d\n", heap->lotto[i].scadenza, heap->lotto[i].quantita);
    }
}

// Funzione di supporto per stampare l'AVL
void stampaAVL(NodoAVL* nodo) {
    if (nodo == NULL)
        return;
    stampaAVL(nodo->sinistro);
    printf("Ingrediente: %s, Peso totale: %d\n", nodo->nome, nodo->peso_totale);
    stampaAVL(nodo->destro);
}

// Funzione di supporto per stampare il BST delle ricette
void stampaBST(NodoBST* nodo) {
    if (nodo == NULL)
        return;
    stampaBST(nodo->sinistro);
    printf("Ricetta: %s\n", nodo->ricetta.nome);
    IngredienteRicetta* ing = nodo->ricetta.ingredienti;
    while (ing) {
        printf("  Ingrediente: %s, Quantita: %d\n", ing->nome, ing->quantita);
        ing = ing->next;
    }
    stampaBST(nodo->destro);
}

// Funzione di supporto per stampare la coda FIFO degli ordini
void stampaCodaFIFO(CodaOrdini* coda) {
    printf("Coda FIFO degli ordini:\n");
    Ordine* corrente = coda->testa;
    while (corrente) {
        printf("  Ordine ricetta: %s, Quantita: %d, Tempo arrivo: %d\n", corrente->nome_ricetta, corrente->quantita, corrente->tempo_arrivo);
        corrente = corrente->next;
    }
}

// Funzione di supporto per stampare il min-heap degli ordini fatti
void stampaMinHeapOrdini(MinHeap* heap) {
    printf("Min-heap degli ordini fatti:\n");
    for (int i = 0; i < heap->dimensione; i++) {
        printf("  Ordine ricetta: %s, Quantita: %d, Tempo arrivo: %d\n", heap->ordini[i].ricetta, heap->ordini[i].quantita, heap->ordini[i].tempo_arrivo);
    }
}

// Funzione di supporto per stampare il max-heap delle spedizioni
void stampaMaxHeapSpedizioni(MaxHeapSpedizioni* heap) {
    printf("Max-heap delle spedizioni:\n");
    for (int i = 0; i < heap->dimensione; i++) {
        printf("  Spedizione: %s, Quantita: %d, Peso: %d, Istante arrivo: %d\n", heap->spedizioni[i].nome, heap->spedizioni[i].quantita, heap->spedizioni[i].peso, heap->spedizioni[i].istante_arrivo);
    }
}

// Funzione principale per stampare tutte le strutture
void stampaTutto() {
    printf("Stato corrente delle strutture dati:\n");
    stampaAVL(avl);  // Stampa l'AVL
    stampaHeapIngredienti(&avl->heap);  // Assumendo che avl non sia NULL per la demo
    stampaBST(bst);  // Stampa il BST delle ricette
    stampaCodaFIFO(coda_ordini);  // Stampa la coda FIFO degli ordini
    stampaMinHeapOrdini(heap_ordini_fatti);  // Stampa il min-heap degli ordini fatti
    stampaMaxHeapSpedizioni(max_heap_spedizioni);  // Stampa il max-heap delle spedizioni
    printf("Istante di tempo corrente: %d\n", tempoCorrente);
}