/**
* Source file that contains implementations for all
* translation functionality, both from ASCII->MORSE
* and MORSE->ASCII. This logic is used in mtrans.c
* for translation operations. It is connected to
* mtrans.c through translation.h
*
* Author: Joosep Tavits
* 2021/2022 Fall semester
* Programming in C and Assembly Language (MTAT.03.219)
**/
#include "translation.h"


// Function for converting all ascii code lowercase characters to
// their uppercase representation. This function discards characters
// that are not in [a-z], therefore safety with unknown symbols is guaranteed
void strToUpper(char s[]){
    int c = 0;

    while (s[c] != '\0') {
        if (s[c] >= 'a' && s[c] <= 'z') {
            s[c] = s[c] - 32;
        }
        c++;
    }
}//strToUpper


// Function for deciding whether a given string can be translated
// to morse code according to the rules of this program
bool isValidAscii(const char s[]){
    int c = 0;

    while (s[c] != '\0') {
        // s[c] may also be in '\r\n'. This makes the program flexible enough
        // to still attempt translating upon meeting an unexpected sequence of whitespaces
        // or newlines when reading from file. Additionally, we can be sure that no lowercase
        // characters are inside the input, as this function is always used after uppercasing the input.
        bool unfit = !((s[c] >= '0' && s[c] <= '9') || (s[c] >= 'A' && s[c] <= 'Z'));
        if (unfit && s[c] != ' ' && s[c] != '\r' && s[c] != '\n') {
            printf("Illegal symbol in input text:\n %c\nCharacter number %d.\n", s[c], c);
            return false;
        }
        c++;
    }
    return true;
}//isValidAscii


// Function for deciding whether a given string can be translated
// into a fraction of ascii code symbols according to the rules of this program
bool isValidMorse(const char s[]){
    int c = 0;

    while (s[c] != '\0') {
        // s[c] may also be in '\r\n'. This makes the program flexible enough
        // to still attempt translating upon meeting an unexpected sequence of whitespaces
        // or newlines when reading from file. The rest of the conditions check for invalid
        // characters in morse code. The symbol '/' is used to separate words.
        if (s[c] != '.' && s[c] != '-' && s[c] != ' ' && s[c] != '/' && s[c] != '\r' && s[c] != '\n') {
            printf("Illegal symbol in morse code: %c\nCharacter number %d.\n", s[c], c);
            return false;
        }
        c++;
    }
    return true;
}//isValidMorse


// Function for translating a string of ascii characters to morse code.
// ASCII and MORSE are the corresponding amarrays. For each character in the
// ASCII array, its morse code representation is on the same index in the MORSE array.
// Output morse code is written into 'out' before the function terminates.
void asciiToMorse(const char s[], amarray_t *ASCII, amarray_t *MORSE, char **out){
    // Initializing a result array, since every characters morse representation
    // takes at most 6 bytes, we allocate the input strings length * 6 bytes of memory
    int i = 0;
    size_t s_len = strlen(s);
    char *res = malloc(s_len*MORSE_LEN);
    res[0] = '\0';
    // Whitespace is initially true to jump over any sequence of whitespaces in the beginning
    bool ws = true;

    while (s[i] != '\0'){
        // If current character is translatable and we havent reached the end
        // of the input string
        if (s[i] != '\n' && s[i] != '\r') {
            // Attempt finding the current characters index in the ASCII amarray
            char c;
            int mIDX = -1;
            for (int j = 0; j < 37; j++) {
                amarray_error_t a_err = ASCII->getc(ASCII, j, &c);
                if (a_err == 0 && s[i] == c) {
                    mIDX = j;
                    break;
                }
            }
            // After finding the index of the character
            // attempt finding the index of the corresponding
            // morse code character
            char *str;
            amarray_error_t m_err = MORSE->gets(MORSE, mIDX, &str);
            // If the character existed in ASCII, then it has
            // a representation in MORSE
            // m_err is 0 iff mIDX > -1 && mIDX < 37
            if (m_err == 0) {
                // If the character is a whitespace symbol
                if (strcmp(str, "/") == 0) {
                    // If current is and previous wasnt
                    if (!ws) {
                        // Append single whitespace
                        strcat(res, str);
                        if (s[i + 1] != '\0') strcat(res, " ");
                    }// Otherwise ignore, helps with trailing unnecessary whitespaces
                    // Remember that the previous parsed character was a whitespace
                    ws = true;
                } else {
                    // If character isn't a whitespace character
                    // Add its morse representation to the result and set whitespace to false
                    strcat(res, str);
                    ws = false;
                    // Add space if it isnt the last character in the input string
                    if (s[i + 1] != '\0') strcat(res, " ");
                }
            } // If s[i] is and unknown symbol
            else {
                strcat(res, "(?)");
                if (s[i + 1] != '\0') strcat(res, " ");
            }
        }
        i++;
    }
    *out = res;
}//asciiToMorse


