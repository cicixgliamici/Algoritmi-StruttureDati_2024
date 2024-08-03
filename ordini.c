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
#include <string.h>

void ordine(const char* nome_ricetta, int numero_elementi_ordinati) {
    NodoBST* nodo_ricetta = cercaBST(bst, (char*)nome_ricetta);
    if (nodo_ricetta == NULL) {
        printf("rifiutato\n");
        return;
    }
    if (fattibilita(nome_ricetta, numero_elementi_ordinati)) {
        preparazione(nome_ricetta, numero_elementi_ordinati, tempoCorrente);
        printf("accettato\n");
    } else {
        aggiungiCoda(coda_ordini, (char*)nome_ricetta, numero_elementi_ordinati, tempoCorrente);
        printf("accettato\n");
    }
}

bool fattibilita(const char* nome_ricetta, int numero_elementi_ordinati) {
    NodoBST* nodo_ricetta = cercaBST(bst, (char*)nome_ricetta);
    if (nodo_ricetta == NULL) return false;
    IngredienteRicetta* ing = nodo_ricetta->ricetta.ingredienti;
    while (ing != NULL) {
        NodoTreap* nodo_ingrediente = cercaTreap(trap, ing->nome);
        int quantita_totale = 0;
        while (nodo_ingrediente != NULL) {
            if (nodo_ingrediente->scadenza > tempoCorrente) {
                quantita_totale += nodo_ingrediente->quantita;
            }
            nodo_ingrediente = (strcmp(ing->nome, nodo_ingrediente->nome) < 0) ? nodo_ingrediente->sinistro : nodo_ingrediente->destro;
        }
        if (quantita_totale < ing->quantita * numero_elementi_ordinati) {
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
        int quantita_richiesta = ing->quantita * numero_elementi_ordinati;
        while (quantita_richiesta > 0) {
            NodoTreap* nodo_ingrediente = cercaTreap(trap, ing->nome);
            if (nodo_ingrediente == NULL || nodo_ingrediente->quantita == 0) {
                return; // Non ci sono abbastanza ingredienti
            }
            if (nodo_ingrediente->scadenza <= tempoCorrente) {
                trap = eliminaTreap(trap, nodo_ingrediente->nome);
                nodo_ingrediente = cercaTreap(trap, ing->nome);
                continue;
            }
            if (nodo_ingrediente->quantita <= quantita_richiesta) {
                quantita_richiesta -= nodo_ingrediente->quantita;
                trap = eliminaTreap(trap, nodo_ingrediente->nome);
            } else {
                nodo_ingrediente->quantita -= quantita_richiesta;
                quantita_richiesta = 0;
            }
        }
        ing = ing->next;
    }
    inserisciOrdineHeap(heap_ordini_fatti, tempo_arrivo, (char*)nome_ricetta, numero_elementi_ordinati);
}

void verificaOrdini() {
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
    int capienzaRestante = capienzaCamion;
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

/**
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
**/