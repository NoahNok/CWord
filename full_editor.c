/**
 * @file full_editor.c
 * @author Noah Hollowell (Noah.Hollowell@warwick.ac.uk)
 * @brief The wannabe nano file
 * Contains all the methods relating to the full editor
 * @version 0.1
 * @date 2020-12-13
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "full_editor.h"
#include "file_operations.h"
#include "line_operations.h"
#include "interface.h"
#include "utils.h"
#include "change_log.h"

#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <ncurses.h>

#ifndef CTRL
#define CTRL(c) ((c) & 037)
#endif

/**
 * @brief Initiates the editor
 * 
 * @param fileName The file to edit
 */
void editor(char *fileName){
    
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
    

    int linesToShow = getIntegerInput("How many lines would you like to view at a time (odd number only): ");
    infoScreen("Please note that changes saved immediately, accidentally closing will only loose\n a line that hasn't been entered!");

    // Calc deviation
    if (linesToShow % 2 == 1){
        linesToShow--;
    } else {
        linesToShow++;
    }

    

    linesToShow = linesToShow / 2;


    int lineNumber = 1;

    
    clearScreen();

    // Initiate Ncurse's
    initscr();
    //noecho();
    cbreak();
    keypad(stdscr, TRUE);
    clear();
    char line[1000];

    

    while (1 == 1){
        
        size_t totalLines = fileLines(fileName);
        size_t min, max;
        calculateMinMax(&min, &max, &lineNumber, totalLines, linesToShow);
        printLinesNCurse(fileName, min, max, lineNumber);

        int i;
        for (i = 0; i < 1000; i++){
            line[i] = '\0';
        }
        
        int lineCounter = 0;
        

        int key = getch();

        if (key == KEY_UP){
            lineNumber--;
        } else if (key == KEY_DOWN){
            lineNumber++;
        } else if (key == '\n'){
           
            attemptToAddLine(fileName, lineNumber, totalLines, "\n");
            lineNumber++;
            
        } else if (key == CTRL('d') && totalLines != 0) {
            char *dLine = getLineNOfFile(fileName, lineNumber);
            int i;
            for (i = 0; i < strlen(dLine); i++){
                if (dLine[i] == '\n') dLine[i] = '\0';
            }
            internalDeleteLine(fileName, lineNumber);
            char *ln = intToString(lineNumber);

            char *info = concat3(ln, "::", dLine);
            free(ln);
    
            addToChangeLog(fileName, "DELETE", info);
          free(info);
        } else if (key == CTRL('e')) {
            erase();
            refresh();
            endwin();
            break;
        } else {
            line[0] = key;
            while((key = getch()) != '\n'){
                if (key == KEY_BACKSPACE){
                    if (lineCounter >= 0){
                        printw(" \b");
                        line[lineCounter] = '\0';
                        lineCounter--;
                    } else
                    {
                        printw(" ");
                    }
                } else {
                    if (lineCounter > 999){
                        printw("\b \b");
                        line[lineCounter] = '\0';
                    } else {
                        lineCounter++;
                        line[lineCounter] = key;
                    }
                }
            }
            line[lineCounter+1] = '\n';

            if (line[0] == '!' && (line[1] == 'h' || line[1] == 'H') && strlen(line) == 3){
                endwin();
                infoScreen("The following are available:\n\n!h - This screen\nCTRL + D - Deletes current line\nCTRL + E - Exit editor\n");
                refresh();
                continue;
            }

            attemptToAddLine(fileName, lineNumber, totalLines, line);
            lineNumber++;
        }

        totalLines = fileLines(fileName);
        if (lineNumber > totalLines) lineNumber = totalLines;
        if (lineNumber < 1) lineNumber = 1;

        
        
    }
}

/**
 * @brief Adds the inputted line to the file by insertion or appendage
 * 
 * @param fileName File to insert to
 * @param lineNumber Line number to insert at
 * @param maxLines Total file lines
 * @param line The line to add
 */
void attemptToAddLine(char *fileName, int lineNumber, int maxLines, char *line){
    if (lineNumber == maxLines){
        internalAppendLine(fileName, line);
        addToChangeLog(fileName, "APPEND", "1");
        
    } else {
        internalInsertLine(fileName, lineNumber, line);
        char *ln = intToString(lineNumber);
        addToChangeLog(fileName, "INSERT", ln);
        free(ln);
    }
}

/**
 * @brief Prints files lines between x and y highlighting z using NCurse's methods
 * 
 * @param fileName The file to print lines for
 * @param x The minimum line
 * @param y The maximum line
 * @param z Line the highlight
 */
void printLinesNCurse(char *fileName, int x, int y, int z){
    int totalLines = fileLines(fileName);
    erase();
    printw("######################################## CWord ########################################\n");


    FILE *file;
    file = fopen(fileName, "r");

    char *line;
    size_t len = 0;
    size_t lineCount = 0;    

    while (getline(&line, &len, file) != -1) {
        lineCount++;
        if (lineCount <= y && lineCount >= x){
            lineCount == z ? printw("%ld > %s", lineCount, line) : printw("%ld  %s", lineCount, line);
        }
    }
    fclose(file);
    printw("[!h - Help]> ");
    refresh();
}

