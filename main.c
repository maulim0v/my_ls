#include "ls_base.h"
#include "ls_linked_list_operations.h"

#include <sys/stat.h>

int main( int argc, const char* argv[] )
{
    bool show_hidden = false, is_recursive = false, sort_by_time_bool = false;
    struct Node *directory_operands_head = NULL, *non_directory_operands_head = NULL, *exception_operands_head = NULL;
    struct Data* tmp_data;

    for (int i = 1; i < argc; ++i)
    {
        if (argv[i] != NULL && my_str_len(argv[i]) != 0)
        {
            if (argv[i][0] == '-')
            {
                for (int j = 1; j < my_str_len(argv[i]); ++j)
                {
                    if (argv[i][j] == 'R')
                    {
                        is_recursive = true;
                    }
                    else if (argv[i][j] == 't')
                    {
                        sort_by_time_bool = true;
                    }
                    else if (argv[i][j] == 'a')
                    {
                        show_hidden = true;
                    }
                    else 
                    {
                        return 0;    
                    }
                }
            }
            else 
            {
                tmp_data = (struct Data*) malloc (sizeof(struct Data));
                tmp_data->name = my_new_str(argv[i]);
                tmp_data->folder_structure = NULL;
                if (argv[i][0] == '.')
                {
                    tmp_data->is_hidden = false;
                }
                else
                {
                    tmp_data->is_hidden = false;
                }
                if (is_directory(argv[i]) == 1)
                {                    
                    directory_operands_head = add_data_sorted(directory_operands_head, tmp_data);
                }
                else if (is_directory(argv[i]) == 0)
                {
                    struct stat mystat;
                    char* path_1;
                    char* path_2;
                    int i = stat(tmp_data->name, &mystat);
                    tmp_data->sec = mystat.st_mtim.tv_sec;
                    tmp_data->nsec = mystat.st_mtim.tv_nsec;
                    non_directory_operands_head = add_data_sorted(non_directory_operands_head, tmp_data);
                }
                else
                {
                    exception_operands_head = add_data(exception_operands_head, tmp_data);
                }
            }
        }
    }

    if (non_directory_operands_head == NULL && directory_operands_head == NULL && exception_operands_head == NULL)
    {
        tmp_data = (struct Data*) malloc (sizeof(struct Data));
        tmp_data->name = my_new_str(".");
        tmp_data->folder_structure = NULL;
        directory_operands_head = add_data_sorted(directory_operands_head, tmp_data);
    }

    if (sort_by_time_bool == true)
    {
        non_directory_operands_head = sort_by_time(non_directory_operands_head);
    }

    ls_main(show_hidden, is_recursive, sort_by_time_bool, directory_operands_head, non_directory_operands_head, exception_operands_head);

    free_node(directory_operands_head);
    free_node(non_directory_operands_head);
    free_node(exception_operands_head);

    return 0;
}