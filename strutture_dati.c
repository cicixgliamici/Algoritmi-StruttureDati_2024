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
#include <limits.h>

//Elenco di tutte le funzioni riguardanti le strutture dati
//Funzioni min-Heap - Ingredienti
void scambia(IngredienteMinHeap* a, IngredienteMinHeap* b) {
    IngredienteMinHeap temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyIngredienti(MinHeapIngrediente* heap, int i) {
    while (true) {
        int piuPiccolo = i;
        int sinistra = 2 * i + 1;
        int destra = 2 * i + 2;
        if (sinistra < heap->dimensione && heap->lotto[sinistra].scadenza < heap->lotto[piuPiccolo].scadenza) {
            piuPiccolo = sinistra;
        }
        if (destra < heap->dimensione && heap->lotto[destra].scadenza < heap->lotto[piuPiccolo].scadenza) {
            piuPiccolo = destra;
        }
        if (piuPiccolo != i) {
            scambia(&heap->lotto[i], &heap->lotto[piuPiccolo]);
            i = piuPiccolo;
        } else {
            break;
        }
    }
}

void inserisciIngrediente(MinHeapIngrediente* heap, int scadenza, int quantita) {
    //printf("Inserendo Ingrediente: Scadenza = %d, Quantita = %d\n", scadenza, quantita); // Debugging
    if (heap->dimensione == heap->capacita) {
        heap->capacita *= 2;
        heap->lotto = (IngredienteMinHeap*) realloc(heap->lotto, heap->capacita * sizeof(IngredienteMinHeap));
    }
    int i = heap->dimensione++;
    heap->lotto[i].scadenza = scadenza;
    heap->lotto[i].quantita = quantita;
    while (i != 0 && heap->lotto[(i - 1) / 2].scadenza > heap->lotto[i].scadenza) {
        scambia(&heap->lotto[i], &heap->lotto[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

IngredienteMinHeap rimuoviIngrediente(MinHeapIngrediente* heap) {
    if (heap->dimensione == 0) {
        IngredienteMinHeap ingredienteVuoto = { INT_MAX, 0 };
        return ingredienteVuoto;
    }
    IngredienteMinHeap radice = heap->lotto[0];
    heap->lotto[0] = heap->lotto[--heap->dimensione];
    heapifyIngredienti(heap, 0);
    return radice;
}

MinHeapIngrediente nuovoHeapIngredienti(int capacita) {
    MinHeapIngrediente heap;
    heap.lotto = (IngredienteMinHeap*) malloc(capacita * sizeof(IngredienteMinHeap));
    if (heap.lotto == NULL) {
        fprintf(stderr, "Errore di allocazione della memoria per lotto\n");
        exit(EXIT_FAILURE);
    }
    heap.dimensione = 0;
    heap.capacita = capacita;
    return heap;
}

void liberaLotto(MinHeapIngrediente* heap) {
    if (heap != NULL && heap->lotto != NULL) {
        free(heap->lotto);
        heap->lotto = NULL;
        heap->dimensione = 0;
        heap->capacita = 0;
        //printf("Heap ingredienti liberato\n");
    }
}

//Funzioni AVL di min-Heap - Ingredienti
void liberaAVL(NodoAVL *root){
    if(root!= NULL) {
        liberaAVL(root->sinistro);
        liberaAVL(root->destro);
        liberaLotto(&root->heap);
        free(root);
        }
}

int valBilancia(NodoAVL *nodo){
    if(nodo==NULL)
        return 0;
    return altezza(nodo->sinistro)-altezza(nodo->destro);
}

int altezza(NodoAVL *nodo) {
    if(nodo==NULL)
        return 0;
    return nodo->altezza;
}

NodoAVL* nuovoAVL(char* nome, int capacita) {
    NodoAVL* nodo = (NodoAVL*)malloc(sizeof(NodoAVL));
    strcpy(nodo->nome, nome);
    nodo->heap = nuovoHeapIngredienti(capacita);
    nodo->sinistro = NULL;
    nodo->destro = NULL;
    nodo->altezza = 1;
    return nodo;
}

NodoAVL* ruotaDestra(NodoAVL *y) {
    NodoAVL *x= y->sinistro;
    NodoAVL *z = x->destro;
    x-> destro=y;
    y-> sinistro= z;
    y->altezza=max(altezza(y->sinistro), altezza((y->destro)))+1;
    x->altezza=max(altezza(y->sinistro), altezza((y->destro)))+1;
    return x;
}

NodoAVL* ruotaSinistra(NodoAVL *x) {
    NodoAVL *y= x->destro;
    NodoAVL *z = y->sinistro;
    y-> sinistro= x;
    x-> destro= z;
    x->altezza=max(altezza(x->sinistro), altezza((x->destro)))+1;
    y->altezza=max(altezza(y->sinistro), altezza((y->destro)))+1;
    return y;
}

NodoAVL* minValueAVL(NodoAVL *nodo) {
    NodoAVL *corrente=nodo;
    while(corrente->sinistro!=NULL)
        corrente=corrente->sinistro;
    return corrente;
}

NodoAVL* eliminaAVL(NodoAVL* root, char *nome) {
    if (root == NULL)
        return root;
    if (strcmp(nome, root->nome) < 0) {
        root->sinistro = eliminaAVL(root->sinistro, nome);
    } else if (strcmp(nome, root->nome) > 0) {
        root->destro = eliminaAVL(root->destro, nome);
    } else {
        if (root->sinistro == NULL || root->destro == NULL) {
            NodoAVL *temp = root->sinistro ? root->sinistro : root->destro;
            liberaLotto(&root->heap);  // Libera la memoria del lotto
            if (temp == NULL) {
                free(root);
                return NULL;
            } else {
                *root = *temp; // Copia il contenuto del nodo non vuoto
                free(temp);
            }
        } else {
            NodoAVL* temp = minValueAVL(root->destro);
            strcpy(root->nome, temp->nome);
            root->heap = temp->heap;
            root->destro = eliminaAVL(root->destro, temp->nome);
        }
    }
    root->altezza = 1 + max(altezza(root->sinistro), altezza(root->destro));
    int bilancio = valBilancia(root);
    if (bilancio > 1 && valBilancia(root->sinistro) >= 0)
        return ruotaDestra(root);
    if (bilancio > 1 && valBilancia(root->sinistro) < 0) {
        root->sinistro = ruotaSinistra(root->sinistro);
        return ruotaDestra(root);
    }
    if (bilancio < -1 && valBilancia(root->destro) <= 0)
        return ruotaSinistra(root);
    if (bilancio < -1 && valBilancia(root->destro) > 0) {
        root->destro = ruotaDestra(root->destro);
        return ruotaSinistra(root);
    }
    return root;
}

NodoAVL* cercaAVL(NodoAVL* nodo, const char* nome) {
    //print_string_info(nome);
    if (nodo == NULL || strcmp(nodo->nome, nome) == 0) {
        //printf("Nodo trovato per ingrediente: %s\n", nome);
        return nodo;
    }
    //printf("Confronto nome ingrediente: %s con nodo corrente: %s\n", nome, nodo->nome);
    if (strcmp(nome, nodo->nome) < 0) {
        return cercaAVL(nodo->sinistro, nome);
    }
    return cercaAVL(nodo->destro, nome);
}


NodoAVL* inserisciAVL(NodoAVL* nodo, char* nome, int scadenza, int quantita, int capacita) {
    //print_string_info(nome);
    if (nodo == NULL) {
        //printf("Creazione nuovo nodo per ingrediente: %s\n", nome);
        NodoAVL* nuovo = nuovoAVL(nome, capacita);
        inserisciIngrediente(&nuovo->heap, scadenza, quantita);
        return nuovo;
    }
    //printf("Confronto nome ingrediente: %s con nodo corrente: %s\n", nome, nodo->nome);
    if (strcmp(nome, nodo->nome) < 0) {
        nodo->sinistro = inserisciAVL(nodo->sinistro, nome, scadenza, quantita, capacita);
    } else if (strcmp(nome, nodo->nome) > 0) {
        nodo->destro = inserisciAVL(nodo->destro, nome, scadenza, quantita, capacita);
    } else {
        //printf("Aggiunta ingrediente esistente al nodo AVL: %s\n", nome);
        inserisciIngrediente(&nodo->heap, scadenza, quantita);
    }
    //verificaInserimento(nodo, nome);
    nodo->altezza = 1 + max(altezza(nodo->sinistro), altezza(nodo->destro));
    return bilanciaAVL(nodo);
}

NodoAVL* bilanciaAVL(NodoAVL* nodo) {
    int bilancio = valBilancia(nodo);
    if (bilancio > 1) {
        if (valBilancia(nodo->sinistro) < 0) {
            nodo->sinistro = ruotaSinistra(nodo->sinistro);
        }
        return ruotaDestra(nodo);
    }
    if (bilancio < -1) {
        if (valBilancia(nodo->destro) > 0) {
            nodo->destro = ruotaDestra(nodo->destro);
        }
        return ruotaSinistra(nodo);
    }
    return nodo;
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
    if(heap->dimensione==heap->capacita) {
        heap->capacita*=2;
        heap->ordini=(OrdineHeap*)realloc(heap->ordini, heap->capacita*sizeof(OrdineHeap));
    }
    int i=heap->dimensione++;
    heap->ordini[i].tempo_arrivo=tempo_arrivo;
    strcpy(heap->ordini[i].ricetta, ricetta);
    heap->ordini[i].quantita=quantita;
    while(i!=0 && heap->ordini[(i-1)/2].tempo_arrivo > heap->ordini[i].tempo_arrivo) {
        scambiaOrdini(&heap->ordini[i], &heap->ordini[(i-1)/2]);
        i=(i-1)/2;
    }
}

int heapVuotoMinOrdine(MinHeap* heap) {
    return heap->dimensione==0;
}

MinHeap* creaMinHeap(int capacita) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->dimensione = 0;
    heap->capacita = capacita;
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
