/* Gestione degli ordini       ordine:             se non esiste la ricetta, lo rifiuto, altrimenti prima lo aggiungo in coda
*                                                  poi ne verifico la fattibilita ed eventualmente lo preparo
*
*                              fattibilita:        verifica la fattibilita dell'ordine guardando la quantita totale di ingredienti
*                                                  nell'AVL, senza togliere nulla
*
*                              preparazione:       prepara effetivamente l'ordine togliendo sempre gli ingredienti con scadenza minore
*                                                  svuota anche l'heap degli ingredienti scaduti
*
*                              verificaOrdini:     sostanzialmente è la funzione ordine ma chiamata su tutta la coda, usata quando si fa
*                                                  rifornimento
*
*                              caricaCamion:       passa gli ordini dal minHeap (istante arrivo) al maxHeap (peso), si ferma al primo che supera la
*                                                  capacità restante
*
*                              calcolaPeso:        calcola il peso dell'ordine in toto
*
*                              controllaScadenza:  controlla se la scadenza del primo elemento del minHeap è minore del tempoCorrente, se si rimuove
*                                                  e continua a controllare
*/
#include "header.h"
#include <stdio.h>
#include <stdlib.h>

void ordine(const char* nome_ricetta, int numero_elementi_ordinati) {
    NodoBST* nodo_ricetta = cercaBST(bst, (char*)nome_ricetta);
    if (nodo_ricetta == NULL) {
        printf("rifiutato\n");
        return;
    }
    controllaScadenzaAVL(avl);
    if (fattibilita(nome_ricetta, numero_elementi_ordinati)) {
        preparazione(nome_ricetta, numero_elementi_ordinati, tempoCorrente);
        printf("accettato\n");
    } else {
        aggiungiCoda(coda_ordini, (char*)nome_ricetta, numero_elementi_ordinati, tempoCorrente);
        //stampaCodaFIFO(coda_ordini);
        printf("accettato\n");
    }
}

bool fattibilita(const char* nome_ricetta, int numero_elementi_ordinati) {
    NodoBST* nodo_ricetta = cercaBST(bst, (char*)nome_ricetta);
    IngredienteRicetta* ing = nodo_ricetta->ricetta.ingredienti;
    while (ing != NULL) {
        NodoAVL* nodo_ingrediente = cercaAVL(avl, ing->nome);
        if (nodo_ingrediente == NULL) {
            return false;
        }
        MinHeapIngrediente* heap = &nodo_ingrediente->heap;
        int quantita_totale = 0;
        for (int i = 0; i < nodo_ingrediente->heap.dimensione; i++) {
            quantita_totale += heap->lotto[i].quantita;
        }
        int peso_totale_richiesto = ing->quantita * numero_elementi_ordinati;
        if (quantita_totale < peso_totale_richiesto) {
            return false;
        }
        ing = ing->next;
    }
    return true;
}

void preparazione(const char* nome_ricetta, int numero_elementi_ordinati, int tempo_arrivo) {
    NodoBST* nodo_ricetta = cercaBST(bst, (char*)nome_ricetta);
    IngredienteRicetta* ing = nodo_ricetta->ricetta.ingredienti;
    while (ing != NULL) {
        NodoAVL* nodo_ingrediente = cercaAVL(avl, ing->nome);
        int quantita_richiesta = ing->quantita * numero_elementi_ordinati;
        while (quantita_richiesta > 0) {
            if (nodo_ingrediente->heap.dimensione == 0) {
                return; // Non ci sono abbastanza ingredienti
            }
            IngredienteMinHeap min_ingrediente = rimuoviIngrediente(&nodo_ingrediente->heap);
            if (min_ingrediente.quantita <= quantita_richiesta) {
                quantita_richiesta -= min_ingrediente.quantita;
            } else {
                min_ingrediente.quantita -= quantita_richiesta;
                quantita_richiesta = 0;
                inserisciIngrediente(&nodo_ingrediente->heap, min_ingrediente.scadenza, min_ingrediente.quantita, nodo_ingrediente);
            }
        }
        ing = ing->next;
    }
    inserisciOrdineHeap(heap_ordini_fatti, tempo_arrivo, (char*)nome_ricetta, numero_elementi_ordinati);
}

