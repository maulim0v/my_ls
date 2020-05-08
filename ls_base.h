#ifndef LS_BASE_H
#define LS_BASE_H

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/// @{
struct Data
{
    char* name;
    char* folder_structure;
    bool is_dir;
    bool is_hidden;
    long unsigned sec;
    long unsigned nsec;
};

struct Node
{
    struct Data* store;
    struct Node* next;
};

struct Node* create_node();
struct Node* add_data_sorted(struct Node* head, struct Data* data);
struct Node* sort_by_time(struct Node* head);
void print_node(struct Node* head, bool show_hidden);
void free_node(struct Node* head);
/// @}

/// @{
struct Chain
{
    struct Node* container;
    struct Chain* next_chain;
};

struct Chain* create_chain();
struct Chain* add_node(struct Chain* chain_head, struct Node* node);
void print_chain(struct Chain* chain_head, bool show_hidden);
void free_chain(struct Chain* chain_head);
/// @}

void my_str_write(char* str);

bool my_str_compare(char* left, char* right);

bool my_int_compare(unsigned left, unsigned right);

size_t my_str_len(const char* str);

char* my_str_cat(char* left, char* right);

char* my_new_str(char* str);

bool my_str_equal(char* left, char* right);

int is_directory(const char *path);

struct Chain* read_directories(struct Chain* chain_head, struct Node* directory_operands_head, bool is_recursive, bool sort_by_time);
struct Chain* read_directory(struct Chain* chain_head, char* path, bool is_recursive, bool sort_by_time);

void ls_main(bool show_hidden, bool is_recursive, bool sort_by_time, struct Node *directory_operands_head, struct Node *non_directory_operands_head);

#endif // LS_BASE_H