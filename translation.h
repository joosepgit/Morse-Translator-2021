/**
* Header file to make translation functions visible for mtrans.c
*
* Author: Joosep Tavits
* 2021/2022 Fall semester
* Programming in C and Assembly Language (MTAT.03.219)
**/
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "amarray.h"


void strToUpper(char s[]);
bool isValidAscii(const char s[]);
bool isValidMorse(const char s[]);
void asciiToMorse(const char s[], amarray_t *ASCII, amarray_t *MORSE, char **out);
void morseToAscii(const char s[], amarray_t *ASCII, amarray_t *MORSE, char **out);
void translate(char **flag, char **input);
void translateFile(char **flag, FILE **fp1);
void alertOrHelp(char** flag);
