/**
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
    printf("Utilizzata quantita %d\n", quantita_usata);
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
    //preparazione("RicettaTest", 1);
    printf("Stato MinHeapIngrediente dopo la preparazione:\n");
    testHeapOrder(&nodo->heap);
    liberaAVL(avl);
    liberaBST(bst);
    free(ingrediente_ricetta);
}

void testAggiungiRimuoviIngredienti_AVL() {
    NodoAVL *avl = NULL;
    avl = inserisciAVL(avl, "Farina", 10, 5, 10);
    printf("Aggiunto Farina con scadenza 10 e quantita 5\n");
    avl = inserisciAVL(avl, "Farina", 5, 10, 10);
    printf("Aggiunto Farina con scadenza 5 e quantita 10\n");
    stampaAVL1(avl);
    IngredienteMinHeap rimosso = rimuoviIngrediente(&avl->heap);
    printf("Rimosso Farina con scadenza %d e quantita %d\n", rimosso.scadenza, rimosso.quantita);
    stampaAVL1(avl);
    rimosso = rimuoviIngrediente(&avl->heap);
    printf("Rimosso Farina con scadenza %d e quantita %d\n", rimosso.scadenza, rimosso.quantita);
    stampaAVL1(avl);
    avl = eliminaAVL(avl, "Farina");
    printf("Nodo Farina rimosso\n");
    if (avl == NULL) {
        printf("AVL e vuoto\n");
    } else {
        stampaAVL1(avl);
    }
}

void testDeallocazioneAVL() {
    avl = NULL;
    tempoCorrente = 0;
    avl = inserisciAVL(avl, "Ingrediente1", 3, 10, 10);
    avl = inserisciAVL(avl, "Ingrediente1", 6, 15, 10);
    printf("Stato iniziale AVL:\n");
    stampaAVL1(avl);
    tempoCorrente = 3;
    NodoAVL* nodo_ingrediente = cercaAVL(avl, "Ingrediente1");
    if (nodo_ingrediente != NULL) {
        controllaScadenza(nodo_ingrediente);
    }
    printf("\nStato AVL dopo aver impostato il tempo a 3:\n");
    stampaAVL1(avl);
    tempoCorrente = 6;
    nodo_ingrediente = cercaAVL(avl, "Ingrediente1");
    if (nodo_ingrediente != NULL) {
        controllaScadenza(nodo_ingrediente);
    }
    printf("\nStato AVL dopo aver impostato il tempo a 6:\n");
    if (avl != NULL) {
        stampaAVL1(avl);
    } else {
        printf("AVL e vuoto\n");
    }
}

void testMinHeapIngrediente() {
    testMinHeapInserimento();
    testMinHeapRimozione();
    testMinHeapOverflow();
    testMinHeapInit();
}

void testCodaOrdini() {
    CodaOrdini* coda = creaCoda();
    printf("Test Coda Ordini\n");
    aggiungiCoda(coda, "Ciambella", 6, 5);
    stampaCodaFIFO(coda);
    aggiungiCoda(coda, "Profiterole", 3, 6);
    stampaCodaFIFO(coda);
    aggiungiCoda(coda, "Ciambella", 3, 9);
    stampaCodaFIFO(coda);
    Ordine* corrente = coda->testa;
    liberaCoda(coda);
}

void verificaInserimento(NodoAVL* nodo, const char* nome) {
    NodoAVL* risultato = cercaAVL(nodo, nome);
    if (risultato != NULL) {
        printf("Verifica riuscita: Ingrediente '%s' è stato inserito correttamente.\n", nome);
    } else {
        printf("Verifica fallita: Ingrediente '%s' non è stato trovato.\n", nome);
    }
}

//Funzioni di stampa
void stampaHeapIngredienti1(MinHeapIngrediente* heap) {
    for (int i = 0; i < heap->dimensione; i++) {
        printf("  Ingrediente scadenza: %d, Quantita: %d\n", heap->lotto[i].scadenza, heap->lotto[i].quantita);
    }
}

void stampaHeapIngredienti2(MinHeapIngrediente* heap, int* count) {
    for (int i = 0; i < heap->dimensione; i++) {
        printf("  Ingrediente scadenza: %d, Quantita: %d\n", heap->lotto[i].scadenza, heap->lotto[i].quantita);
        (*count)++;
    }
}

void stampaAVL1(NodoAVL* nodo) {
    if (nodo == NULL) {
        return;
    }
    stampaAVL1(nodo->sinistro);
    printf("Ingrediente: %s\n", nodo->nome);
    if (nodo->heap.lotto != NULL) {
        stampaHeapIngredienti1(&nodo->heap);
    } else {
        printf("Heap degli ingredienti e vuoto o non allocato.\n");
    }
    stampaAVL1(nodo->destro);
}


// Funzione di supporto per stampare l'AVL e contare gli elementi
void stampaAVL2(NodoAVL* nodo, int* count) {
    if (nodo == NULL) {
        return;
    }
    stampaAVL2(nodo->sinistro, count);
    printf("Ingrediente: %s\n", nodo->nome);
    if (nodo->heap.lotto != NULL) {
        stampaHeapIngredienti2(&nodo->heap, count);
    } else {
        printf("Heap degli ingredienti e vuoto o non allocato.\n");
    }
    stampaAVL2(nodo->destro, count);
}

void stampaBST1(NodoBST* nodo) {
    if (nodo == NULL) {
        return;
    }
    stampaBST1(nodo->sinistro);
    printf("Ricetta: %s\n", nodo->ricetta.nome);
    IngredienteRicetta* ing = nodo->ricetta.ingredienti;
    while (ing) {
        printf("  Ingrediente: %s, Quantita: %d\n", ing->nome, ing->quantita);
        ing = ing->next;
    }
    stampaBST1(nodo->destro);
}

// Funzione di supporto per stampare il BST delle ricette e contare gli elementi
void stampaBST2(NodoBST* nodo, int* count) {
    if (nodo == NULL) {
        return;
    }
    stampaBST2(nodo->sinistro, count);
    printf("Ricetta: %s\n", nodo->ricetta.nome);
    int peso = 0;
    IngredienteRicetta* ing = nodo->ricetta.ingredienti;
    while (ing) {
        printf("  Ingrediente: %s, Quantita: %d\n", ing->nome, ing->quantita);
        peso += ing->quantita;
        ing = ing->next;
    }
    printf("Peso %s di %d\n", nodo->ricetta.nome, peso);
    (*count)++;
    stampaBST2(nodo->destro, count);
}

// Funzione di supporto per stampare la coda FIFO degli ordini e contare gli elementi
int stampaCodaFIFO(CodaOrdini* coda) {
    int count = 0;
    printf("Coda FIFO degli ordini:\n");
    Ordine* corrente = coda->testa;
    while (corrente) {
        printf("  Ordine: Tempo arrivo: %d, Ricetta: %s, Quantita: %d\n", corrente->tempo_arrivo, corrente->nome_ricetta, corrente->quantita);
        corrente = corrente->next;
        count++;
    }
    return count;
}

// Funzione di supporto per stampare il min-heap degli ordini fatti e contare gli elementi
void stampaMinHeapOrdini(MinHeap* heap) {
    printf("Min-heap degli ordini fatti (Dimensione: %d):\n", heap->dimensione);
    for (int i = 0; i < heap->dimensione; i++) {
        printf("  Ordine ricetta: %s, Quantita: %d, Tempo arrivo: %d\n", heap->ordini[i].ricetta, heap->ordini[i].quantita, heap->ordini[i].tempo_arrivo);
    }
}

// Funzione di supporto per stampare il max-heap delle spedizioni e contare gli elementi
void stampaMaxHeapSpedizioni(MaxHeapSpedizioni* heap) {
    printf("Max-heap delle spedizioni (Dimensione: %d):\n", heap->dimensione);
    for (int i = 0; i < heap->dimensione; i++) {
        printf("  Spedizione: %s, Quantita: %d, Peso: %d, Istante arrivo: %d\n", heap->spedizioni[i].nome, heap->spedizioni[i].quantita, heap->spedizioni[i].peso, heap->spedizioni[i].istante_arrivo);
    }
}

void print_string_info(const char* str) {
    printf("Stringa: '%s', Lunghezza: %zu\n", str, strlen(str));
}

// Funzione principale per stampare tutte le strutture e contare gli elementi
void stampaTutto() {
    printf("\nStato corrente delle strutture dati:\n");
    int count = 0;
    if (avl != NULL) {
        stampaAVL2(avl, &count);
        printf("Numero di ingredienti AVL: %d\n", count);
    } else {
        printf("AVL e vuoto.\n");
    }
    count = 0;
    if (bst != NULL) {
        stampaBST2(bst, &count);
        printf("Numero di ricette BST: %d\n", count);
    } else {
        printf("BST e vuoto.\n");
    }
    if (coda_ordini != NULL) {
        count = stampaCodaFIFO(coda_ordini);
        printf("Numero di ordini nella coda FIFO: %d\n", count);
    } else {
        printf("Coda degli ordini e vuota.\n");
    }
    if (heap_ordini_fatti != NULL) {
        stampaMinHeapOrdini(heap_ordini_fatti);
    } else {
        printf("Min-heap degli ordini fatti e vuoto.\n");
    }
    if (max_heap_spedizioni != NULL) {
        stampaMaxHeapSpedizioni(max_heap_spedizioni);
    } else {
        printf("Max-heap delle spedizioni e vuoto.\n");
    }
    printf("Istante di tempo corrente: %d\n", tempoCorrente);
}
*/