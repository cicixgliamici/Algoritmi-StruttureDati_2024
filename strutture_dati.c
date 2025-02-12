#include "header.h"

/* ========================================================= */
/*           Ingredient Data Structure Functions           */
/* ========================================================= */

// Swap two IngredientHeapNodes
void swapIngredientNodes(IngredientHeapNode* a, IngredientHeapNode* b) {
    IngredientHeapNode temp = *a;
    *a = *b;
    *b = temp;
}

// Maintains the min-heap property for ingredients by expiration time
void heapifyIngredient(IngredientMinHeap* heap, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < heap->size && heap->batch[left].expiration < heap->batch[smallest].expiration) {
        smallest = left;
    }
    if (right < heap->size && heap->batch[right].expiration < heap->batch[smallest].expiration) {
        smallest = right;
    }
    if (smallest != i) {
        swapIngredientNodes(&heap->batch[i], &heap->batch[smallest]);
        heapifyIngredient(heap, smallest);
    }
}

// Inserts a new ingredient lot into the min-heap
void insertIngredient(IngredientMinHeap* heap, int expiration, int quantity) {
    if (heap->size == heap->capacity) {
        heap->capacity *= 2;
        heap->batch = (IngredientHeapNode*)realloc(heap->batch, heap->capacity * sizeof(IngredientHeapNode));
    }
    int i = heap->size++;
    heap->batch[i].expiration = expiration;
    heap->batch[i].quantity = quantity;
    heap->total_quantity += quantity;

    // Bubble up to maintain heap property
    while (i != 0 && heap->batch[(i - 1) / 2].expiration > heap->batch[i].expiration) {
        swapIngredientNodes(&heap->batch[i], &heap->batch[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

// Removes the ingredient lot with the earliest expiration
IngredientHeapNode removeIngredient(IngredientMinHeap* heap) {
    if (heap->size == 0) {
        IngredientHeapNode empty = { INT_MAX, 0 };
        return empty;
    }
    IngredientHeapNode root = heap->batch[0];
    heap->batch[0] = heap->batch[--heap->size];
    heap->total_quantity -= root.quantity;
    heapifyIngredient(heap, 0);
    return root;
}

// Creates a new IngredientMinHeap with the specified capacity
IngredientMinHeap createIngredientMinHeap(int capacity) {
    IngredientMinHeap heap;
    heap.batch = (IngredientHeapNode*)malloc(capacity * sizeof(IngredientHeapNode));
    heap.capacity = capacity;
    heap.size = 0;
    heap.total_quantity = 0;
    return heap;
}

/* ========================================================= */
/*          Ingredient Hash Table Functions                */
/* ========================================================= */

// Creates a new hash table node for an ingredient
IngredientHashNode* createIngredientHashNode(const char* name, int capacity) {
    IngredientHashNode* node = (IngredientHashNode*)malloc(sizeof(IngredientHashNode));
    strcpy(node->name, name);
    node->heap = createIngredientMinHeap(capacity);
    node->next = NULL;
    return node;
}

// Inserts an ingredient lot into the ingredient hash table
void insertIngredientHash(IngredientHashTable* table, const char* name, int expiration, int quantity) {
    // Resize table if load factor threshold is reached
    if (table->count >= table->size * LOAD_FACTOR_THRESHOLD) {
        int newSize = table->size * 2;
        IngredientHashNode** newBuckets = (IngredientHashNode**)calloc(newSize, sizeof(IngredientHashNode*));
        for (int i = 0; i < table->size; i++) {
            IngredientHashNode* node = table->buckets[i];
            while (node) {
                IngredientHashNode* next = node->next;
                unsigned int newBucket = hashString(node->name, newSize);
                node->next = newBuckets[newBucket];
                newBuckets[newBucket] = node;
                node = next;
            }
        }
        free(table->buckets);
        table->buckets = newBuckets;
        table->size = newSize;
    }

    unsigned int bucket = hashString(name, table->size);
    IngredientHashNode* node = table->buckets[bucket];
    while (node) {
        if (strcmp(node->name, name) == 0) {
            insertIngredient(&node->heap, expiration, quantity);
            return;
        }
        node = node->next;
    }
    // Create new node if not found
    IngredientHashNode* newNode = createIngredientHashNode(name, INITIAL_HEAP_CAPACITY);
    insertIngredient(&newNode->heap, expiration, quantity);
    newNode->next = table->buckets[bucket];
    table->buckets[bucket] = newNode;
    table->count++;
}

// Searches for an ingredient in the hash table by name
IngredientHashNode* searchIngredientHash(IngredientHashTable* table, const char* name) {
    unsigned int bucket = hashString(name, table->size);
    IngredientHashNode* node = table->buckets[bucket];
    while (node) {
        if (strcmp(node->name, name) == 0)
            return node;
        node = node->next;
    }
    return NULL;
}

// Creates a new ingredient hash table with a given size
IngredientHashTable* createIngredientHashTable(int size) {
    IngredientHashTable* table = (IngredientHashTable*)malloc(sizeof(IngredientHashTable));
    table->buckets = (IngredientHashNode**)calloc(size, sizeof(IngredientHashNode*));
    table->size = size;
    table->count = 0;
    return table;
}

/* ========================================================= */
/*            Recipe Hash Table Functions                  */
/* ========================================================= */

// Inserts a recipe into the recipe hash table
void insertRecipeHash(RecipeHashTable* table, Recipe recipe) {
    if (table->count >= table->size * LOAD_FACTOR_THRESHOLD) {
        int newSize = table->size * 2;
        RecipeHashNode** newBuckets = (RecipeHashNode**)calloc(newSize, sizeof(RecipeHashNode*));
        for (int i = 0; i < table->size; i++) {
            RecipeHashNode* node = table->buckets[i];
            while (node) {
                RecipeHashNode* next = node->next;
                unsigned int newBucket = hashString(node->name, newSize);
                node->next = newBuckets[newBucket];
                newBuckets[newBucket] = node;
                node = next;
            }
        }
        free(table->buckets);
        table->buckets = newBuckets;
        table->size = newSize;
    }

    unsigned int bucket = hashString(recipe.name, table->size);
    RecipeHashNode* node = table->buckets[bucket];
    while (node) {
        if (strcmp(node->name, recipe.name) == 0)
            return;
        node = node->next;
    }
    RecipeHashNode* newNode = createRecipeHashNode(recipe);
    newNode->next = table->buckets[bucket];
    table->buckets[bucket] = newNode;
    table->count++;
}

// Removes a recipe from the recipe hash table
void removeRecipeHash(RecipeHashTable* table, const char* name) {
    unsigned int bucket = hashString(name, table->size);
    RecipeHashNode* node = table->buckets[bucket];
    RecipeHashNode* prev = NULL;
    while (node) {
        if (strcmp(node->name, name) == 0) {
            if (prev)
                prev->next = node->next;
            else
                table->buckets[bucket] = node->next;
            // Free the linked list of recipe ingredients
            RecipeIngredient* ingredient = node->recipe.ingredients;
            while (ingredient) {
                RecipeIngredient* temp = ingredient;
                ingredient = ingredient->next;
                free(temp);
            }
            free(node);
            table->count--;
            return;
        }
        prev = node;
        node = node->next;
    }
}

// Creates a new recipe hash table with a given size
RecipeHashTable* createRecipeHashTable(int size) {
    RecipeHashTable* table = (RecipeHashTable*)malloc(sizeof(RecipeHashTable));
    table->buckets = (RecipeHashNode**)calloc(size, sizeof(RecipeHashNode*));
    table->size = size;
    table->count = 0;
    return table;
}

// Creates a new recipe hash node
RecipeHashNode* createRecipeHashNode(Recipe recipe) {
    RecipeHashNode* node = (RecipeHashNode*)malloc(sizeof(RecipeHashNode));
    strcpy(node->name, recipe.name);
    node->recipe = recipe;
    node->next = NULL;
    return node;
}

// Searches for a recipe in the hash table by name
RecipeHashNode* searchRecipeHash(RecipeHashTable* table, const char* name) {
    unsigned int bucket = hashString(name, table->size);
    RecipeHashNode* node = table->buckets[bucket];
    while (node) {
        if (strcmp(node->name, name) == 0)
            return node;
        node = node->next;
    }
    return NULL;
}

/* ========================================================= */
/*             Order Queue Functions                       */
/* ========================================================= */

// Enqueues a new order into the pending orders queue
void enqueueOrder(OrderQueue* queue, const char* recipeName, int quantity, int arrivalTime) {
    Order* newOrder = (Order*)malloc(sizeof(Order));
    newOrder->arrivalTime = arrivalTime;
    strcpy(newOrder->recipeName, recipeName);
    newOrder->quantity = quantity;
    newOrder->next = NULL;
    if (queue->tail == NULL) {
        queue->head = newOrder;
        queue->tail = newOrder;
    } else {
        queue->tail->next = newOrder;
        queue->tail = newOrder;
    }
}

// Creates a new empty order queue
OrderQueue* createOrderQueue() {
    OrderQueue* queue = (OrderQueue*)malloc(sizeof(OrderQueue));
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

// Dequeues an order from the pending orders queue
Order* dequeueOrder(OrderQueue* queue) {
    if (queue->head == NULL)
        return NULL;
    Order* order = queue->head;
    queue->head = queue->head->next;
    if (queue->head == NULL)
        queue->tail = NULL;
    return order;
}

/* ========================================================= */
/*          Processed Order Min-Heap Functions             */
/* ========================================================= */

// Frees the memory allocated for a processed order min-heap
void freeMinOrderHeap(MinOrderHeap* heap) {
    free(heap->orders);
    free(heap);
}

// Swap two processed orders
void swapProcessedOrders(ProcessedOrder* x, ProcessedOrder* y) {
    ProcessedOrder temp = *x;
    *x = *y;
    *y = temp;
}

// Maintains the min-heap property based on arrival time
void heapifyProcessedOrders(MinOrderHeap* heap, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < heap->size && heap->orders[left].arrivalTime < heap->orders[smallest].arrivalTime)
        smallest = left;
    if (right < heap->size && heap->orders[right].arrivalTime < heap->orders[smallest].arrivalTime)
        smallest = right;
    if (smallest != i) {
        swapProcessedOrders(&heap->orders[i], &heap->orders[smallest]);
        heapifyProcessedOrders(heap, smallest);
    }
}

// Inserts a processed order into the min-heap
void insertOrderHeap(MinOrderHeap* heap, int arrivalTime, char* recipe, int quantity) {
    if (heap->size == heap->capacity) {
        heap->capacity *= 2;
        heap->orders = (ProcessedOrder*)realloc(heap->orders, heap->capacity * sizeof(ProcessedOrder));
    }
    int i = heap->size++;
    heap->orders[i].arrivalTime = arrivalTime;
    strcpy(heap->orders[i].recipe, recipe);
    heap->orders[i].quantity = quantity;

    // Bubble up to maintain heap property
    while (i != 0 && heap->orders[(i - 1) / 2].arrivalTime > heap->orders[i].arrivalTime) {
        swapProcessedOrders(&heap->orders[i], &heap->orders[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

// Returns 1 if the min-heap is empty, 0 otherwise
int isMinOrderHeapEmpty(MinOrderHeap* heap) {
    return heap->size == 0;
}

// Creates a new processed order min-heap with the given capacity
MinOrderHeap* createMinOrderHeap(int capacity) {
    MinOrderHeap* heap = (MinOrderHeap*)malloc(sizeof(MinOrderHeap));
    heap->orders = (ProcessedOrder*)malloc(capacity * sizeof(ProcessedOrder));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

// Removes and returns the processed order with the smallest arrival time
ProcessedOrder removeMinOrder(MinOrderHeap* heap) {
    if (heap->size == 0) {
        ProcessedOrder empty = {0, "", 0};
        return empty;
    }
    ProcessedOrder minOrder = heap->orders[0];
    heap->orders[0] = heap->orders[--heap->size];
    heapifyProcessedOrders(heap, 0);
    return minOrder;
}

/* ========================================================= */
/*         Shipment Max-Heap Functions                     */
/* ========================================================= */

// Maintains the max-heap property for shipments based on weight.
// For equal weight, the shipment with the earlier arrival comes first.
void heapifyShipments(MaxShipmentHeap* heap, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < heap->size && (heap->shipments[left].weight > heap->shipments[largest].weight ||
       (heap->shipments[left].weight == heap->shipments[largest].weight &&
        heap->shipments[left].arrivalTime < heap->shipments[largest].arrivalTime)))
    {
        largest = left;
    }
    if (right < heap->size && (heap->shipments[right].weight > heap->shipments[largest].weight ||
       (heap->shipments[right].weight == heap->shipments[largest].weight &&
        heap->shipments[right].arrivalTime < heap->shipments[largest].arrivalTime)))
    {
        largest = right;
    }
    if (largest != i) {
        Shipment temp = heap->shipments[i];
        heap->shipments[i] = heap->shipments[largest];
        heap->shipments[largest] = temp;
        heapifyShipments(heap, largest);
    }
}

// Inserts a shipment into the max-heap
void insertShipment(MaxShipmentHeap* heap, char* name, int arrivalTime, int quantity, int weight) {
    if (heap->size == heap->capacity) {
        heap->capacity *= 2;
        heap->shipments = (Shipment*)realloc(heap->shipments, heap->capacity * sizeof(Shipment));
    }
    int i = heap->size++;
    strcpy(heap->shipments[i].name, name);
    heap->shipments[i].arrivalTime = arrivalTime;
    heap->shipments[i].quantity = quantity;
    heap->shipments[i].weight = weight;

    // Bubble up to maintain heap property (by weight, then by arrival time)
    while (i != 0) {
        int parent = (i - 1) / 2;
        if (heap->shipments[parent].weight < heap->shipments[i].weight ||
           (heap->shipments[parent].weight == heap->shipments[i].weight &&
            heap->shipments[parent].arrivalTime > heap->shipments[i].arrivalTime))
        {
            Shipment temp = heap->shipments[i];
            heap->shipments[i] = heap->shipments[parent];
            heap->shipments[parent] = temp;
            i = parent;
        } else {
            break;
        }
    }
}

// Returns 1 if the max-heap is empty, 0 otherwise
int isMaxShipmentHeapEmpty(MaxShipmentHeap* heap) {
    return heap->size == 0;
}

// Creates a new shipment max-heap with the given capacity
MaxShipmentHeap* createMaxShipmentHeap(int capacity) {
    MaxShipmentHeap* heap = (MaxShipmentHeap*)malloc(sizeof(MaxShipmentHeap));
    heap->shipments = (Shipment*)malloc(capacity * sizeof(Shipment));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

// Removes and returns the shipment with the maximum weight
Shipment removeMaxShipment(MaxShipmentHeap* heap) {
    if (heap->size <= 0) {
        Shipment nullShipment = {"", 0, 0, 0};
        return nullShipment;
    }
    Shipment root = heap->shipments[0];
    if (heap->size == 1) {
        heap->size--;
        return root;
    }
    heap->shipments[0] = heap->shipments[--heap->size];
    heapifyShipments(heap, 0);
    return root;
}

/* ========================================================= */
/*                    Utility Functions                    */
/* ========================================================= */

// Generic hash function: shifts the hash 6 bits to the left and adds the ASCII code
unsigned int hashString(const char* str, int size) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash << 6) + *str++;
    }
    return hash % size;
}