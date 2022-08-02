/*
Description: This program, which runs solely as a command line utility,
             converts morse code to english and vice versa.
             Based on the flags that the user enters, the input will be taken
             from a file and the output will be written to a file as well.
Usage: <exe> <input file> <-mc|-e|-mcx|-ex> <output file> |<-v>|
        *The verbose flag is optional, if the user wants detailed info*
*/

// Include libraries needed for the program
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Prototypes for the functions used in this program
const char* englishToMorse(char, bool);
char morseToEnglish(const char*, bool);
char toUpperCase(char);
void englishToMorseHandler(FILE*, FILE*, bool, bool);
void morseToEnglishHandler(FILE*, FILE*, bool, bool);

// Main function that handles error checking as well as calling the respective handler function
int main(int argc, char** argv)
{
    // Check if the number of arguments is either 4 or 5, if not throw error
    if (argc == 4 || argc == 5)
    {}
    else
    {
        printf("Error -- usage: <exe> <input file> <-mc|-e|-mcx|-ex> <output file> |<-v>|\n");
        return 0;
    }

    // Get the respective fields from the user argument into variables
    char *file1 = argv[1];
    char *conv = argv[2];
    char *file2 = argv[3];
    bool includePunc = false;
    bool verbose = false;

    // if the conversion flag is not correct, throw an error
    if (strcmp(conv, "-mc") == 0 || strcmp(conv, "-mcx") == 0 || strcmp(conv, "-e") == 0 || strcmp(conv, "-ex") == 0)
    {}
    else
    {
        printf("Error -- usage: <exe> <input file> <-mc|-e|-mcx|-ex> <output file> |<-v>|\n");
        return 0;
    }
    // if the verbose flag is entered incorrectly, throw an error
    if (argc == 5 && strcmp(argv[4], "-v") != 0)
    {
        printf("Error -- usage: <exe> <input file> <-mc|-e|-mcx|-ex> <output file> |<-v>|\n");
    }
    // if the user chooses to include the verbose flag, set verbose boolean to true
    if (argc == 5)
    {
        verbose = true;
    }
    // check if the user wants to include punctuation, if so, set includePunc boolean to true
    if (strcmp(conv, "-mcx") == 0 || strcmp(conv, "-ex") == 0)
    {
        includePunc = true;
    }

    // create file objects 
    FILE *infile = fopen(file1, "r");
    FILE *outfile = fopen(file2, "w");
    // if file was not opened successfully, throw an error
    if (infile == NULL || outfile == NULL)
    {
        printf("There was an error accessing the files\n");
        printf("Error -- usage: <exe> <input file> <-mc|-e|-mcx|-ex> <output file> |<-v>|\n");
        return 0;
    }
    
    // if the conversion is to morse code
    if (strcmp(conv, "-mc") == 0 || strcmp(conv, "-mcx") == 0)
    {
        // if verbose, print different statement
        if (verbose)
        {
            printf("\nProcessing %s...\n", file1);
        }
        else
        {
            printf("\nTranslating %s to morse code\n", file1);
        }
        // call respective handler function
        englishToMorseHandler(infile, outfile, includePunc, verbose);
        printf("\n%s file completed.\n", file2);
        return 1;
    }
    // else if the conversion is to english
    else if (strcmp(conv, "-e") == 0 || strcmp(conv, "-ex") == 0)
    {
        // if verbose, print different statement
        if (verbose)
        {
            printf("\nProcessing %s...\n", file1);
        }
        else
        {
            printf("\nTranslating %s to English\n", file1);
        }
        // call respective handler function
        morseToEnglishHandler(infile, outfile, includePunc, verbose);
        printf("\n%s file completed.\n", file2);
        return 1;
    }
    // if function is not returned by now, there must be error, so throw an error and exit
    printf("Error -- usage: <exe> <input file> <-mc|-e|-mcx|-ex> <output file> |<-v>|\n");
    return 0;
}

