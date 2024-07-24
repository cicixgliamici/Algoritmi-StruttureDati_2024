#include "header.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Dichiarazione globale delle strutture dati
NodoBST* bst = NULL;
NodoAVL* avl = NULL;
CodaOrdini* coda_ordini = NULL;
MinHeap* heap_ordini_fatti = NULL;
MaxHeapSpedizioni* max_heap_spedizioni = NULL;

//Funzioni generiche
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Funzioni Algoritmo
void gestisciComandi(FILE *file) {
}

void aggiungi_ricetta(Ricetta nuova_ricetta) {
    printf("DEBUG: Aggiungo ricetta: %s\n", nuova_ricetta.nome);  // Debug
}

void rimuovi_ricetta(const char* nome_ricetta) {
    printf("Rimuovi ricetta: %s\n", nome_ricetta);
}

void rifornimento(const char* nome_ingrediente, int quantita, int scadenza) {
    printf("Rifornimento: Ingrediente: %s, Quantita: %d, Scadenza: %d\n", nome_ingrediente, quantita, scadenza);
}

void ordine(const char* nome_ricetta, int numero_elementi_ordinati) {
    printf("Ordine: Ricetta: %s, Numero elementi ordinati: %d\n", nome_ricetta, numero_elementi_ordinati);
}

// Main - Gestione del giorno
int main(void) {
    FILE *file = fopen("C:/Users/39392/CLionProjects/API/input.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Errore nell'apertura del file input.txt\n");
        return 1;
    }
    gestisciComandi(file);
    fclose(file);
    return 0;
}
