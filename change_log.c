/**
 * @file change_log.c
 * @author Noah Hollowell (Noah.Hollowell@warwick.ac.uk)
 * @brief Methods relating to handling the change log
 * @version 0.1
 * @date 2020-12-13
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "change_log.h"
#include "file_operations.h"
#include "line_operations.h"
#include "interface.h"
#include "utils.h"

#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Initiates the change log making sure it can exits
 * 
 * @return int 1 If ok, 0 if something went wrong
 */
int initiateChangeLog(){
    if (canWrite(".") == 0){
        infoScreen("CWord doesn't have permission to create folders in this directory!\nPlease fix this in order to use CWord!\nMake you have permission to write here!");
        return 0;
    }

    if (dirExists(".cword") == 0){
        infoScreen("CWord can't access/create its settings folder\nMake sure you have permission to create files!\nMake sure you are the user that first ran CWord here!");
        return 0;
    } else {
        return 1;
    }
        
    
}

/**
 * @brief Adds a line to the change log with the given information
 * 
 * @param fileName The filename, may also be the path to the file
 * @param operation The operation to perform
 * @param info The info about the operation
 */
void addToChangeLog(char *fileName, char *operation, char *info){

    char *dirLocation = concat(".cword/", fileName);
    if (dirExists(dirLocation) == 0){
        infoScreen("CWord couldn't find this files changelog folder.\nThis change hasn't been recorded!");
        return;
    }
    free(dirLocation);

    char out[256];


    time_t now;
    time(&now);
    struct tm *time = localtime(&now);
    char stringTime[20];
    sprintf(stringTime, "%02d:%02d:%02d %02d/%02d/%d", time->tm_hour, time->tm_min, time->tm_sec, time->tm_mday, time->tm_mon + 1, time->tm_year + 1900);

    sprintf(out, "[%s]||%s||%s\n", stringTime, operation, info);
       

    char *location = concat3(".cword/", fileName, "/changelog.txt");

    internalAppendLine(location, out);
    free(location);
    
}

/**
 * @brief Copies the change log file 
 * 
 * @param from Name of the source file
 * @param to Name of the new file
 */
void copyChangeLog(char *from, char *to){

    char *fromLocation = concat3(".cword/", from, "/changelog.txt");
    char *toLocation = concat3(".cword/", to, "/changelog.txt");

    if (fileExists(fromLocation) == 0) {
        free(fromLocation);
        free(toLocation);
        return;
    }

    char *toDir = concat(".cword/", to);
    dirExists(toDir);
    free(toDir);

    

    internalCopyFile(fromLocation, toLocation);
    free(fromLocation);
    free(toLocation);
}
 /**
  * @brief Shows the change log to the user for viewing
  * 
  * @param fileName The name of the file
  */
void viewChangeLog(char *fileName){
    char *location = concat3(".cword/", fileName, "/changelog.txt");
    showFile(location);
    free(location);
}







