/**
 * @file interface.c
 * @author Noah Hollowell (Noah.Hollowell@warwick.ac.uk)
 * @brief Contains all the methods allowing us to take user inputs
 * @version 0.1
 * @date 2020-12-13
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "interface.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



/**
 * @brief Prints the header
 * 
 */
void printHeader(){
    printLine("######################################## CWord ########################################\n\n");
}

/**
 * @brief Prints a message, resetting the screen first
 * 
 * @param message the message to print
 */
void printMessage(char *message){
    clearScreen();
    printHeader();
    printLine(message);
}

/**
 * @brief Prints a line
 * 
 * @param line pthe line to print
 */
void printLine(char *line){
    printf("%s", line);
}

/**
 * @brief Prints a char with a new line after
 * 
 * @param cchar The char to print
 */
void printChar(char cchar){
    printf("%c\n", cchar);
}

/**
 * @brief Shows an info screen with the given message
 * 
 * @param info The info to show
 */
void infoScreen(char *info){
    printMessage(info);
    waitForKey();

}

/**
 * @brief Clears the input buffer for new inputs
 * 
 */
void clearInputBuffer(){
    int ch = 0;
    while ((ch = getchar()) != '\n' && ch != '\r');
}

/**
 * @brief Shows a screen with a message whilst a task is running
 * 
 * @param message 
 */
void waitScreen(char *message){
    clearScreen();
    printHeader();
    printLine(message);
}

/**
 * @brief Prints a message and waits for the user to press enter to continue
 * 
 */
void waitForKey(){
    printLine("\n\nPress ENTER to continue");
    while(getchar()!='\n');
}

/**
 * @brief Provides the user with a list of options, they must selected one of the options.
 * If not it will re-ask until they do
 * 
 * @param question The question to ask
 * @param options A array of QuestionOption structures containing the options
 * @param length The amount of options to choose from
 * @return char The character of the selected option
 */
char getUserOption(char *question, struct QuestionOption options[], int length){
    int runs = 0;
    while (1 == 1){
        clearScreen();
        printHeader();
        int i;
        printf("%s\n\n", question);
        for (i = 0; i < length; i++){
            printf("    [%c] %s\n", options[i].representativeChar, options[i].option);
        }
        
        if (runs > 0){
            printLine("\nInvalid option, please try again: ");
        } else {
            printLine("\nPlease type the option to perform: ");
        }

        char input = tolower(getchar());
        clearInputBuffer();

        // Check if char was a valid option
        for (i = 0; i < length; i++){
            if (input == options[i].representativeChar){
                return input;
            }
        }
        runs++;
    }
}

/**
 * @brief Get the User Input
 * 
 * @param question The question to ask
 * @return char* The users input
 */
char* getUserInput(char *question){

    while (1 == 1){
        clearScreen();
        printHeader();
        int i;
        printf("%s", question);
        
        char *input = malloc(256*sizeof(char));
        scanf("%s", input);
        clearInputBuffer();
        
        if (strcmp(input, "\0") != 0){
            return input;
        }

    }
}

/**
 * @brief Get the users input as an integer
 * 
 * @param question The question to ask
 * @return int The parsed integer given
 */
int getIntegerInput(char *question){
    while (1 == 1){
        clearScreen();
        printHeader();
        int i;
        printf("%s", question);
        
        int input;
        scanf("%d", &input);
        clearInputBuffer();
        
        return input;

    }
}
