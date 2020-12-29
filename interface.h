#ifndef INTERFACE_H
#define INTERFACE_H

#include <stddef.h>

#define clearScreen() printf("\033[H\033[J")

struct QuestionOption
{
    char option[50];
    char representativeChar;
};
void printHeader();
void printMessage(char *message);
void printLine(char *line);
void printChar(char cchar);
void infoScreen(char *info);
void clearInputBuffer();
void waitScreen(char *message);
void waitForKey();
char getUserOption(char *question, struct QuestionOption options[], int length);
char* getUserInput(char *question);

// Part of line op
int getIntegerInput(char *question);

#endif