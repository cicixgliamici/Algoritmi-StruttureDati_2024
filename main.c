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
    /* Initialize hash tables */
    ingredientHashTable = createIngredientHashTable(INITIAL_TABLE_SIZE);
    recipeHashTable = createRecipeHashTable(INITIAL_TABLE_SIZE);

    /* Use standard input as the command file */
    processCommands(stdin);

    return 0;
}
