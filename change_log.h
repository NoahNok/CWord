#ifndef CHANGE_LOG_H
#define CHANGE_LOG_H


int initiateChangeLog();
void addToChangeLog(char *fileName, char *operation, char *info);
void copyChangeLog(char *from, char *to);
void viewChangeLog(char *fileName);


#endif