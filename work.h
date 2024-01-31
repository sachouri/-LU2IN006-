#ifndef work_H
#define work_H

#define SIZE 20

typedef struct {
  char *name; /* nom du fichier */
  char *hash; /* hash associé à son contenu */
  int mode;   /* autorisations associés au fichier */
} WorkFile;

typedef struct {
  WorkFile *tab;
  int size; /* taille du taleau */
  int n;    /*  nombre d'élément présent dans le tableau */
} WorkTree;

/* recupere les autorisations */
int getChmod(const char *path);

/* modifie les autorisations */
void setMode(int mode, char *path);

/* crée un WorkFile et l’initialise */
WorkFile *createWorkFile(char *name);

/* libère le WorkFile */
void freeWorkFile(WorkFile *wf);

/* affiche un WorkFile */
void afficheWorkFile(WorkFile* wf);

/* converti un WorkFile en chaine de caractères */
char *wfts(WorkFile *wf);

/* convertir une chaine de caractères en WorkFile */
WorkFile *stwf(char *ch);

/* alloue un WorkTree l’initialise */
WorkTree *initWorkTree();

/* libère le WorkTree */
void freeWorkTree(WorkTree *wt);

/* affiche un WorkTree */
void afficheWorkTree(WorkTree* wt);

/* vérifie la présence d’un fichier ou répertoire dans un WorkTree */
int inWorkTree(WorkTree *wt, char *name);

/* ajoute un fichier ou r épertoire au WorkTree */
int appendWorkTree(WorkTree *wt, char *name, char *hash, int mode);

/* convertit un WorkTree en une chaine de caractères */
char *wtts(WorkTree *wt);

/* convertit une chaine de caractères en un WorkTree */
WorkTree *stwt(char *s);

/* écrit le WorkTree dans le fichier file */
int wttf(WorkTree *wt, char *file);

/* construit un WorkTree à partir d’un fichier */
WorkTree *ftwt(char *file);

/* renvoie le path du Worktree avec son hash */
char* hashToPathWorkTree(char* hash);

/* créer l’enregistrement instantanée du WorkTree */
char *blobWorkTree(WorkTree *wt);

/* retourne 1 si c'est un fichier */
int isFile(char *name);

/* indique si c'est un repertoire */
int isFolder(const char* file);

/* concatène les deux path */
char *concat_paths(char *path1, char *path2);

/* fait un enregistrement instantané du WorkTree */
char *saveWorkTree(WorkTree *wt, char *path);

/* renvoie 1 si le hash correspond à un WorkTree 0 sinon */
int isWorkTree(char *hash);

/* restaure le WorkTree */
void restoreWorkTree(WorkTree *wt, char *path);

#endif