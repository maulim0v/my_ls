#include "ls_base.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#include <string.h>

struct Chain* read_directories(struct Chain* chain_head, struct Node* directory_operands_head, bool is_recursive, bool sort_by_time, bool show_hidden)
{
    struct Node *tmp_directory_head = directory_operands_head;
    while (tmp_directory_head != NULL) 
    {
        chain_head = read_directory(chain_head, tmp_directory_head->store->name, is_recursive, sort_by_time, show_hidden);
        tmp_directory_head = tmp_directory_head->next;
    }
    return chain_head;
}

struct Chain* read_directory(struct Chain* chain_head, char* path, bool is_recursive, bool sort_by_time_bool, bool show_hidden)
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

            if (show_hidden == false)
            {
                if (tmp_data->is_hidden == true)
                {
                    if (my_str_equal(tmp_data->name, ".") || my_str_equal(tmp_data->name, ".."))
                    {
                        head = add_data_sorted(head, tmp_data);
                    }
                    else 
                    {
                        free(tmp_data->name);
                        free(tmp_data->folder_structure);
                        free(tmp_data);    
                    }
                }
                else 
                {
                    head = add_data_sorted(head, tmp_data);    
                }
            }
            else 
            {
                head = add_data_sorted(head, tmp_data);
            }

            // if (show_hidden == false && tmp_data->is_hidden == true && (my_str_equal(tmp_data->name, ".") || my_str_equal(tmp_data->name, "..")))
            // {

            // }

            // head = add_data_sorted(head, tmp_data);

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
                chain_head = read_directory(chain_head, path_2, is_recursive, sort_by_time_bool, show_hidden);
                free(path_1);
                free(path_2);                
            }
            tmp_head = tmp_head->next;
        }
    }

    return chain_head;
}

void ls_main(bool show_hidden, bool is_recursive, bool sort_by_time, struct Node *directory_operands_head, struct Node *non_directory_operands_head, struct Node *exception_operands_head)
{
    print_exception_case(exception_operands_head);
    print_node(non_directory_operands_head, show_hidden);
    if (non_directory_operands_head != NULL && directory_operands_head != NULL)
    {
        my_str_write("\n\n");
    }

    struct Chain* chain_head = NULL;
    chain_head = read_directories(chain_head, directory_operands_head, is_recursive, sort_by_time, show_hidden);
    bool donot_print_folder_structure_path = (non_directory_operands_head == NULL) && (exception_operands_head == NULL) && (get_chain_size(chain_head) == 1); 
    print_chain(chain_head, show_hidden, donot_print_folder_structure_path);
    free_chain(chain_head);
}