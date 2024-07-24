#include "header.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

//Funzioni di stampa
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

void stampaAVL(NodoAVL* nodo) {
    if (nodo != NULL) {
        stampaAVL(nodo->sinistro);
        printf("Ingrediente: %s\n", nodo->nome);
        stampaAVL(nodo->destro);
    }
}

//Funzioni di Test
void testMinHeapIngrediente() {
    MinHeapIngrediente heap = nuovoHeapIngredienti(2);
    inserisciIngrediente(&heap, 100, 10);
    inserisciIngrediente(&heap, 99, 11);
    inserisciIngrediente(&heap, 4, 12);
    printf("Stato MinHeapIngrediente dopo inserimenti:\n");
    IngredienteMinHeap rimosso = rimuoviIngrediente(&heap);
    printf("Rimosso ingrediente: scadenza = %d, quantita = %d\n", rimosso.scadenza, rimosso.quantita);
    printf("Stato MinHeapIngrediente dopo rimozione:\n");
    liberaLotto(&heap);
}

bool verificaMinHeap(MinHeapIngrediente* heap) {
    for (int i = 0; i <= (heap->dimensione - 2) / 2; i++) {
        // Verifica se il nodo genitore è maggiore del figlio sinistro
        if (heap->lotto[i].scadenza > heap->lotto[2 * i + 1].scadenza) {
            return false;
        }
        // Verifica se il nodo genitore è maggiore del figlio destro (se esiste)
        if (2 * i + 2 < heap->dimensione && heap->lotto[i].scadenza > heap->lotto[2 * i + 2].scadenza) {
            return false;
        }
    }
    return true;
}

void testNodoAVL() {
    NodoAVL* avl = NULL;
    avl = inserisciAVL(avl, "Pomodoro", 5, 10, 10);
    avl = inserisciAVL(avl, "Pomodoro", 3, 20, 10);
    avl = inserisciAVL(avl, "Carota", 8, 15, 10);
    avl = inserisciAVL(avl, "Zucchina", 2, 5, 10);
    printf("Stato AVL dopo inserimenti:\n");
    stampaAVL(avl);
    avl = eliminaAVL(avl, "Pomodoro");
    printf("Stato AVL dopo eliminazione di Pomodoro:\n");
    stampaAVL(avl);
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
    MaxHeapSpedizioni* heap = creaMaxHeap(10);
    inserisciSpedizione(heap, "Pizza", 1, 2, 5);
    inserisciSpedizione(heap, "Pasta", 3, 3, 3);
    inserisciSpedizione(heap, "Insalata", 2, 1, 7);
    printf("Stato MaxHeapSpedizioni dopo inserimenti:\n");
    for (int i = 0; i < heap->dimensione; i++) {
        printf("Spedizione %d: nome = %s, peso = %d\n", i, heap->spedizioni[i].nome, heap->spedizioni[i].peso);
    }
    Spedizione spedizione = rimuoviMax(heap);
    printf("Rimossa spedizione: nome = %s, peso = %d\n", spedizione.nome, spedizione.peso);
    printf("Stato MaxHeapSpedizioni dopo rimozione:\n");
    for (int i = 0; i < heap->dimensione; i++) {
        printf("Spedizione %d: nome = %s, peso = %d\n", i, heap->spedizioni[i].nome, heap->spedizioni[i].peso);
    }
    liberaMaxHeap(heap);
}

void testGestisciComandi() {
    const char *input = "aggiungi_ricetta Pasta Pomodoro 2\n"
                        "rimuovi_ricetta Pasta\n"
                        "rifornimento Pomodoro 100 10\n"
                        "ordine Pizza 5\n";
    FILE *inputFile = fopen("test_input.txt", "w");
    fprintf(inputFile, "%s", input);
    fclose(inputFile);
    freopen("test_input.txt", "r", stdin);
    printf("Eseguo gestisciComandi:\n");
    gestisciComandi(inputFile);
    freopen("/dev/tty", "w", stdout);
    freopen("/dev/tty", "r", stdin);
    remove("test_input.txt");
}

void testAggiungiRicetta() {
    // Creazione di una nuova ricetta
    Ricetta ricetta1;
    strcpy(ricetta1.nome, "Pasta");
    IngredienteRicetta* ingrediente1 = (IngredienteRicetta*)malloc(sizeof(IngredienteRicetta));
    if (ingrediente1 == NULL) {
        printf("Errore di allocazione memoria per ingrediente1\n");
        return;
    }
    strcpy(ingrediente1->nome, "Pomodoro");
    ingrediente1->quantita = 2;
    ingrediente1->next = NULL;
    ricetta1.ingredienti = ingrediente1;

    // Aggiunta della ricetta al BST (BST è vuoto)
    printf("Aggiungo la ricetta 'Pasta'\n");
    aggiungi_ricetta(ricetta1);

    // Tentativo di aggiungere la stessa ricetta
    // Poiché 'aggiungi_ricetta' libera la lista ingredienti in caso di duplicato, dobbiamo riallocare la lista
    IngredienteRicetta* ingrediente2 = (IngredienteRicetta*)malloc(sizeof(IngredienteRicetta));
    if (ingrediente2 == NULL) {
        printf("Errore di allocazione memoria per ingrediente2\n");
        return;
    }
    strcpy(ingrediente2->nome, "Pomodoro");
    ingrediente2->quantita = 2;
    ingrediente2->next = NULL;
    ricetta1.ingredienti = ingrediente2;

    printf("Tento di aggiungere nuovamente la ricetta 'Pasta'\n");
    aggiungi_ricetta(ricetta1);

    // Pulizia della memoria
    liberaListaIng(ricetta1.ingredienti);
    liberaBST(bst);
    bst = NULL;  // Reset della variabile globale bst
}


void testMenu() {
    int choice;
    do {
        printf("\nSeleziona un test da eseguire:\n");
        printf("1. Test MinHeapIngrediente\n");
        printf("2. Test NodoAVL\n");
        printf("3. Test NodoBST\n");
        printf("4. Test CodaOrdini\n");
        printf("5. Test MinHeapOrdini\n");
        printf("6. Test MaxHeapSpedizioni\n");
        printf("7. Test GestisciComandi\n");
        printf("8. Test AggiungiRicetta\n");
        printf("0. Esci\n");
        printf("Scelta: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                testMinHeapIngrediente();
            break;
            case 2:
                testNodoAVL();
            break;
            case 3:
                testNodoBST();
            break;
            case 4:
                testCodaOrdini();
            break;
            case 5:
                testMinHeapOrdini();
            break;
            case 6:
                testMaxHeapSpedizioni();
            break;
            case 7:
                testGestisciComandi();
            break;
            case 8:
                testAggiungiRicetta();
            break;
            case 0:
                printf("Uscita...\n");
            break;
            default:
                printf("Scelta non valida. Riprova.\n");
        }
    } while (choice != 0);
}
