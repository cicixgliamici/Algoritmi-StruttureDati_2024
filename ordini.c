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
*
*                              calcolaPeso:        calcola il peso dell'ordine in toto
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
        //stampaCodaFIFO(coda_ordini);
        printf("accettato\n");
    }
}

bool fattibilita(const char* nome_ricetta, int numero_elementi_ordinati) {
    /*
    if(tempoCorrente==23) {
        printf("Provando %s\n", nome_ricetta);
    }
    */
    NodoBST* nodo_ricetta = cercaBST(bst, (char*)nome_ricetta);
    IngredienteRicetta* ing = nodo_ricetta->ricetta.ingredienti;
    while (ing != NULL) {
        NodoAVL* nodo_ingrediente = cercaAVL(avl, ing->nome);
        if (nodo_ingrediente == NULL) {
            //printf("postposto %s di elementi %d al tempo %d\n", nome_ricetta, numero_elementi_ordinati, tempoCorrente);
            return false;
        }
        controllaScadenza(nodo_ingrediente);
        int peso_totale_richiesto = ing->quantita * numero_elementi_ordinati;
        if (nodo_ingrediente->peso_totale < peso_totale_richiesto) {
            //printf("postposto %s di elementi %d al tempo %d\n", nome_ricetta, numero_elementi_ordinati, tempoCorrente);
            return false;
        }
        ing = ing->next;
    }
    return true;
}

void preparazione(const char* nome_ricetta, int numero_elementi_ordinati, int tempo_arrivo) {
    /*
    if(tempoCorrente==23) {
        printf("Facendo %s\n", nome_ricetta);
    }
    */
    NodoBST* nodo_ricetta = cercaBST(bst, (char*)nome_ricetta);
    IngredienteRicetta* ing = nodo_ricetta->ricetta.ingredienti;
    while (ing != NULL) {
        NodoAVL* nodo_ingrediente = cercaAVL(avl, ing->nome);
        int quantita_richiesta = ing->quantita * numero_elementi_ordinati;
        while (quantita_richiesta > 0) {
            if (nodo_ingrediente->heap.dimensione == 0) {
                //printf("Errore: ingrediente %s non sufficiente per la ricetta %s\n", ing->nome, nome_ricetta);
                return;
            }
            IngredienteMinHeap min_ingrediente = rimuoviIngrediente(&nodo_ingrediente->heap);
            if (min_ingrediente.quantita <= quantita_richiesta) {
                quantita_richiesta -= min_ingrediente.quantita;
                nodo_ingrediente->peso_totale -= min_ingrediente.quantita;
            } else {
                min_ingrediente.quantita -= quantita_richiesta;
                nodo_ingrediente->peso_totale -= quantita_richiesta;
                quantita_richiesta = 0;
                inserisciIngrediente(&nodo_ingrediente->heap, min_ingrediente.scadenza, min_ingrediente.quantita, nodo_ingrediente);
            }
        }
        ing = ing->next;
    }
    inserisciOrdineHeap(heap_ordini_fatti, tempo_arrivo, (char*)nome_ricetta, numero_elementi_ordinati);
    //printf("fatto %s di elementi %d al tempo %d\n", nome_ricetta, numero_elementi_ordinati, tempoCorrente);
}

void verificaOrdini() {
    Ordine* ordineCorrente = coda_ordini->testa;
    Ordine* precedente = NULL;
    while (ordineCorrente != NULL) {
        //printf("Verifico fattibilita %s\n", ordineCorrente->nome_ricetta);
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
    //printf("Finita verifica ordini\n");
}

void caricaCamion() {
    if (heapVuotoMinOrdine(heap_ordini_fatti)) {
        printf("camioncino vuoto\n");
        return;
    }
    int capienzaRestante = max_heap_spedizioni->capacita;
    //printf("Capienza iniziale del camion: %d\n", capienzaRestante);
    MinHeap* tempHeap = creaMinHeap(heap_ordini_fatti->capacita); // Temp heap to hold orders that can't be loaded

    while (!heapVuotoMinOrdine(heap_ordini_fatti) && capienzaRestante > 0) {
        OrdineHeap ordine = rimuoviMin(heap_ordini_fatti);
        //printf("Ordine rimosso dal min-heap: Ricetta: %s, Quantita: %d, Tempo arrivo: %d\n", ordine.ricetta, ordine.quantita, ordine.tempo_arrivo);
        NodoBST* nodo_ricetta = cercaBST(bst, ordine.ricetta);
        int peso_ordine = calcolaPeso(nodo_ricetta->ricetta, ordine.quantita);
        //printf("Peso dell'ordine calcolato: %d\n", peso_ordine);
        if (peso_ordine <= capienzaRestante) {
            //printf("Ordine inserito nel max-heap delle spedizioni: %s\n", ordine.ricetta);
            inserisciSpedizione(max_heap_spedizioni, ordine.ricetta, ordine.tempo_arrivo, ordine.quantita, peso_ordine);
            capienzaRestante -= peso_ordine;
            //printf("Capienza restante del camion: %d\n", capienzaRestante);
        } else {
            // Reinsert the order back into the temporary heap if it cannot be loaded
            inserisciOrdineHeap(tempHeap, ordine.tempo_arrivo, ordine.ricetta, ordine.quantita);
            //printf("Ordine troppo grande per il camion: %s, Peso: %d, Capienza restante: %d\n", ordine.ricetta, peso_ordine, capienzaRestante);
        }
    }

    // Move the remaining orders back to the original heap
    while (!heapVuotoMinOrdine(tempHeap)) {
        OrdineHeap ordine = rimuoviMin(tempHeap);
        inserisciOrdineHeap(heap_ordini_fatti, ordine.tempo_arrivo, ordine.ricetta, ordine.quantita);
    }
    liberaMinHeapOrdini(tempHeap);
    //printf("Stato min-heap degli ordini fatti dopo caricaCamion:\n");
    //stampaMinHeapOrdini(heap_ordini_fatti);
    if (heapVuotoMax(max_heap_spedizioni)) {
        //printf("Nessuna spedizione caricata nel camion.\n");
    } else {
        //printf("Spedizioni nel camion:\n");
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
    while (nodo_ingrediente->heap.dimensione > 0 && nodo_ingrediente->heap.lotto[0].scadenza == tempoCorrente) {
        IngredienteMinHeap scaduto = rimuoviIngrediente(&nodo_ingrediente->heap);
        nodo_ingrediente->peso_totale -= scaduto.quantita;
        //printf("Rimosso ingrediente scaduto: %s, Quantita: %d, Scadenza: %d\n", nodo_ingrediente->nome, scaduto.quantita, scaduto.scadenza);
    }
}