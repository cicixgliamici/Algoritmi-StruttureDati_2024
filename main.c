#include "header.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Funzioni generiche
int max(int a, int b) {
    return (a>b) ? a : b;
}

//Funzioni Algoritmo
void gestisciComandi(const int tempo_camioncino, const int capienza_camioncino) {
    char comando[256];
    while (scanf("%s", comando) != EOF) {
        if (strcmp(comando, "aggiungi_ricetta") == 0) {
            char nome_ricetta[256];
            char nome_ingrediente[256];
            int quantita;
            scanf("%s", nome_ricetta);
            while (scanf("%s %d", nome_ingrediente, &quantita) == 2) {
                aggiungi_ricetta(nome_ricetta, nome_ingrediente, quantita);
            }
        } else if (strcmp(comando, "rimuovi_ricetta") == 0) {
            char nome_ricetta[256];
            scanf("%s", nome_ricetta);
            rimuovi_ricetta(nome_ricetta);
        } else if (strcmp(comando, "rifornimento") == 0) {
            char nome_ingrediente[256];
            int quantita, scadenza;
            scanf("%s %d %d", nome_ingrediente, &quantita, &scadenza);
            rifornimento(nome_ingrediente, quantita, scadenza);
        } else if (strcmp(comando, "ordine") == 0) {
            char nome_ricetta[256];
            int numero_elementi_ordinati;
            scanf("%s %d", nome_ricetta, &numero_elementi_ordinati);
            ordine(nome_ricetta, numero_elementi_ordinati);
        }
    }
}

void aggiungi_ricetta(const char* nome_ricetta, const char* nome_ingrediente, int quantita) {
    printf("Aggiungi ricetta: %s, Ingrediente: %s, Quantita: %d\n", nome_ricetta, nome_ingrediente, quantita);
}

void rimuovi_ricetta(const char* nome_ricetta) {
    printf("Rimuovi ricetta: %s\n", nome_ricetta);
}

void rifornimento(const char* nome_ingrediente, int quantita, int scadenza) {
    printf("Rifornimento: Ingrediente: %s, Quantita: %d, Scadenza: %d\n", nome_ingrediente, quantita, scadenza);
}

void ordine(const char* nome_ricetta, int numero_elementi_ordinati) {
    printf("Ordine: Ricetta: %s, Numero elementi ordinati: %d\n", nome_ricetta, numero_elementi_ordinati);
}

//Main - Gestione del giorno
int main(void){
    int temp1, temp2;
    int t=0;
    scanf("%d %d", &temp1, &temp2);
    const int TEMPO_CAMIONCINO = temp1;
    const int CAPIENZA_CAMIONCINO = temp2;
    //testGenerale();
    return 0;
}
