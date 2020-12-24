#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LENGTH 1000
#define MAX_NUMBER_TO_PRINT 100
#define NUMBER_OF_PARAMETERS 2
#define FIRST_COUNT_OF_ELEMENTS 10

int compare_des (const void* line1 , const void* line2){
    return -strcmp( *(char**) line1, *(char**) line2);
}

void add_slash_n(char* line){
    int count_symbols = 0;
    while(line[count_symbols] != '\n' && line[count_symbols] != '\0' && count_symbols < MAX_STRING_LENGTH){
        count_symbols++;
    }
    line[count_symbols] = '\n';
}

_Bool is_punctuation_mark(char symbol){
    if( ( symbol == '.' ) || ( symbol == ',' ) || ( symbol == ';' ) || ( symbol == ':' ) || ( symbol == '!' )
        || ( symbol == '?' )){
        return 1;
    }
    else
        return 0;
}
void rewrite_symbols(char *line , int number_of_symbol){
    if(number_of_symbol >= MAX_STRING_LENGTH)
        return;
    if(number_of_symbol == MAX_STRING_LENGTH - 1){
        line[number_of_symbol] = '\0';
        return;
    }

    while(line[number_of_symbol] != '\n' && line[number_of_symbol] != '\0' && number_of_symbol < MAX_STRING_LENGTH - 1){
        line[number_of_symbol] = line[number_of_symbol + 1];
        number_of_symbol ++;
    }
    line[number_of_symbol] = '\0';
}
void delete_symbols_in_line(char *line){
    int number_of_symbol = 0;
    while(line[number_of_symbol] != '\n' && line[number_of_symbol] != '\0' && number_of_symbol < MAX_STRING_LENGTH){
        if(is_punctuation_mark(line[number_of_symbol])){
            rewrite_symbols(line, number_of_symbol);
        }
        else{
            number_of_symbol++;
        }
    }
}
void delete_symbols(char **array , unsigned int const* number_of_lines){
    for(unsigned int i = 0 ; i < *number_of_lines ; i++){
        delete_symbols_in_line(array[i]);
    }
}

int read_arguments(int argc) {
    if (argc != NUMBER_OF_PARAMETERS) {
        fprintf(stderr, "Incorrect number of arguments. Should be one argument\n");
        return -1;
    }
    return 0;
}

void free_results(char **results, unsigned int count) {
    for (unsigned int i = 0; i < count; ++i) {
        free(results[i]);
    }
    free(results);
    results = NULL;
}

void print_results(char** array, unsigned int length_of_array){
    unsigned int count_number_print_el = 0;
    while(count_number_print_el < MAX_NUMBER_TO_PRINT && count_number_print_el < length_of_array){
        printf("%s", array[count_number_print_el]);
        count_number_print_el++;
    }
}


int main(int argc, char** argv) {
    if (read_arguments(argc)) return -1;

    FILE *input_file = fopen (argv[1], "rb");
    if (input_file == NULL){
        fprintf(stderr, "Error opening input file\n");
        return -1;
    }

    char** sorting_lines = (char**) malloc(FIRST_COUNT_OF_ELEMENTS * sizeof(char*));
    if (sorting_lines == NULL){
        fprintf(stderr,"Can not allocate memory\n");
        return -1;
    }
    unsigned int number_of_lines = 0;
    while(1){
        if(number_of_lines >= FIRST_COUNT_OF_ELEMENTS){
            sorting_lines = (char**)realloc(sorting_lines , (number_of_lines + 1) * sizeof(char*));
            if (sorting_lines == NULL){
                fclose(input_file);
                free_results(sorting_lines , number_of_lines);
                fprintf(stderr,"Can not allocate memory\n");
                return -1;
            }
        }
        sorting_lines[number_of_lines] = (char*)malloc(MAX_STRING_LENGTH * sizeof(char));
        if(sorting_lines[number_of_lines] == NULL){
            fclose(input_file);
            free_results(sorting_lines, number_of_lines);
            fprintf(stderr,"Can not allocate memory\n");
            return -1;
        }

        if(fgets(sorting_lines[number_of_lines], MAX_STRING_LENGTH , input_file) == NULL){
            break;
        }
        number_of_lines++;
    }

    add_slash_n(sorting_lines[number_of_lines - 1]);
    delete_symbols(sorting_lines, &number_of_lines);

    qsort(sorting_lines, number_of_lines, sizeof(char*), compare_des);
    print_results(sorting_lines, number_of_lines);

    free_results(sorting_lines , number_of_lines);
    fclose(input_file);
    return 0;
}
