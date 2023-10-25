# Note et solutions

- [Note et solutions](#note-et-solutions)
  - [Printf et appel système](#printf-et-appel-système)
  - [Malloc](#malloc)
  - [Files](#files)
  - [Appel système en assembleur](#appel-système-en-assembleur)

## Printf et appel système

```c
for(int i = 0; i < 10; i++){
    printf("On est à %d", i);
}

for(int i = 0; i < 10; i++){
    printf("On est à %d\n", i);
}
```

Dans la première boucle, le printf va mettre dans un **buffer** la chaine de caractère jusqu'à une nouvelle ligne où là il fait l'appel système à `write` qui est très couteux car arrête le programme.

## Malloc

Voir le code [ici](code/2-malloc.c).

On peut voir qu'on incrémente de $135168$ bytes ou en hexadecimal de `0x21000`. On voit qu'on fait 80 appels à `brk`.

## Files

Le `fseek` permet de déplacer le curseur d'écriture.

En Linux, tout est considéré comme un fichier.

## Appel système en assembleur

Pour faire un tel appel on utilise `int $0x80`

```
  ;--- Début du code à modifier
  movl $78, %eax
  movl %esp, %ebx
  movl $0, %ecx
  int $0x80
  ;--- Fin du code à modifier
```

C'est la première ligne qui réalise l'appel système.
