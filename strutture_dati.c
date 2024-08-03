/*  Problemi riscontrati e edge cases:  1)  A parità di peso, gli ordini del camioncino vanno per istante di arrivo
*                                       2)  Il camioncino passa anche se l'ultima riga è al comando n*(tempoCamion-1) quindi devo fare un ultimo controllo
*                                       3)  Nell'example.txt la ciambella 9-3 sostituiva la 5-6, tolto l'inseirmento a priori in Coda FIFO
*                                       4)  La liberazione di un nodo nell'AVL e la stampa del minHeap generava problemi nell'example.txt sulla torta 10 1
*                                       5)  Rifornimento al tempo 8/9 non va a buon fine in open4 -> facevo lettura fino a 256
*                                       6)  Secondo giro di open4 il camion svuotava scorrettamente il minHeapOrdini
*                                       7)  Secondo giro di open4 il camion carica due ordini di troppo, ha la capacità per farlo,
*                                           quindi non andavano prodotti -> Camion si ferma al primo che incontra troppo pesante
*                                       8)  open5 access violation al primo caricamento del camion, sembra essere un problema nel minHeap ordini -> era un 
*                                           problema di dimensione del maxHeap basato sulla capienzaCamion -> credo ci sia un "limite" al malloc
*                                       9)  open7 sfaciola totalmente ordini -> problema legato alla lunghezza delle righe di input e come venivano lette
*                                       10) passati tutti gli open tranne 8 (rimarrà un mistero), passato a Open -> usava troppo spazio -> cambiati i char
*                                           da 256 a 25 (il minimo è 20)
*                                       11) installato con non poca difficolta valgrind, dava controlloScadenza come funzione più chiamata, insieme alla 
*                                           lettura stringhe, qualche sospetto sulla struttura degli ingredienti
*                                       11) Per passare 18 bisogna grattare 15.857 secondi -> 
*                                                                                             le puts non sembrano aver funzionato
*                                                                                             accorpato il controlloScadenza alla fattibilita
*                                                                                             provare a cambiare il caricamento del camion
*                                                                                             def4.1 passa da AVL di minHeap a Treap
*                                                                                             def4.2 passa da AVL di minHeap a minHeap
*                                                        
*
*/
//Elenco di tutte le funzioni riguardanti le strutture dati
#include "header.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Funzione per creare un nuovo nodo Treap
NodoTreap* nuovoNodoTreap(char* nome, int scadenza, int quantita) {
    NodoTreap* nodo = (NodoTreap*)malloc(sizeof(NodoTreap));
    strcpy(nodo->nome, nome);
    nodo->scadenza = scadenza;
    nodo->quantita = quantita;
    nodo->priorita = rand() % 100; // Assegna una priorità casuale
    nodo->sinistro = nodo->destro = NULL;
    return nodo;
}

// Funzione di rotazione a destra
NodoTreap* ruotaDestraTreap(NodoTreap* y) {
    NodoTreap* x = y->sinistro;
    NodoTreap* T2 = x->destro;
    x->destro = y;
    y->sinistro = T2;
    return x;
}

// Funzione di rotazione a sinistra
NodoTreap* ruotaSinistraTreap(NodoTreap* x) {
    NodoTreap* y = x->destro;
    NodoTreap* T2 = y->sinistro;
    y->sinistro = x;
    x->destro = T2;
    return y;
}

// Funzione di inserimento nel Treap
NodoTreap* inserisciTreap(NodoTreap* root, char* nome, int scadenza, int quantita) {
    if (root == NULL) {
        return nuovoNodoTreap(nome, scadenza, quantita);
    }

    if (strcmp(nome, root->nome) == 0 && scadenza == root->scadenza) {
        root->quantita += quantita;
    } else if (scadenza < root->scadenza || (scadenza == root->scadenza && strcmp(nome, root->nome) < 0)) {
        root->sinistro = inserisciTreap(root->sinistro, nome, scadenza, quantita);
        if (root->sinistro->priorita > root->priorita) {
            root = ruotaDestraTreap(root);
        }
    } else {
        root->destro = inserisciTreap(root->destro, nome, scadenza, quantita);
        if (root->destro->priorita > root->priorita) {
            root = ruotaSinistraTreap(root);
        }
    }

    return root;
}

