#ifndef commit_H
#define commit_H
#include "work.h"

#define SIZE_C 50

typedef struct key_value_pair {
  char *key;
  char *value;
} kvp;

typedef struct hash_table {
  kvp **T;
  int n;
  int size;
} HashTable;

typedef HashTable Commit;


/* alloue et initialise un  element */
kvp* createKeyVal(char* key, char* val);

/* libere un element */
void freeKeyVal(kvp* kv); 

/* affiche un element */
void afficheKeyVal(kvp* kv);

/* convertit un element en une chaine de caracteres de la forme ”cle : valeur” */
char* kvts(kvp* k);

/* converit une chaine "cle :valeur" en un element */
kvp* stkv(char* str);

/* alloue et initialiser un Commit de taille fixe */
Commit* initCommit();

/* affiche un commit */
void afficheCommit(Commit *c);

/* fonction de hashage */
unsigned long hash(const char *str);

/* insere la paire (key, value) dans la table, en gerant les collisions par adressage ouvert et probing lineaire */
void commitSet(Commit* c, char* key, char* value);

/* alloue et initialise un commit */
Commit* createCommit(char* hash); 

/* recherche l'element de cle key dans un commit */
char* commitGet(Commit* c, char* key);

/* libere un commit */
void freeCommit(Commit* c);

/* convertit un commit en une chaine de caracteres */
char* cts(Commit* c); 

/* convertit une chaine de caractere en commit */
Commit* stc(char* ch);

/* ecrit dans le fichier file la chaine de caracteres representant le commit c */
void ctf(Commit* c, char* file);

/* permet de charger un Commit depuis un fichier */
Commit* ftc(char* file );

/* renvoie le WorkTree associé au commit */
WorkTree *ctwt(Commit *c);

/* renvoie le path du commit avec son hash */
char *hashToPathCommit(char *hash);

/* realise un enregistrement instantané d’un Commit */ 
char* blobCommit(Commit* c);

#endif