#include "header.h"

// Reads a line from a file (dynamically increasing buffer if necessary)
char* readLine(FILE* file) {
    size_t bufferSize = 256;
    char *line = (char *)malloc(bufferSize);
    if (fgets(line, bufferSize, file) != NULL) {
        size_t len = strlen(line);
        while (len > 0 && line[len - 1] != '\n') {
            bufferSize *= 2;
            line = (char *)realloc(line, bufferSize);
            if (fgets(line + len, bufferSize - len, file) == NULL)
                break;
            len = strlen(line);
        }
        line[strcspn(line, "\n")] = 0; // Remove newline
    } else {
        free(line);
        line = NULL;
    }
    return line;
}

/* ========================================================= */
/*              Command and Order Functions                */
/* ========================================================= */

// Processes commands from the input file
void processCommands(FILE *file) {
    int truckTime, shipmentHeapCapacity = 0;
    
    // Read the truck loading interval and capacity from the first line
    if (fscanf(file, "%d %d", &truckTime, &truckCapacity) != 2);
    
    // Choose a capacity for the shipment max-heap based on truckCapacity
    if (truckCapacity >= 100000) {
        shipmentHeapCapacity = truckCapacity / 1000;
    } else {
        shipmentHeapCapacity = truckCapacity;
    }
    maxShipmentHeap = createMaxShipmentHeap(shipmentHeapCapacity);
    
    orderQueue = createOrderQueue();
    processedOrderHeap = createMinOrderHeap(INITIAL_HEAP_CAPACITY);
    
    char *line;
    // Read each command line
    while ((line = readLine(file)) != NULL) {
        char command[20];
        if (sscanf(line, "%19s", command) == 1) {
            currentTime++;
            // At each truckTime interval (except time 0), load the truck
            if (currentTime % truckTime == 0 && currentTime != 0) {
                loadTruck();
            }
            
            if (strcmp(command, "add_recipe") == 0) {
                char recipeName[25];
                if (sscanf(line + strlen(command), "%24s", recipeName) == 1) {
                    if (searchRecipeHash(recipeHashTable, recipeName) != NULL) {
                        printf("ignored\n");
                    } else {
                        Recipe new_recipe;
                        strncpy(new_recipe.name, recipeName, sizeof(new_recipe.name) - 1);
                        new_recipe.name[sizeof(new_recipe.name) - 1] = '\0';
                        new_recipe.ingredients = NULL;
                        RecipeIngredient *tail = NULL;
                        char ingredient[21];
                        int quantity;
                        char *ptr = line + strlen(command) + strlen(recipeName) + 1;
                        // Parse pairs: ingredient name and quantity
                        while (sscanf(ptr, "%20s %d", ingredient, &quantity) == 2) {
                            RecipeIngredient *newIngredient = (RecipeIngredient*)malloc(sizeof(RecipeIngredient));
                            strncpy(newIngredient->name, ingredient, sizeof(newIngredient->name) - 1);
                            newIngredient->name[sizeof(newIngredient->name) - 1] = '\0';
                            newIngredient->quantity = quantity;
                            newIngredient->next = NULL;
                            if (tail == NULL) {
                                new_recipe.ingredients = newIngredient;
                            } else {
                                tail->next = newIngredient;
                            }
                            tail = newIngredient;
                            // Advance the pointer (estimate the space needed)
                            ptr += strlen(ingredient) + 1 + snprintf(NULL, 0, "%d", quantity) + 1;
                        }
                        insertRecipeHash(recipeHashTable, new_recipe);
                        printf("added\n");
                    }
                }
            }
            else if (strcmp(command, "remove_recipe") == 0) {
                char recipeName[21];
                if (sscanf(line + strlen(command), "%20s", recipeName) == 1) {
                    remove_recipe(recipeName);
                }
            }
            else if (strcmp(command, "restock") == 0) {
                restock(line + strlen(command) + 1);
            }
            else if (strcmp(command, "order") == 0) {
                char recipeName[21];
                int quantity;
                if (sscanf(line + strlen(command), "%20s %d", recipeName, &quantity) == 2) {
                    order_command(recipeName, quantity);
                }
            }
        }
        free(line);
    }
    
    // Final truck loading if the time is at the end of a truckTime interval
    if ((currentTime + 1) % truckTime == 0 && currentTime != 0) {
        loadTruck();
    }
}

