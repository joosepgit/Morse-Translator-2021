/**
* Driver code for morse translator project.
* Meant to be run from command line with 2 parameter
* compile and run ./mtrans -h or ./mtrans --help for
* more specific usage guidelines.
*
* Output can be verified without having to modify its format here (works both ways):
* https://morsecode.world/international/translator.html
*
* Author: Joosep Tavits
* 2021/2022 Fall semester
* Programming in C and Assembly Language (MTAT.03.219)
**/
#include "translation.h"


int main(int argc, char *argv[])
{
    char *flag;
    char *code_or_file;

    if(argc == 3) {
        flag = argv[1];
        code_or_file = argv[2];
        FILE *fp1 = fopen(code_or_file, "r");

        if (fp1) {
            translateFile(&flag, &fp1);
        } else {
            translate(&flag, &code_or_file);
        }
    } else if (argc == 2) {
        flag = argv[1];
        alertOrHelp(&flag);
    } else {
        printf("Two arguments expected!\n");
        printf("Run 'mtrans -h' or 'mtrans --help' for usage guide.\n");
    }

    return 0;
} //main
