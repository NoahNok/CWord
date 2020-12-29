/**
 * @file utils.c
 * @author Noah Hollowell (Noah.Hollowell@warwick.ac.uk)
 * @brief All the utility methods used by the program
 * @version 0.1
 * @date 2020-12-13
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "utils.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>


/**
 * @brief Concatenates 2 strings
 * 
 * https://stackoverflow.com/questions/8465006/how-do-i-concatenate-two-strings-in-c
 *  Make sure to free after use!
 * 
 * @param s1 string 1
 * @param s2 string 2
 * @return char* Concatenated string
 */
char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

/**
 * @brief Concatenates 3 strings
 * 
 * @param s1 string 1
 * @param s2 string 2
 * @param s3 string 3
 * @return char* Concatenated string
 */
char* concat3(const char *s1, const char *s2, const char *s3){
    char *result = malloc(strlen(s1) + strlen(s2) + strlen(s3) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    strcat(result, s3);
    return result;
}

/**
 * @brief Concatenates 4 strings
 * 
 * @param s1 string 1
 * @param s2 string 2
 * @param s3 string 3
 * @param s4 string 4
 * @return char* Concatenated string
 */
char* concat4(const char *s1, const char *s2, const char *s3, const char *s4){
    char *result = malloc(strlen(s1) + strlen(s2) + strlen(s3) + strlen(s4) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    strcat(result, s3);
    strcat(result, s4);
    return result;
}

/**
 * @brief Converts a string to and integer
 * 
 * @param number The string to convert
 * @return int The resulting integer
 */
int stringToInt(char *number){
    return strtol(number, NULL, 10);
}

/**
 * @brief Converts a integer to a string
 * 
 * @param number The integer to convert
 * @return char* The resulting string
 */
char * intToString(int number){
    int requiredChars = (int)((ceil(log10(number))+1)*sizeof(char));
    char * string = malloc(requiredChars);
    sprintf(string, "%d", number);
    return string;
}

/**
 * @brief Hashes the given string
 * 
 * http://www.cse.yorku.ca/~oz/hash.html
 * 
 * @param str The string to hash
 * @return char* The hashed string
 */
char * hashString(unsigned char *str){
    unsigned long hash = 5381;
    int c;

    while (c = *str++){
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    char *string = malloc(strlen(str));
    sprintf(string, "%lu", hash);
    return string;
}

/**
 * @brief Replaces a given word in the string with another
 * 
 * https://www.geeksforgeeks.org/c-program-replace-word-text-another-given-word/
 * 
 * @param s 
 * @param oldW 
 * @param newW 
 * @return char* 
 */
char* replaceWord(const char* s, const char* oldW, const char* newW) 
{ 
    char* result; 
    int i, cnt = 0; 
    int newWlen = strlen(newW); 
    int oldWlen = strlen(oldW); 
  
    // Counting the number of times old word 
    // occur in the string 
    for (i = 0; s[i] != '\0'; i++) { 
        if (strstr(&s[i], oldW) == &s[i]) { 
            cnt++; 
  
            // Jumping to index after the old word. 
            i += oldWlen - 1; 
        } 
    } 
  
    // Making new string of enough length 
    result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1); 
  
    i = 0; 
    while (*s) { 
        // compare the substring with the result 
        if (strstr(s, oldW) == s) { 
            strcpy(&result[i], newW); 
            i += newWlen; 
            s += oldWlen; 
        } 
        else
            result[i++] = *s++; 
    } 
  
    result[i] = '\0'; 
    return result; 
} 

/**
 * @brief Sanitises inputs removing || and ::
 * 
 * @param string The string to sanitise
 * @return char* The sanitised string
 */
char * sanitise(const char *string){
    return replaceWord(replaceWord(string, "::", ""), "||", "");
}

/**
 * @brief Checks if the given directory exists,
 * creates it if it doesnt
 * 
 * @param dirPath The directory
 * @return int 1 if exists/created 0 for problem (permission)
 */
int dirExists(char *dirPath){
    DIR *dir = opendir(dirPath);

    if (dir){
        return 1;
    } else if (ENOENT == errno) {
        mkdir(dirPath, 0770);
        return 1;
    } else {
        return 0;
    }
}