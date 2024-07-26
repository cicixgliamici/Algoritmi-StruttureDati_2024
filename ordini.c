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
*                              caricaCamion:       passa gli ordini dal minHeap (istante arrivo) al maxHeap (peso), e ne passa tanti quanti
*                                                  qta ordini*peso ordine = capacita camion (o lascia il minimo spazio)
*                              calcolaPeso:        calcola il peso dell'ordine
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
    aggiungiCoda(coda_ordini, (char*)nome_ricetta, numero_elementi_ordinati, tempoCorrente);
    if (fattibilita(nome_ricetta, numero_elementi_ordinati)) {
        preparazione(nome_ricetta, numero_elementi_ordinati);
    }
    printf("accettato\n");
}

bool fattibilita(const char* nome_ricetta, int numero_elementi_ordinati) {
    NodoBST* nodo_ricetta = cercaBST(bst, (char*)nome_ricetta);
    IngredienteRicetta* ing = nodo_ricetta->ricetta.ingredienti;
    while (ing != NULL) {
        NodoAVL* nodo_ingrediente = cercaAVL(avl, ing->nome);
        if (nodo_ingrediente == NULL) {
            return false;
        }
        // Rimuove ingredienti scaduti
        while (nodo_ingrediente->heap.dimensione > 0 && nodo_ingrediente->heap.lotto[0].scadenza <= tempoCorrente) {
            IngredienteMinHeap scaduto = rimuoviIngrediente(&nodo_ingrediente->heap);
            // Va tolto anche il nodo dell'AVL ma non capisco come
            //printf("Rimosso ingrediente scaduto: %s, Quantita: %d, Scadenza: %d\n", nodo_ingrediente->nome, scaduto.quantita, scaduto.scadenza);
        }
        int peso_totale_richiesto = ing->quantita * numero_elementi_ordinati;
        if (nodo_ingrediente->peso_totale < peso_totale_richiesto) {
            return false;
        }
        ing = ing->next;
    }
    return true;
}


void preparazione(const char* nome_ricetta, int numero_elementi_ordinati) {
    NodoBST* nodo_ricetta = cercaBST(bst, (char*)nome_ricetta);
    IngredienteRicetta* ing = nodo_ricetta->ricetta.ingredienti;
    while (ing != NULL) {
        NodoAVL* nodo_ingrediente = cercaAVL(avl, ing->nome);
        int quantita_richiesta = ing->quantita * numero_elementi_ordinati;
        while (quantita_richiesta > 0) {
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
    inserisciOrdineHeap(heap_ordini_fatti, tempoCorrente, (char*)nome_ricetta, numero_elementi_ordinati);
}

void verificaOrdini() {
    Ordine* ordineCorrente = coda_ordini->testa;
    Ordine* precedente = NULL;
    while (ordineCorrente != NULL) {
        if (fattibilita(ordineCorrente->nome_ricetta, ordineCorrente->quantita)) {
            preparazione(ordineCorrente->nome_ricetta, ordineCorrente->quantita);
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
            free(ordineCorrente);
            ordineCorrente = precedente ? precedente->next : coda_ordini->testa;
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
    int capienzaRestante = max_heap_spedizioni->capacita;
    while (!heapVuotoMinOrdine(heap_ordini_fatti) && capienzaRestante > 0) {
        OrdineHeap ordine = rimuoviMin(heap_ordini_fatti);
        NodoBST* nodo_ricetta = cercaBST(bst, ordine.ricetta);
        if (nodo_ricetta == NULL) {
            continue;
        }
        int peso_ordine = calcolaPeso(nodo_ricetta->ricetta, ordine.quantita);
        if (peso_ordine <= capienzaRestante) {
            inserisciSpedizione(max_heap_spedizioni, ordine.ricetta, ordine.tempo_arrivo, ordine.quantita, peso_ordine);
            capienzaRestante -= peso_ordine;
        } else {
            inserisciOrdineHeap(heap_ordini_fatti, ordine.tempo_arrivo, ordine.ricetta, ordine.quantita);
            break;
        }
    }
    while (!heapVuotoMax(max_heap_spedizioni)) {
        Spedizione spedizione = rimuoviMax(max_heap_spedizioni);
        printf("%d %s %d\n", spedizione.istante_arrivo, spedizione.nome, spedizione.quantita);
    }
}

int calcolaPeso(Ricetta ricetta, int numero_elementi_ordinati) {
    int peso = 0;
    IngredienteRicetta* ingrediente = ricetta.ingredienti;
    while (ingrediente != NULL) {
        peso += ingrediente->quantita * numero_elementi_ordinati;
        ingrediente = ingrediente->next;
    }
    return peso;
}