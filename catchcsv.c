/*#include <stdio.h>
#include <string.h>

char *trim(char *str)
{
    char *p = str; 
    while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')
        p ++;
    str = p; 
    p = str + strlen(str) - 1; 
    while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')
        -- p;
    *(p + 1) = '\0'; 
    return str; 
}
int main()
{
	FILE *fp = fopen("AnalyseErreurRelevé4.csv", "r");
	if(fp == NULL) {
		return -1;
	}
	char line[1024];
	while(fgets(line, sizeof(line), fp)) {
		printf("%s", line);
		char *save_ptr;
		char *name = strtok_r(line, ",", &save_ptr);
		if (name == NULL) {
			return -1;
		}  
		char *age = strtok_r(NULL, ",", &save_ptr);
		char *birthday = strtok_r(NULL, ",", &save_ptr);
		printf("%s\t%s\t%s\n", trim(Itération), trim(Orientation (en degrés)), trim(∆distance));
	}	
	return 0;
}
*/
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* getfield(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ";");
            tok && *tok;
            tok = strtok(NULL, ";\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}

int main()
{
    FILE* stream = fopen("AnalyseErreurRelevé4.csv", "r");

    char line[1024];
    while (fgets(line, 1024, stream))
    {
        char* tmp = strdup(line);
        printf("Field 3 would be %s\n", getfield(tmp, 3));
        // NOTE strtok clobbers tmp
        free(tmp);
    }
	return 0;
}
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main()
{
   char buffer[1024] ;
   char *record,*line;
   int i=0,j=0;
   int mat[100][100];
   FILE *fstream = fopen("AnalyseErreurRelevé4.xlsx","r");
   //FILE *fstream = fopen("AnalyseErreurRelevé4.csv","r");

   if(fstream == NULL)
   {
      printf("\n file opening failed ");
      return -1 ;
   }
   while((line=fgets(buffer,sizeof(buffer),fstream))!=NULL)
   {
     record = strtok(line,";");
     while(record != NULL)
     {
     printf("record : %s",record) ;    //here you can put the record into the array as per your requirement.
     mat[i][j++] = atoi(record) ;
     record = strtok(line,";");
     }
     ++i ;
   }
   return 0 ;
 }
