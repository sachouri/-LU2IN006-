#include "work.h"
#include "dir.h"
#include "liste.h"
#include "projet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

int getChmod(const char *path) {
  struct stat ret;
  if (stat(path, &ret) == -1) {
    return -1;
  }
  return (ret.st_mode & S_IRUSR) | (ret.st_mode & S_IWUSR) | (ret.st_mode & S_IXUSR) | /*owner*/
         (ret.st_mode & S_IRGRP) | (ret.st_mode & S_IWGRP) | (ret.st_mode & S_IXGRP) | /*group*/
         (ret.st_mode & S_IROTH) | (ret.st_mode & S_IWOTH) | (ret.st_mode & S_IXOTH); /*other*/
}

void setMode(int mode, char *path) {
  char buff[1000];
  sprintf(buff, "chmod %o %s ", mode, path);
  system(buff);
}

WorkFile *createWorkFile(char *name) {
  WorkFile *wf = (WorkFile *)malloc(sizeof(WorkFile));
  wf->name = strdup(name);
  wf->hash = NULL;
  wf->mode = 0;
  return wf;
}

void freeWorkFile(WorkFile *wf) {
  free(wf->name);
  free(wf->hash);
  free(wf);
}

void afficheWorkFile(WorkFile* wf){
  if (wf==NULL){
    printf("WorkFile vide\n");
  } else {
    printf("%s\t%s\t%d\n",wf->name,wf->hash,wf->mode);
  }
}

char *wfts(WorkFile *wf) {
  if (wf != NULL) {
    char *chaine = (char *)malloc(sizeof(char) * 300);
    sprintf(chaine, "%s\t%s\t%d", wf->name, wf->hash, wf->mode);
    return chaine;
  }
  return NULL;
}

WorkFile *stwf(char *ch) {
  if (ch==NULL || strcmp(ch,"")==0){
    return NULL;
  } 
  char name[100];
  char *hash = malloc(sizeof(char) * 500);
  int mode;
  sscanf(ch, "%s\t%s\t%d", name, hash, &mode);
  WorkFile *wf = createWorkFile(name);
  wf->hash = hash;
  wf->mode = mode;
  return wf;
}

WorkTree *initWorkTree() {
  WorkTree *wt = (WorkTree *)malloc(sizeof(WorkTree));
  wt->tab = (WorkFile *)malloc(SIZE * sizeof(WorkFile));
  wt->size = SIZE;
  wt->n = 0;
  return wt;
}

void freeWorkTree(WorkTree *wt) {
  if (wt == NULL) {
    printf("WorkTree vide\n");
    return;
  }
  for (int i = 0; i < wt->n; i++) {
    free((wt->tab[i]).name);
    free((wt->tab[i]).hash);
  }
  free(wt->tab);
  free(wt);
}

void afficheWorkTree(WorkTree* wt){
    if (wt==NULL){
        printf("WorkTree vide\n");
        return ;
    }
    for (int i =0; i<wt->n;i++){
        afficheWorkFile(&(wt->tab[i]));
    }
}

int inWorkTree(WorkTree *wt, char *name) {
  if (wt==NULL){
    printf("WorkTree vide\n");
    return -1;
  }
  for (int i = 0; i < wt->n; i++) {
    if (strcmp(wt->tab[i].name, name) == 0) {
      return i;
    }
  }
  return -1;
}

int appendWorkTree(WorkTree *wt, char *name, char *hash, int mode) {
  if (wt == NULL) {
    wt = initWorkTree();
  }
  int ind = wt->n;
  if (wt->size <= ind) {
    printf("Le WorkTree est complet\n");
    return -1;
  }
  if (inWorkTree(wt, name) == -1) {

    wt->tab[ind].name = strdup(name);
    wt->tab[ind].mode = mode;

    if (hash != NULL) {
      wt->tab[ind].hash = strdup(hash);
    } 
    else {
      wt->tab[ind].hash = NULL;
    }
    
    wt->n++;
    return ind;
  }
  printf("%s existe deja dans le WorkTree\n", name);
  return -1;
}

char *wtts(WorkTree *wt) {
  if (wt == NULL){
    printf("WorkTree null \n");
    return NULL;
  }
  char *chaine = malloc(50000 * sizeof(char));
  for (int i = 0; i < wt->n; i++) {
    char *s = wfts(&(wt->tab[i]));
    strcat(chaine, s);
    if (i != wt->n - 1) {
      strcat(chaine, "\n");
    }
    free(s);
  }
  return chaine;
}

