#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <stdio.h>

void createFile(char *fileName);
void copyFile(char *fileName, char* copyName);
void deleteFile(char *fileName);
void showFile(char *fileName);
int fileExists(char *fileName);
int canWrite(char *fileName);
int canRead(char *fileName);
size_t fileLines(char *fileName);

// Added as part of GENERAL OP

void internalCopyFile(char *fileName, char* copyName);


#endif