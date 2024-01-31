#include "liste.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

List *initList() {
  List *l = (List *)malloc(sizeof(List));
  (*l) = NULL;
  return l;
}

int listSize(List *L) {
  List l = *L;
  int cpt = 0;
  while (l != NULL) {
    l = l->next;
    cpt++;
  }
  return cpt;
}

Cell *buildCell(char *ch) {
  Cell *cellule = (Cell *)malloc(sizeof(Cell));
  cellule->data = strdup(ch);
  cellule->next = NULL;
  return cellule;
}

void freeCell(Cell *c) {
  free(c->data);
  free(c);
}

void insertFirst(List *L, Cell *C){
  if (C != NULL) {
    C->next = (*L);
    (*L) = C;
  }
}

void insertLast(List *L, Cell *C) {
  List l = (*L);
  if (*L == NULL) {
    *L = C;
    return;
  }
  while (l->next) {
    l = l->next;
  }
  C->next = l->next;
  l->next = C;
}

void afficheList(List *L) {
  if (L==NULL){
    printf("liste nulle\n");
    return ;
  }
  List l = (*L);
  while (l != NULL) {
    printf("%s \t", l->data);
    l = l->next;
  }
  printf("\n");
}

void freeList(List *L) {
  if (L==NULL){
    return ;
  }
  List l = (*L);
  List tmp = NULL;
  while (l != NULL) {
    tmp = l->next;
    free(l->data);
    free(l);
    l = tmp;
  }
  free(L);
}

char *ctos(Cell *c) { 
  return c->data; 
}

char *ltos(List *L) {
  if (L == NULL) {
    return NULL;
  }
  List l = *L;
  char *chaine = (char *)malloc(sizeof(char) * 100);
  strcpy(chaine, "");
  while (l != NULL) {
    char *c = ctos(l);
    chaine = strcat(chaine, c);
    if (l->next != NULL) {
      chaine = strcat(chaine, "|");
    }
    l = l->next;
  }
  return chaine;
}

List *stol(char *s) {
  if (s == NULL) {
    return NULL;
  }
  List *L = initList();
  if(strcmp(s,"") == 0){
    return L;
  }
  char *data = (char *)malloc(256 * sizeof(char));
  int r = 0;
  int i = 0;
  while (s[r] != '\0') {
    if (s[r] == '|') {
      data[i] = '\0';
      insertLast(L, buildCell(data));
      free(data);
      data = (char *)malloc(256 * sizeof(char));
      i = 0;
    } else {
      data[i] = s[r];
      i++;
    }
    r++;
  }
  data[i] = '\0';
  insertLast(L, buildCell(data));
  free(data);
  return L;
}

Cell *listGet(List *L, int i) {
  List l = *L;
  for (int j = 0; j < i; j++) {
    if (l == NULL) {
      return NULL;
    }
    l = l->next;
  }
  return l;
}

Cell *searchList(List *L, char *str) {
  List l = *L;
  while (l != NULL) {
    if (strcmp(l->data, str) == 0) {
      return l;
    }
    l = l->next;
  }
  return NULL;
}

void ltof(List *L, char *path) {
  char *chaine = ltos(L);
  FILE *f = fopen(path, "w");
  if (f == NULL) {
    printf("ltof : Erreur ouverture fichier\n");
    return;
  }
  fprintf(f, "%s", chaine);
  fclose(f);
  free(chaine);
}

List *ftol(char *path) {
  FILE *f = fopen(path, "r");
  if (f == NULL) {
    printf("ftol : Erreur ouverture fichier\n");
    return NULL;
  }
  char buffer[100];
  fgets(buffer, 100, f);
  fclose(f);
  return stol(buffer);
}