
#include "ls_utils.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#include <string.h>

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

char* my_new_str(const char* str)
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