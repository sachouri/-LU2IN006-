#ifndef liste_H
#define liste_H

typedef struct cell {
char* data;
struct cell* next;
} Cell;

typedef Cell* List;

/* initialise une liste vide */
List* initList();

/* renvoie la taille de la liste */
int listSize(List *L);

/* alloue et de retourne une cellule de la liste */
Cell* buildCell(char* ch);

/* libere une cellule */
void freeCell(Cell *c);

/* affiche une liste */
void afficheList(List *L);

/* libère la liste */
void freeList(List *L);

/* ajoute un element en tete d’une liste */
void insertFirst(List* L, Cell* C);

/* ajoute un element en fin d'une liste */
void insertLast(List *L, Cell *C);

/* retourne la chaine de caracteres qu’une cellule represente */
char* ctos(Cell* c);

/* transforme une liste en une chaine de caracteres */
char* ltos(List* L);

/* renvoie le i eme element d’une liste */
Cell* listGet(List* L, int i);

/* recherche un element dans une liste a partir de son contenu */
Cell* searchList(List* L, char* str);

/* transforme une chaine de caracteres representant une liste en une liste chainee */
List* stol(char* s);

/* transforme une chaine de caracteres representant une liste en une liste chainee */
void ltof(List* L, char* path);

/* lit une liste enregistree dans un fichier */
List* ftol(char* path);

#endif