/*  4 Azioni da svolgere   aggiungi_ricetta: devo controllare nel BST se è già presente
*                          rimuovi_ricetta:  devo controllare sia se è presente nel BST sia nelle 3 strutture di ordini
*                          rifornimento:     non devo fare controlli anteriori, ma devo controllare dopo se posso fare ordini sulla coda
*                          ordine:           devo prima controllare se esiste nel BST, se no rifiutare
*                                            poi lo inserisco nella Coda, e se fattibile lo tolgo e metto sul minHeap
*                                            sennò lo tengo in coda ed aspetto ogni rifornimento
*
*   Il Tempo               Ogni azione costituisce un istante di tempo, escluso il setting del camioncino, si parte da t=0
*                          Il riempimento del camion va fatto prima dell'azione, e fatto ad ogni multiplo (anche a riga vuota, vedi esempio specifica), ogni n*tempoCamion multipli
*                          Condiziona anche gli ingredienti con scadenza, che non viene aggiornata ogni istante ma nella funzione "fattibilita"  
*
*   Gestione comandi       Leggo la riga da input con fgets, la salvo e poi la analizzo con sscanf
*                          Aggiorno il tempo ad ogni lettura di riga e per i multipi del tempo camioncino, prima lo carico e poi faccio l'azione
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
int tempoCorrente = -1;
int ultimoAggiornamento = -1;

// Funzioni generiche
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Funzioni Algoritmo
void gestisciComandi(FILE *file) {
    int tempoCamion, capienzaCamion;
    fscanf(file, "%d %d", &tempoCamion, &capienzaCamion);
    //printf("%d %d\n", tempoCamion, capienzaCamion);
    max_heap_spedizioni = creaMaxHeap(capienzaCamion);                                          //Nel caso peggiore ho capienzaCamion ordini di peso 1
    coda_ordini = creaCoda();
    heap_ordini_fatti = creaMinHeap(10);
    ultimoAggiornamento = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char command[256];
        if (sscanf(line, "%s", command) == 1) {
            tempoCorrente++;
            printf("t=%d --->       ", tempoCorrente);
            if(tempoCorrente%tempoCamion==0 && tempoCorrente!=0){
                //stampaTutto();
                caricaCamion();
            }
            if (strcmp(command, "aggiungi_ricetta") == 0) {                                     //Per non portarmi appresso tutta la lista ingredienti faccio il controllo qui
                char nome_ricetta[256];
                if (sscanf(line + strlen(command), "%s", nome_ricetta) == 1) {
                    if (cercaBST(bst, nome_ricetta) != NULL) {
                        printf("ignorato\n");
                    } else {
                        Ricetta nuova_ricetta;
                        strcpy(nuova_ricetta.nome, nome_ricetta);
                        nuova_ricetta.ingredienti = NULL;
                        IngredienteRicetta *tail = NULL;
                        char ingrediente[256];
                        int quantita;
                        char *ptr = line + strlen(command) + strlen(nome_ricetta) + 1;
                        while (sscanf(ptr, "%s %d", ingrediente, &quantita) == 2) {
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
                rifornimento(line + strlen(command) + 1);
            } else if (strcmp(command, "ordine") == 0) {
                char nome_ricetta[256];
                int quantita;
                if (sscanf(line + strlen(command), "%s %d", nome_ricetta, &quantita) == 2) {
                    ordine(nome_ricetta, quantita);
                }
            }
        }
    }
    if((tempoCorrente+1)%tempoCamion==0 && tempoCorrente!=0)
        caricaCamion();
    //stampaTutto();
}

void aggiungi_ricetta(Ricetta nuova_ricetta) {
    bst = inserisciBST(bst, nuova_ricetta);
    printf("aggiunta\n");
}

void rimuovi_ricetta(const char* nome_ricetta) {
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
    if (cercaBST(bst, (char*)nome_ricetta) == NULL) {                                          //Controllo nel BST
        printf("non presente\n");
    } else {
        bst = eliminaBST(bst, (char*)nome_ricetta);
        printf("rimossa\n");
    }
}

void rifornimento(const char* comando) {
    char nome_ingrediente[256];
    int quantita, scadenza;
    const char *ptr = comando;
    while (sscanf(ptr, "%s %d %d", nome_ingrediente, &quantita, &scadenza) == 3) {
        NodoAVL* nodo = cercaAVL(avl, nome_ingrediente);
        if (nodo != NULL) {
            inserisciIngrediente(&nodo->heap, scadenza, quantita, nodo);
        } else {
            avl = inserisciAVL(avl, nome_ingrediente, scadenza, quantita, 10);
        }
        //printf("Rifornito %s con %d unità, scadenza %d\n", nome_ingrediente, quantita, scadenza); // Debugging
        ptr += strlen(nome_ingrediente) + 1 + snprintf(NULL, 0, "%d", quantita) + 1 + snprintf(NULL, 0, "%d", scadenza) + 1;
    }
    printf("rifornito\n");
    verificaOrdini();
}

// Main - Gestione del giorno
int main(void) {
    FILE *file = fopen("C:/Users/39392/CLionProjects/API/tests/open4.txt", "r"); //stdin
    gestisciComandi(file);
    fclose(file);
    stampaTutto();
    return 0;
}
