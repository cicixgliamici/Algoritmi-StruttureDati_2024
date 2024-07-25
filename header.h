/* 5 Strutture dati   Ingredienti:      AVL di min-Heap  -> Ingredienti ordinati per scadenza nel min-Heap
*                     Ricette:          BST di liste     -> Ordinamento lessicografico
*                     Ordini da Fare:   Coda FIFO        -> Mantiene l'ordine di arrivo degli ordini
*                     Ordini fatti:     min-Heap         -> Istante di arrivo dell'ordine
*                     Camioncino:       max-Heap         -> Peso totale dell'ordine
*/
#ifndef HEADER_H
#define HEADER_H
#include <stdio.h>
#include <stdbool.h>

// Ingredienti - Heap - ordino tutti i prodotti dello stesso tipo per scadenza(asc), così da rispettare la specifica
//                      è presente un puntatore al suo nodo per aggiornare il peso_totale del nodo
typedef struct IngredienteMinHeap {
    int scadenza;
    int quantita;
    struct NodoAVL* nodoAVL;
} IngredienteMinHeap;

typedef struct {
    IngredienteMinHeap* lotto;
    int dimensione;
    int capacita;
} MinHeapIngrediente;

void scambia(IngredienteMinHeap* a, IngredienteMinHeap* b);                                                    //Scambia due elementi
void heapifyIngredienti(MinHeapIngrediente* heap, int i);                                                      //Riordina gli elementi
void inserisciIngrediente(MinHeapIngrediente* heap, int scadenza, int quantita, struct NodoAVL* nodo);         //Inserisce un ingrediente
void liberaLotto(MinHeapIngrediente* heap);                                                                    //Libera la memoria
IngredienteMinHeap rimuoviIngrediente(MinHeapIngrediente* heap);                                               //Rimuove e ritorna l'ingrediente
MinHeapIngrediente nuovoHeapIngredienti(int capacita);                                                         //Inizializza un nuovo heap (e restituisce il suo puntatore)

bool verificaMinHeap(MinHeapIngrediente* heap);                                          //Verifica la correttezza del min-heap
void testHeapOrder(MinHeapIngrediente* heap);                                            //Stampa verificando la correttezza dell'ordine
void testMinHeapInserimento();                                                           //Test di inserimento
void testMinHeapRimozione();                                                             //Test di rimozione
void testMinHeapOverflow();                                                              //Test di overflow
void testMinHeapInit();                                                                  //Test di inizializzazione
void testMinHeapIngrediente();                                                           //Raccoglie i test
void testUsoParzialeIngrediente();                                                       //Test per verificare la correttezza dell'inserimento
void testRifornimentoRicetta();                                                          //Test per verificare la correttezza dell'inserimento(migliore)

void stampaHeapIngredienti(MinHeapIngrediente* heap);

//Ingredienti - AVL - ordino i lotti di ingredienti lessicograficamente(disc), per velocizzare aggiunta, eliminazione e ricerca
typedef struct NodoAVL {
    char nome[256];
    MinHeapIngrediente heap;
    int peso_totale;
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
NodoAVL* inserisciAVL(NodoAVL* nodo, char* nome, int scadenza, int quantita, int capacita);     //Inserisce un nodo (da bilanciare)
NodoAVL* bilanciaAVL(NodoAVL *nodo);                                                            //Bilancia il nodo inserito

void stampaAVL(NodoAVL* nodo);                                                                  //Stampa l'AVL per ingredienti

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

void stampaBST(NodoBST* nodo);

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

void stampaCodaFIFO(CodaOrdini* coda);

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
int heapVuotoMinOrdine(MinHeap* heap);                                                         //Verifica se il min-heap è vuoto
MinHeap* creaMinHeap(int capacita);                                                            //Crea un nuovo min-heap (e restituisce il suo puntatore)
OrdineHeap rimuoviMin(MinHeap* heap);                                                          //Rimuove e ritorna l'ordine (puntatore)

void stampaMinHeapOrdini(MinHeap* heap);

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
MaxHeapSpedizioni* creaMaxHeap(int capacita);                                                                 // Crea un nuovo max-heap (e restituisce il suo puntatore)
Spedizione rimuoviMax(MaxHeapSpedizioni* heap);                                                               //Rimuove e ritorna la spedizione (puntatore)
int heapVuotoMax(MaxHeapSpedizioni* heap);

void stampaMaxHeapSpedizioni(MaxHeapSpedizioni* heap);

//Funzioni Generiche//Attivare tutti i test
int max(int a, int b);

//Funzioni per l'Algoritmo
void gestisciComandi(FILE *file);                                                                //Leggi comando con strcmp
void aggiungi_ricetta(Ricetta nuova_ricetta);                                                    //Controlla se già esiste, se no aggiungi
void rimuovi_ricetta(const char* nome_ricetta);                                                  //Controlla se esiste o se è in ordinazione, se no rimuovi
void rifornimento(const char* comando);                                                          //Nessun controllo
void ordine(const char* nome_ricetta, int numero_elementi_ordinati);                             //Controlla esistenza in ricettario, se si aggiungi a Coda e poi prova a fare

bool fattibilita(const char* nome_ricetta, int numero_elementi_ordinati);                        //Verifica la fattiblità di un ordine guardando peso_totale e peso ordine
void preparazione(const char* nome_ricetta, int numero_elementi_ordinati);                       //Effettua la preparazione rispettando la specifica di rimozione ingredienti scadenza minore
void verificaOrdini();                                                                           //Verifica la fattibilità di tutti gli ordini in Coda
void caricaCamion();                                                                             //Passaggio da minHeap ordini fatti a maxHeap spedizione
int calcolaPeso(Ricetta ricetta, int numero_elementi_ordinati);                                  //Calcola il peso di un ordine

void aggiornaScadenza();                                                                         //Aggiorna le scadenze degli ingredienti, chiamata a rifornimento e ordine
void decrementaScadenzaAVL(NodoAVL* nodo, int decremento);                                       //Cicla tutto l'AVL
void decrementaScadenzaHeap(MinHeapIngrediente* heap, int decremento);                           //Cicla tutto il minHeap

void stampaTutto();

// Dichiarazione delle variabili globali (rimuovere quando metti tutto insieme)
extern NodoBST* bst;
extern NodoAVL* avl;
extern CodaOrdini* coda_ordini;
extern MinHeap* heap_ordini_fatti;
extern MaxHeapSpedizioni* max_heap_spedizioni;
extern int tempoCorrente;
extern int ultimoAggiornamento;

#endif //HEADER_H