// Funzione di eliminazione dal Treap
NodoTreap* eliminaTreap(NodoTreap* root, char* nome) {
    if (root == NULL) {
        return root;
    }

    if (strcmp(nome, root->nome) < 0) {
        root->sinistro = eliminaTreap(root->sinistro, nome);
    } else if (strcmp(nome, root->nome) > 0) {
        root->destro = eliminaTreap(root->destro, nome);
    } else {
        if (root->sinistro == NULL) {
            NodoTreap* temp = root->destro;
            free(root);
            return temp;
        } else if (root->destro == NULL) {
            NodoTreap* temp = root->sinistro;
            free(root);
            return temp;
        }

        if (root->sinistro->priorita < root->destro->priorita) {
            root = ruotaSinistraTreap(root);
            root->destro = eliminaTreap(root->destro, nome);
        } else {
            root = ruotaDestraTreap(root);
            root->sinistro = eliminaTreap(root->sinistro, nome);
        }
    }

    return root;
}

// Funzione di ricerca nel Treap
NodoTreap* cercaTreap(NodoTreap* root, char* nome) {
    if (root == NULL) {
        return NULL;
    }

    NodoTreap* result = NULL;
    if (strcmp(root->nome, nome) == 0) {
        if (result == NULL || root->scadenza < result->scadenza) {
            result = root;
        }
    }

    NodoTreap* left_result = cercaTreap(root->sinistro, nome);
    NodoTreap* right_result = cercaTreap(root->destro, nome);

    if (result == NULL) {
        result = left_result ? left_result : right_result;
    } else {
        if (left_result && left_result->scadenza < result->scadenza) {
            result = left_result;
        }
        if (right_result && right_result->scadenza < result->scadenza) {
            result = right_result;
        }
    }

    return result;
}

// Funzione per liberare la memoria del Treap
void liberaTreap(NodoTreap* root) {
    if (root != NULL) {
        liberaTreap(root->sinistro);
        liberaTreap(root->destro);
        free(root);
    }
}

//Funzioni BST - Ricette
void liberaBST(NodoBST* root) {
   if(root!=NULL) {
       liberaBST(root->sinistro);
       liberaBST(root->destro);
       liberaListaIng(root->ricetta.ingredienti);
       free(root);
   }
}

void liberaListaIng(IngredienteRicetta* ingrediente) {
    IngredienteRicetta* temp;
    while(ingrediente!=NULL) {
        temp=ingrediente;
        ingrediente=ingrediente->next;
        free(temp);
    }
}

NodoBST* nuovoBST(Ricetta ricetta) {
    NodoBST* nodo=(NodoBST*)malloc(sizeof(NodoBST));
    nodo->ricetta =ricetta;
    nodo-> sinistro= nodo ->destro=NULL;
    return nodo;
}

NodoBST* minValueBST(NodoBST* nodo) {
    NodoBST* corrente=nodo;
    while(corrente && corrente->sinistro!=NULL)
        corrente=corrente->sinistro;
    return corrente;
}

NodoBST* inserisciBST(NodoBST* nodo, Ricetta ricetta) {
    if (nodo == NULL)
        return nuovoBST(ricetta);
    if (strcmp(ricetta.nome, nodo->ricetta.nome) < 0)
        nodo->sinistro = inserisciBST(nodo->sinistro, ricetta);
    else if (strcmp(ricetta.nome, nodo->ricetta.nome) > 0)
        nodo->destro = inserisciBST(nodo->destro, ricetta);
    return nodo;
}

NodoBST* cercaBST(NodoBST* nodo, char* nome) {
    if (nodo == NULL || strcmp(nodo->ricetta.nome, nome) == 0)
        return nodo;
    if (strcmp(nome, nodo->ricetta.nome) < 0)
        return cercaBST(nodo->sinistro, nome);
    return cercaBST(nodo->destro,nome);
}

NodoBST* eliminaBST(NodoBST* root, char* nome) {
    if (root == NULL)
        return root;
    if (strcmp(nome, root->ricetta.nome) < 0)
        root->sinistro = eliminaBST(root->sinistro, nome);
    else if (strcmp(nome, root->ricetta.nome) > 0)
        root->destro = eliminaBST(root->destro, nome);
    else {
        if (root->sinistro == NULL) {
            NodoBST* temp = root->destro;
            free(root);
            return temp;
        } else if (root->destro == NULL) {
            NodoBST* temp = root->sinistro;
            free(root);
            return temp;
        }
        NodoBST* temp = minValueBST(root->destro);
        root->ricetta = temp->ricetta;
        root->destro = eliminaBST(root->destro, temp->ricetta.nome);
    }
    return root;
}

