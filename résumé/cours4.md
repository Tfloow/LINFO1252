# Cours 4

- [Cours 4](#cours-4)
  - [Rappel](#rappel)
    - [Neumann](#neumann)
  - [Mémoire](#mémoire)
    - [Instructions et Registres](#instructions-et-registres)
    - [Architecture](#architecture)
    - [Technologies mémoires](#technologies-mémoires)
    - [Cache](#cache)
    - [Localité](#localité)
    - [Hiérarchie](#hiérarchie)
    - [Fonctionnement d'un cache](#fonctionnement-dun-cache)
  - [Jeu d'instructions IA 32](#jeu-dinstructions-ia-32)
    - [Registres](#registres)
    - [Types de données](#types-de-données)
    - [Instructions](#instructions)
    - [Modes d'adressage](#modes-dadressage)


## Rappel

### Neumann

On se souvient de la structure de Neumann où on va stocker en mémoire **les instructions et les données**.

Ceci est composé d'un CPU qui a lui même:
- **Unité arithmétique et logique**: Opérateurs matériel pour les opérations *arithmétiques* (`+,-,...`) et *logiques* (`&,|,^,...`).
- **Unité de commande**: Met en oeuvre le cycle *fetch/decode/execute* des instructions depuis la mémoire.


## Mémoire

On l'organise en octet d'adresses consécutives. $2^k$ octets donne le nombre d'adresse mémoire possible sur $k$ bits. Avec $k=32$ on a une taille de mémoire de maximum 4GO contre 16 Exa-octets pour $k=64$.

### Instructions et Registres

On code les instructions en binaires. Les instructions sont de tailles fixes ou variables. 

Un processeur possède un **nombre limité de zone mémoire** appelés **registres**. C'est directement accessible par les instructions et en tandem avec la mémoire principale.

### Architecture
![Alt text](image-7.png)

### Technologies mémoires

On a la **SRAM** (statique) et la **DRAM** (dynamique). La SRAM est plus couteuse mais plus rapide. On va les utiliser en tandem pour ne pas être ralenti par notre mémoire.

En effet, les cycles de CPU sont devenus extrêment cours. Encore plus cours que ceux de la mémoire. C'est-à-dire on perdrait ce gain de vitesse si on utilisait pas de la DRAM car on devrait attendre pour plusieurs cycles avant d'avoir l'information.

Ces deux technologies sont dites **volatiles**. Quand plus de jus c'est mort.

#### SRAM

![Alt text](image-8.png)

On a donc de multiples bascules (6 transistor par bit (ou 4 + des resistances)). On lit la valeur en fonction du passage ou non du courant.

#### DRAM

On stocke l'information dans des condensateurs

![Alt text](image-9.png)

La lecture des bits se fait sous forme de matrice donc si on veut lire une valeur, on lit en réalité toute la ligne. Donc le temps pour lire 1 valeur et 64 est la même.

![Alt text](image-10.png)

On doit donc combiner un peu de SRAM coûteuse avec de la DRAM peu chère. On utilise la SRAM comme cache.

### Cache

Le cache interagit directement avec le Cache. On y conserve les données récemment accédées.

### Localité

#### Temporelle

On peut accéder à la même donnée se trouvant au même endroit, on va souvent repasser dessus.

#### Localité

On va passer sur toutes les adresses car nos données sont juxtaposé.

### Hiérarchie

![Alt text](image-11.png)


### Fonctionnement d'un cache

#### Lecture

On va lire l'octet à une adresse spécifique. Si cette adresse se trouve déjà en cache on va lire l'adresse depuis le cache.

Sinon, la mémoire cache va récupérer une copie de la mémoire à cette adresse. On a donc toute la granularité donc 64 octets ! On sauvegarde le tout dans le cache.

#### Écriture

Si on veut écrire `A`, on va regarder si on a l'adresse de destination en cache. On écrit directement dans le cache ou bien on doit récupérer depuis la mémoire.

1. **Write through**: écriture immédiate.
2. **Write back**: on écrit au moment où la ligne de cache est retiré du cache.

#### Réalité

On a dans un processeur 3 niveaux de caches : `L1, L2, L3`

## Jeu d'instructions IA 32

Ce jeu d'instructions est 32 bits ! (donc max 4 Go de mémoire).

### Registres

On a 8 registres génériques de **32 bits**:
- `EAX`
- `EBX`
- `ECX`
- `EDX`
- `ESI`
- `EDI`
- `EBP`: gère la pile
- `ESP`: gère la pile

On a 1 registres qui stocke le compteur de programme:
- `EIP`

On a également des registres pour traiter les floats et double.

### Types de données

|    Type    | Taille (octets) | Suffixe assembleur |
| :--------: | :-------------: | :----------------: |
|   `char`   |        1        |         b          |
|  `short`   |        2        |         w          |
|   `int`    |        4        |         l          |
| `long int` |        4        |         l          |
|  `void *`  |        4        |         l          |

### Instructions

| Fonctions | Variantes |                         Explications                         |
| :-------: | :-------: | :----------------------------------------------------------: |
|   `mov`   |  `movb`   | `movb src, dest`: déplace de `src` vers `dest` pour taille 1 |
|           |  `movw`   | `movb src, dest`: déplace de `src` vers `dest` pour taille 2 |
|           |  `movl`   | `movb src, dest`: déplace de `src` vers `dest` pour taille 4 |


### Modes d'adressage

Pour spécifier un registre, on utilise `%eax` pour spécifier le registre `EAX`. Pour écrire une valeur dans un registre, on fait `$123` pour écrire `123`.

```assembly
movl $123, %eax ; écris dans le registre eax 
```
