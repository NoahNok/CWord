/**
 * @file cw2_2.c
 * @author Noah Hollowell (Noah.Hollowell@warwick.ac.uk)
 * @brief The start point for the program
 * @version 0.1
 * @date 2020-12-13
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "interface.h"
#include "file_operations.h"
#include "line_operations.h"
#include "change_log.h"
#include "version_control.h"
#include "full_editor.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <ncurses.h>

void mainProgramRun();
void mainMenu();
void fileMenu();
void lineMenu();
void generalMenu();

struct QuestionOption options[4], fileOptions[5], lineOptions[5], generalOptions[5];

/**
 * @brief Program start point, initiates values and makes method calls to ensure that everything is
 * setup
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char *argv[]){

    // Make sure changelog can be made, also checks if script can create folders/files in dir
    if (initiateChangeLog() == 0) {
        clearScreen();
        return 0;
    }


    // Set defaults that wont be changed
    struct QuestionOption back = {"Back", 'b'};

    fileOptions[0] = (struct QuestionOption) {"Create File", 'c'};
    fileOptions[1] = (struct QuestionOption) {"Copy File", 'p'};
    fileOptions[2] = (struct QuestionOption) {"Delete File", 'd'};
    fileOptions[3] = (struct QuestionOption) {"Show File\n", 's'};
    fileOptions[4] = back;

    lineOptions[0] = (struct QuestionOption) {"Append Line", 'a'};
    lineOptions[1] = (struct QuestionOption) {"Delete Line", 'd'};
    lineOptions[2] = (struct QuestionOption) {"Insert Line", 'i'};
    lineOptions[3] = (struct QuestionOption) {"Show Line\n", 's'};
    lineOptions[4] = back;

    generalOptions[0] = (struct QuestionOption) {"Show Change Log", 's'};
    generalOptions[1] = (struct QuestionOption) {"Rollback file", 'r'};
    generalOptions[2] = (struct QuestionOption) {"Show # Lines in File\n", 'l'};
    generalOptions[3] = (struct QuestionOption) {"Full Editor\n", 'f'};
    generalOptions[4] = back;

    options[0] = (struct QuestionOption) {"File Operations", 'f'};
    options[1] = (struct QuestionOption) {"Line Operations", 'l'};
    options[2] = (struct QuestionOption) {"General Operations\n", 'g'};
    options[3] = (struct QuestionOption) {"Exit", 'e'};

    mainProgramRun();
    
    clearScreen();
    printHeader();
    printf("Goodbye!\n ");
    sleep(1);
    clearScreen();
    clear();
    

}

/**
 * @brief Shows the welcome screen
 * 
 */
void mainProgramRun(){
    clearScreen();
    infoScreen("Welcome to CWord\n\nWritten by Noah Hollowell\nNoah.Hollowell@warwick.ac.uk\nCopyright 2020-2021");
    mainMenu();
}

/**
 * @brief Shows the main menu option screen
 * 
 */
void mainMenu(){
    char input = getUserOption("Select an Option", options, 4);
    switch (input) {
        
        case 'f':
            fileMenu();
            break;

        case 'l':
            lineMenu();
            break;  

        case 'g':
            generalMenu();
            break;      

        case 'e':
            return;
            break;
    }
    mainMenu();
}

/**
 * @brief Shows the file operations option screen
 * 
 */
void fileMenu(){
    char input = getUserOption("Select an Option", fileOptions, 5);
    switch (input){
        case 'c':
            {
                char *input = getUserInput("Please provide a file name: ");
                createFile(input);
                free(input);
                break;
            }

        case 'p':
            {
                char *file = getUserInput("Please provide the name of the file to copy: ");
                char *copy = getUserInput("Please provide the name for the copy (inc extension): ");
                copyFile(file, copy);
                free(file);
                free(copy);
                break;
            }
        case 'd':
            {
                char *input = getUserInput("Please provide the name of the file to delete: ");
                deleteFile(input);
                free(input);
                break;
            }

        case 's':
            {
                char *input = getUserInput("Please provide the name of the file to show: ");
                showFile(input);
                free(input);
                break;
            }

        case 'b':
            return;
    }
    clearInputBuffer();
    fileMenu();
}

/**
 * @brief Shows the line operations option list
 * 
 */
void lineMenu(){
    char input = getUserOption("Select an Option", lineOptions, 5);
    switch (input){
        case 'a':
            {
                char *input = getUserInput("Please provide the name of the file to append: ");
                appendLine(input);
                free(input);
                break;
            }

        case 'd':
        {
            char* file = getUserInput("Please provide the name of the file to delete from: ");
            int line = getIntegerInput("Please provide the line number to delete: ");
            deleteLine(file, line);
            free(file);
            break;
        }

        case 'i':
        {
            char* file = getUserInput("Please provide the name of the file to insert to: ");
            int line = getIntegerInput("Please provide the line number to insert: ");
            insertLine(file, line);
            free(file);
            break;
        }
        case 's':
        {
            char* file = getUserInput("Please provide the name of the file to show: ");
            int line = getIntegerInput("Please provide the line number to show: ");
            showLine(file, line);
            free(file);
            break;
        }

        case 'b':
            return;
    }
    clearInputBuffer();
    lineMenu();
}

/**
 * @brief Shows the general operations option list
 * Includes extension operations
 * 
 */
void generalMenu(){
    char input = getUserOption("Select an Option", generalOptions, 5);
    switch (input){
        case 's':
            {
                char *input = getUserInput("Please provide the file name to view its changlog (Inc deleted files): ");
                viewChangeLog(input);
                free(input);
                break;
            }

        case 'r':
            {
                char *input = getUserInput("Please provide the name of the file to rollback: ");
                rollback(input);
                free(input);
                break;  
            } 
        
        case 'l':
            {
                char *input = getUserInput("Please provide the name of the file to count the lines for: ");
                showLineCount(input);
                free(input);
                break;
            }

        case 'f':
            {
                char *input = getUserInput("Please provide the name of the file to edit: ");
                editor(input);
                free(input);
                clearScreen();
            }    

        case 'b':
            return;
    }
    clearInputBuffer();
    generalMenu();
}




