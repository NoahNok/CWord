/**
 * @file line_operations.c
 * @author Noah Hollowell (Noah.Hollowell@warwick.ac.uk)
 * @brief All the methods relating to line operations
 * @version 0.1
 * @date 2020-12-13
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "line_operations.h"
#include "file_operations.h"
#include "interface.h"
#include "utils.h"
#include "change_log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * @brief Appends a lien to the file once asked
 * 
 * @param fileName The file to append to
 */
void appendLine(char *fileName){

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
    if (canWrite(fileName) == 0){
        infoScreen("You don't have permission to write to this file!");
        return;
    }

    clearScreen();
    printHeader();

    printLastNLines(fileName, 15);

    char *input = malloc(256*sizeof(char));
   
    char *help = "[New Line !n, Exit !c] >";

    int lineAdds = 0;
    printLine(help);
    while (fgets(input, 256, stdin)){

        if (input[0] == '!' && (input[1] == 'c' || input[1] == 'C') && strlen(input) == 3){
            free(input);
            char number[4];
            sprintf(number, "+%d", lineAdds);
            addToChangeLog(fileName, "APPEND", number);
            return;
        }
        lineAdds++;

        input = sanitise(input);
        internalAppendLine(fileName, input);

        clearScreen();
        printHeader();
        printLastNLines(fileName, 15);
        printLine(help);
    }
    
}

/**
 * @brief Deletes the line of a file
 * 
 * @param fileName The file to delete from
 * @param lineNumber The line to delete
 */
void deleteLine(char *fileName, int lineNumber){
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
    if (canWrite(fileName) == 0){
        infoScreen("You don't have permission to write to this file!");
        return;
    }

    clearScreen();
    printHeader();

    size_t min, max;
    calculateMinMax(&min, &max, &lineNumber, fileLines(fileName), 5);

    printLinesFromXToYHighlightingZ(fileName, min, max, lineNumber);
    printLine("\nAre you sure you want to delete this line (y/N)? ");
    char input = getchar();
    clearInputBuffer();
    if (tolower(input) != 'y'){
        infoScreen("Line removal cancelled!");
        return;
    }
    // Delete line, write file contents to itself skipping invalid line

    char *tempName = concat(fileName,".replica.cword.txt");

    FILE *temp, *source;
    temp = fopen(tempName, "w");
    source = fopen(fileName, "r");

    char *line;
    size_t len = 0;
    size_t lineCount = 0;  
    ssize_t l = 0;

    char *deletedLine;  

    while ((l = getline(&line, &len, source)) != -1) {
        lineCount++;
        if (lineCount == lineNumber) {
            deletedLine = malloc((l + 1) * sizeof(char));
            strcpy(deletedLine, line);
            continue;
        }

        fprintf(temp, "%s", line);
    }
    fclose(temp);
    fclose(source);
    remove(fileName);
    rename(tempName, fileName);
    free(tempName);
    

    deletedLine[strlen(deletedLine)-1] = '\0';

  
    char *ln = intToString(lineNumber);

    char *info = concat3(ln, "::", deletedLine);
    free(ln);
    free(deletedLine);
    
    addToChangeLog(fileName, "DELETE", info);
    free(info);

    infoScreen("Line successfully deleted!");
}

/**
 * @brief Inserts a line into a file
 * 
 * @param fileName The file to insert into
 * @param lineNumber The line number to insert at
 */
void insertLine(char *fileName, int lineNumber){
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
    if (canWrite(fileName) == 0){
        infoScreen("You don't have permission to write to this file!");
        return;
    }

    clearScreen();
    printHeader();

    size_t min, max;
    calculateMinMax(&min, &max, &lineNumber, fileLines(fileName), 5);
   
    printLinesFromXToYHighlightingZ(fileName, min, max, lineNumber);
    printLine("\n[Type !c to cancel?]> ");

    char *input = malloc(sizeof(char) * 256);
    fgets(input, 256, stdin);
    if (input[0] == '!' && (input[1] == 'c' || input[1] == 'C') && strlen(input) == 3){
            free(input);
            infoScreen("Line insertion cancelled!");
            return;
    }

    input = sanitise(input);
    internalInsertLine(fileName, lineNumber, input);

    char *ln = intToString(lineNumber);
    addToChangeLog(fileName, "INSERT", ln);
    free(ln);
    

    infoScreen("Line successfully inserted!");
}

