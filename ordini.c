#include "header.h"
#include <stdio.h>
#include <string.h>
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
    if (nodo_ricetta == NULL) {
        return false;
    }
    IngredienteRicetta* ing = nodo_ricetta->ricetta.ingredienti;
    while (ing != NULL) {
        NodoAVL* nodo_ingrediente = cercaAVL(avl, ing->nome);
        if (nodo_ingrediente == NULL) {
            return false;
        }
        int quantita_totale = 0;
        for (int i = 0; i < nodo_ingrediente->heap.dimensione; i++) {
            quantita_totale += nodo_ingrediente->heap.lotto[i].quantita;
        }
        if (quantita_totale < ing->quantita * numero_elementi_ordinati) {
            return false;
        }
        ing = ing->next;
    }
    return true;
}

void preparazione(const char* nome_ricetta, int numero_elementi_ordinati) {
    NodoBST* nodo_ricetta = cercaBST(bst, (char*)nome_ricetta);
    if (nodo_ricetta == NULL) {
        return;
    }
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
                inserisciIngrediente(&nodo_ingrediente->heap, min_ingrediente.scadenza, min_ingrediente.quantita);
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
    if (heapVuotoMax(heap_ordini_fatti)) {
        printf("camioncino vuoto\n");
        return;
    }
    int capienzaRestante = max_heap_spedizioni->capacita;
    while (!heapVuotoMax(heap_ordini_fatti) && capienzaRestante > 0) {
        OrdineHeap ordine = rimuoviMin(heap_ordini_fatti);
        NodoBST* nodo_ricetta = cercaBST(bst, ordine.ricetta);
        if (nodo_ricetta == NULL) {
            continue; // La ricetta non è trovata, passa al prossimo ordine
        }
        int peso_ordine = calcolaPesoOrdine(nodo_ricetta->ricetta, ordine.quantita);
        if (peso_ordine <= capienzaRestante) {
            inserisciSpedizione(max_heap_spedizioni, ordine.ricetta, ordine.tempo_arrivo, ordine.quantita, peso_ordine);
            capienzaRestante -= peso_ordine;
        } else {
            // Non c'è spazio sufficiente per questo ordine, quindi rimetterlo nel heap
            inserisciOrdineHeap(heap_ordini_fatti, ordine.tempo_arrivo, ordine.ricetta, ordine.quantita);
            break;
        }
    }
    // Svuotare il max-Heap e stampare gli ordini caricati
    while (!heapVuotoMax(max_heap_spedizioni)) {
        Spedizione spedizione = rimuoviMax(max_heap_spedizioni);
        printf("%d %s %d\n", spedizione.istante_arrivo, spedizione.nome, spedizione.quantita);
    }
}

int calcolaPesoOrdine(Ricetta ricetta, int numero_elementi_ordinati) {
    int peso = 0;
    IngredienteRicetta* ingrediente = ricetta.ingredienti;
    while (ingrediente != NULL) {
        peso += ingrediente->quantita * numero_elementi_ordinati; // Moltiplica quantità per numero di elementi ordinati
        ingrediente = ingrediente->next;
    }
    return peso;
}
