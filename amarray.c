/**
* Source file that contains implementations for two different
* constant arrays, "amarray" stands for ascii/morse array. Both
* implementations are required for the main program to function.
* The arrays ASCII and MORSE are indexed the same way, meaning
* that upon initialization the index of 'A' in ASCII array
* is the same as 'A'-s morse representation in the MORSE array
*
* Author: Joosep Tavits
* 2021/2022 Fall semester
* Programming in C and Assembly Language (MTAT.03.219)
**/
#include "amarray.h"


// Implementation struct, strings are either ascii characters or
// their morse representation depending on chosen implementation
typedef struct {
    size_t amt_el;
    char *strings;
} amarray_impl_t;


// Memory free function used by both implementations
static void am_free(amarray_t *AM){
    amarray_impl_t *impl = AM->impl;

    if (impl->strings != NULL){
        free(impl->strings);
        impl->strings = NULL;
        impl->amt_el = 0;
    }

    free(AM->impl);
    free(AM);
    AM = NULL;
}//am_free



//______________________ASCII___________________________//



// Get ascii character representation by index
static amarray_error_t asc_get(struct amarray_t *ASCII, int i, char *out){
    amarray_impl_t *impl = ASCII->impl;

    if(i >= impl->amt_el || i < 0){
        return ASC_OUTOFRANGE;
    }
    *out = *(impl->strings + ASCII_LEN * i);
    return ASC_OK;
}//asc_get


// Print function used by ascii array implementation
static void asc_print(amarray_t *AM){
    amarray_impl_t *impl = AM->impl;

    for (int i = 0; i< impl->amt_el; i++){
        if (i % 8 == 0){
            printf("\n");
        }
        printf("%s ", impl->strings + ASCII_LEN*i);
    }
    printf("\n");
}//asc_print


// Initialize ascii array
amarray_t *ascii_init(void){
    amarray_t *ASCII = malloc(sizeof(amarray_t));

    ASCII->getc = asc_get;
    ASCII->print = asc_print;
    ASCII->free = am_free;

    amarray_impl_t *impl = malloc(sizeof(amarray_impl_t));

    impl->strings = malloc(SIZE*ASCII_LEN);
    char arr[SIZE][ASCII_LEN] = {
            " ",
            "A","B","C","D","E","F","G","H","I",
            "J","K","L","M","N","O","P","Q","R",
            "S","T","U","V","W","X","Y","Z","1",
            "2","3","4","5","6","7","8","9","0"
    };
    int i;
    int space = 0;
    for (i = 0; i < SIZE; i++){
        strcpy(impl->strings+space, arr[i]);
        space = space+ASCII_LEN;
    }
    impl->amt_el = SIZE;

    ASCII->impl = impl;

    return ASCII;
}//ascii_init



//______________________MORSE___________________________//



// Get morse representation by index
static amarray_error_t mor_get(struct amarray_t *MORSE, int i, char **out){
    amarray_impl_t *impl = MORSE->impl;

    if(i >= impl->amt_el || i < 0){
        return ASC_OUTOFRANGE;
    }
    *out = impl->strings + MORSE_LEN * i;
    return ASC_OK;
}//mor_get


// Print function used by morse array implementation
static void mor_print(amarray_t *AM){
    amarray_impl_t *impl = AM->impl;

    for (int i = 0; i< impl->amt_el; i++){
        if (i % 8 == 0){
            printf("\n");
        }
        printf("%s ", impl->strings + MORSE_LEN*i);
    }
    printf("\n");
}//mor_print


// Initialize morse array
amarray_t *morse_init(void){
    amarray_t *MORSE = malloc(sizeof(amarray_t));

    MORSE->gets = mor_get;
    MORSE->print = mor_print;
    MORSE->free = am_free;

    amarray_impl_t *impl = malloc(sizeof(amarray_impl_t));

    impl->strings = malloc(SIZE*MORSE_LEN);
    char arr[SIZE][MORSE_LEN] = {
            "/",
            ".-","-...","-.-.","-..",".","..-.","--.","....",
            "..",".---","-.-",".-..","--","-.","---",".--.",
            "--.-",".-.","...","-","..-","...-",".--","-..-",
            "-.--","--..",".----","..---","...--","....-",
            ".....","-....","--...","---..","----.","-----"
    };
    int i;
    int space = 0;
    for (i = 0; i < SIZE; i++){
        strcpy(impl->strings+space, arr[i]);
        space = space+MORSE_LEN;
    }
    impl->amt_el = SIZE;

    MORSE->impl = impl;

    return MORSE;
}//morse_init


//amarray.c