// Command: Add a recipe
void add_recipe(Recipe new_recipe) {
    if (searchRecipeHash(recipeHashTable, new_recipe.name) != NULL) {
        printf("ignored\n");
    } else {
        insertRecipeHash(recipeHashTable, new_recipe);
        printf("added\n");
    }
}

// Command: Remove a recipe. If any pending or processed orders use it, output "orders pending"
void remove_recipe(const char* recipeName) {
    // Check processed orders min-heap for orders with this recipe
    for (int i = 0; i < processedOrderHeap->size; i++) {
        if (strcmp(processedOrderHeap->orders[i].recipe, recipeName) == 0) {
            printf("orders pending\n");
            return;
        }
    }
    // Check pending orders queue
    Order* current = orderQueue->head;
    while (current != NULL) {
        if (strcmp(current->recipeName, recipeName) == 0) {
            printf("orders pending\n");
            return;
        }
        current = current->next;
    }
    if (searchRecipeHash(recipeHashTable, recipeName) == NULL) {
        printf("not present\n");
    } else {
        removeRecipeHash(recipeHashTable, recipeName);
        printf("removed\n");
    }
}

// Command: Restock ingredients. Multiple groups: ingredient name, quantity, expiration.
void restock(const char* command) {
    char ingredientName[25];
    int quantity, expiration;
    const char *ptr = command;
    while (sscanf(ptr, "%24s %d %d", ingredientName, &quantity, &expiration) == 3) {
        insertIngredientHash(ingredientHashTable, ingredientName, expiration, quantity);
        // Advance pointer (estimate the length to move past current triplet)
        ptr += strlen(ingredientName) + 1 + snprintf(NULL, 0, "%d", quantity) + 1 + snprintf(NULL, 0, "%d", expiration) + 1;
    }
    printf("restocked\n");
    checkPendingOrders();
}

// Command: Process an order for a given recipe and quantity
void order_command(const char* recipeName, int quantityOrdered) {
    RecipeHashNode* recipeNode = searchRecipeHash(recipeHashTable, recipeName);
    if (recipeNode == NULL) {
        printf("rejected\n");
        return;
    }
    if (isFeasible(recipeName, quantityOrdered)) {
        processOrder(recipeName, quantityOrdered, currentTime);
        printf("accepted\n");
    } else {
        enqueueOrder(orderQueue, recipeName, quantityOrdered, currentTime);
        printf("accepted\n");
    }
}

// Checks if an order is feasible (sufficient ingredients available and not expired)
bool isFeasible(const char* recipeName, int quantityOrdered) {
    RecipeHashNode* recipeNode = searchRecipeHash(recipeHashTable, recipeName);
    if (recipeNode == NULL)
        return false;
    RecipeIngredient* ing = recipeNode->recipe.ingredients;
    while (ing != NULL) {
        IngredientHashNode* ingredientNode = searchIngredientHash(ingredientHashTable, ing->name);
        if (ingredientNode == NULL)
            return false;
        // Remove expired lots
        if (ingredientNode->heap.batch[0].expiration <= currentTime) {
            while (ingredientNode->heap.size > 0 && ingredientNode->heap.batch[0].expiration <= currentTime) {
                removeIngredient(&ingredientNode->heap);
            }
        }
        if (ingredientNode->heap.total_quantity < ing->quantity * quantityOrdered)
            return false;
        ing = ing->next;
    }
    return true;
}