// Function that handles conversion of english from input text to morse in output text
void englishToMorseHandler(FILE *infile, FILE *outfile, bool includePunc, bool verbose)
{
    // initialize some variables
    char data[255];
    char *token;
    const char *str;
    int i;
    int j;
    // loop over each line until we get null(file ends)
    while (fgets(data, 255, infile) != NULL) {
        // tokenize the line on each character
        token = strtok(data, "");
        while (token != NULL)
        {
            // get rid of the newline character at the end of a line
            // or else it will interfere with the intended output
            if (token[strlen(token) - 1] == '\n' || token[strlen(token) - 1] == '\r')
            {
                token[strlen(token) - 1] = 0;
            }
            // if verbose, print detailed statements
            if (verbose)
            {
                printf("READ:: %s\n", token);
            }
            if (verbose)
            {
                printf("WROTE:: ");
            }
            // loop over each character and call function to get morse output
            for (i = 0, j = strlen(token); i < j; i++)
            {
                str = englishToMorse(token[i], includePunc);
                if (verbose)
                {
                    printf("%s", str);
                }
                // print the morse into output file
                fprintf(outfile, str);
                // if it is not the end of the line, don't print "|"
                if (i == j - 1 || token[i + 1] == '\n' || token[i + 1] == '\r')
                {}
                else
                {
                    if (verbose)
                    {
                        printf("|");
                    }
                    fprintf(outfile, "|");
                }
            }
            if (verbose)
            {
                printf("\n");
            }
            fprintf(outfile, "\n");
            token = strtok(NULL, "");
        }
    }
    // close files
    fclose(infile);
    fclose(outfile);
}

// Function that handles conversion of morse code from input text to english in output text
void morseToEnglishHandler(FILE *infile, FILE *outfile, bool includePunc, bool verbose)
{
    // initialize some variables
    char data[255];
    char *token;
    char str;
    // loop until end of file
    while (fgets(data, 255, infile) != NULL) {
        // get rid of the newline character at the end of a line
        // or else it will interfere with the intended output
        if (data[strlen(data) - 1] == '\n' || data[strlen(data) - 1] == '\r')
        {
            data[strlen(data) - 1] = 0;
        }
        if (verbose)
        {
            printf("READ:: %s\n", data);
        }
        // tokenize the line on the "|" symbol
        token = strtok(data, "|");
        if (verbose)
        {
            printf("WROTE:: ");
        }
        while (token != NULL)
        {
            // get character by passing in morse code to this function
            str = morseToEnglish(token, includePunc);

            // print to output file
            fprintf(outfile, "%c", str);

            // tokenize again
            token = strtok(NULL, "|");
            if (verbose)
            {
                printf("%c", str);
            }
        }
        // print nextline in the command line and the output file
        if (verbose)
        {
            printf("\n");
        }
        fprintf(outfile, "\n");
    }
    // close files
    fclose(infile);
    fclose(outfile);
}

// Function that converts a single character into morse code
const char* englishToMorse(char alphabet, bool includePunc)
{
    // initialize arrays of alphabet and number morse codes
    char *morse[26] = {"*-", "-***", "-*-*", "-**", "*", "**-*", "--*",
                    "****", "**", "*---", "-*-", "*-**", "--", "-*", "---",
                    "*--*", "--*-", "*-*", "***", "-", "**-", "***-",
                    "*--", "-**-", "-*--", "--**"};
    
    char *numbers[10] = {"-----", "*----", "**---", "***--", "****-", "*****",
                         "-****", "--***", "---**", "----*"};

    // check if it is whitespace, and return accordingly
    if (alphabet == '\n' || alphabet == '\r')
    {
        return "";
    }
    if (alphabet == ' ')
    {
        return "/";
    }
    // based on the ascii table, use the above arrays to return corresponding morse code
    int num = (int)(toUpperCase(alphabet));
    char *to_return;
    // if alphabet, use the morse array
    if (num >= 65 && num <= 90)
    {
        to_return = morse[num - 65];
        return to_return;
    }
    // else if number, use the numbers array
    else if (num >= 48 && num <= 57 && includePunc)
    {
        to_return = numbers[num - 48];
        return to_return;
    }
    // else if punctuation, use switch statements to get right morse code
    else if (includePunc)
    {
        // check if it matches any of the given symbols
        switch (alphabet)
        {
        case '*':
            return "*-*-*-";
            break;

        case ',':
            return "--**--";
            break;
        
        case '?':
            return "**--**";
            break;

        case '\'':
            return "*----*";
            break;
        
        case '!':
            return "-*-*--";
            break;
        
        case '/':
            return "-**-*";
            break;
        
        case '(':
            return "-*--*";
            break;

        case ')':
            return "-*--*-";
            break;

        case '&':
            return "*-***";
            break;

        case ':':
            return "---***";
            break;

        case ';':
            return "-*-*-*";
            break;

        case '=':
            return "-***-";
            break;

        case '+':
            return "*-*-*";
            break;

        case '-':
            return "-****-";
            break;

        case '_':
            return "**--*-";
            break;

        case '\"':
            return "*-**-*";
            break;

        case '$':
            return "***-**-";
            break;

        case '@':
            return "*--*-*";
            break;

        default:
            return "XXXX";
            break;
        }
    }
    // if none of the above, return the default output
    else
    {
        return "XXXX";
    }
}

