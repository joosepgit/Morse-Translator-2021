The observable program is meant to translate morse code to ascii uppercase characters and numbers
and vice-versa. It should be compiled using the command 'gcc amarray.c translation.c mtrans.c -o mtrans'
in the folder where all of the following files are located:
amarray.c
amarray.h
mtrans.c
translation.c
translation.h

After compiling, you will have an executable called mtrans in the same folder. There you can run it
using './mtrans', for usage guidelines run './mtrans -h' or './mtrans --help'

LIMITATIONS:
The program only recognizes letters A-Z and numbers 0-9, any other symbol is considered unknown
or discarded depending on the situation. Letters are separated using whitespaces and words using
'/' symbols in morse code. The program struggles sometimes if for example in the input file, a
single morse code letter is split into two by a newline. Excess whitespaces are disregarded both ways
and newlines that do not break morse representations also have no effect.

Author: Joosep Tavits
2021/2022 Fall semester
Programming in C and Assembly Language (MTAT.03.219)