// Processes an order by removing the required ingredients from the corresponding heaps,
// then inserting the order into the processed orders min-heap.
void processOrder(const char* recipeName, int quantityOrdered, int arrivalTime) {
    RecipeHashNode* recipeNode = searchRecipeHash(recipeHashTable, recipeName);
    RecipeIngredient* ing = recipeNode->recipe.ingredients;
    while (ing != NULL) {
        IngredientHashNode* ingredientNode = searchIngredientHash(ingredientHashTable, ing->name);
        int requiredQuantity = ing->quantity * quantityOrdered;
        while (requiredQuantity > 0) {
            IngredientHeapNode lot = removeIngredient(&ingredientNode->heap);
            if (lot.quantity <= requiredQuantity) {
                requiredQuantity -= lot.quantity;
            } else {
                lot.quantity -= requiredQuantity;
                requiredQuantity = 0;
                insertIngredient(&ingredientNode->heap, lot.expiration, lot.quantity);
            }
        }
        ing = ing->next;
    }
    insertOrderHeap(processedOrderHeap, arrivalTime, (char*)recipeName, quantityOrdered);
}

// Checks the pending orders queue and processes any orders that have become feasible
void checkPendingOrders() {
    Order* current = orderQueue->head;
    Order* previous = NULL;
    while (current != NULL) {
        if (isFeasible(current->recipeName, current->quantity)) {
            processOrder(current->recipeName, current->quantity, current->arrivalTime);
            // Remove the order from the queue
            if (previous == NULL) {
                orderQueue->head = current->next;
                if (orderQueue->head == NULL)
                    orderQueue->tail = NULL;
            } else {
                previous->next = current->next;
                if (previous->next == NULL)
                    orderQueue->tail = previous;
            }
            Order* temp = current;
            current = current->next;
            free(temp);
        } else {
            previous = current;
            current = current->next;
        }
    }
}

// Loads the truck with processed orders from the min-heap.
// Orders are loaded by their weight, and the process stops when the truck capacity is exceeded.
void loadTruck() {
    if (isMinOrderHeapEmpty(processedOrderHeap)) {
        printf("truck empty\n");
        return;
    }
    int remainingCapacity = truckCapacity;
    MinOrderHeap* tempHeap = createMinOrderHeap(processedOrderHeap->capacity);
    while (!isMinOrderHeapEmpty(processedOrderHeap)) {
        ProcessedOrder order = removeMinOrder(processedOrderHeap);
        RecipeHashNode* recipeNode = searchRecipeHash(recipeHashTable, order.recipe);
        int orderWeight = calculateWeight(recipeNode->recipe, order.quantity);
        if (orderWeight <= remainingCapacity) {
            insertShipment(maxShipmentHeap, order.recipe, order.arrivalTime, order.quantity, orderWeight);
            remainingCapacity -= orderWeight;
        } else {
            insertOrderHeap(tempHeap, order.arrivalTime, order.recipe, order.quantity);
            break;
        }
    }
    while (!isMinOrderHeapEmpty(processedOrderHeap)) {
        ProcessedOrder order = removeMinOrder(processedOrderHeap);
        insertOrderHeap(tempHeap, order.arrivalTime, order.recipe, order.quantity);
    }
    while (!isMinOrderHeapEmpty(tempHeap)) {
        ProcessedOrder order = removeMinOrder(tempHeap);
        insertOrderHeap(processedOrderHeap, order.arrivalTime, order.recipe, order.quantity);
    }
    freeMinOrderHeap(tempHeap);
    
    // Print out all shipments in order (by max weight)
    if (!isMaxShipmentHeapEmpty(maxShipmentHeap)) {
        while (!isMaxShipmentHeapEmpty(maxShipmentHeap)) {
            Shipment shipment = removeMaxShipment(maxShipmentHeap);
            printf("%d %s %d\n", shipment.arrivalTime, shipment.name, shipment.quantity);
        }
    }
}

// Calculates the total weight of an order based on its recipe and quantity ordered.
int calculateWeight(Recipe recipe, int quantityOrdered) {
    int weight = 0;
    RecipeIngredient* ingredient = recipe.ingredients;
    while (ingredient != NULL) {
        weight += ingredient->quantity;
        ingredient = ingredient->next;
    }
    weight *= quantityOrdered;
    return weight;
}