#ifndef header_h
#define header_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

/*
Data Structures Used in This Project:
    Ingredients:       Hash table of min-heaps
                       -> Each ingredient is stored in a min-heap (organized by expiration date,
                          and, if needed, lexicographically by lot) within a hash table for fast lookup.

    Recipes:           Hash table of linked lists
                       -> Recipes are stored in a hash table with linked lists to enable rapid search,
                          insertion, and deletion.

    Pending Orders:    FIFO Queue
                       -> Maintains the order in which orders arrive (First-In-First-Out).

    Processed Orders:  Min-Heap
                       -> Orders that have been processed are stored in a min-heap, ordered by the order’s arrival time.

    Delivery Truck:    Max-Heap
                       -> Orders to be loaded on the truck are stored in a max-heap, ordered by the total order weight;
                          in case of equal weight, the order with the earlier arrival time is given priority.

How It Works:
    Min/Max Heap:      A binary tree in which every node is smaller/larger than its children, meaning the root is the minimum/maximum.
                       (There are essentially two similar min-heaps in use – one for ingredients and one for processed orders – with the same basic operations,
                        but they are kept separate for clarity during debugging.)

    Hash Table:        A hash function generates a key that is used as an index in an array, providing O(1) average time for search and insertion.
                       (Both the ingredient and recipe hash tables share the same hash function implementation.)

    Queue:             A First-In-First-Out (FIFO) structure, ensuring that orders are handled in the order they arrive.
*/


/* Macros */
#define INITIAL_TABLE_SIZE 101          // Chosen prime number to reduce collisions
#define LOAD_FACTOR_THRESHOLD 0.75      // Determines when to resize the hash table
#define INITIAL_HEAP_CAPACITY 256

/* ------------------------------ */
/*       Ingredient Structures    */
/* ------------------------------ */

// Node stored in the min-heap for ingredients,
// ordered by expiration date (and lexicographically by lot if needed)
typedef struct {
    int expiration;    // Expiration time
    int quantity;      // Quantity of the ingredient in that lot
} IngredientHeapNode;

// Min-heap structure for ingredients, keeping track of the total quantity
typedef struct {
    IngredientHeapNode* batch;
    int size;
    int capacity;
    int total_quantity;
} IngredientMinHeap;

// Node for the ingredient hash table
typedef struct IngredientHashNode {
    char name[21];
    IngredientMinHeap heap;
    struct IngredientHashNode* next;
} IngredientHashNode;

// Hash table for ingredients
typedef struct {
    IngredientHashNode** buckets;
    int size;
    int count;
} IngredientHashTable;

/* Function prototypes for Ingredient data structures */
void swapIngredientNodes(IngredientHeapNode* a, IngredientHeapNode* b);
void heapifyIngredient(IngredientMinHeap* heap, int i);
void insertIngredient(IngredientMinHeap* heap, int expiration, int quantity);
IngredientHeapNode removeIngredient(IngredientMinHeap* heap);
IngredientMinHeap createIngredientMinHeap(int capacity);

void insertIngredientHash(IngredientHashTable* table, const char* name, int expiration, int quantity);
IngredientHashNode* createIngredientHashNode(const char* name, int capacity);
IngredientHashNode* searchIngredientHash(IngredientHashTable* table, const char* name);
IngredientHashTable* createIngredientHashTable(int size);

/* ------------------------------ */
/*        Recipe Structures       */
/* ------------------------------ */

// Linked list for a recipe’s ingredients
typedef struct RecipeIngredient {
    char name[21];
    int quantity;
    struct RecipeIngredient* next;
} RecipeIngredient;

// Recipe structure (name + list of ingredients)
typedef struct Recipe {
    char name[21];
    RecipeIngredient* ingredients;
} Recipe;

// Node for the recipe hash table
typedef struct RecipeHashNode {
    char name[21];
    Recipe recipe;
    struct RecipeHashNode* next;
} RecipeHashNode;

// Hash table for recipes
typedef struct {
    RecipeHashNode** buckets;
    int size;
    int count;
} RecipeHashTable;

