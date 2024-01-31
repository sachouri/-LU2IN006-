#include "commit.h"
#include "dir.h"
#include "liste.h"
#include "projet.h"
#include "work.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

kvp *createKeyVal(char *key, char *val) {
  kvp *kv = (kvp *)malloc(sizeof(kvp));
  kv->key = strdup(key);
  kv->value = strdup(val);
  return kv;
}

void freeKeyVal(kvp *kv) {
  free(kv->key);
  free(kv->value);
  free(kv);
}

void afficheKeyVal(kvp *kv) {
  if (kv != NULL) {
    printf("%s : %s\n", kv->key, kv->value);
  }
}

char *kvts(kvp *k) {
  if (k == NULL) {
    return "";
  }
  char *str = (char *)malloc(sizeof(char) * 100);
  sprintf(str, "%s : %s", k->key, k->value);
  return str;
}

kvp *stkv(char *str) {
  char key[100];
  char val[100];
  sscanf(str, "%s : %[^\n]", key, val);
  return createKeyVal(key, val);
}

Commit *initCommit() {
  Commit *commit = (Commit *)malloc(sizeof(Commit));
  commit->T = (kvp **)malloc(SIZE_C * sizeof(kvp *));
  commit->size = SIZE_C;
  commit->n = 0;
  for (int i = 0; i < commit->size; i++) {
    commit->T[i] = NULL;
  }
  return commit;
}

unsigned long hash(const char *str) {
  unsigned long hash = 5381;
  for (int i = 0; i < (int)strlen(str); i++) {
    hash = hash * 33 + str[i];
  }
  return hash;
}

void commitSet(Commit *c, char *key, char *value) {
  if (c == NULL) {
    c = initCommit();
  }
  int hashkey = hash(key) % (c->size);
  while (c->T[hashkey] != NULL) {
    hashkey = (hashkey) % (c->size) + 1;
  }
  c->T[hashkey] = createKeyVal(key, value);
  (c->n)++;
}

Commit *createCommit(char *hash) {
  Commit *c = initCommit();
  commitSet(c, "tree", hash);
  return c;
}

void afficheCommit(Commit *c) {
  if (c == NULL) {
    printf("le commit est vide\n");
    return;
  }
  printf("nb commit : %d\tsize : %d\n", c->n, c->size);
  for (int i = 0; i < c->size; i++) {
    if (c->T[i]) {
      afficheKeyVal(c->T[i]);
    }
  }
}

void freeCommit(Commit *c) {
  if (c == NULL) {
    return;
  }
  for (int i = 0; i < c->size; i++) {
    if (c->T[i]) {
      freeKeyVal(c->T[i]);
    }
  }
  free(c->T);
  free(c);
}

char *commitGet(Commit *c, char *key) {
  if (c == NULL) {
    return NULL;
  }
  int hashkey = hash(key) % (c->size);
  int i = 0;
  while (c->T[hashkey] != NULL && i < c->size) {
    if (strcmp(c->T[hashkey]->key, key) == 0) {
      return c->T[hashkey]->value;
    }
    hashkey = (hashkey) % (c->size) + 1;
    i++;
  }
  return NULL;
}

char *cts(Commit *c) {
  if (c == NULL) {
    return NULL;
  }
  char *str = malloc(sizeof(char) * 500 * c->n);
  for (int i = 0; i < c->size; i++) {
    if (c->T[i] != NULL) {
      char* s = kvts(c->T[i]);
      strcat(str,s);
      strcat(str, "\n");
      free(s);
    }
  }
  return str;
}

Commit *stc(char *s) {
  if (s == NULL) {
    return NULL;
  }
  Commit *c = initCommit();
  if (strcmp(s, "") == 0) {
    return c;
  }
  char data[200];
  int r = 0;
  int i = 0;
  while (s[r] != '\0') {
    if (s[r] == '\n') {
      data[i] = '\0';
      kvp *kv = stkv(data);
      commitSet(c, kv->key, kv->value);
      freeKeyVal(kv);
      i = 0;
    } else {
      data[i] = s[r];
      i++;
    }
    r++;
  }
  return c;
}

void ctf(Commit *c, char *file) {
  FILE *f = fopen(file, "w");
  if (f == NULL) {
    printf("ctf : Erreur d'ouverture du fichier\n");
  } else {
    char* s = cts(c);
    fputs(s, f);
    fclose(f);
    free(s);
  }
}

Commit *ftc(char *file) {
  FILE *f = fopen(file, "r");
  if (f == NULL) {
    printf("ftc : Erreur d'ouverture du fichier\n");
    return NULL;
  }
  char s[10000] = "";
  char buffer[100];
  while (fgets(buffer, 100, f) != NULL) {
    strcat(s, buffer);
  }
  Commit *c = stc(s);
  fclose(f);
  return c;
}

WorkTree *ctwt(Commit *c) {
  char hash_path_wt[100] = "";
  strcpy(hash_path_wt, commitGet(c, "tree"));
  strcat(hash_path_wt, ".t");
  char* path_wt = hashToPath(hash_path_wt);
  WorkTree* wt = ftwt(path_wt);  
  free(path_wt);
  return wt;
}

char* hashToPathCommit(char* hash){
    char* buff = malloc(sizeof(char)*100);
    char* path = hashToPath(hash);
    sprintf(buff, "%s.c", path); 
    free(path);
    return buff;
}

char *blobCommit(Commit *c) {
  static char template[] = "/tmp/myfileXXXXXX";
  char fname[100];
  strcpy(fname, template);
  int fd = mkstemp(fname);

  ctf(c, fname);

  char *hash = sha256file(fname);
  char *path = hashToPathCommit(hash);
  char *dir = strdup(path);
  dir[2] = '\0';

  /* si le repertoire n'existe pas on le cree */
  if (file_exists(dir) == 0) {
    char command[500];
    sprintf(command, "mkdir %c%c", path[0], path[1]);
    system(command);
  }
  
  cp(path, fname);
  close(fd);
  free(path);
  free(dir);
  return hash;
}