//Funzioni Coda FIFO - Ordini da Fare
void liberaCoda(CodaOrdini* coda) {
    Ordine* temp;
    while(coda->testa!=NULL) {
        temp=coda->testa;
        coda->testa=coda->testa->next;
        free(temp);
    }
    free(coda);
}

void aggiungiCoda(CodaOrdini* coda, const char* nome_ricetta, int quantita, int tempo_arrivo) {
    Ordine* nuovoOrdine = (Ordine*)malloc(sizeof(Ordine));
    nuovoOrdine->tempo_arrivo = tempo_arrivo;
    strcpy(nuovoOrdine->nome_ricetta, nome_ricetta);
    nuovoOrdine->quantita = quantita;
    nuovoOrdine->next = NULL;
    if (coda->coda == NULL) {
        coda->testa = nuovoOrdine;
        coda->coda = nuovoOrdine;
    } else {
        coda->coda->next = nuovoOrdine;
        coda->coda = nuovoOrdine;
    }
}

int codaVuota(CodaOrdini* coda) {
    return coda->testa==NULL;
}

CodaOrdini* creaCoda() {
    CodaOrdini* coda=(CodaOrdini*)malloc(sizeof(CodaOrdini));
    coda->testa=NULL;
    coda->coda=NULL;
    return coda;
}

Ordine* rimuoviCoda(CodaOrdini* coda) {
    if (coda->testa == NULL) {
        return NULL;
    }
    Ordine* ordineRimosso = coda->testa;
    coda->testa = coda->testa->next;
    if (coda->testa == NULL) {
        coda->coda = NULL;
    }
    free(ordineRimosso);
    return coda->testa;
}

//Funzioni minHeap - Ordini Fatti
void liberaMinHeapOrdini(MinHeap* heap) {
    free(heap->ordini);
    free(heap);
}

void scambiaOrdini(OrdineHeap* a, OrdineHeap* b) {
    OrdineHeap temp= *a;
    *a=*b;
    *b=temp;
}

void heapifyOrdini(MinHeap* heap, int i) {
    int minore = i;
    int sx = 2 * i + 1;
    int dx = 2 * i + 2;
    if (sx < heap->dimensione && heap->ordini[sx].tempo_arrivo < heap->ordini[minore].tempo_arrivo)
        minore = sx;
    if (dx < heap->dimensione && heap->ordini[dx].tempo_arrivo < heap->ordini[minore].tempo_arrivo)
        minore = dx;
    if (minore != i) {
        scambiaOrdini(&heap->ordini[i], &heap->ordini[minore]);
        heapifyOrdini(heap, minore);
    }
}

