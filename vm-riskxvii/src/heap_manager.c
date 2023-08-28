// Name:   Isaak Choi
// UniKey: icho6322
// SID:    520488399


/* heap_manager.c

    Contains methods used to construct and operate the heap manager.
    * malloc() and free() API
    * heap manager initialiser and destructor
    * command to link to virtual machine's virtual routine mem

*/


// INCLUDE HEADER ...
#include "heap_manager.h"


// DOUBLE LINKED LIST METHODS ...

/* Initialises a new list object
    Zeroes all attributes and links methods
*/
void list_init(linked_list_t* const list) {

    // Zero all attributes
    list->head = NULL;
    list->size = 0;

    // Link methods
    list->insert = &list_insert_node;
    list->remove = &list_remove_node;
    list->get = &list_get_node;
    list->deallocate = &list_deallocate;
}

/* Create a new node with the given data as contents

    PARAMETERS
    <int> start | The index (zero start) of the first allocated bank
    <int> size  | The number of banks allocated

    RETURNS
    On success | Pointer to the new node
    On error   | NULL
*/
node_t* new_node(const int start, const int size) {

    // Allocate memory for head node
    node_t* new_node = malloc(sizeof(node_t));

    // Ensure allocation worked
    if (new_node == NULL) {
        printf("Error: Host system failed to malloc node\n");
        throw_host_malloc_failed_err();
        return NULL;
    }

    // Set attributes
    new_node->next = NULL;
    new_node->prev = NULL;
    new_node->start = start;
    new_node->size = size;

    // Return
    return new_node;
}

/* Inserts the given node at the given index in the given list

    * Throws error if index is out of range
    * Given index must be > 0
    * Index range: [0, list.size - 1]

*/
void list_insert_node(linked_list_t* const list, 
                        node_t* const node, const int index) {

    // Ensure index is within range - Only check in debug build
    #ifdef DEBUG_CHECK_LIST_RANGE
        if (index < 0) {
            printf("Err inserting node in list:\n");
            printf("  index [%d] out of range\n", index);
            return;
        }
    #endif

    // Ensure node to insert isn't null
    if (node == NULL) {
        printf("Error: list_insert_node() given null ptr as node\n");
        return;
    }

    // Get nodes before and after insertion location
    node_t* prev_cursor = NULL;
    node_t* cursor = list->head;
    for (int i = 0; i < index; i++) {

        // Ensure node exists - Only check in debug build
        #ifdef DEBUG_CHECK_LIST_RANGE
            if (cursor == NULL) {
                printf("Error inserting new node in list: Encountered null\n");
                return;
            }
        #endif
        
        // Get next
        prev_cursor = cursor;
        cursor = cursor->next;
    }

    // Link node
      // Empty list
    if (list->size == 0) {
        list->head = node;
        node->prev = NULL;
        node->next = NULL;  
    }
      // At beginning
    else if (index == 0) { 
        list->head = node;
        node->prev = NULL;
        node->next = cursor;
        cursor->prev = node;
    }
      // At end
    else if (index == list->size) { 
        node->prev = prev_cursor;
        node->next = NULL;
        prev_cursor->next = node;
    } 
      // Somewhere in middle
    else { 
        node->prev = prev_cursor;
        node->next = cursor;
        cursor->prev = node;
        prev_cursor->next = node;
    }

    // Increment list metadata
    list->size++;
}

/* Returns the node at the given index in the given list

    * Returns NULL if given index is out of range
    * Index range: [0, list.size - 1]

*/
node_t* list_get_node(const linked_list_t* const list, const int index) {

    // If index out of range - Won't occur in final build
    #ifdef DEBUG_CHECK_LIST_RANGE
        if (index < 0 || index >= list->size) {
            return NULL;
        }
    #endif

    // Iterate to find requested node
    node_t* cursor = list->head;
    for (int i = 0; i < index; i++) {

        // Check for out of range / invalid error only if in debug mode
        #ifdef DEBUG_CHECK_LIST_RANGE
            if (cursor == NULL) {
                return NULL;
            }
        #endif

        // Move to next node
        cursor = cursor->next;
    }

    // Return
    return cursor;
}

