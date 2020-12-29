#ifndef FULL_EDITOR_H
#define FULL_EDITOR_H


void editor(char *fileName);

void printLinesNCurse(char *fileName, int x, int y, int z);

void attemptToAddLine(char *fileName, int lineNumber, int maxLines, char *line);




#endif