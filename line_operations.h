#ifndef LINE_OPERATIONS_H
#define LINE_OPERATIONS_H

#include <stdio.h>

void appendLine(char *fileName);
void deleteLine(char *fileName, int lineNumber);
void insertLine(char *fileName, int lineNumber);
void showLine(char *fileName, int lineNumber);
void calculateMinMax(size_t *min, size_t *max, int *lineNumber, size_t totalLines, size_t deviation);
void printLastNLines(char *fileName, size_t n);
void printLinesFromXToYHighlightingZ(char *fileName, size_t x, size_t y, size_t z);

// Added for General
void showLineCount(char *fileName);

void internalAppendLine(char *fileName, char *line);

// Added for ext
char * getLastLineOfFile(char *fileName);
void deleteLastNLinesOfFile(char *fileName, size_t numberToDelete);
void internalDeleteLine(char *fileName, int lineNumber);
void internalInsertLine(char *fileName, int lineNumber, char *lineContent);

// Added for full editor
char * getLineNOfFile(char *fileName, int lineNumber);
#endif