/* Removes the node at the given index in the given list
    Index range: [0, list.size - 1]
*/
void list_remove_node(linked_list_t* const list, const int index) {

    // If index out of range or list is empty
      // Won't occur in final build
    #ifdef DEBUG_CHECK_LIST_RANGE
        if (index < 0 || index >= list->size) {
            printf("Err removing node from list: index [%d] out of range\n", index);
            return;
        }
    #endif

    // Iterate to find requested node
    node_t* cursor = list->head;
    for (int i = 0; i < index; i++) {
        
        // Catch null nodes when iterating
          // For optimisation only check during debug because in final (correct)
          // build this isn't possible
        #ifdef DEBUG_CHECK_LIST_RANGE
            if (cursor == NULL) {
                printf("Error removing node from list... index out of range\n");
                return;
            }
        #endif

        // Move to next node
        cursor = cursor->next;
    }

    // Relink adjacent nodes
      // Only one node in list
    if (list->size == 1) {
        list->head = NULL;
    }
      // At beginning
    else if (index == 0) {
        list->head = cursor->next;
        cursor->prev = NULL;
    }
      // At end
    else if (index == list->size - 1) {
        cursor->prev->next = NULL;
    } 
      // Somewhere in middle
    else {
        cursor->next->prev = cursor->prev;
        cursor->prev->next = cursor->next;
    }

    // Delete node
    free(cursor);

    // Update list metadata
    list->size--;
}

/* Frees all malloc'd memory used by the list
    * Removes all elements
    * Frees all malloc'd memory
*/
void list_deallocate(linked_list_t* const list) {
    for (int i = 0; i < list->size; i++) {
        list_remove_node(list, 0);
    }
}


// HEAP MANAGER METHODS ...

/* Initialises the referenced heap bank manager object
    * Sets up empty list to store bank allocation data
    * Links methods as attributes

    PARAMETERS
    <heap_manager*> manager | Pointer to the heap manager
*/
heap_manager_t* heap_manager_init() {

    // Allocate new heap manager
    heap_manager_t* const manager = malloc(sizeof(heap_manager_t));
    if (manager == NULL) {
        printf("Error: Host system failed to malloc heap manager\n");
        throw_host_malloc_failed_err();
    }

    // Init attributes
    list_init(&manager->allocations);

    // Link methods
    manager->free = &heap_free;
    manager->malloc = &heap_malloc;
    manager->is_valid_memory = &heap_is_valid_memory;
    manager->deallocate = &heap_manager_deallocate;

    // Return pointer to new heap manager
    return manager;
}

/* Deallocates all malloc'd memory within the heap manager
    NOTE: Invalidates the heap manager. Only call when finished using it.
*/
void heap_manager_deallocate(heap_manager_t* const manager) {

    // Free allocations list
    manager->allocations.deallocate(&manager->allocations);

    // Free self
    free(manager);
}

/* Converts the given heap bank index to a memory address in the virtual machine

    PARAMETERS
    <int32_t> heap_index | The index (zero start) of a heap bank

    RETURNS
    <int32_t> | The vm memory address corresponding to the given heap bank index
*/
int32_t heap_index_to_addr(const int32_t heap_index) {
    return HEAP_MEM_START + (heap_index * HEAP_BANK_SIZE);
}

/* Converts the given address in virtual machine memory space to the index of 
    the corresponding heap bank. 

    NOTE
    Will truncate conversion equivalent floor() if not exact.

    PARAMETERS
    <int32_t> addr  | The address to be converted

    RETURNS
    <int32_t> index | The heap bank index corresponding to the given address
*/
int32_t addr_to_heap_index(const int32_t addr) {
    return (addr - HEAP_MEM_START) / HEAP_BANK_SIZE;
}

/* Returns whether the given address points to the beginning of a heap bank
    
    PARAMETERS
    <int32_t> addr | The virtual machine memory address to be checked

    RETURNS
    <bool> true  | Given address points to the beginning of a heap bank
    <bool> false | Given address does not point to the beginning of a heap bank
*/ 
bool is_heap_bank_addr(int32_t addr) {
    
    // Shift to zero start
    addr -= HEAP_MEM_START;

    // Whether the address is aligned with the start position of heap banks
    //  i.e., not pointing to the middle of a heap bank
    const bool is_aligned = (addr % HEAP_BANK_SIZE == 0);

    // Whether the address is within the bound of heap bank memory
    const bool is_within_bounds = (addr >= 0 && addr < HEAP_MEM_SIZE);

    // If both are true then it is a heap bank address
    return (is_aligned && is_within_bounds);
}

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
void heap_malloc(heap_manager_t* const manager, const int32_t size) {
   
    // Check if requested alloction is within allowed memory bounds
    if (size <= 0 || size > (HEAP_BANK_NUM * HEAP_BANK_SIZE)) {
        registers[HEAP_PTR_OUT_REGISTER] = MALLOC_FAIL_RETURN_VAL;
        return;
    }

    // Calculate number of required heap banks
    int num_banks = size / HEAP_BANK_SIZE;
    num_banks += (size % HEAP_BANK_SIZE != 0) ? 1 : 0;

    // Start at -1 because the first index after -1 is 0
    int prev_end = -1;

    // Iterate to find enough consecutive free memory banks
    for (int i = 0; i < manager->allocations.size; i++) {

        // Get next node
        const node_t* const n = manager->allocations.get(
            &manager->allocations, i);
        
        // Check for and allocate if possible
        const int n_free = n->start - prev_end - 1;
        const int start = prev_end + 1;
        if (n_free >= num_banks) {
            node_t* const heap_node = new_node(prev_end + 1, num_banks);
            manager->allocations.insert(&manager->allocations, heap_node, i);
            registers[HEAP_PTR_OUT_REGISTER] = heap_index_to_addr(start);
            return;
        }
        prev_end = n->start + n->size - 1;
    }

    // Check space from final allocation to end of heap banks
    const int space = HEAP_BANK_NUM - prev_end - 1;
    if (space >= num_banks) {
        const int alloc_location = prev_end + 1;
        const int index = manager->allocations.size;
        node_t* const heap_node = new_node(alloc_location, num_banks);
        manager->allocations.insert(&manager->allocations, heap_node, index);
        registers[HEAP_PTR_OUT_REGISTER] = heap_index_to_addr(alloc_location);
        return;
    }

    // If failed to allocate return null
    registers[HEAP_PTR_OUT_REGISTER] = MALLOC_FAIL_RETURN_VAL;
}