/* Function prototypes for Recipe data structures */
void insertRecipeHash(RecipeHashTable* table, Recipe recipe);
void removeRecipeHash(RecipeHashTable* table, const char* name);
RecipeHashTable* createRecipeHashTable(int size);
RecipeHashNode* createRecipeHashNode(Recipe recipe);
RecipeHashNode* searchRecipeHash(RecipeHashTable* table, const char* name);

/* ------------------------------ */
/*         Order Structures       */
/* ------------------------------ */

// Order in the pending orders queue (FIFO)
typedef struct Order {
    int arrivalTime;
    char recipeName[21];
    int quantity;
    struct Order* next;
} Order;

// FIFO queue for pending orders
typedef struct {
    Order* head;
    Order* tail;
} OrderQueue;

/* Function prototypes for Order Queue */
void enqueueOrder(OrderQueue* queue, const char* recipeName, int quantity, int arrivalTime);
OrderQueue* createOrderQueue();
Order* dequeueOrder(OrderQueue* queue);

// Processed orders are stored in a min-heap ordered by arrival time
typedef struct ProcessedOrder {
    int arrivalTime;
    char recipe[21];
    int quantity;
} ProcessedOrder;

typedef struct {
    ProcessedOrder* orders;
    int size;
    int capacity;
} MinOrderHeap;

/* Function prototypes for Processed Order Min-Heap */
void freeMinOrderHeap(MinOrderHeap* heap);
void swapProcessedOrders(ProcessedOrder* x, ProcessedOrder* y);
void heapifyProcessedOrders(MinOrderHeap* heap, int i);
void insertOrderHeap(MinOrderHeap* heap, int arrivalTime, char* recipe, int quantity);
int isMinOrderHeapEmpty(MinOrderHeap* heap);
MinOrderHeap* createMinOrderHeap(int capacity);
ProcessedOrder removeMinOrder(MinOrderHeap* heap);

/* ------------------------------ */
/*        Shipment Structures     */
/* ------------------------------ */

// Shipment structure for truck loading; orders are loaded by weight (max-heap)
// (for equal weight, the earlier arrival comes first)
typedef struct Shipment {
    char name[20];
    int arrivalTime;
    int quantity;
    int weight;
} Shipment;

typedef struct {
    Shipment* shipments;
    int size;
    int capacity;
} MaxShipmentHeap;

/* Function prototypes for Shipment Max-Heap */
void heapifyShipments(MaxShipmentHeap* heap, int i);
void insertShipment(MaxShipmentHeap* heap, char* name, int arrivalTime, int quantity, int weight);
int isMaxShipmentHeapEmpty(MaxShipmentHeap* heap);
MaxShipmentHeap* createMaxShipmentHeap(int capacity);
Shipment removeMaxShipment(MaxShipmentHeap* heap);

/* ------------------------------ */
/*           Utility              */
/* ------------------------------ */

// Generic hash function used by both hash tables
unsigned int hashString(const char* str, int size);

// File reading and command processing functions
char* readLine(FILE* file);
void processCommands(FILE* file);

/* ------------------------------ */
/*       Command Functions        */
/* ------------------------------ */

// Recipe commands
void add_recipe(Recipe new_recipe);
void remove_recipe(const char* recipeName);

// Stocking and ordering commands
void restock(const char* command);
void order_command(const char* recipeName, int quantityOrdered);

// Order processing and truck loading functions
bool isFeasible(const char* recipeName, int quantityOrdered);
void processOrder(const char* recipeName, int quantityOrdered, int arrivalTime);
void checkPendingOrders();
void loadTruck();
int calculateWeight(Recipe recipe, int quantityOrdered);

/* ------------------------------ */
/*         Test Functions         */
/* ------------------------------ */

void testIngredientMinHeap();
void testIngredientHashTable();
void testRecipeHashTable();
void testOrderQueue();
void testMinOrderHeap();
void testMaxShipmentHeap();

/* ------------------------------ */
/*       Global Variables         */
/* ------------------------------ */

// Global variables (defined in main.c)
extern int currentTime;                     // Current time (starts at -1)
extern int truckCapacity;                   // Truck capacity
extern IngredientHashTable* ingredientHashTable;
extern RecipeHashTable* recipeHashTable;
extern OrderQueue* orderQueue;
extern MinOrderHeap* processedOrderHeap;
extern MaxShipmentHeap* maxShipmentHeap;

#endif // header_h