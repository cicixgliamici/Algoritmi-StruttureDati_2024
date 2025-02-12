# Data Structures-Driven Food Order Management System

## Project Overview
This project is a robust and modular Food Order Management System implemented in C. It simulates the process of managing ingredients, recipes, and orders using advanced data structures such as hash tables, min-heaps, max-heaps, and queues. The system efficiently handles ingredient inventory, recipe management, order processing, and truck shipment planning.

---

## Key Features

### 1. Ingredient Management
- **Hash Table of Min-Heaps**: Stores ingredient lots ordered by expiration date (and lexicographically by lot if needed).  
- **Real-time Tracking**: Efficiently removes expired ingredient lots and keeps track of total available quantities.

### 2. Recipe Management
- **Hash Table of Linked Lists**: Rapid insertion, search, and deletion of recipes.
- **Multi-Ingredient Recipes**: Supports customizable quantities for each recipe.

### 3. Order Processing
- **FIFO Queue**: Manages pending orders to preserve the order of arrival.
- **Min-Heap**: Prioritizes orders based on arrival time for timely delivery.

### 4. Truck Shipment Optimization
- **Max-Heap**: Optimizes truck loading based on the total order weight.
- **Tie-Break by Arrival Time**: In cases of equal weight, orders with an earlier arrival time are prioritized.

---

## Project Structure

1. main.c:
     - Contains the main entry point and global variable definitions.
2. strutture_dati.c:
     - Implements the core data structures (hash tables, heaps, queues).
3. ordini.c:
     - Handles order processing and command management.
4. header.h:
     - Header file with all structure definitions and function prototypes.
5. test.c:
     - Contains tests verifying the correctness of the data structures.

---

## Skills Acquired

1. **Advanced Data Structures & Algorithms**  
   - Developed and implemented hash tables, min-heaps, max-heaps, and queues from scratch in C.

2. **Modular Programming**  
   - Broke down a complex system into multiple files (header, main, data structures, orders) with a clear separation of concerns.

3. **Memory Management**  
   - Gained in-depth knowledge of dynamic memory allocation and optimization in C, ensuring robust and efficient code.

4. **Performance Optimization**  
   - Achieved O(1) average-case operations for hash table lookups and O(log n) operations for heap insertions and deletions.

5. **Problem Solving**  
   - Addressed real-world challenges like inventory expiration management and order prioritization through careful algorithm design.

---

## Future Enhancements

1. Error Handling: Improve input validation and error reporting.
2. Extended Functionality: Add features for a more interactive ordering system.

---

## Conclusion
This project showcases my proficiency in C programming, data structures, and algorithm design. It demonstrates my ability to build scalable, modular systems and solve complex problems efficiently. I’m excited to bring these skills to new challenges and opportunities.

Feel free to connect with me on [LinkedIn](www.linkedin.com/in/leonardochiaretti) or [GitHub](https://github.com/cicixgliamici) for further discussion or opportunities!
