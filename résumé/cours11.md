[<--](../README.md)

___



# Gestion des utilisateurs

Unix est pensé pour supporter plusieurs utilisateurs. On doit d'abord s'authentifier

## Authentification

On se connecte via un mot de passe en local ou distant (*ssh*). Il existe aussi les authentifications par données biométriques, cryptographie asymétrique (private key and public key).

Le premier processus au démarrage est le processus de `login` qui va lui fork tous les autres sous-processus. On liste tous les users dans `/etc/passwd`. On y stockait avant les mots de passe dedans mais maintenant on les stocke dans des fichiers séparés (*shadow password*).

### `/etc/passwd`

`oracle:x:1021:1020:Oracle user:/data/network/oracle:/bin/bash`

`1:2:3:4:5:6:7`

1. Nom d'utilisateur
2. Mot de passe (stocké dans `/etc/shadow`)
3. Identifiant User (**UID**)
4. Identifiant du groupe *principal* de l'user (**GID**)
5. Informations supplémentaires, nom complet de l'user
6. Répertoire *home*
7. Shell à exécuter au moment du login de l'user


### Types d'user

| type d'user  |                                             Description                                             |
| :----------: | :-------------------------------------------------------------------------------------------------: |
|     root     |                      Aucune restriction et peut tout faire. Admin du système.                       |
| User normal  |                    Droit limité mais peut obtenir des droits de root via `sudo`.                    |
| User système | Ne correspond pas un réel user mais correspond à des services du SE. (typiquement des daemons, ...) |

#### Obtenir leur **UID**

On utilise la fonction `getuid(2)` pour obtenir l'**UID** et `geteuid(2)` pour obtenir l'*effective **UID*** car il se peut que les droits changent temporairement (`sudo`).

#### Changer l'**UID**

On peut faire cela via `setuid(2)` qui modifie l'UID de l'utilisateur en cours d'exécution. Donc Root appelle `setuid` avant `execve` pour se donner les droits d'administration.

# Systèmes de fichiers

Il y a énormément de diversité en terme de support de stockage de données. Il faut donc une interface *commune*. En **UNIX**, les fichiers sont regroupés sous forme d'une arborescence unique.

## Hiérarchie et Montage

![Alt text](image-34.png)

En utilisant `mnt` on peut attacher ou détacher des systèmes de fichiers dans la hiérarchie commune.

## contenu d'un répertoire

```
$ ls -la / 
total 1488
drwxr-xr-x  19 root root    4096 Dec  9 14:35 .
drwxr-xr-x  19 root root    4096 Dec  9 14:35 ..
lrwxrwxrwx   1 root root       7 Apr 23  2020 bin -> usr/bin
drwxr-xr-x   2 root root    4096 Apr 23  2020 boot
drwxr-xr-x   9 root root    2820 Dec  9 14:35 dev
drwxr-xr-x 131 root root   12288 Dec  9 14:35 etc
drwxr-xr-x   3 root root    4096 Nov 15  2021 home
-rwxr-xr-x   3 root root 1440152 May  7  2022 init
lrwxrwxrwx   1 root root       7 Apr 23  2020 lib -> usr/lib
lrwxrwxrwx   1 root root       9 Apr 23  2020 lib32 -> usr/lib32
lrwxrwxrwx   1 root root       9 Apr 23  2020 lib64 -> usr/lib64
lrwxrwxrwx   1 root root      10 Apr 23  2020 libx32 -> usr/libx32
drwx------   2 root root   16384 Apr 10  2019 lost+found
drwxr-xr-x   2 root root    4096 Apr 23  2020 media
drwxr-xr-x   6 root root    4096 Sep 19 23:43 mnt
drwxr-xr-x   2 root root    4096 Apr 23  2020 opt
dr-xr-xr-x 244 root root       0 Dec  9 14:35 proc
drwx------   2 root root    4096 Feb 21  2023 root
drwxr-xr-x   6 root root     120 Dec  9 14:35 run
lrwxrwxrwx   1 root root       8 Apr 23  2020 sbin -> usr/sbin
drwxr-xr-x   2 root root    4096 Apr 10  2020 snap
drwxr-xr-x   2 root root    4096 Apr 23  2020 srv
dr-xr-xr-x  11 root root       0 Dec  9 14:35 sys
drwxrwxrwt  13 root root    4096 Dec  9 14:35 tmp
drwxr-xr-x  14 root root    4096 Apr 23  2020 usr
drwxr-xr-x  13 root root    4096 Apr 23  2020 var
```

### Permissions

En analysant la première colonne, on peut savoir quelles sont les permissions et si c'est un lien symbolique, hardlink ou directory.

On encode les permissions sur 9 bits via User (rwx), Groupe (rwx), Autres (rwx). Ainsi, on connait les droits si on a le même UID, GUID ou UID et GUID différents.

| Droit |                                                         Description                                                          |
| :---: | :--------------------------------------------------------------------------------------------------------------------------: |
|  `r`  |                                   *read*: lire le fichier, lister le contenu du répertoire                                   |
|  `w`  |                             *write*: écrire dans le fichier, créer une entrée dans le répertoire                             |
|  `x`  | *execute*: accepter pour faire `execve` pour un fichier. Pour un répertoire, on peut accéder à un fichier ou sous-répertoire |


#### Subtilité du `x`

```
$ mkdir -p repertoire 
$ echo "LINFO1252" > repertoire/fichier 
$ chmod 000 repertoire/ 
$ ls -al repertoire/
ls: cannot open directory repertoire/: Permission denied 
$ cat repertoire/fichier
cat: repertoire/fichier: Permission denied 
$ chmod +x repertoire/ 
$ ls -al repertoire/
ls: cannot open directory repertoire/: Permission denied
$ cat repertoire/fichier
LINFO1252
```

`chmod` va changer les droits. On ne peut plus lister le fichier car on a pas `r` mais on peut quand même y accéder car on a `x` pour aller dans d'autres sous fichiers.

On représente les permissions comme 3 séquences de 3 bits donc `rwxr-xr--` = `754`. En utilisant `S_ISUID` pour un exécutable, permet l’exécution avec les permissions du propriétaire de l’exécutable et pas celles de l’utilisateur.

### Navigation

Pour changer de répertoire en C où on exécute, on peut réaliser l'appel suivant:

```c
#include <unistd.h> 
int chdir(const char *path);
```

### Fonctions Utiles

|              Fonction               |                                   description                                   |
| :---------------------------------: | :-----------------------------------------------------------------------------: |
|              ``stat``               |         récupère les méta-données associées à un fichier ou répertoire          |
|         ``chmod``/``chown``         |               modifier les permissions ou le propriétaire/groupe                |
|              ``utime``              | modifier les dates de création/modifications d’un fichier (e.g. commande ``touch``) |
|             ``rename``              |                          changer de nom, d’emplacement                          |
|         ``mkdir``/``rmdir``         |                          créer/détruire un répertoire                           |
| ``opendir``/``closedir``/``readir`` |                      consulter le contenu des répertoires                       |

### Parcours de répertoire

```c
struct dirent { 
    ino_t d_ino;                /* inode number */
    off_t d_off;                /* offset to the next dirent */ 
    unsigned short d_reclen;    /* length of this record */
    unsigned char d_type;       /* type of file; not supported by all file system types */
    char d_name[256];           /* filename */
};
```

Il faut d'abord avoir ouvert un répertoire via `opendir`. `readdir` pour accéder aux entrées du système.

TODO le reste


___

[<--](../README.md)
