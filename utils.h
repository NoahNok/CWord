#ifndef UTILS_H
#define UTILS_H

char* concat(const char *s1, const char *s2);
char* concat3(const char *s1, const char *s2, const char *s3);
char* concat4(const char *s1, const char *s2, const char *s3, const char *s4);
int stringToInt(char *number);
char* intToString(int number);
char * hashString(unsigned char *str);

char* replaceWord(const char* s, const char* oldW, const char* newW);
char * sanitise(const char *string);

int dirExists(char *dirPath);

#endif