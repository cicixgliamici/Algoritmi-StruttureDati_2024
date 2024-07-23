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
void inserisciIngredienteHeap(MinHeapIngrediente* heap, int scadenza, int quantita);
void liberaMinHeapIngredienti(MinHeapIngrediente* heap);
IngredienteMinHeap rimuoviIngredienteHeap(MinHeapIngrediente* heap);
MinHeapIngrediente creaHeapIngredienti(int capacita);

//Ingredienti - AVL - ordino i lotti di ingredienti lessicograficamente(disc), per velocizzare aggiunta, eliminazione e ricerca
typedef struct NodoAVL {
    char nome[256];
    MinHeapIngrediente heap;
    struct NodoAVL *sinistro;
    struct NodoAVL *destro;
    int altezza;
} NodoAVL;

void inOrder(NodoAVL *root);
void liberaAVL(NodoAVL *root);
int getBilancio(NodoAVL *nodo);
int altezza(NodoAVL *nodo);
int max(int a, int b);
NodoAVL* nuovoNodo(char* nome, int capacita);
NodoAVL* ruotaDestra(NodoAVL *y);
NodoAVL* ruotaSinistra(NodoAVL *x);
NodoAVL* minValueNode(NodoAVL *nodo);
NodoAVL* elimina(NodoAVL* root, char *nome);
NodoAVL* inserisci(NodoAVL* nodo, char* nome, int scadenza, int quantita, int capacita);

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
void inOrderRicetta(NodoBST* root);
void liberaListaIngredienti(IngredienteRicetta* ingrediente);
NodoBST* nuovoNodoRicetta(Ricetta ricetta);
NodoBST* minValueNodoRicetta(NodoBST* nodo);
NodoBST* inserisciRicetta(NodoBST* nodo, Ricetta ricetta);
NodoBST* cercaRicetta(NodoBST* nodo, char* nome);
NodoBST* eliminaRicetta(NodoBST* root, char* nome);

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
void aggiungiOrdine(CodaOrdini* coda, char* nome_ricetta, int quantita, int tempo_arrivo);
int codaVuota(CodaOrdini* coda);
CodaOrdini* creaCoda();
Ordine* rimuoviOrdine(CodaOrdini* coda);

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

int main(void) {
    return 0;
}
