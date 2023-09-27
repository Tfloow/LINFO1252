# Note TP

- [Note TP](#note-tp)
  - [S1](#s1)
    - [Bash](#bash)
  - [C](#c)


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


## C

Bien faire la différence entre un `"a"` et `'a'`. Le premier est un pointeur vers un `char` et le second est un `char`.