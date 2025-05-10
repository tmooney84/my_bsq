#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <fcntl.h>

#define BUFFER_LENGTH 8
#define NAME_LENGTH 255

int min(int a, int b)
{
    return a < b ? a : b;
}

int max(int a, int b)
{
    return a > b ? a : b;
}

void allocation_error()
{
    perror("Unable to allcate memory.\n");
}

int find_max_square(char **matrix, int length, int *square_info)
{
    int corner_x = -1;
    int corner_y = -1;
    int max_len = 0;

    char dp_array[length + 1]; // extra 1 to create "dummy left border" to array
    memset(dp_array, '\0', sizeof(dp_array));

    for (int i = 0; i < length; i++)
    {
        int prev = 0;

        for (int j = 1; j <= length; j++)
        {
            int tmp = dp_array[j];

            if (matrix[i][j - 1] == '.')
            {
                dp_array[j] = min(prev, min(dp_array[j], dp_array[j - 1])) + 1;
                if (dp_array[j] > max_len)
                {
                    max_len = dp_array[j];
                    corner_y = i;
                    corner_x = j - 1;
                }
            }
            else
            {
                dp_array[j] = 0;
            }
                prev = tmp;
        }
    }

    square_info[0] = corner_y;
    square_info[1] = corner_x;
    square_info[2] = max_len;

    return 0;
}

int main(int argc, char ** argv)
{
    // open file
    if(argc != 2)
    {
        perror("Please add filename to run my_bsq: ./my_bsq [filename]");
        return -1;
    }

    char file_name[NAME_LENGTH];
    memset(file_name, '\0', NAME_LENGTH);
    
    strncpy(file_name, argv[1], NAME_LENGTH);
    FILE *fptr = fopen(file_name, "r");
    if (!fptr)
    {
        allocation_error();
        return -1;
    }

    char buffer[BUFFER_LENGTH];
    memset(buffer, '\0', sizeof(buffer));

    // get length
    fgets(buffer, BUFFER_LENGTH, fptr);
    int length = atoi(buffer);

    // create matrix
    char **matrix = (char **)calloc(length, sizeof(char *));
    if (!matrix)
    {
        allocation_error();
        fclose(fptr);
        return -1;
    }

    // fill matrix

    for (int i = 0; i < length; i++)
    {
        matrix[i] = (char *)calloc(length + 1, sizeof(char *));
        if(!matrix[i])
        {
            allocation_error();
            free(matrix);
            fclose(fptr);
            return -1;

        }

        if(fgets(matrix[i], length + 1, fptr) == NULL)
        {
            allocation_error();
            free(matrix[i]);
            free(matrix);
            fclose(fptr);
            return -1;
        }
        fgetc(fptr); //consumes the '\n'
    }

    /*
    square_info[0] >>> corner_y
    square_info[1] >>> corner_x
    squere_info[2] >>> max length of the square
    */

    int *square_info = (int *)calloc(3, sizeof(int));
    if (!square_info)
    {
        allocation_error();
        for(int i = 0; i < length; i++)
        {
            free(matrix[i]);
        }
        free(matrix);
        fclose(fptr);
        return -1;
    }

    // run algorithm
    find_max_square(matrix, length, square_info);

    // max found, change '.' to 'o'
    int corner_y = square_info[0];
    int corner_x = square_info[1];
    int max_len = square_info[2];

    for(int i = corner_y - (max_len - 1); i <= corner_y; i++)
    {
        for(int j = corner_x - (max_len - 1); j <= corner_x; j++)
        {
            matrix[i][j] = 'x';
        }
    }

    //print updated array
    for(int i = 0; i < length; i++)
    {
        printf("%s\n",matrix[i]);
    }

    // free allocations
    for(int i = 0; i < length; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
    free(square_info);
    fclose(fptr);

    return 0;
}