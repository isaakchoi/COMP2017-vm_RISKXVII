// Name:   Isaak Choi
// UniKey: icho6322
// SID:    520488399


/* heap_manager.h

    Contains typedefs, structs, constants, macros, and declarations for 
    methods used to construct and operate the heap manager.

    * node_t           // Each node within the allocation tracking list
    * linked_list_t    // List used to track currently allocated chunks
    * heap_manager_t   // Heap manager object that abstracts all underlying
                       //  methods into single malloc() and free() calls

*/


// HEADER GUARD ...
#ifndef HEAP_MANAGER_H
#define HEAP_MANAGER_H


// DEPENDENCIES ...
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "system.h"


// THIRD PARTY MEMORY LEAK DETECTOR ...
#ifdef DEBUG_DETECT_LEAKS
    #include "leak_detector_c.h"
#endif


// CONSTANTS ...

// The register that stores output from the malloc command
#define HEAP_PTR_OUT_REGISTER  (28)

// The value to be returned from a failed malloc request
#define MALLOC_FAIL_RETURN_VAL (0x00000000)


// DATA STRUCTURES ...

// Node for the heap manager linked list
typedef struct node_t node_t;
struct node_t{
    int start;    // The index (zero start) of the first allocated bank
    int size;     // The number of banks allocated
    node_t* next; // Pointer to the next node
    node_t* prev; // Pointer to the previous node
};

// Double linked list to hold vr_malloc information
typedef struct linked_list_t linked_list_t;
struct linked_list_t {

    // Attributes
    node_t* head; // Pointer to the first node of the list
    int size;     // The number of elements in the list

    // Methods
    void (*insert)(linked_list_t* const, node_t* const, const int);
    void (*remove)(linked_list_t* const, const int);
    node_t* (*get)(const linked_list_t* const, const int);
    void (*deallocate)(linked_list_t* const list);
};

// Heap manager to manage memory allocation and deallocation
typedef struct heap_manager_t heap_manager_t;
struct heap_manager_t {

    // Attributes
    linked_list_t allocations; // List containing heap bank allocation information

    // Methods
    void (*malloc)(heap_manager_t* const, const int32_t);
    void (*free)(heap_manager_t* const, const int32_t);
    bool (*is_valid_memory)(heap_manager_t* const, const int32_t, const int32_t);
    void (*deallocate)(heap_manager_t* const);
};


// DOUBLE LINKED LIST METHODS ...

/* Initialises a new list object
    Zeroes all attributes
*/
void list_init(linked_list_t* const list);

/* Create a new node with the given data as contents
    Returns NULL if an error occured
*/
node_t* new_node(const int, const int);

/* Inserts the given node at the given index in the given list

    * Throws error if index is out of range
    * Given index must be > 0
    * Index range: [0, list.size - 1]

*/
void list_insert_node(linked_list_t* const list,
                      node_t* const node, const int index);

/* Returns the node at the given index in the given list

    * Returns NULL if given index is out of range
    * Index range: [0, list.size - 1]

*/
node_t* list_get_node(const linked_list_t* const list, const int index);

/* Removes the node at the given index in the given list
    Index range: [0, list.size - 1]
*/
void list_remove_node(linked_list_t* const list, const int index);

/* Frees all malloc'd memory used by the list

    * Removes all elements
    * Frees all malloc'd memory

*/
void list_deallocate(linked_list_t* const list);


// HEAP MANAGER METHODS ...

/* Initialises the referenced heap bank manager object
    * Sets up empty list to store bank allocation data
    * Links methods as attributes

    PARAMETERS
    <heap_manager*> manager | Pointer to the heap manager
*/
extern heap_manager_t* heap_manager_init();

/* Deallocates all malloc'd memory within the heap manager
    NOTE: Invalidates the heap manager. Only call when done.
*/
extern void heap_manager_deallocate(heap_manager_t* const manager);

/* Converts the given heap bank index to a memory address in the virtual machine

    PARAMETERS
    <int32_t> heap_index | The index (zero start) of a heap bank

    RETURNS
    <int32_t> | The vm memory address corresponding to the given heap bank index
*/
int32_t heap_index_to_addr(const int32_t heap_index);

/* Converts the given address in virtual machine memory space to the index of 
    the corresponding heap bank. 

    NOTE
    Will truncate conversion equivalent floor() if not exact.

    PARAMETERS
    <int32_t> addr  | The address to be converted

    RETURNS
    <int32_t> index | The heap bank index corresponding to the given address
*/
int32_t addr_to_heap_index(const int32_t addr);

/* Returns whether the given address points to the beginning of a heap bank
    
    PARAMETERS
    <int32_t> addr | The virtual machine memory address to be checked

    RETURNS
    <bool> true  | Given address points to the beginning of a heap bank
    <bool> false | Given address does not point to the beginning of a heap bank
*/ 
bool is_heap_bank_addr(int32_t addr);

/* Attempts to allocate the given ammount of memory in the heap banks

    PARAMETERS
    <heap_manager*> manager | Pointer to the heap manager
    <int32_t> size          | The size of the memory to be allocated in bytes

    RETURNS
    None - However ...
    Saves pointer to allocated memory in register 28:
    * NULL if couldn't allocate
    * Pointer to allocated memory if successful
*/
void heap_malloc(heap_manager_t* const manager, const int32_t size);

/* Attempts to free the referenced memory
    Throws an illegal operation error if the given memory is not allocated
    or outside vm heap memory space.

    PARAMETERS
    <heap_manager*> manager | Pointer to the heap manager
    <int32_t> addr          | The address of the memory block to be freed
*/
void heap_free(heap_manager_t* const manager, const int32_t addr);

/* Returns whether the given memory field is allocated

    PARAMETERS
    <heap_manager*> manager | Pointer to the heap manager
    <int32_t> start_addr    | The address (in vm memory space) to be checked
    <int32_t> size          | The size in bytes of the mem block to be checked
*/
bool heap_is_valid_memory(heap_manager_t* const manager, 
                  const int32_t start_addr, const int32_t size);


// MANAGER INTERFACE METHODS ...

/* Links the given heap manager to the system virtual routines
    Saves a pointer to the heap manager in virtual routine memory
    when is then accessed by other virtual machine sys calls
*/
extern void link_heap_manager(heap_manager_t*);

/* Returns a pointer to the heap manager object
    Retrieves and returns the pointer to the heap manager object stored in the
    virtual routine memory space. This pointer must originally be set using 
    link_heap_manager()
*/
extern heap_manager_t* get_heap_manager();


// END HEADER GUARD ...
#endif