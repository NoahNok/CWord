#ifndef VERSION_CONTROL_H
#define VERSION_CONTROL_H

#include <stddef.h>

void rollback(char *fileName);

void rollbackAppend(char *fileName, size_t numberOfLines);
void rollbackInsert(char *fileName, size_t lineNumber);
void rollbackDelete(char *fileName, size_t lineNumber, char *line);
void rollbackCreated(char *fileName);
void rollbackDeleted(char *fileName, char *timeHash);
char * saveDeletedFileForVersionControl(char *fileName);


#endif