void verificaOrdini() {
    controllaScadenzaAVL(avl);
    Ordine* ordineCorrente = coda_ordini->testa;
    Ordine* precedente = NULL;
    while (ordineCorrente != NULL) {
        if (fattibilita(ordineCorrente->nome_ricetta, ordineCorrente->quantita)) {
            preparazione(ordineCorrente->nome_ricetta, ordineCorrente->quantita, ordineCorrente->tempo_arrivo);
            if (precedente == NULL) {
                coda_ordini->testa = ordineCorrente->next;
                if (coda_ordini->testa == NULL) {
                    coda_ordini->coda = NULL;
                }
            } else {
                precedente->next = ordineCorrente->next;
                if (precedente->next == NULL) {
                    coda_ordini->coda = precedente;
                }
            }
            Ordine* temp = ordineCorrente;
            ordineCorrente = ordineCorrente->next;
            free(temp);
        } else {
            precedente = ordineCorrente;
            ordineCorrente = ordineCorrente->next;
        }
    }
}

void caricaCamion() {
    if (heapVuotoMinOrdine(heap_ordini_fatti)) {
        printf("camioncino vuoto\n");
        return;
    }
    int capienzaRestante = capienzaCamion;  // Usa capienzaCamion invece di max_heap_spedizioni->capacita
    MinHeap* tempHeap = creaMinHeap(heap_ordini_fatti->capacita);
    while (!heapVuotoMinOrdine(heap_ordini_fatti)) {
        OrdineHeap ordine = rimuoviMin(heap_ordini_fatti);
        NodoBST* nodo_ricetta = cercaBST(bst, ordine.ricetta);
        int peso_ordine = calcolaPeso(nodo_ricetta->ricetta, ordine.quantita);
        if (peso_ordine <= capienzaRestante) {
            inserisciSpedizione(max_heap_spedizioni, ordine.ricetta, ordine.tempo_arrivo, ordine.quantita, peso_ordine);
            capienzaRestante -= peso_ordine;
        } else {
            inserisciOrdineHeap(tempHeap, ordine.tempo_arrivo, ordine.ricetta, ordine.quantita);
            break;
        }
    }
    while (!heapVuotoMinOrdine(heap_ordini_fatti)) {
        OrdineHeap ordine = rimuoviMin(heap_ordini_fatti);
        inserisciOrdineHeap(tempHeap, ordine.tempo_arrivo, ordine.ricetta, ordine.quantita);
    }
    while (!heapVuotoMinOrdine(tempHeap)) {
        OrdineHeap ordine = rimuoviMin(tempHeap);
        inserisciOrdineHeap(heap_ordini_fatti, ordine.tempo_arrivo, ordine.ricetta, ordine.quantita);
    }
    liberaMinHeapOrdini(tempHeap);
    if (!heapVuotoMax(max_heap_spedizioni)) {
        while (!heapVuotoMax(max_heap_spedizioni)) {
            Spedizione spedizione = rimuoviMax(max_heap_spedizioni);
            printf("%d %s %d\n", spedizione.istante_arrivo, spedizione.nome, spedizione.quantita);
        }
    }
}


int calcolaPeso(Ricetta ricetta, int numero_elementi_ordinati) {
    int peso = 0;
    IngredienteRicetta* ingrediente = ricetta.ingredienti;
    while (ingrediente != NULL) {
        peso += ingrediente->quantita;
        ingrediente = ingrediente->next;
    }
    peso *= numero_elementi_ordinati;
    return peso;
}

void controllaScadenza(NodoAVL* nodo_ingrediente) {
    if (nodo_ingrediente == NULL) {
        return;
    }
    MinHeapIngrediente* heap = &nodo_ingrediente->heap;
    int i = 0;
    while (i < heap->dimensione) {
        if (heap->lotto[i].scadenza <= tempoCorrente) {
            heap->lotto[i] = heap->lotto[--heap->dimensione];
            heapifyIngredienti(heap, i);
        } else {
            i++;
        }
    }
}

void controllaScadenzaAVL(NodoAVL* nodo) {
    if (nodo == NULL) {
        return;
    }
    controllaScadenzaAVL(nodo->sinistro);
    controllaScadenza(nodo);
    controllaScadenzaAVL(nodo->destro);
}
