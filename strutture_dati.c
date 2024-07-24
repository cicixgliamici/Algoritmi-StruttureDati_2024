#include "header.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Funzioni min-Heap - Ingredienti
void heapifyIngredienti(MinHeapIngrediente* heap, int i) {
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
        heapifyIngredienti(heap, minore);
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
    heapifyIngredienti(heap,0);
    return root;
}

MinHeapIngrediente nuovoHeapIngredienti(int capacita) {
    MinHeapIngrediente heap;
    heap.lotto=(IngredienteMinHeap*)malloc(capacita*sizeof(IngredienteMinHeap));
    heap.dimensione=0;
    heap.capacita=capacita;
    return heap;
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
    nodo->sinistro=nodo->destro=NULL;
    nodo->altezza=1;
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
    if(root==NULL)
        return root;
    if(strcmp(nome, root->nome)<0)
        root->sinistro=eliminaAVL(root->sinistro, nome);
    else if(strcmp(nome, root->nome)>0)
        root->destro=eliminaAVL(root->destro, nome);
    else {
        if((root->sinistro==NULL) || (root->destro==NULL)) {
            NodoAVL *temp=root->sinistro ? root->sinistro : root->destro;
            if(temp==NULL) {
                temp=root;
                root=NULL;
            }
            else
                *root=*temp;
            free(temp);
        }
        else {
            NodoAVL* temp = minValueAVL(root->destro);
            strcpy(root->nome, temp->nome);
            root->heap=temp->heap;
            root->destro=eliminaAVL(root->destro, temp->nome);
        }
    }
    if (root==NULL)
        return root;
    root->altezza= 1 + max(altezza(root->sinistro), altezza(root->destro));
    int bilancio = valBilancia(root);
    if (bilancio> 1 && valBilancia(root->sinistro)>= 0)
        return ruotaDestra(root);
    if (bilancio> 1 && valBilancia(root->sinistro)< 0) {
        root->sinistro = ruotaSinistra(root->sinistro);
        return ruotaDestra(root);
    }
    if (bilancio< -1 && valBilancia(root->destro)<= 0)
        return ruotaSinistra(root);
    if (bilancio< -1 && valBilancia(root->destro)> 0) {
        root->destro = ruotaDestra(root->destro);
        return ruotaSinistra(root);
    }
    return root;
}

NodoAVL* inserisciAVL(NodoAVL* nodo, char* nome, int scadenza, int quantita, int capacita) {
        if (nodo == NULL)
            return nuovoAVL(nome, capacita);
        if (strcmp(nome, nodo->nome) < 0)
            nodo->sinistro=inserisciAVL(nodo->sinistro, nome, scadenza, quantita, capacita);
        else if (strcmp(nome, nodo->nome) > 0)
            nodo->destro=inserisciAVL(nodo->destro, nome, scadenza, quantita, capacita);
        else {
            inserisciIngrediente(&nodo->heap, scadenza, quantita);
            return nodo;
        }
        nodo->altezza = 1 + max(altezza(nodo->sinistro), altezza(nodo->destro));
        int bilancio= valBilancia(nodo);
        if (bilancio> 1 && strcmp(nome, nodo->sinistro->nome) < 0)
            return ruotaDestra(nodo);
        if (bilancio< -1 && strcmp(nome, nodo->destro->nome) > 0)
            return ruotaSinistra(nodo);
        if (bilancio> 1 && strcmp(nome, nodo->sinistro->nome) > 0) {
            nodo->sinistro=ruotaSinistra(nodo->sinistro);
            return ruotaDestra(nodo);
        }
        if (bilancio< -1 && strcmp(nome, nodo->destro->nome) < 0) {
            nodo->destro=ruotaDestra(nodo->destro);
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

void aggiungiCoda(CodaOrdini* coda, char* nome_ricetta, int quantita, int tempo_arrivo) {
    Ordine* nuovoOrdine=(Ordine*)malloc(sizeof(Ordine));
    strcpy(nuovoOrdine->nome_ricetta, nome_ricetta);
    nuovoOrdine->quantita=quantita;
    nuovoOrdine->tempo_arrivo=tempo_arrivo;
    nuovoOrdine->next=NULL;
    if(coda->coda==NULL) {
        coda->testa=nuovoOrdine;
        coda->coda=nuovoOrdine;
    }
    else {
        coda->coda->next=nuovoOrdine;
        coda->coda=nuovoOrdine;
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
    if(coda->testa == NULL)
        return NULL;
    Ordine* ordineRimosso = coda->testa;
    coda->testa = coda->testa->next;
    if(coda->testa == NULL)
        coda->coda = NULL;
    return ordineRimosso;
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
        int minore=i;
        int sx= 2*i+1;
        int dx= 2*i+2;
        if(sx<heap->dimensione && heap->ordini[sx].tempo_arrivo< heap->ordini[minore].tempo_arrivo)
            minore=sx;
        if(dx<heap->dimensione && heap->ordini[dx].tempo_arrivo< heap->ordini[minore].tempo_arrivo)
            minore=dx;
        if(minore!=i) {
            OrdineHeap temp = heap->ordini[i];
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

int heapVuoto(MinHeap* heap) {
    return heap->dimensione==0;
}

MinHeap* creaMinHeap(int capacita) {
    MinHeap* heap=(MinHeap*)malloc(sizeof(MinHeap));
    heap->ordini=(OrdineHeap*)malloc(capacita*sizeof(OrdineHeap));
    heap->dimensione=0;
    heap->capacita=capacita;
    return heap;
}

OrdineHeap rimuoviMin(MinHeap* heap) {
    if (heap->dimensione <= 0) {
        OrdineHeap ordineVuoto = {0, "", 0};
        return ordineVuoto;
    }
    OrdineHeap root = heap->ordini[0];
    if (heap->dimensione == 1) {
        heap->dimensione--;
        return root;
    }
    heap->ordini[0] = heap->ordini[--heap->dimensione];
    heapifyOrdini(heap, 0);
    return root;
}


//Funzioni maxHeap - Camioncino Spedizioni
void heapifySpedizioni(MaxHeapSpedizioni* heap, int i) {
    int maggiore = i;
    int sinistro = 2 * i + 1;
    int destro = 2 * i + 2;
    if (sinistro < heap->dimensione && heap->spedizioni[sinistro].peso > heap->spedizioni[maggiore].peso)
        maggiore = sinistro;
    if (destro < heap->dimensione && heap->spedizioni[destro].peso > heap->spedizioni[maggiore].peso)
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
    heap->spedizioni[i].istante_arrivo= istante_arrivo;
    heap->spedizioni[i].quantita= quantita;
    heap->spedizioni[i].peso= peso;
    while (i!=0 && heap->spedizioni[(i-1)/2].peso < heap->spedizioni[i].peso) {
        Spedizione temp=heap->spedizioni[i];
        heap->spedizioni[i]=heap->spedizioni[(i - 1) / 2];
        heap->spedizioni[(i-1)/2] = temp;
        i=(i-1)/2;
    }
}

void liberaMaxHeap(MaxHeapSpedizioni* heap) {
    free(heap->spedizioni);
}

MaxHeapSpedizioni creaMaxHeap(int capacita) {
    MaxHeapSpedizioni heap;
    heap.spedizioni=(Spedizione*) malloc(capacita * sizeof(Spedizione));
    heap.dimensione=0;
    heap.capacita=capacita;
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