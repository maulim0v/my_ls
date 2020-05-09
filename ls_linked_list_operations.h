#ifndef LS_LINKED_LIST_OPERATIONS_H
#define LS_LINKED_LIST_OPERATIONS_H

#include "ls_utils.h"

/// @brief Main linked list which stores all useful data
/// @{
struct Node* create_node();
struct Node* add_data_sorted(struct Node* head, struct Data* data);
struct Node* add_data(struct Node* head, struct Data* data);
struct Node* sort_by_time(struct Node* head);
bool print_node(struct Node* head, bool show_hidden);
void print_exception_case(struct Node* head);
void free_node(struct Node* head);
size_t get_node_size(struct Node* head);
/// @}

/// @brief Driving linked list for recursively storing the linked list node
/// @{
struct Chain* create_chain();
struct Chain* add_node(struct Chain* chain_head, struct Node* node);
void print_chain(struct Chain* chain_head, bool show_hidden, bool donot_print_folder_structure_path);
void free_chain(struct Chain* chain_head);
size_t get_chain_size(struct Chain* chain_head);
/// @}

#endif // LS_LINKED_LIST_OPERATIONS_H