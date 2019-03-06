#ifndef _RESEARCH_H_
#define _RESEARCH_H_

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <regex.h>

bool regex_search(char *string, const char *regularExpression)
{
    regex_t reg;

    if(regcomp(&reg, regularExpression, REG_EXTENDED) != 0)
    {
        fprintf(
            stderr,
            "%s: %s: \"%s\".\n",
            "regex_search()",
            "Couldn't compile",
            regularExpression
        );
        exit(EXIT_FAILURE);
    }
    
    if(regexec(&reg, string, 0, NULL, 0) == REG_NOMATCH)
        return false;
    else
        return true;
}

#endif // _RESEARCH_H_
