#include "projet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int hashFile(char* source, char* dest){
    char command[50];
    sprintf(command, "cat %s | sha256sum > %s",source,dest);
    system(command);
    return 0;
}


char* sha256file(char* file){
    static char template[] = "/tmp/myfileXXXXXX";
    char fname[100];
    strcpy(fname,template);
    int fd = mkstemp(fname);
    char command1[200];
    sprintf(command1, "cat %s | sha256sum > %s",file,fname);
    system(command1);
    FILE* f = fopen(fname,"r");
    if (f==NULL) {
        printf("sha256file : Erreur ouverture fichier\n");
        return NULL;
    }
    char* hash = (char*)malloc(500*sizeof(char));
    fscanf(f,"%s",hash);
    fclose(f);
    char command2[500];
    sprintf(command2, "rm %s",fname);
    system(command2);
    close(fd);
    return hash;
}