void inserisciOrdineHeap(MinHeap* heap, int tempo_arrivo, char* ricetta, int quantita) {
    if (heap->dimensione == heap->capacita) {
        heap->capacita *= 2;
        heap->ordini = (OrdineHeap*)realloc(heap->ordini, heap->capacita * sizeof(OrdineHeap));
    }
    int i = heap->dimensione++;
    heap->ordini[i].tempo_arrivo = tempo_arrivo;
    strncpy(heap->ordini[i].ricetta, ricetta, sizeof(heap->ordini[i].ricetta) - 1);
    heap->ordini[i].ricetta[sizeof(heap->ordini[i].ricetta) - 1] = '\0';
    heap->ordini[i].quantita = quantita;
    while (i != 0 && heap->ordini[(i - 1) / 2].tempo_arrivo > heap->ordini[i].tempo_arrivo) {
        scambiaOrdini(&heap->ordini[i], &heap->ordini[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int heapVuotoMinOrdine(MinHeap* heap) {
    return heap->dimensione==0;
}

MinHeap* creaMinHeap(int capacita) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->dimensione = 0;
    heap->capacita = capacita;
    heap->ordini = (OrdineHeap*)malloc(capacita * sizeof(OrdineHeap));
    return heap;
}

OrdineHeap rimuoviMin(MinHeap* heap) {
    if (heap->dimensione == 0) {
        OrdineHeap ordineVuoto;
        strcpy(ordineVuoto.ricetta, "");
        ordineVuoto.quantita = 0;
        ordineVuoto.tempo_arrivo = 0;
        return ordineVuoto;
    }
    OrdineHeap min = heap->ordini[0];
    heap->ordini[0] = heap->ordini[heap->dimensione - 1];
    heap->dimensione--;
    heapifyOrdini(heap, 0);
    return min;
}

//Funzioni maxHeap - Camioncino Spedizioni
void heapifySpedizioni(MaxHeapSpedizioni* heap, int i) {
    int maggiore = i;
    int sinistro = 2 * i + 1;
    int destro = 2 * i + 2;
    if (sinistro < heap->dimensione && heap->spedizioni[sinistro].peso > heap->spedizioni[maggiore].peso)
        maggiore = sinistro;
    else if (sinistro < heap->dimensione && heap->spedizioni[sinistro].peso == heap->spedizioni[maggiore].peso &&
             heap->spedizioni[sinistro].istante_arrivo < heap->spedizioni[maggiore].istante_arrivo)
        maggiore = sinistro;
    if (destro < heap->dimensione && heap->spedizioni[destro].peso > heap->spedizioni[maggiore].peso)
        maggiore = destro;
    else if (destro < heap->dimensione && heap->spedizioni[destro].peso == heap->spedizioni[maggiore].peso &&
             heap->spedizioni[destro].istante_arrivo < heap->spedizioni[maggiore].istante_arrivo)
        maggiore = destro;
    if (maggiore != i) {
        Spedizione temp = heap->spedizioni[i];
        heap->spedizioni[i] = heap->spedizioni[maggiore];
        heap->spedizioni[maggiore] = temp;
        heapifySpedizioni(heap, maggiore);
    }
}

void inserisciSpedizione(MaxHeapSpedizioni* heap, char* nome, int istante_arrivo, int quantita, int peso) {
    if (heap->dimensione == heap->capacita) {
        heap->capacita *= 2;
        heap->spedizioni = (Spedizione*) realloc(heap->spedizioni, heap->capacita * sizeof(Spedizione));
    }
    int i = heap->dimensione++;
    strcpy(heap->spedizioni[i].nome, nome);
    heap->spedizioni[i].istante_arrivo = istante_arrivo;
    heap->spedizioni[i].quantita = quantita;
    heap->spedizioni[i].peso = peso;
    while (i != 0) {
        int parent = (i - 1) / 2;
        if (heap->spedizioni[parent].peso < heap->spedizioni[i].peso) {
            Spedizione temp = heap->spedizioni[i];
            heap->spedizioni[i] = heap->spedizioni[parent];
            heap->spedizioni[parent] = temp;
            i = parent;
        } else if (heap->spedizioni[parent].peso == heap->spedizioni[i].peso &&
                   heap->spedizioni[parent].istante_arrivo > heap->spedizioni[i].istante_arrivo) {
            Spedizione temp = heap->spedizioni[i];
            heap->spedizioni[i] = heap->spedizioni[parent];
            heap->spedizioni[parent] = temp;
            i = parent;
                   } else {
                       break;
                   }
    }
}

void liberaMaxHeap(MaxHeapSpedizioni* heap) {
    free(heap->spedizioni);
}

MaxHeapSpedizioni* creaMaxHeap(int capacita) {
    MaxHeapSpedizioni* heap = (MaxHeapSpedizioni*)malloc(sizeof(MaxHeapSpedizioni));
    heap->spedizioni = (Spedizione*) malloc(capacita * sizeof(Spedizione));
    heap->dimensione = 0;
    heap->capacita = capacita;
    return heap;
}

Spedizione rimuoviMax(MaxHeapSpedizioni* heap) {
    if (heap->dimensione<=0) {
        Spedizione nullSpedizione = {"", 0, 0, 0};
        return nullSpedizione;
    }
    Spedizione root=heap->spedizioni[0];
    if (heap->dimensione==1) {
        heap->dimensione--;
        return root;
    }
    heap->spedizioni[0]=heap->spedizioni[--heap->dimensione];
    heapifySpedizioni(heap, 0);
    return root;
}

int heapVuotoMax(MaxHeapSpedizioni* heap) {
    return heap->dimensione == 0;
}
