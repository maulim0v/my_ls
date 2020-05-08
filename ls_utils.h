#ifndef LS_UTILS_H
#define LS_UTILS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/// @brief Node and Data data structures
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
/// @}

/// @brief Chain data structure
/// @{
struct Chain
{
    struct Node* container;
    struct Chain* next_chain;
};
/// @}

/// @brief All helper functions
/// @{
void my_str_write(char* str);
bool my_str_compare(char* left, char* right);
bool my_time_compare(struct Data left, struct Data right);
size_t my_str_len(const char* str);
char* my_str_cat(char* left, char* right);
char* my_new_str(char* str);
bool my_str_equal(char* left, char* right);
int is_directory(const char *path);
/// @}

#endif // LS_UTILS_H