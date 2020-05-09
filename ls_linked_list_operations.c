
#include "ls_linked_list_operations.h"

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

bool print_node(struct Node* head, bool show_hidden)
{
    struct Node* tmp_head = head;
    bool printed_something = false;
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
        printed_something = true;
        if (tmp_head->next != NULL)
        {
            my_str_write("  ");
        }
        tmp_head = tmp_head->next;
    }
    return printed_something;
}

void print_exception_case(struct Node* head)
{
    struct Node* tmp_head = head;
    while (tmp_head != NULL)
    {
        my_str_write("Hey! I really cannot access '");
        my_str_write(tmp_head->store->name);
        my_str_write("': No such file or directory!\n");
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

void print_chain(struct Chain* chain_head, bool show_hidden, bool donot_print_folder_structure_path)
{
    struct Chain* tmp_head = chain_head;
    while (tmp_head != NULL)
    {
        if (donot_print_folder_structure_path == false)
        {
            my_str_write(tmp_head->container[0].store->folder_structure);
            my_str_write(":\n");
        }
        bool printed_something = print_node(tmp_head->container, show_hidden);
        if (tmp_head->next_chain != NULL && printed_something == true)
        {            
            my_str_write("\n\n");
        }
        else if (tmp_head->next_chain != NULL && printed_something == false)
        {
            my_str_write("\n");
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

size_t get_chain_size(struct Chain* chain_head)
{
    size_t sz = 0;
    struct Chain* tmp_head = chain_head;
    while (tmp_head != NULL)
    {
        ++sz;
        tmp_head = tmp_head->next_chain;
    }

    return sz;
}
/// @}