WorkTree *stwt(char *s) {
  if (s == NULL) {
    return NULL;
  }
  WorkTree *wt = initWorkTree();
  if (strcmp(s, "") == 0) {
    return wt;
  }
  char data[100];
  int r = 0;
  int i = 0;
  while (s[r] != '\0') {
    if (s[r] == '\n') {
      data[i] = '\0';
      WorkFile *wf = stwf(data);
      appendWorkTree(wt, wf->name, wf->hash, wf->mode);
      freeWorkFile(wf);
      i = 0;
    } else {
      data[i] = s[r];
      i++;
    }
    r++;
  }
  data[i] = '\0';
  WorkFile *wf = stwf(data);
  appendWorkTree(wt, wf->name, wf->hash, wf->mode);
  freeWorkFile(wf);
  return wt;
}

int wttf(WorkTree *wt, char *file) {
  char *chaine = wtts(wt);
  FILE *f = fopen(file, "w");
  if (f == NULL) {
    printf("wttf : Erreur ouverture fichier\n");
    return 1;
  }
  fprintf(f, "%s", chaine);
  fclose(f);
  free(chaine);
  return 0;
}

WorkTree *ftwt(char *file) {
  FILE *f = fopen(file, "r");
  if (f == NULL) {
    printf("ftwt : Erreur ouverture fichier\n");
    return NULL;
  }
  WorkTree *wt = initWorkTree();
  char buffer[100];
  while (fgets(buffer, 100, f) != NULL) {
    WorkFile *wf = stwf(buffer);
    appendWorkTree(wt, wf->name, wf->hash, wf->mode);
    freeWorkFile(wf);
  }
  fclose(f);
  return wt;
}

char* hashToPathWorkTree(char* hash){
  char* path = hashToPath(hash);
  strcat(path,".t");
  return path ;
}

char* blobWorkTree(WorkTree* wt){
    static char template[] = "/tmp/myfileXXXXXX";
    char fname[100];
    strcpy(fname,template);
    mkstemp(fname);
    wttf(wt,fname);
    char* hash = sha256file(fname);
    char* path = hashToPathWorkTree(hash);
    char* dir = strdup(path);
    dir[2] = '\0';
    /* si le repertoire n'existe pas on le cree */
    if (file_exists(dir) == 0){
        char command[500];
        sprintf(command, "mkdir %c%c",path[0],path[1]);
        system(command);
    }
    cp(path,fname);
    free(path);
    free(dir);
    return hash;
}

int isFile(char *name) {
  struct stat path;
  stat(name, &path);
  return S_ISREG(path.st_mode);
}

int isFolder(const char* file){
    struct stat path;
    stat(file,&path);
    return S_ISDIR(path.st_mode);
}

char *concat_paths(char *path1, char *path2) {
  char *result = malloc(strlen(path1) + strlen(path2) + 2);
  if (result == NULL) {
    printf("concat_paths : Erreur");
    return NULL;
  }
  result[0] = '\0';
  strcpy(result, path1);
  strcat(result, "/");
  strcat(result, path2);
  return result;
}

char *saveWorkTree(WorkTree *wt, char *path) {
  for (int i = 0; i < wt->n; i++) {
    char *absPath = concat_paths(path, wt->tab[i].name);
    if (isFile(absPath) == 1) {
      blobFile(absPath);
      free(wt->tab[i].hash);
      wt->tab[i].hash = sha256file(absPath);
      wt->tab[i].mode = getChmod(absPath);
    } else {
      WorkTree *newWT = initWorkTree();
      List *L = listdir(absPath);
      List l = *L;
      while (l != NULL) {
        if (l->data[0] == '.') {
          l = l->next;
          continue;
        }
        appendWorkTree(newWT, l->data, "", 0);
        l = l->next;
      }
      freeList(L);
      free(wt->tab[i].hash);
      wt->tab[i].hash = saveWorkTree(newWT, absPath);
      wt->tab[i].mode = getChmod(absPath);
      freeWorkTree(newWT);
    }
    free(absPath);
  }
  char* hash = blobWorkTree(wt);
  return hash;
}

int isWorkTree(char *hash) {
  char *path = hashToPath(hash);
  if (file_exists(path) != 0) {
    free(path);
    return 0;
  }
  char*path_wt = hashToPathWorkTree(hash);
  if (file_exists(path_wt) != 0) {
    free(path);
    free(path_wt);
    return 1;
  }
  free(path);
  free(path_wt);
  return -1;
}

void restoreWorkTree(WorkTree *wt, char *path) {
  for (int i = 0; i < wt->n; i++) {
    char *absPath = concat_paths(path, wt->tab[i].name);
    char *copyPath = hashToPath(wt->tab[i].hash);
    char *hash = wt->tab[i].hash;
    if (isWorkTree(hash) == 0) { // si c’est un fichier
      cp(absPath, copyPath);
      setMode(wt->tab[i].mode, absPath) ;
    }
    if (isWorkTree(hash) == 1) { // si c’est un repertoire
      strcat(copyPath, ".t");
      WorkTree *newWT = ftwt(copyPath);
      restoreWorkTree(newWT, absPath);
      setMode(wt->tab[i].mode, absPath) ;
      freeWorkTree(newWT);
    }
    free(absPath);
    free(copyPath);
  }
}