#ifndef ref_H
#define ref_H

/* cree le repertoire cache .refs (s’il n’existe pas deja), puis cree les fichiers master et HEAD (vides) */
void initRefs();

/* met a jour une reference en remplacant son contenu par hash */
void createUpdateRef(char* ref_name, char* hash);

/* supprime une reference */
void deleteRef(char* ref_name);

/* recupere vers quoi pointe une reference */
char* getRef(char* ref_name);

/* ajoute un fichier ou un repertoire dans le WorkTree correspondant a la zone de preparation */
void myGitAdd(char* file_or_folder);

/*  cree un point de sauvegarde */
void myGitCommit(char* branch_name, char* message);


#endif