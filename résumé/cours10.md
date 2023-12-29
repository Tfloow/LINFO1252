

# Cours 10

## La mémoire

Accéder à la mémoire directement n'est pas une bonne solution car on doit connaitre son organisation à la compilation. Impossible car si on passe de 2 Go de RAM à 4 il faut tout recompiler !! On va virtualiser tout ça

### La Mémoire Virtuelle

On va devoir faire une traduction du virtuelle à physique via le **MMU** ou **Memory Management Unit** (qui est dans le processeur).

![MMU Simple](image-52.png)

Ainsi on peut découpler la taille et écrire les adresses sur + de 32 bits.

On peut faire de la librairie partagée très facilement, on peut voir que 2 programmes pointes vers la même zone de la RAM !

#### Avantages

On peut utiliser le stockage sur le disque pour mettre des objets inutilisées de la RAM. C'est le principe de **`swap`**. Et on peut faire vice-versa.

### Fonctionnement de la mémoire Virtuelle

La mémoire a un accès par *adresse* et les SSD par *secteur*. La mémoire virtuelle est divisée par **pages**. C'est une zone de mémoire **contiguë** de taille 4 Ko (4096 octets (on peut vérifier via `getpagesize()`)).

On a toujours un nombre entier de pages. De plus, chaque segment (les 6) occupe leurs propres pages.

Ainsi, les pages virtuelles peuvent être placées dans n'importe quelle zone (*frame*/cadre de page) de la mémoire physique.

Adresse Virtuelle est composée de:

- Numéro de la page
- Offset sur cette page à faire

Le MMU se charge de la traduction.

![Découpage en page](image-53.png)

### Mise en Oeuvre de la Traduction

Il doit avoir accès à l'allocation actuelle entre pages virtuelles et cadres de pages physique. On va utiliser une table des pages:

- Tableau indexé par le numéro de page
- Bit de validité si la page existe dans l'espace mémoire du *processus*
- Si valide: ligne du tableau indique le lien vers le numéro de cadre de page.

![Traduction](image-54.png)
