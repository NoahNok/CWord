/**
 * @file version_control.c
 * @author Noah Hollowell (Noah.Hollowell@warwick.ac.uk)
 * @brief Methods allowing version control
 * @version 0.1
 * @date 2020-12-13
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "version_control.h"
#include "file_operations.h"
#include "line_operations.h"
#include "utils.h"
#include "interface.h"
#include "change_log.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * @brief Initiates the rollback sequence
 * 
 * @param fileName The file to rollback
 */
void rollback(char *fileName){

    char *location = concat3(".cword/", fileName, "/changelog.txt");

    if (fileExists(location) == 0){
        free(location);
        infoScreen("That file doesn't have any changelog history!");
        return;
    }

    char *lastLine = getLastLineOfFile(location);

    if (strcmp(lastLine, "") == 0){
        infoScreen("There is nothing to rollback!");
        return;
    }
    

    char *operation;
    strtok(lastLine, "||");
    operation = strtok(NULL, "||");

    

    if (fileExists(fileName) == 1){
        if (strcmp(operation, "APPEND") == 0){
            rollbackAppend(fileName, stringToInt(strtok(NULL, "||")));
        } else if (strcmp(operation, "INSERT") == 0){
            rollbackInsert(fileName, stringToInt(strtok(NULL, "||")));
        } else if (strcmp(operation, "DELETE") == 0){
            char *together, *lineNumber, *lineContent;
            together = strtok(NULL, "||");
            lineNumber = strtok(together, "::");
            lineContent = strtok(NULL, "::");

            rollbackDelete(fileName, stringToInt(lineNumber), lineContent);
        } else if (strcmp(operation, "CREATED") == 0){
            rollbackCreated(fileName);
        } 
    } else {
        if (strcmp(operation, "DELETED") == 0){
            rollbackDeleted(fileName, strtok(NULL, "||"));
        } else {
            infoScreen("The file you are trying to rollback has been deleted!");
            return;
        }
    }
    
    
    deleteLastNLinesOfFile(location, 1);
    free(location);
}

/**
 * @brief Rolls back the append operation
 * 
 * @param fileName File to rollbakc
 * @param numberOfLines Amount of lines to rollback
 */
void rollbackAppend(char *fileName, size_t numberOfLines){
    deleteLastNLinesOfFile(fileName, numberOfLines);

    char *ln = intToString(numberOfLines);
    char *message = concat3("APPEND Operation Rolledback\nLast ", ln, " were deleted");
    free(ln);
    infoScreen(message);
    free(message);
}

/**
 * @brief Rolls back the insert operation
 * 
 * @param fileName File to rollback
 * @param lineNumber The line to rollback
 */
void rollbackInsert(char *fileName, size_t lineNumber){
    internalDeleteLine(fileName, lineNumber);
    char *ln = intToString(lineNumber);
    char *message = concat3("INSERT Operation Rolledback\nLine ", ln, " was deleted");
    free(ln);
    infoScreen(message);
    free(message);
}

/**
 * @brief Rolls back the delete operation
 * 
 * @param fileName The file to rollback
 * @param lineNumber The line number to rollbak
 * @param line The line content to rollback
 */
void rollbackDelete(char *fileName, size_t lineNumber, char *line){
    internalInsertLine(fileName, lineNumber, line);
    char *ln = intToString(lineNumber);
    char *message = concat3("DELETE Operation Rolledback\nLine ", ln, " was inserted");
    free(ln);
    infoScreen(message);
    free(message);
}

/**
 * @brief Rolls back the created operation
 * 
 * @param fileName The file to rollback
 */
void rollbackCreated(char *fileName){
    remove(fileName);
    infoScreen("CREATED Operation Rolledback\nThe file was deleted");
}

/**
 * @brief Rolls back the deleted operation
 * 
 * @param fileName The file to rollback
 * @param timeHash The hash of the time relating to the rolledback file
 */
void rollbackDeleted(char *fileName, char *timeHash){
    char *location = concat4(".cword/", fileName, "/", timeHash);
    location[strlen(location)-1] = '\0';
    internalCopyFile(location, fileName);
    infoScreen("DELETED Operation Rolledback\nThe file was created");
    remove(location);
    free(location);
}

/**
 * @brief Saves a copy of the deleted file for rolling back
 * 
 * @param fileName The file to copy
 * @return char* The hashed time string
 */
char * saveDeletedFileForVersionControl(char *fileName){
    time_t now;
    time(&now);
    struct tm *time = localtime(&now);
    char stringTime[20];
    sprintf(stringTime, "%02d:%02d:%02d %02d/%02d/%d", time->tm_hour, time->tm_min, time->tm_sec, time->tm_mday, time->tm_mon + 1, time->tm_year + 1900);

    char *hash = hashString(stringTime);

    char *location = concat4(".cword/", fileName, "/", hash);

    char *toDir = concat(".cword/", fileName);
    dirExists(toDir);
    free(toDir);
    internalCopyFile(fileName, location);

    free(location);
    

    return hash;
}