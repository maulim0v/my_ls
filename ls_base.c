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

struct Node* add_data_sorted(struct Node* head, struct Data* data)
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
        if (my_str_compare(tmp_head->store->name, data->name) == true)
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
            if (my_str_compare(tmp_head->store->name, data->name) == true)
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
            if (my_str_compare(tmp_head->store->name, data->name) == true)
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

struct Node* add_data(struct Node* head, struct Data* data)
{
    if (data == NULL)
    {
        return head;
    }

    struct Node *tmp_node, *tmp_head = head;
    tmp_node = create_node();
    tmp_node->store = data;
    if(head == NULL)
    {
        head = tmp_node;
    }
    else
    {
        while(tmp_head->next != NULL)
        {
            tmp_head = tmp_head->next;
        }
        tmp_head->next = tmp_node;
    }
    return head;
}

struct Node* sort_by_time(struct Node* head)
{
    struct Node *tmp_head = head, *tmp_node, *tmp_prev, *tmp_next;
    size_t sz_node = get_node_size(tmp_head);

    if (sz_node < 2)
    {
        return head;
    }

    while(sz_node > 1)
    {
        size_t count = 0;
        while (tmp_head != NULL)
        {
            if ( (sz_node - 1) == count)
            {
                break;
            }

            if (tmp_head == head)
            {
                tmp_next = tmp_head->next;

                if ( my_time_compare(*tmp_head->store, *tmp_next->store) == true )
                {
                    tmp_prev = tmp_head;
                    tmp_head = tmp_next;
                }       
                else 
                {
                    tmp_node = tmp_next->next;
                    head = tmp_next;
                    tmp_head->next = tmp_node;
                    tmp_next->next = tmp_head;
                    tmp_prev = tmp_next;
                }
            }
            else if (tmp_head->next == NULL)
            {
                break;
            }
            else 
            {                
                tmp_next = tmp_head->next;
                if ( my_time_compare(*tmp_head->store, *tmp_next->store) == true )
                {
                    tmp_prev = tmp_head;
                    tmp_head = tmp_next;
                }       
                else 
                {
                    tmp_node = tmp_next->next;
                    tmp_prev->next = tmp_next;
                    tmp_head->next = tmp_node;
                    tmp_next->next = tmp_head;
                    tmp_prev = tmp_next;
                }
            }
            ++count;
        }
        tmp_head = head;
        --sz_node;
    }

    return head;
}

void print_node(struct Node* head, bool show_hidden)
{
    struct Node* tmp_head = head;
    while (tmp_head != NULL)
    {
        if (tmp_head->store->is_hidden == true)
        {
            if (show_hidden == false)
            {
                tmp_head = tmp_head->next;
                continue;
            }
        }

        my_str_write(tmp_head->store->name);
        if (tmp_head->next != NULL)
        {
            my_str_write("  ");
        }
        tmp_head = tmp_head->next;
    }
}

void free_node(struct Node* head)
{
   struct Node* tmp;
   while (head != NULL)
    {
        tmp = head;
        if (tmp->store->name != NULL)
        {
            free(tmp->store->name);
        }
        if (tmp->store->folder_structure != NULL)
        {
            free(tmp->store->folder_structure);
        }
        if (tmp->store != NULL)
        {
            free(tmp->store);
        }
        head = head->next;
        free(tmp);
    }
}

size_t get_node_size(struct Node* head)
{
    size_t sz = 0;
    struct Node* tmp_head = head;
    while (tmp_head != NULL)
    {
        ++sz;
        tmp_head = tmp_head->next;
    }

    return sz;
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
    if (node == NULL)
    {
        return head;
    }

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
        if (my_str_equal(tmp_head->container[0].store->folder_structure, ".") == false)
        {
            my_str_write(tmp_head->container[0].store->folder_structure);
            my_str_write(":\n");
        }
        print_node(tmp_head->container, show_hidden);
        if (tmp_head->next_chain != NULL)
        {            
            my_str_write("\n\n");
        }

        tmp_head = tmp_head->next_chain;
    }
}

