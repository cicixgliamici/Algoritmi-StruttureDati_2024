/* 5 Strutture dati:  Ingredienti - AVL di min-Heap
*                     Ricette - BST di liste
*                     Ordini da Fare - Coda FIFO
*                     Ordini fatti - min-Heap
*                     Camioncino - max-Heap
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Ingredienti - Heap - ordino tutti i prodotti dello stesso tipo per scadenza(asc), così da rispettare la specifica 
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
void inserisciIngrediente(MinHeapIngrediente* heap, int scadenza, int quantita);
void liberaLotto(MinHeapIngrediente* heap);
IngredienteMinHeap rimuoviIngrediente(MinHeapIngrediente* heap);
MinHeapIngrediente creaHeapIngredienti(int capacita);

//Ingredienti - AVL - ordino i lotti di ingredienti lessicograficamente(disc), per velocizzare aggiunta, eliminazione e ricerca
typedef struct NodoAVL {
    char nome[256];
    MinHeapIngrediente heap;
    struct NodoAVL *sinistro;
    struct NodoAVL *destro;
    int altezza;
} NodoAVL;

void ordineAVL(NodoAVL *root);
void liberaAVL(NodoAVL *root);
int bilancia(NodoAVL *nodo);
int altezza(NodoAVL *nodo);
int max(int a, int b);
NodoAVL* nuovoNodoAVL(char* nome, int capacita);
NodoAVL* ruotaDestra(NodoAVL *y);
NodoAVL* ruotaSinistra(NodoAVL *x);
NodoAVL* minValueAVL(NodoAVL *nodo);
NodoAVL* eliminaAVL(NodoAVL* root, char *nome);
NodoAVL* inserisciAVL(NodoAVL* nodo, char* nome, int scadenza, int quantita, int capacita);

//Ricette - Lista
typedef struct IngredienteRicetta {
    char nome[256];
    int quantita;
    struct IngredienteRicetta *next;
} IngredienteRicetta;

typedef struct Ricetta {
    char nome[256];
    IngredienteRicetta *ingredienti;
} Ricetta;

//Ricette - BST
typedef struct NodoBST {
    Ricetta ricetta;
    struct NodoBST *sinistro;
    struct NodoBST *destro;
} NodoBST;

void liberaBST(NodoBST* root);
void ordineBST(NodoBST* root);
void liberaListaIng(IngredienteRicetta* ingrediente);
NodoBST* nuovoNodoBST(Ricetta ricetta);
NodoBST* minValueBST(NodoBST* nodo);
NodoBST* inserisciBST(NodoBST* nodo, Ricetta ricetta);
NodoBST* cercaBST(NodoBST* nodo, char* nome);
NodoBST* eliminaBST(NodoBST* root, char* nome);

//Ordine da Fare - Coda FIFO
typedef struct Ordine {
    char nome_ricetta[256];
    int quantita;
    int tempo_arrivo;
    struct Ordine* next;
} Ordine;

typedef struct {
    Ordine* testa;
    Ordine* coda;
} CodaOrdini;

void liberaCoda(CodaOrdini* coda);
void aggiungiCoda(CodaOrdini* coda, char* nome_ricetta, int quantita, int tempo_arrivo);
int codaVuota(CodaOrdini* coda);
CodaOrdini* creaCoda();
Ordine* rimuoviCoda(CodaOrdini* coda);

//Ordini fatti - minHeap
typedef struct OrdineHeap {
    int tempo_arrivo;
    char nome_ricetta[256];
    int quantita;
} OrdineHeap;

typedef struct {
    OrdineHeap* ordini;
    int dimensione;
    int capacita;
} MinHeap;

void liberaMinHeapOrdini(MinHeap* heap);
void scambiaOrdini(OrdineHeap* a, OrdineHeap* b);
void heapifyOrdini(MinHeap* heap, int i);
void inserisciOrdineHeap(MinHeap* heap, int tempo_arrivo, char* nome_ricetta, int quantita);
int heapVuoto(MinHeap* heap);
MinHeap creaMinHeap(int capacita);
OrdineHeap* rimuoviMin(MinHeap* heap);

//Spedizione - maxHeap
typedef struct Spedizione {
    char nome[256];
    int istante_arrivo;
    int quantita;
    int peso;
} Spedizione;

typedef struct {
    Spedizione* spedizioni;
    int dimensione;
    int capacità;
} MaxHeapSpedizioni;

void heapifySpedizioni(MaxHeapSpedizioni* heap, int i);
void inserisciSpedizione(MaxHeapSpedizioni* heap, char* nome, int istante_arrivo, int quantita, int peso);
void stampaSpedizioni(MaxHeapSpedizioni* heap);
void liberaMaxHeap(MaxHeapSpedizioni* heap);
int heapVuotoSpedizioni(MaxHeapSpedizioni* heap);
MaxHeapSpedizioni creaMaxHeap(int capacita);
Spedizione rimuoviMac(MaxHeapSpedizioni* heap);

//Funzioni min-Heap Ingredienti
void heapify(MinHeapIngrediente* heap, int i) {
    int minore=i;
    int sx= 2*i+1;
    int dx= 2*i+2;
    if(sx<heap->dimensione && heap->lotto[sx].scadenza< heap->lotto[minore].scadenza)
        minore=sx;
    if(dx<heap->dimensione && heap->lotto[dx].scadenza< heap->lotto[minore].scadenza)
        minore=dx;
    if(minore!=i) {
        IngredienteMinHeap temp= heap->lotto[i];
        heap->lotto[i]=heap->lotto[minore];
        heap->lotto[minore]=temp;
        heapify(heap, minore);
        }
}

void inserisciIngrediente(MinHeapIngrediente* heap, int scadenza, int quantita) {
    if(heap->dimensione==heap->capacita) {
        heap->capacita *=2; //amplia per due lo spazio, potremmo provare sommando
        heap->lotto =(IngredienteMinHeap*)realloc(heap->lotto, heap->capacita*sizeof(IngredienteMinHeap));
        }
    int i= heap->dimensione++;
    heap->lotto[i].scadenza=scadenza;
    heap->lotto[i].quantita=quantita;
    while(i!=0 && heap->lotto[(i-1)/2].scadenza > heap->lotto[i].scadenza) {
        IngredienteMinHeap temp= heap->lotto[i];
        heap->lotto[i]= heap->lotto[(i-1)/2];
        heap->lotto[(i-1)/2] = temp;
        i=(i-1)/2;
        }
}

void liberaLotto(MinHeapIngrediente* heap) {
    free(heap->lotto);
}

IngredienteMinHeap rimuoviIngrediente(MinHeapIngrediente* heap) {
    if(heap->dimensione <=0) {
        IngredienteMinHeap LottoVuoto = {0,0};
        return LottoVuoto;
        }
    IngredienteMinHeap root=heap->lotto[0];
    if(heap->dimensione==1) {
        heap->dimensione--;
        return root;
        }
    heap->lotto[0]=heap->lotto[--heap->dimensione];
    heapify(heap,0);
    return root;
}

MinHeapIngrediente creaHeapIngredienti(int capacita) {
    MinHeapIngrediente heap;
    heap.lotto=(IngredienteMinHeap*)malloc(capacita*sizeof(IngredienteMinHeap));
    heap.dimensione=0;
    heap.capacita=capacita;
    return heap;
}

//Funzioni AVL di min-Heap

//Main - Gestione del giorno
int main(void) {
    return 0;
}