/* Attempts to free the referenced memory
    Throws an illegal operation error if the given memory is not allocated
    or outside vm heap memory space.

    PARAMETERS
    <heap_manager*> manager | Pointer to the heap manager
    <int32_t> addr          | The address of the memory block to be freed
*/
void heap_free(heap_manager_t* const manager, const int32_t addr) {

    // Do nothing if null address given as per C spec
    if (addr == NULL_ADDRESS) {
        return;
    }

    // Convert address to heap bank index
    const int32_t index = addr_to_heap_index(addr);

    // Find and free allocation block with given start addr
    bool found_match = false;
    for (int i = 0; i < manager->allocations.size; i++) {

        // Get next node
        const node_t* const n = manager->allocations.get(
            &manager->allocations, i);
        
        // Check for match with free location
        if (n->start == index) {
            found_match = true;
            manager->allocations.remove(&manager->allocations, i);
        }
    }

    // Throw error if none found or not valid addr
    // NOTE: If not valid addr check fails, a heap allocation node may have
    //       already been removed. however this is ok because the program 
    //       will exit with the thrown illegal operation error.
    if (!found_match || !is_heap_bank_addr(addr)) {
        throw_illegal_operation_err();
    }
}

/* Returns whether the given memory field is valid within the heap

    * Checks that all addresses within the given field are allocated
    * Checks addresses within the given field are in the same allocated chunk

    PARAMETERS
    <heap_manager*> manager | Pointer to the heap manager
    <int32_t> start_addr    | The address (in vm memory space) to be checked
    <int32_t> size          | The size in bytes of the mem block to be checked
*/
bool heap_is_valid_memory(heap_manager_t* const manager, 
                  const int32_t start_addr, const int32_t size) {

    // Iterate and check if allocated for each byte in range
    const linked_list_t* const allocations = &manager->allocations;
    const int end_addr_iter = start_addr + size; 
    for (int sub_addr = start_addr; sub_addr < end_addr_iter; sub_addr++) {

        // Holds whether the current sub address is valid
        bool valid = false;

        // Iterate allocation nodes
        for (int j = 0; j < allocations->size; j++) {

            // Get next node
            const node_t* const n = allocations->get(allocations, j);
            
            // Check whether the current sub address is valid
            const int after_end_index = n->start + n->size;
            if ((sub_addr >= heap_index_to_addr(n->start)) && 
                  (sub_addr < heap_index_to_addr(after_end_index))) {

                // Set sub address to valid
                valid = true;
            }
        }

        // If sub address isn't valid return invalid for whole range
        if (!valid) {
            return false;
        }
    }

    // Must be valid if this point reached
    return true;
}


// MANAGER INTERFACE METHODS ...

/* Links the given heap manager to the system virtual routines
    Saves a pointer to the heap manager in virtual routine memory
    when is then accessed by other virtual machine sys calls
*/
void link_heap_manager(heap_manager_t* const manager) {
    *(heap_manager_t**)&memory[VR_HEAP_MANAGER_ADDR] = manager;
}

/* Returns a pointer to the heap manager object
    Retrieves and returns the pointer to the heap manager object stored in the
    virtual routine memory space. This pointer must originally be set using 
    link_heap_manager()
*/
heap_manager_t* get_heap_manager() {
    return *(heap_manager_t**)&memory[VR_HEAP_MANAGER_ADDR];
}

