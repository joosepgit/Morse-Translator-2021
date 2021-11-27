/**
* Header file to make amarrays usable in translation.c
*
* Author: Joosep Tavits
* 2021/2022 Fall semester
* Programming in C and Assembly Language (MTAT.03.219)
**/
#pragma once
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// There are 36 letters in international morse code
// 26 letters and 10 numbers, we add whitespace to separate words
#define SIZE 37
// Ascii representation as a string takes 2 bytes,
// one for the uppercase character and one for null-byte
#define ASCII_LEN 2
// Morse representation takes at most 6 bytes, because
// the longest more representation strings are 5 bytes long
// and we need an extra space for the null-byte
#define MORSE_LEN 6


typedef enum {
    ASC_OK = 0,
    ASC_OUTOFRANGE = 1
} amarray_error_t;


typedef struct amarray_t
{
    amarray_error_t (* getc) (struct amarray_t *ASCII, int i, char *out);

    amarray_error_t (* gets) (struct amarray_t *ASCII, int i, char **out);

    void (* print)(struct amarray_t *ASCII);

    void (* free)(struct amarray_t *ASCII);

    void *impl;
} amarray_t;


amarray_t* ascii_init(void);
amarray_t* morse_init(void);

