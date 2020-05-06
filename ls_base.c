#include "ls_base.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#include <string.h>

/// {@
struct Node* create_node()
{
    struct Node* tmp_node;
    tmp_node = (struct Node*) malloc (sizeof(struct Node));
    tmp_node->next = NULL;
    return tmp_node;
}

struct Node* add_data_sorted(struct Node* head, struct Data data)
{
    struct Node *tmp_node, *tmp_head = head, *tmp_prev = head;
    tmp_node = create_node();
    tmp_node->store = data;
    if(head == NULL)
    {
        head = tmp_node;
    }
    else if (head->next == NULL)
    {
        if (my_str_compare(tmp_head->store.name, data.name) == true)
        {
            head = tmp_node;
            head->next = tmp_head;
        }
        else 
        {
            tmp_head->next = tmp_node;
        }
    }
    else
    {
        bool inserted = false;
        while(tmp_head->next != NULL)
        {
            if (my_str_compare(tmp_head->store.name, data.name) == true)
            {
                if (tmp_head == head)
                {
                    head = tmp_node;
                    head->next = tmp_head;
                    inserted = true;
                }
                else 
                {
                    tmp_prev->next = tmp_node;
                    tmp_node->next = tmp_head;
                    inserted = true;
                }
                break;
            }
            else 
            {
                tmp_prev = tmp_head;
                tmp_head = tmp_head->next;
            }
        }

        if (inserted == false)
        {
            if (my_str_compare(tmp_head->store.name, data.name) == true)
            {
                tmp_prev->next = tmp_node;
                tmp_node->next = tmp_head;
            }
            else 
            {
                tmp_head->next = tmp_node;
            }
        }
    }
    return head;
}

void print_node(struct Node* head, bool show_hidden)
{
    struct Node* tmp_head = head;
    while (tmp_head != NULL)
    {
        if (tmp_head->store.is_hidden == true)
        {
            if (show_hidden == false)
            {
                tmp_head = tmp_head->next;
                continue;
            }
        }

        my_str_write(tmp_head->store.name);
        my_str_write("  ");
        tmp_head = tmp_head->next;
    }
}
/// @}

/// @{
struct Chain* create_chain()
{
    struct Chain* tmp_chain;
    tmp_chain = (struct Chain*) malloc (sizeof(struct Chain));
    tmp_chain->next_chain = NULL;
    return tmp_chain;
}

struct Chain* add_node(struct Chain* head, struct Node* node)
{
    struct Chain *tmp_chain, *tmp_head = head;
    tmp_chain = create_chain();
    tmp_chain->container = node;
    if(head == NULL)
    {
        head = tmp_chain;
    }
    else
    {
        while(tmp_head->next_chain != NULL)
        {
            tmp_head = tmp_head->next_chain;
        }
        tmp_head->next_chain = tmp_chain;
    }
    return head;
}

void print_chain(struct Chain* chain_head, bool show_hidden)
{
    struct Chain* tmp_head = chain_head;
    while (tmp_head != NULL)
    {
        my_str_write(tmp_head->container[0].store.folder_structure);
        my_str_write(":\n");
        print_node(tmp_head->container, show_hidden);
        my_str_write("\n\n");
        tmp_head = tmp_head->next_chain;
    }
}
/// @}

void my_str_write(char * str)
{
    if (str != NULL)
    {
        write(1, str, my_str_len(str));
    }
    else 
    {
        write(1, "(null)", 6);    
    }
}

bool my_str_compare(char* left, char* right)
{
    if (left == NULL || right == NULL)
    {
        return false;
    }

    int sz_left  = my_str_len(left);
    int sz_right = my_str_len(right);

    if (sz_left == 0 && sz_right == 0)
    {
        return false;
    }

    int sz_min = sz_left > sz_right ? sz_right : sz_left;

    for (int i = 0; i < sz_min; ++i)
    {
        if (left[i] > right[i])
        {
            return true;
        }
        else if (left[i] < right[i])
        {
            return false;
        }
    }

    if (sz_left == sz_right)
    {
        return false;
    }
    else if (sz_left > sz_right)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool my_int_compare(unsigned left, unsigned right)
{
    return left > right;
}

size_t my_str_len(const char* str)
{
    if (str == NULL)
    {
        return 0;
    }
    else 
    {
        size_t count = 0;
        while (*str++ != '\0' && ++count);  
        return count;
    }
}

char* my_str_cat(char* left, char* right)
{
    char* res = (char*)  malloc ( 1 + ( my_str_len(left) + my_str_len(right) ) * sizeof(char) );

    while(*left != '\0')
    {
        *res = *left;
        ++res;
        ++left;
    }

    while(*right != '\0')
    {
        *res = *right;
        ++res;
        ++right;
    }

    *res = '\0';

    return &res[0];
}

int is_directory(const char *path)
{
    struct stat path_stat;
    
    if (stat(path, &path_stat) == -1)
    {
        return -1;
    }

    return S_ISDIR(path_stat.st_mode);
}

struct Chain* read_directories(struct Chain* chain_head, struct Node* directory_operands_head)
{
    while (directory_operands_head != NULL) 
    {
        chain_head = read_directory(chain_head, directory_operands_head->store.name);
        directory_operands_head = directory_operands_head->next;
    }

    return chain_head;
}

struct Chain* read_directory(struct Chain* chain_head, char* path)
{
    DIR *mydir;
    struct dirent *myfile;
    struct stat mystat;

    struct Node* head = NULL;
    struct Data tmp_data;

    char buf[512];
    mydir = opendir(path);
    while((myfile = readdir(mydir)) != NULL)
    {
        if (my_str_len(myfile->d_name) != 0)
        {
            if (myfile->d_name[0] == '.')
            {
                tmp_data.is_hidden = true;
            }
            else 
            {
                tmp_data.is_hidden = false;
            }
            tmp_data.name = myfile->d_name;
            tmp_data.folder_structure = path;
            head = add_data_sorted(head, tmp_data);
        }

        // sprintf(buf, "%s/%s", ".", myfile->d_name);
        // int i = stat(buf, &mystat);
        // printf("%d  ", i);
        // printf("%zu",mystat.st_size);
        // printf(" %s\n", myfile->d_name);

        //my_str_write(myfile->d_name);
        //my_str_write("\n");

    }
    closedir(mydir); 

    chain_head = add_node(chain_head, head);
    return chain_head;
}

void ls_main(bool show_hidden, bool is_recursive, bool sort_by_time, struct Node *directory_operands_head, struct Node *non_directory_operands_head)
{
    print_node(non_directory_operands_head, show_hidden);
    my_str_write("\n\n");

    struct Chain* chain_head = NULL;
    chain_head = read_directories(chain_head, directory_operands_head);
    print_chain(chain_head, show_hidden);
}