// Function that converts a morse input into its equivalent alphabet
char morseToEnglish(const char *input, bool includePunc)
{
    // initialize morse arrays for alphabets and numbers
    char *morse[26] = {"*-", "-***", "-*-*", "-**", "*", "**-*", "--*",
                    "****", "**", "*---", "-*-", "*-**", "--", "-*", "---",
                    "*--*", "--*-", "*-*", "***", "-", "**-", "***-",
                    "*--", "-**-", "-*--", "--**"};
    
    char *numbers[10] = {"-----", "*----", "**---", "***--", "****-", "*****",
                         "-****", "--***", "---**", "----*"};
    
    // check if it is a newline, return accordingly
    if (strcmp(input, "\n") == 0 || strcmp(input, "\r") == 0)
    {
        return '\0';
    }
    // check if input is "XXXX", return accordingly
    if (strcmp(input, "XXXX") == 0)
    {
        return '#';
    }
    // check if input is "/", return accordingly
    else if (strcmp(input, "/") == 0)
    {
        return ' ';
    }
    // loop over morse array, if input matches, convert int to char and return it
    int i;
    for (i = 0; i < 26; i++)
    {
        if (strcmp(input, morse[i]) == 0)
        {
            char to_return = i + 65;
            return to_return;
        }
    }
    // loop over number morse array, if input matches, convert int to char and return it
    if (includePunc)
    {
        for (i = 0; i < 10; i++)
        {
            if (strcmp(input, numbers[i]) == 0)
            {
                char to_return = i + 48;
                return to_return;
            }
        }
        if (strcmp(input, "*-*-*-") == 0)
            return '*';
        else if (strcmp(input, "--**--") == 0)
            return ',';
        else if (strcmp(input, "**--**") == 0)
            return '?';
        else if (strcmp(input, "*----*") == 0)
            return '\'';
        else if (strcmp(input, "-*-*--") == 0)
            return '!';
        else if (strcmp(input, "-**-*") == 0)
            return '/';
        else if (strcmp(input, "-*--*") == 0)
            return '(';
        else if (strcmp(input, "-*--*-") == 0)
            return ')';
        else if (strcmp(input, "*-***") == 0)
            return '&';
        else if (strcmp(input, "---***") == 0)
            return ':';
        else if (strcmp(input, "-*-*-*") == 0)
            return ';';
        else if (strcmp(input, "-***-") == 0)
            return '=';
        else if (strcmp(input, "*-*-*") == 0)
            return '+';
        else if (strcmp(input, "-****-") == 0)
            return '-';
        else if (strcmp(input, "**--*-") == 0)
            return '_';
        else if (strcmp(input, "*-**-*") == 0)
            return '\"';
        else if (strcmp(input, "***-**-") == 0)
            return '$';
        else if (strcmp(input, "*--*-*") == 0)
            return '@';
        else
            return '%';
    }
    // if nothing, return '%' by default
    return '%';
}

// Function that converts characters to upper case using int to char conversions
char toUpperCase(char x)
{
    int num = (int)x;
    if (num >= 97 && num <= 122)
    {
        return (char)(num - 32);
    }
    else
    {
        return x;
    }
}