/**
 * @brief Shows the line of a file and some surrounding context
 * 
 * @param fileName The file to show from
 * @param lineNumber The number of the line to show
 */
void showLine(char *fileName, int lineNumber){
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
    if (canWrite(fileName) == 0){
        infoScreen("You don't have permission to write to this file!");
        return;
    }

    clearScreen();
    printHeader();

    
    size_t min, max;
    calculateMinMax(&min, &max, &lineNumber, fileLines(fileName), 6);

    //printf("min:%ld max:%ld ln: %d", min, max, lineNumber); Debugging as min was wrong
    //waitForKey();
    printLinesFromXToYHighlightingZ(fileName, min, max, lineNumber);

    waitForKey();
    return;
}

/**
 * @brief Calculates the minimum and maximum lines of the file to view given the current line,
 * total lines and amount of lines you want to see each side of the current line
 * 
 * @param min Pointer to the minimum variable
 * @param max Pointer to the maximum variable
 * @param lineNumber Pointer to the current line number
 * @param totalLines The total lines in the file
 * @param deviation How many lines to print either side of the current line
 */
void calculateMinMax(size_t *min, size_t *max, int *lineNumber, size_t totalLines, size_t deviation){
    if (*lineNumber < 1) *lineNumber = 1;
    if (*lineNumber > totalLines) *lineNumber = totalLines;

    *min = ((int)(*lineNumber-deviation) < 1) ? 1 : (*lineNumber-deviation);

    *max = ((*lineNumber+deviation) > totalLines) ? totalLines+1 : (*lineNumber+deviation);// +1 for last line text no \n

    if (*max < (deviation * 2) + 1) *max = (deviation * 2) + 1;

    if (*lineNumber > (totalLines - deviation)){
        int minn = totalLines - (2* deviation);
        if (minn < 1) {
            *min = 1;
        } else {
            *min = minn;
        }

    }
}

/**
 * @brief Prints the last n lines of the file
 * 
 * @param fileName The file to print from
 * @param n The amount of lines to print
 */
void printLastNLines(char *fileName, size_t n){
    int totalLines = fileLines(fileName);

    FILE *file;
    file = fopen(fileName, "r");

    char *line;

    size_t len = 0;

    size_t lineCount = 0;    

    while (getline(&line, &len, file) != -1) {
        lineCount++;
        if ((totalLines-lineCount) <= n){
            printLine(line);
        }
    }
    fclose(file);

}

/**
 * @brief Prints lines from x to y highlighting the zth line
 * 
 * @param fileName The file to print from
 * @param x The minimum line
 * @param y The maximum line
 * @param z The line to highlight
 */
void printLinesFromXToYHighlightingZ(char *fileName, size_t x, size_t y, size_t z){
    int totalLines = fileLines(fileName);


    FILE *file;
    file = fopen(fileName, "r");

    char *line;
    size_t len = 0;
    size_t lineCount = 0;    

    while (getline(&line, &len, file) != -1) {
        lineCount++;
        if (lineCount <= y && lineCount >= x){
            lineCount == z ? printf("%ld > %s", lineCount, line) : printf("%ld  %s", lineCount, line);
        }
    }
    fclose(file);
}


/**
 * @brief Shows the line count and read write status to the user
 * 
 * @param fileName The file to check
 */
void showLineCount(char *fileName){
    if (fileExists(fileName) == 0){
        char *m = concat(fileName, " doesn't exist!");
        infoScreen(m);
        free(m);
        return;
    }
    if (canRead(fileName) == 0){
        infoScreen("You don't have permission to read this file!\n(Required to count lines!)");
        return;
    }
    size_t lineCount = fileLines(fileName);

    clearScreen();
    printHeader();

    char *message = concat(fileName, " Info:\n\n");
    printLine(message);
    free(message);
    printf("Lines: %ld\n", lineCount);
    printf("Readable: %s\n", (canRead(fileName) == 1 ? "Yes" : "No"));
    printf("Writable: %s\n", (canWrite(fileName) == 1 ? "Yes" : "No"));

    waitForKey();

}

