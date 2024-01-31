#include "dir.h"
#include "liste.h"
#include "projet.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

List *listdir(char *root_dir) {
  List *L = initList();
  DIR *dp = opendir(root_dir);
  struct dirent *ep;
  if (dp != NULL) {
    while ((ep = readdir(dp)) != NULL) {
      insertFirst(L, buildCell(ep->d_name));
    }
  }
  closedir(dp);
  return L;
}

struct stat st = {0};
int file_exists(char *file) {
  struct stat buffer;
  return (stat(file, &buffer) == 0);
}

void cp(char *to, char *from) {
  if (file_exists(from) != 0) {
    char ligne[256];
    FILE *ficfrom = fopen(from, "r");
    if (ficfrom == NULL) {
      printf("cp : Erreur Ouverture\n");
    }
    FILE *ficto = fopen(to, "w");
    while (fgets(ligne, 256, ficfrom) != NULL) {
      fputs(ligne, ficto);
    }
    fclose(ficto);
    fclose(ficfrom);
  } else {
    printf("cp : le fichier %s n'existe pas\n", from);
  }
}

char* hashToPath(char* hash){
  int n = strlen(hash);
  char* dir = malloc((n+2)*sizeof(char));
  dir[0] = hash[0]; 
  dir[1] = hash[1];
  dir[2] = '/';
  int i = 0;
  for (i = 3 ; i <= n; i++){ 
    dir[i] = hash[i-1];
  }
  dir[n+1]= '\0'; 
  return dir;
}

void blobFile(char *file) {
  char *hash = sha256file(file);
  char *path = hashToPath(hash);
  char command[500];
  char *dir = strdup(hash);
  dir[2] = '\0';
  if (file_exists(dir) == 0) {
    sprintf(command, "mkdir %s", dir);
    system(command);
  }
  cp(path, file);
  free(dir);
  free(path);
  free(hash);
}