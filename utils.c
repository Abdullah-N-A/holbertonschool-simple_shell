#include "shell.h"

/* String copy */
char *_strcpy(char *dest, const char *src)
{
    char *ptr = dest;
    while ((*ptr++ = *src++))
        ;
    return dest;
}

/* String concatenate */
char *_strcat(char *dest, const char *src)
{
    char *ptr = dest + strlen(dest);
    while ((*ptr++ = *src++))
        ;
    return dest;
}