void free_chain(struct Chain* chain_head)
{
   struct Chain* tmp;
   while (chain_head != NULL)
    {
        tmp = chain_head;
        free_node(tmp->container);
        chain_head = chain_head->next_chain;
        free(tmp);
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

bool my_time_compare(struct Data left, struct Data right)
{
    if (left.sec > right.sec)
    {
        return true;
    }
    else if (left.sec == right.sec && left.nsec >= right.nsec)
    {
        return true;
    }
    
    return false;
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
    size_t sz_left = my_str_len(left);
    size_t sz_right = my_str_len(right);

    char* res = (char*)  malloc ( 1 + ( sz_left + sz_right ) * sizeof(char) );

    for (size_t i = 0; i < sz_left; ++i)
    {
        res[i] = left[i];
    }
    for (size_t i = 0; i < sz_right; ++i)
    {
        res[sz_left + i] = right[i];
    }
    res[sz_left + sz_right] = '\0';

    return res;
}

char* my_new_str(char* str)
{
    if (str == NULL)
    {
        return NULL;
    }

    size_t sz_str = my_str_len(str);

    char* res = (char*) malloc (sz_str * sizeof(char) + 1);
    for (size_t i = 0; i < sz_str; ++i)
    {
        res[i] = str[i];
    }

    res[sz_str] = '\0';
    return res;
}

bool my_str_equal(char* left, char* right)
{
    if (left == NULL || right == NULL)
    {
        return false;
    }

    size_t sz_left = my_str_len(left);
    size_t sz_right = my_str_len(right);

    if (sz_left != sz_right)
    {
        return false;
    }

    for (size_t i = 0; i < sz_left; ++i)
    {
        if (left[i] != right[i])
        {
            return false;
        }
    }

    return true;
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

struct Chain* read_directories(struct Chain* chain_head, struct Node* directory_operands_head, bool is_recursive, bool sort_by_time)
{
    struct Node *tmp_directory_head = directory_operands_head;
    while (tmp_directory_head != NULL) 
    {
        chain_head = read_directory(chain_head, tmp_directory_head->store->name, is_recursive, sort_by_time);
        tmp_directory_head = tmp_directory_head->next;
    }
    return chain_head;
}

struct Chain* read_directory(struct Chain* chain_head, char* path, bool is_recursive, bool sort_by_time_bool)
{
    DIR *mydir;
    struct dirent *myfile;
    struct stat mystat;

    struct Node* head = NULL;
    struct Data* tmp_data;

    char* path_1;
    char* path_2;
    mydir = opendir(path);
    while((myfile = readdir(mydir)) != NULL)
    {
        if (my_str_len(myfile->d_name) != 0)
        {
            tmp_data = (struct Data*) malloc (sizeof(struct Data));
            if (myfile->d_name[0] == '.')
            {
                tmp_data->is_hidden = true;
            }
            else 
            {
                tmp_data->is_hidden = false;
            }
            path_1 = my_str_cat(path, "/");
            path_2 = my_str_cat(path_1, myfile->d_name);
            int i = stat(path_2, &mystat);
            tmp_data->sec = mystat.st_mtim.tv_sec;
            tmp_data->nsec = mystat.st_mtim.tv_nsec;

            tmp_data->name = my_new_str(myfile->d_name);
            tmp_data->folder_structure = my_new_str(path);
            
            if ( my_str_equal(tmp_data->name, ".") || my_str_equal(tmp_data->name, "..") )
            {
                tmp_data->is_dir = false;                
            }
            else 
            {
                tmp_data->is_dir = is_directory(path_2);      
            }

            head = add_data_sorted(head, tmp_data);

            free(path_1);
            free(path_2);
        }
    }
    closedir(mydir); 

    if (sort_by_time_bool == true)
    {
        head = sort_by_time(head);
    }
    chain_head = add_node(chain_head, head);

    if (is_recursive == true)
    {
        struct Node* tmp_head = head;
        while (tmp_head != NULL)
        {
            if (tmp_head->store->is_dir == true)
            {
                path_1 = my_str_cat(tmp_head->store->folder_structure, "/");
                path_2 = my_str_cat(path_1, tmp_head->store->name);
                chain_head = read_directory(chain_head, path_2, is_recursive, sort_by_time_bool);
                free(path_1);
                free(path_2);                
            }
            tmp_head = tmp_head->next;
        }
    }

    return chain_head;
}

void ls_main(bool show_hidden, bool is_recursive, bool sort_by_time, struct Node *directory_operands_head, struct Node *non_directory_operands_head)
{
    print_node(non_directory_operands_head, show_hidden);
    if (non_directory_operands_head != NULL && directory_operands_head != NULL)
    {
        my_str_write("\n\n");
    }

    struct Chain* chain_head = NULL;
    chain_head = read_directories(chain_head, directory_operands_head, is_recursive, sort_by_time);
    print_chain(chain_head, show_hidden);
    free_chain(chain_head);
}