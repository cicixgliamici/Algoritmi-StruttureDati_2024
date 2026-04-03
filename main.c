#include "header.h"

/* Global variables definitions */
int currentTime = -1;
int truckCapacity;

IngredientHashTable* ingredientHashTable = NULL;
RecipeHashTable* recipeHashTable = NULL;
OrderQueue* orderQueue = NULL;
MinOrderHeap* processedOrderHeap = NULL;
MaxShipmentHeap* maxShipmentHeap = NULL;

int main(void) {
    ingredientHashTable = createIngredientHashTable(INITIAL_TABLE_SIZE);
    recipeHashTable = createRecipeHashTable(INITIAL_TABLE_SIZE);

    processCommands(stdin);

    freeIngredientHashTable(ingredientHashTable);
    freeRecipeHashTable(recipeHashTable);
    freeOrderQueue(orderQueue);
    freeMinOrderHeap(processedOrderHeap);
    freeMaxShipmentHeap(maxShipmentHeap);

    return 0;
}
