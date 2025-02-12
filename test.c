#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

/* ----------------------- IngredientMinHeap Tests ----------------------- */
void testIngredientMinHeap() {
    printf("Testing IngredientMinHeap...\n");
    // Create a min-heap with an initial capacity of 4
    IngredientMinHeap heap = createIngredientMinHeap(4);
    insertIngredient(&heap, 10, 5);
    insertIngredient(&heap, 5, 3);
    insertIngredient(&heap, 7, 2);

    // The smallest expiration is 5
    IngredientHeapNode node = removeIngredient(&heap);
    assert(node.expiration == 5 && node.quantity == 3);
    // Total quantity should now be 7 (5 + 2)
    assert(heap.total_quantity == 7);

    node = removeIngredient(&heap);
    assert(node.expiration == 7 && node.quantity == 2);

    node = removeIngredient(&heap);
    assert(node.expiration == 10 && node.quantity == 5);

    printf("IngredientMinHeap tests passed.\n");
}

/* ----------------------- IngredientHashTable Tests ----------------------- */
void testIngredientHashTable() {
    printf("Testing IngredientHashTable...\n");
    // Create a hash table with 10 buckets
    IngredientHashTable* table = createIngredientHashTable(10);
    insertIngredientHash(table, "Tomato", 15, 10);
    // Inserimento di un nuovo lotto per lo stesso ingrediente
    insertIngredientHash(table, "Tomato", 12, 5);

    IngredientHashNode* node = searchIngredientHash(table, "Tomato");
    assert(node != NULL);
    // La quantità totale deve essere la somma dei lotti: 10 + 5 = 15
    assert(node->heap.total_quantity == 15);
    printf("IngredientHashTable tests passed.\n");
}

/* ----------------------- RecipeHashTable Tests ----------------------- */
void testRecipeHashTable() {
    printf("Testing RecipeHashTable...\n");
    RecipeHashTable* table = createRecipeHashTable(10);
    Recipe recipe;
    strcpy(recipe.name, "Pasta");
    recipe.ingredients = NULL;

    // Creiamo un ingrediente per la ricetta.
    RecipeIngredient* ing = malloc(sizeof(RecipeIngredient));
    strcpy(ing->name, "Tomato");
    ing->quantity = 2;
    ing->next = NULL;
    recipe.ingredients = ing;

    insertRecipeHash(table, recipe);
    RecipeHashNode* node = searchRecipeHash(table, "Pasta");
    assert(node != NULL);
    assert(strcmp(node->recipe.name, "Pasta") == 0);
    assert(node->recipe.ingredients != NULL);
    assert(strcmp(node->recipe.ingredients->name, "Tomato") == 0);
    printf("RecipeHashTable tests passed.\n");

    // Pulizia della lista degli ingredienti per la ricetta
    free(ing);
}

/* ----------------------- OrderQueue Tests ----------------------- */
void testOrderQueue() {
    printf("Testing OrderQueue...\n");
    OrderQueue* queue = createOrderQueue();
    enqueueOrder(queue, "Pasta", 3, 1);
    enqueueOrder(queue, "Salad", 2, 2);

    Order* order = dequeueOrder(queue);
    assert(order != NULL);
    assert(strcmp(order->recipeName, "Pasta") == 0);
    free(order);

    order = dequeueOrder(queue);
    assert(order != NULL);
    assert(strcmp(order->recipeName, "Salad") == 0);
    free(order);

    printf("OrderQueue tests passed.\n");
}

/* ----------------------- MinOrderHeap (Processed Orders) Tests ----------------------- */
void testMinOrderHeap() {
    printf("Testing MinOrderHeap...\n");
    MinOrderHeap* heap = createMinOrderHeap(4);
    insertOrderHeap(heap, 10, "Pasta", 2);
    insertOrderHeap(heap, 5, "Salad", 3);
    insertOrderHeap(heap, 8, "Soup", 1);

    ProcessedOrder order = removeMinOrder(heap);
    assert(order.arrivalTime == 5 && strcmp(order.recipe, "Salad") == 0);
    order = removeMinOrder(heap);
    assert(order.arrivalTime == 8 && strcmp(order.recipe, "Soup") == 0);
    order = removeMinOrder(heap);
    assert(order.arrivalTime == 10 && strcmp(order.recipe, "Pasta") == 0);

    freeMinOrderHeap(heap);
    printf("MinOrderHeap tests passed.\n");
}

/* ----------------------- MaxShipmentHeap Tests ----------------------- */
void testMaxShipmentHeap() {
    printf("Testing MaxShipmentHeap...\n");
    MaxShipmentHeap* heap = createMaxShipmentHeap(4);
    insertShipment(heap, "Pasta", 10, 2, 50);
    insertShipment(heap, "Salad", 5, 3, 70);
    // In caso di pari peso, l'ordine con arrivo minore (più vecchio) ha priorità.
    insertShipment(heap, "Soup", 8, 1, 70);

    Shipment shipment = removeMaxShipment(heap);
    // Tra "Salad" e "Soup" (entrambi peso 70), deve uscire quello con arrivalTime minore ("Salad")
    assert(shipment.weight == 70 && strcmp(shipment.name, "Salad") == 0);
    shipment = removeMaxShipment(heap);
    assert(shipment.weight == 70 && strcmp(shipment.name, "Soup") == 0);
    shipment = removeMaxShipment(heap);
    assert(shipment.weight == 50 && strcmp(shipment.name, "Pasta") == 0);

    printf("MaxShipmentHeap tests passed.\n");
}

/**
/ ----------------------- Main Test Runner ----------------------- /
int main(void) {
    testIngredientMinHeap();
    testIngredientHashTable();
    testRecipeHashTable();
    testOrderQueue();
    testMinOrderHeap();
    testMaxShipmentHeap();
    printf("All tests passed successfully.\n");
    return 0;
}
*/