/**
 * @brief Acutally appends the line to a file
 * 
 * @param fileName The file to append to
 * @param line The line to append
 */
void internalAppendLine(char *fileName, char* line){
    FILE *append;
    append = fopen(fileName, "a");

    fprintf(append, "%s", line);
        
    fclose(append);
}

/**
 * @brief Get the Last Line Of the file
 * 
 * @param fileName The file to get the last line
 * @return char* The lines content
 */
char * getLastLineOfFile(char *fileName){
    int totalLines = fileLines(fileName);

    FILE *file;
    file = fopen(fileName, "r");

    char *line;

    size_t len = 0;

    size_t lineCount = 0;    

    while (getline(&line, &len, file) != -1) {
        lineCount++;
        if ((totalLines-lineCount) == 0){
            break;
        }
    }
    fclose(file);
    return line;
}

/**
 * @brief Deletes the last n lines of a file
 * 
 * @param fileName The file to delete from
 * @param numberToDelete The amount of lines to delete
 */
void deleteLastNLinesOfFile(char *fileName, size_t numberToDelete){

    size_t totalLines = fileLines(fileName);

    char *tempName = concat(fileName, ".replica.cword.txt");

    FILE *temp, *source;
    temp = fopen(tempName, "w");
    source = fopen(fileName, "r");

    char *line;
    size_t len = 0;
    size_t lineCount = 0;  


    while (getline(&line, &len, source) != -1) {
        lineCount++;
        if ((totalLines - lineCount) < numberToDelete) continue;

        fprintf(temp, "%s", line);
    }



    fclose(temp);
    fclose(source);
    remove(fileName);
    rename(tempName, fileName);
    free(tempName);

}

/**
 * @brief Performs the actual delete operation
 * 
 * @param fileName The file to delete from
 * @param lineNumber The line to delete
 */
void internalDeleteLine(char *fileName, int lineNumber){
    char *tempName = concat(fileName,".replica.cword.txt");

    FILE *temp, *source;
    temp = fopen(tempName, "w");
    source = fopen(fileName, "r");

    char *line;
    size_t len = 0;
    size_t lineCount = 0;  

    char *deletedLine;  

    while (getline(&line, &len, source) != -1) {
        lineCount++;
        if (lineCount == lineNumber) continue;

        fprintf(temp, "%s", line);
    }
    fclose(temp);
    fclose(source);
    remove(fileName);
    rename(tempName, fileName);
    free(tempName);
}

/**
 * @brief Inserts the line into the file
 * 
 * @param fileName The file to isnert into
 * @param lineNumber The line to insert at
 * @param lineContent The content to insert
 */
void internalInsertLine(char *fileName, int lineNumber, char *lineContent){
    char *tempName = concat(fileName,".insert.cword.txt");

    FILE *temp, *source;
    temp = fopen(tempName, "w");
    source = fopen(fileName, "r");

    char *line;
    size_t len = 0;
    size_t lineCount = 0;    

    while (getline(&line, &len, source) != -1) {
        lineCount++;
        if (lineCount == lineNumber) {
            fprintf(temp, "%s", lineContent);
        }

        fprintf(temp, "%s", line);
    }

    fclose(temp);
    fclose(source);
    remove(fileName);
    rename(tempName, fileName);
    free(tempName);
}

/**
 * @brief Get the Line N Of the file
 * 
 * @param fileName The file to get the line from
 * @param lineNumber The line to get
 * @return char* The lines content
 */
char * getLineNOfFile(char *fileName, int lineNumber){


    FILE *file;
    file = fopen(fileName, "r");

    char *line;

    size_t len = 0;

    size_t lineCount = 0;    

    while (getline(&line, &len, file) != -1) {
        lineCount++;
        if (lineCount == lineNumber){
            break;
        }
    }
    fclose(file);
    return line;
}
