/* 4 Comandi:    aggiungi_ordine:   controllo se è presente, se no aggiungo a BST
*                rimuovi_ordine:    controllo se è presente sia nel ricettario(BST) che negli ordini(sia Coda che maxHeap e minHeap)
*                rimuovi:
*                ordine:
*/
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

// Funzioni generiche
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Funzioni Algoritmo

//Leggo le righe (e le salvo) con fgets e le analizzo con sscanf
void gestisciComandi(FILE *file) {
    printf("\nLettura comandi\n");
    int tempoCamion, capienzaCamion;
    if (fscanf(file, "%d %d", &tempoCamion, &capienzaCamion) != 2) {
        fprintf(stderr, "Errore nella lettura di tempoCamion e capienzaCamion\n");
        return;
    }
    printf("tempoCamion: %d, capienzaCamion: %d\n", tempoCamion, capienzaCamion);
    max_heap_spedizioni = creaMaxHeap(capienzaCamion);
    coda_ordini = creaCoda();
    heap_ordini_fatti = creaMinHeap(10);
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char command[256];
        if (sscanf(line, "%s", command) == 1) {
            printf("Comando: %s\n", command);
            if (strcmp(command, "aggiungi_ricetta") == 0) {
                char nome_ricetta[256];
                if (sscanf(line + strlen(command), "%s", nome_ricetta) == 1) {
                    printf("Nome ricetta: %s\n", nome_ricetta);
                    // Check if recipe exists in BST
                    if (cercaBST(bst, nome_ricetta) != NULL) {
                        printf("ignorato\n");
                    } else {
                        Ricetta nuova_ricetta;
                        strcpy(nuova_ricetta.nome, nome_ricetta);
                        // Read ingredients and their quantities
                        nuova_ricetta.ingredienti = NULL;
                        IngredienteRicetta *tail = NULL;
                        char ingrediente[256];
                        int quantita;
                        char *ptr = line + strlen(command) + strlen(nome_ricetta) + 1;
                        while (sscanf(ptr, "%s %d", ingrediente, &quantita) == 2) {
                            printf("Ingrediente: %s, Quantita: %d\n", ingrediente, quantita);
                            IngredienteRicetta *nuovo_ingrediente = (IngredienteRicetta*)malloc(sizeof(IngredienteRicetta));
                            strcpy(nuovo_ingrediente->nome, ingrediente);
                            nuovo_ingrediente->quantita = quantita;
                            nuovo_ingrediente->next = NULL;
                            if (tail == NULL) {
                                nuova_ricetta.ingredienti = nuovo_ingrediente;
                            } else {
                                tail->next = nuovo_ingrediente;
                            }
                            tail = nuovo_ingrediente;
                            ptr += strlen(ingrediente) + 1 + snprintf(NULL, 0, "%d", quantita) + 1;
                        }
                        aggiungi_ricetta(nuova_ricetta);
                    }
                }
            } else if (strcmp(command, "rimuovi_ricetta") == 0) {
                char nome_ricetta[256];
                if (sscanf(line + strlen(command), "%s", nome_ricetta) == 1) {
                    rimuovi_ricetta(nome_ricetta);
                }
            } else if (strcmp(command, "rifornimento") == 0) {
                // Implementa il codice per rifornimento
            } else if (strcmp(command, "ordine") == 0) {
                // Implementa il codice per ordine
            }
        }
    }
}

void aggiungi_ricetta(Ricetta nuova_ricetta) {
    printf("DEBUG: Aggiungo ricetta: %s\n", nuova_ricetta.nome);
    bst = inserisciBST(bst, nuova_ricetta);
    printf("aggiunto\n");
}

void rimuovi_ricetta(const char* nome_ricetta) {
    printf("DEBUG: Rimuovo ricetta: %s\n", nome_ricetta);
    Ordine* ordine_corrente = coda_ordini->testa;
    while (ordine_corrente != NULL) {                                                   //Controllo nella coda degli ordini da fare
        if (strcmp(ordine_corrente->nome_ricetta, nome_ricetta) == 0) {
            printf("ordini in sospeso\n");
            return;
        }
        ordine_corrente = ordine_corrente->next;
    }
    for (int i = 0; i < heap_ordini_fatti->dimensione; i++) {                           //Controllo nel minHeap degli ordini fatti
        if (strcmp(heap_ordini_fatti->ordini[i].ricetta, nome_ricetta) == 0) {
            printf("ordini in sospeso\n");
            return;
        }
    }
    for (int i = 0; i < max_heap_spedizioni->dimensione; i++) {                         //Controllo nel maxHeap del camioncino
        if (strcmp(max_heap_spedizioni->spedizioni[i].nome, nome_ricetta) == 0) {
            printf("ordini in sospeso\n");
            return;
        }
    }
    if (cercaBST(bst, nome_ricetta) == NULL) {                                          //Controllo nel BST
        printf("non presente\n");
    } else {
        bst = eliminaBST(bst, nome_ricetta);
        printf("rimossa\n");
    }
}

void rifornimento(const char* nome_ingrediente, int quantita, int scadenza) {
    NodoAVL* nodo = cercaAVL(avl, nome_ingrediente);
    if (nodo != NULL) {
        inserisciIngrediente(&nodo->heap, scadenza, quantita);
    } else {
        avl = inserisciAVL(avl, nome_ingrediente, scadenza, quantita, 10);
    }
}

void ordine(const char* nome_ricetta, int numero_elementi_ordinati) {
    // Implementa il codice per processare un ordine
}

// Main - Gestione del giorno
int main(void) {
    printf("Inizio main\n");
    FILE *file = fopen("C:/Users/39392/CLionProjects/API/tests/input.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Errore nell'apertura del file input.txt\n");
        return 1;
    }
    gestisciComandi(file);
    fclose(file);
    return 0;
}