// Function for translating a string of ascii characters to morse code.
// ASCII and MORSE are the corresponding amarrays. For each character in the
// ASCII array, its morse code representation is on the same index in the MORSE array.
// Output morse code is written into 'out' before the function terminates.
void morseToAscii(const char s[], amarray_t *ASCII, amarray_t *MORSE, char **out){
    // This time we only allocate bytes in the length of the morse code
    // because the character representation takes much less memory,
    // therefore res overflow is impossible.
    size_t s_len = strlen(s);
    char *str = malloc(s_len);
    strcpy(str, s);

    char *res = malloc(s_len);
    res[0] = '\0';

    // Tokens could be delimited by either newlines or whitespaces
    char* delimit = " \r\n";
    // Tokenize string and parse
    char* token = strtok(str, delimit);
    while( token != NULL ) {
        // Attempt finding the index of the morse code character
        // in the MORSE amarray
        char *mrs;
        int aIDX = -1;
        for (int j = 0; j < 37; j++){
            amarray_error_t a_err = MORSE->gets(MORSE, j, &mrs);
            if (a_err == 0 && strcmp(token, mrs) == 0){
                aIDX = j;
                break;
            }
        }
        // Attempt finding the ascii representation at index
        // aIDX
        char c;
        amarray_error_t m_err = ASCII->getc(ASCII, aIDX, &c);
        // If token is a valid morse code string and it has
        // an ascii representation
        if (m_err == 0) {
            // Add character to result
            strncat(res, &c, 1);
        } else {
            // Add (?) for unknown symbol
            strcat(res, "(?)");
        }
        // next token
        token = strtok(NULL, delimit);
    }
    *out = res;
}//morseToAscii


// Function that translates both ways depending on the flag
// Initial expected error handling, frees memory
// allocated for amarray implementations upon completion
void translate(char** flag, char**input){
    // Convert input to uppercase string
    strToUpper(*input);
    // Initialize flag status
    bool am = false;
    bool ma = false;
    // am or ma can be true iff flag is corresponding and input
    // for specified flag is considered valid
    if (strcmp(*flag, "-ma") == 0) {
        ma = isValidMorse(*input);
    } else if (strcmp(*flag, "-am") == 0){
        am = isValidAscii(*input);
    } else {
        // If flag is not -am or -ma we don't know what to do and therefore can't continue
        printf("Faulty flag! Please specify flag as either '-ma' for MORSE->ASCII or '-am' for ASCII->MORSE\n");
        printf("Run './mtrans -h' or './mtrans --help' for usage guide.\n");
        exit(0);
    }
    // If flag was correct, initialize amarrays needed for translation step
    amarray_t *ASCII = ascii_init();
    amarray_t *MORSE = morse_init();

    // Only attempt translating if input was valid based on specified flag
    if (am) {
        printf("Input text:\n%s\n", *input);
        char *mors;
        asciiToMorse(*input, ASCII, MORSE, &mors);
        printf("Morse representation:\n%s\n", mors);
        free(mors);
    } else if (ma) {
        printf("Input morse:\n%s\n", *input);
        char *asc;
        morseToAscii(*input, ASCII, MORSE, &asc);
        printf("Ascii representation:\n%s\n", asc);
        free(asc);
    } else {
        printf("Invalid input for %s! Exiting...\n", *flag);
    }
    ASCII->free(ASCII);
    MORSE->free(MORSE);
}//translate


// Function for reading input to be translated from a specified file
// fp1 points to a pointer of the opened file.
void translateFile(char **flag, FILE **fp1){
    fseek(*fp1, 0, SEEK_END);
    long size = ftell(*fp1);
    fseek(*fp1, 0, SEEK_SET);

    char *buffer = malloc(size * sizeof(char));

    if (buffer != NULL) {
        size_t amt_read = fread(buffer, sizeof(char), size, *fp1);

        // If file read goes wrong for some reason, halt
        if (amt_read != size) {
            printf("Read wrong amount of elements! File read failure. Exiting...\n");
            free(buffer);
            fclose(*fp1);
            exit(0);
        } else {
            translate(flag, &buffer);
        }
        // Free the memory allocated for input as it is no longer needed
        free(buffer);
    }
    // Close file
    fclose(*fp1);
}//translateFile


// Function for defining behaviour upon receiving a single argument
// If the argument is -h or --help, display usage guidelines.
// Otherwise flag must be faulty, alert and suggest running with help flag
void alertOrHelp(char **flag){
    if (strcmp(*flag, "-h") == 0 || strcmp(*flag, "--help") == 0){
        printf("Morse Translator v5.19.0 help section:\n");
        printf("For translating morse code to ascii uppercase charaters and numbers 0-9 -> \n%s",
               "run './mtrans -ma \"yourmorse\" or './mtrans -ma \"nameoffilethatcontainsmorse\"\n");
        printf("For translating a sequence of ascii characters and numbers 0-9 to morse code -> \n%s",
               "run './mtrans -am \"yourmessage\" or './mtrans -am \"nameoffilethatcontainsmessage\"\n\n");
        printf("When translating from ascii to morse, only characters a-z, A-Z, %s\n",
               "numbers 0-9 and spaces are allowed!\nOtherwise an invalid input error will be thrown.\n");
        printf("When translating from morse to ascii, only valid morse (sequence of '.' and '-' symbols),\n%s",
               "whitespaces and the word separator '/' are allowed!\nOtherwise an invalid input error will be thrown.\n");
        printf("\nExample run symbols to morse -> ./mtrans -am \"Hello world\"\n");
        printf("Expected output: .... . .-.. .-.. --- / .-- --- .-. .-.. -..");
        printf("\nExample run morse to symbols -> ./mtrans -ma \".... . .-.. .-.. --- / .-- --- .-. .-.. -..\"\n");
        printf("Expected output: HELLO WORLD");
        printf("\nExample failure run -> ./mtrans -am \"Hello, world!\"\n");
        printf("Expected output: Invalid input for -am Exiting...\n");
    } else {
        printf("Faulty flag or missing input! Please specify flag as either '-ma' for MORSE->ASCII or '-am' for ASCII->MORSE and pass input.\n");
        printf("Run './mtrans -h' or './mtrans --help' for usage guide.\n");
    }
}//alertOrHelp
