# Note TP

- [Note TP](#note-tp)
  - [S1](#s1)
    - [Bash](#bash)
  - [S2](#s2)
  - [S3](#s3)
    - [Static](#static)


## S1

### Bash

Un petit résumé utile pour bash.

| commande |     description     |
| :------: | :-----------------: |
|   `$#`   | nombre d'arguments  |
|   `$@`   | liste des arguments |
|   `$1`   | le premier argument |


#### Couleur

On doit faire la commande `echo -e` et pour changer la couleur on doit à chaque fois faire `\e[XXm` où `XX` est un nombre de ce tableau:

|     Color     | Foreground Code | Background Code |
| :-----------: | :-------------: | :-------------: |
|     Black     |       30        |       40        |
|      Red      |       31        |       41        |
|     Green     |       32        |       42        |
|    Yellow     |       33        |       43        |
|     Blue      |       34        |       44        |
|    Magenta    |       35        |       45        |
|     Cyan      |       36        |       46        |
|  Light Gray   |       37        |       47        |
|     Gray      |       90        |       100       |
|   Light Red   |       91        |       101       |
|  Light Green  |       92        |       102       |
| Light Yellow  |       93        |       103       |
|  Light Blue   |       94        |       104       |
| Light Magenta |       95        |       105       |
|  Light Cyan   |       96        |       106       |
|     White     |       97        |       107       |


| Code  |   Description   |
| :---: | :-------------: |
|   0   |  Reset/Normal   |
|   1   |    Bold text    |
|   2   |   Faint text    |
|   3   |     Italics     |
|   4   | Underlined text |


## S2

Bien faire la différence entre un `"a"` et `'a'`. Le premier est un pointeur vers un `char` et le second est un `char`.

## S3

On va passer en revue les différents endroits de la mémoire:
1. **Segment Text**: correspond à l'endroit où est stocké le programme. (eg: une fonction, programme)
2. **Données initialisées**: on stocke les variables **globales initialisées**  (donc en dehors de toute fonction).
3. **Données non-initialisées**: on stocke les variables **globales non-initialisées**  (donc en dehors de toute fonction). 
4. **Heap**: on enregistre le contenu de malloc ici. C'est une taille extensible.
5. **Stack**: on enregistre les valeurs au cours de l'exécution du programme. (eg: nouvelle variable pointant vers un malloc ou une fonction)
6. **Arg, envp**: les arguments d'environnements.

### Static

On utilise ``static`` en C pour rendre une variable accessible sans passer par la fonction où elle est définie. Donc toute variable ``static`` se trouve dans les **données initialisées** ou **non-initialisées**. **Attention**, ce n'est pas parce qu'une variable *non-statique* dépend de variable statique que cette dernière sera également sur ce segment de la mémoire (elle sera sur le stack).