#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define J 113        
const char* getfield(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ";"); tok && *tok; tok = strtok(NULL, ";\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}

int main()
{   
    int i;  
    double tab[J]; 
    int n;
    char* tmp2;
    FILE* stream = fopen("ReleveSimple.csv", "r");
    n=2;
    i=0;
    char line[1024];
    while (fgets(line, 1024, stream))
    {
        char* tmp = strdup(line);
        tmp2 = getfield(tmp, n);
        printf("%s\n", tmp2);
        tab[i] = atof(tmp2);
        printf("%f\n",tab[i]);
        i++;
        free(tmp);
    }
return 0;
}
