/* 5 Strutture dati:  Ingredienti:      AVL di min-Heap
*                     Ricette:          BST di liste
*                     Ordini da Fare:   Coda FIFO
*                     Ordini fatti:     min-Heap
*                     Camioncino:       max-Heap
*/
#ifndef HEADER_H
#define HEADER_H
#include <stdio.h>

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

void heapifyIngredienti(MinHeapIngrediente* heap, int i);                               //Riordina gli elementi
void inserisciIngrediente(MinHeapIngrediente* heap, int scadenza, int quantita);        //Inserisce un ingrediente
void liberaLotto(MinHeapIngrediente* heap);                                             //Libera la memoria
IngredienteMinHeap rimuoviIngrediente(MinHeapIngrediente* heap);                        //Rimuove e ritorna l'ingrediente
MinHeapIngrediente nuovoHeapIngredienti(int capacita);                                  //Inizializza un nuovo heap (e restituisce il suo puntatore)

void testMinHeapIngrediente();

//Ingredienti - AVL - ordino i lotti di ingredienti lessicograficamente(disc), per velocizzare aggiunta, eliminazione e ricerca
typedef struct NodoAVL {
    char nome[256];
    MinHeapIngrediente heap;
    struct NodoAVL *sinistro;
    struct NodoAVL *destro;
    int altezza;
} NodoAVL;

void liberaAVL(NodoAVL *root);                                                                  //Libera la memoria
int valBilancia(NodoAVL *nodo);                                                                 //Calcola e ritorna il valore di bilanciamento di un nodo
int altezza(NodoAVL *nodo);                                                                     //Calcola e ritorna l'altezza di un nodo
NodoAVL* nuovoAVL(char* nome, int capacita);                                                    //Crea un nuovo nuodo (e restituisce il suo puntatore)
NodoAVL* ruotaDestra(NodoAVL *y);                                                               //Esegue una rotazione a destra
NodoAVL* ruotaSinistra(NodoAVL *x);                                                             //Esegue una rotazione a sinistra
NodoAVL* minValueAVL(NodoAVL *nodo);                                                            //Trova e ritorna il nodo con il valore minimo in un AVL (puntatore)
NodoAVL* eliminaAVL(NodoAVL* root, char *nome);                                                 //Elimina un nodo
NodoAVL* cercaAVL(NodoAVL* nodo, const char* nome);                                             //Cerca un nodo
NodoAVL* inserisciAVL(NodoAVL* nodo, char* nome, int scadenza, int quantita, int capacita);     //Inserisce un nodo

void testNodoAVL();

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

void liberaBST(NodoBST* root);                                 //Libera la memoria del nodo
void liberaListaIng(IngredienteRicetta* ingrediente);          //Libera la memoria della lista di ingredienti di un nodo
NodoBST* nuovoBST(Ricetta ricetta);                            //Crea un nuovo nodo (e restituisce il suo puntatore)
NodoBST* minValueBST(NodoBST* nodo);                           //Trova e ritorna il nodo con il valore minimo in un BST (puntatore)
NodoBST* inserisciBST(NodoBST* nodo, Ricetta ricetta);         //Inserisce un nodo
NodoBST* cercaBST(NodoBST* nodo, char* nome);                  //Cerca un nodo
NodoBST* eliminaBST(NodoBST* root, char* nome);                //Elimina un nodo

void testNodoBST();

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

void liberaCoda(CodaOrdini* coda);                                                          //Libera la memoria
void aggiungiCoda(CodaOrdini* coda, char* nome_ricetta, int quantita, int tempo_arrivo);    //Aggiunge un ordine
int codaVuota(CodaOrdini* coda);                                                            //Verifica se la coda è vuota
CodaOrdini* creaCoda();                                                                     //Crea una nuova coda (e restituisce il suo puntatore)
Ordine* rimuoviCoda(CodaOrdini* coda);                                                      //Rimuove e ritorna un ordine dalla coda (puntatore)

void testCodaOrdini();

//Ordini fatti - minHeap
typedef struct OrdineHeap {
    int tempo_arrivo;
    char ricetta[256];
    int quantita;
} OrdineHeap;

typedef struct {
    OrdineHeap* ordini;
    int dimensione;
    int capacita;
} MinHeap;

void liberaMinHeapOrdini(MinHeap* heap);                                                       //Libera la memoria
void scambiaOrdini(OrdineHeap* a, OrdineHeap* b);                                              //Scambia due ordini
void heapifyOrdini(MinHeap* heap, int i);                                                      //Riordina gli elementi
void inserisciOrdineHeap(MinHeap* heap, int tempo_arrivo, char* nome_ricetta, int quantita);   //Inserisce un ordine
int heapVuoto(MinHeap* heap);                                                                  //Verifica se il min-heap è vuoto
MinHeap* creaMinHeap(int capacita);                                                            //Crea un nuovo min-heap (e restituisce il suo puntatore)
OrdineHeap rimuoviMin(MinHeap* heap);                                                          //Rimuove e ritorna l'ordine (puntatore)

void testMinHeapOrdini();

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
    int capacita;
} MaxHeapSpedizioni;

void heapifySpedizioni(MaxHeapSpedizioni* heap, int i);                                                       //Riordina gli elementi
void inserisciSpedizione(MaxHeapSpedizioni* heap, char* nome, int istante_arrivo, int quantita, int peso);    //Inserisce una spedizione
void liberaMaxHeap(MaxHeapSpedizioni* heap);                                                                  //Libera la memoria
MaxHeapSpedizioni* creaMaxHeap(int capacita);                                                                  // Crea un nuovo max-heap (e restituisce il suo puntatore)
Spedizione rimuoviMax(MaxHeapSpedizioni* heap);                                                               //Rimuove e ritorna la spedizione (puntatore)

void testMaxHeapSpedizioni();

//Funzioni Generiche//Attivare tutti i test
int max(int a, int b);

//Funzioni per l'Algoritmo
void gestisciComandi(FILE *file);                                                                             //Leggi comando con strcmp
void aggiungi_ricetta(Ricetta nuova_ricetta);                                                                 //
void rimuovi_ricetta(const char* nome_ricetta);                                                               //
void rifornimento(const char* nome_ingrediente, int quantita, int scadenza);                                  //
void ordine(const char* nome_ricetta, int numero_elementi_ordinati);                                          //

void testGestisciComandi();
void testAggiungiRicetta();
void testMenu();

// Dichiarazione delle variabili globali (rimuovere quando metti tutto insieme)
extern NodoBST* bst;
extern NodoAVL* avl;
extern CodaOrdini* coda_ordini;
extern MinHeap* heap_ordini_fatti;
extern MaxHeapSpedizioni* max_heap_spedizioni;


#endif //HEADER_H
