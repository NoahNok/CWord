/**
 * @file file_operations.c
 * @author Noah Hollowell (Noah.Hollowell@warwick.ac.uk)
 * @brief Mmmmmmmm file I/O
 * Includs all method relating to file operations
 * @version 0.1
 * @date 2020-12-13
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "file_operations.h"
#include "interface.h"
#include "utils.h"
#include "line_operations.h"
#include "change_log.h"
#include "version_control.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>



/**
 * @brief Create a File object
 * 
 * @param fileName Name of the file
 */
void createFile(char *fileName){


    if (fileExists(fileName) == 1){
        char *m = concat("There is already a file called: ", fileName);
        infoScreen(m);
        free(m);
        return;
    }

    if (canWrite(".") == 0){
        infoScreen("You don't have permission to create the given file!");
        return;
    }



    FILE *file;
    file = fopen(fileName, "w");
    fclose(file);

    addToChangeLog(fileName, "CREATED", "");

    char *message = concat("Created file: ", fileName);
    infoScreen(message);
    free(message);
}

/**
 * @brief Copys a file
 * 
 * @param fileName Source file
 * @param copyName Copy file
 */
void copyFile(char *fileName, char* copyName){
    if (strcmp(fileName, copyName) == 0){
        infoScreen("The copied file cannot have the same name as the original!");
        return;
    }
    if (fileExists(fileName) == 0){
        char *m = concat(fileName, " doesn't exist!");
        infoScreen(m);
        free(m);
        return;
    }
    if (fileExists(copyName) == 1){
        char *m = concat("There is already a file called: ", copyName);
        infoScreen(m);
        free(m);
        return;
    }
    if (canRead(fileName) == 0){
        infoScreen("You don't have permission to copy this file!");
        return;
    }
    if (canWrite(".") == 0){
        infoScreen("You don't have permission to copy this file!");
        return;
    }
    waitScreen("Copying File.\nPlease wait...\n");

    internalCopyFile(fileName, copyName);

    copyChangeLog(fileName, copyName);
    infoScreen("Files successfully copied!");
    return;


}


/**
 * @brief Deletes the given file
 * 
 * @param fileName Name of the file to delete
 */
void deleteFile(char *fileName){
    if (fileExists(fileName) == 0){
        char *m = concat(fileName, " doesn't exist!");
        infoScreen(m);
        free(m);
        return;
    }

    char *deletedHash = saveDeletedFileForVersionControl(fileName);

    if (remove(fileName) == 0){
        addToChangeLog(fileName, "DELETED", deletedHash);
        free(deletedHash);
        infoScreen("File deleted!");
    } else {
        infoScreen("You don't have permission to delete this file!");
    }
}

/**
 * @brief Shows the file to the user, showing 15 lines at a time
 * 
 * @param fileName The file to show
 */
void showFile(char *fileName){
    if (fileExists(fileName) == 0){
        char *m = concat(fileName, " doesn't exist!");
        infoScreen(m);
        free(m);
        return;
    }
    if (canRead(fileName) == 0){
        infoScreen("You don't have permission to read this file!");
        return;
    }


    FILE *read;

    read = fopen(fileName, "r");

    char *line;
    size_t len = 0;
    ssize_t line_length;

    clearScreen();
    printHeader();
    char *m = concat(fileName, ":\n\n");
    printLine(m);
    free(m);
    
    size_t lines = fileLines(fileName);
    if (lines < 15){
        while ((line_length = getline(&line, &len, read)) != -1) {
            printLine(line);
        }
    } else {
        size_t lineCount = 0, totalLineCount = 0;
        
        while ((line_length = getline(&line, &len, read)) != -1) {
            printLine(line);
            lineCount++;
            totalLineCount++;
            if (lineCount > 19){
                printf("[%ld-%ld/%ld](ENTER to continue, c/C to close)>", totalLineCount-19, totalLineCount, lines);
                
                
                char c = getchar();
                
                if (tolower(c) == 'c'){
                    return;
                }
                 // option TWO to clean stdin

                clearScreen();
                printHeader();
                lineCount = 0;
            }
        }
    }
    

    fclose(read);
    printLine("\n\n----------------------------------------");
    waitForKey();
    return;
}

/**
 * @brief Determines if a file exits
 * 
 * @param fileName The file to check
 * @return int 1 if exists, 0 if it doesn't
 */
int fileExists(char *fileName){
    return access(fileName, F_OK) == 0 ? 1 : 0;
}

/**
 * @brief Determines if a file can be written
 * 
 * @param fileName The file to check
 * @return int 1 if you can write, 0 if not
 */
int canWrite(char *fileName){
    return access(fileName, W_OK) == 0 ? 1 : 0;
}

/**
 * @brief Determines if a file can be read
 * 
 * @param fileName The file to check
 * @return int  1 if you can read it, 0 if not
 */
int canRead(char *fileName){
    return access(fileName, R_OK) == 0 ? 1 : 0;
}

/**
 * @brief Returns the amount of lines in a given file
 * 0 if the files can't be read or doesn't exist
 * 
 * @param fileName The file to count
 * @return size_t Amount of line
 */
size_t fileLines(char *fileName){

    if (fileExists(fileName) == 0 || canRead(fileName) == 0) return 0;

    size_t lines = 0;
    char currentChar;

    FILE *file = fopen(fileName, "r");

    for (currentChar = getc(file); currentChar != EOF; currentChar = getc(file)){
        if (currentChar == '\n') {
            lines++;
        }
    }

    fclose(file);
    return lines;
}


/**
 * @brief Performs the actual file copying
 * 
 * @param fileName Source file
 * @param copyName Copy file
 */
void internalCopyFile(char *fileName, char* copyName){
    FILE *source, *copy;
    source = fopen(fileName, "r");
    copy = fopen(copyName, "w");

    char charToCopy;

    while ((charToCopy = fgetc(source)) != EOF){
        fputc(charToCopy, copy);
    }
    fclose(source);
    fclose(copy);
}

