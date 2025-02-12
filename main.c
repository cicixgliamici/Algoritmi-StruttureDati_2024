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
    printf("Hello World");

    /* Initialize hash tables */
    ingredientHashTable = createIngredientHashTable(INITIAL_TABLE_SIZE);
    recipeHashTable = createRecipeHashTable(INITIAL_TABLE_SIZE);

    /* Use standard input as the command file */
    FILE *file = stdin;
    processCommands(file);
    fclose(file);
    return 0;
}
