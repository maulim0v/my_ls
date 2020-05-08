#ifndef LS_BASE_H
#define LS_BASE_H

#include "ls_linked_list_operations.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/// @brief Main function for reading directories
/// @{
struct Chain* read_directories(struct Chain* chain_head, struct Node* directory_operands_head, bool is_recursive, bool sort_by_time);
struct Chain* read_directory(struct Chain* chain_head, char* path, bool is_recursive, bool sort_by_time);
/// @}

/// @brief Main function that get called in main for all ls operations
void ls_main(bool show_hidden, bool is_recursive, bool sort_by_time, struct Node *directory_operands_head, struct Node *non_directory_operands_head);

#endif // LS_BASE_H