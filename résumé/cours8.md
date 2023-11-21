[↰](../README.md)

___

# Cours 8

- [Cours 8](#cours-8)
  - [Rappel](#rappel)
  - [Problème d'exclusion fondamentale](#problème-dexclusion-fondamentale)
    - [Résultat Théorique](#résultat-théorique)
    - [Essais et Erreurs](#essais-et-erreurs)
  - [Algorithme de Peterson](#algorithme-de-peterson)
  - [Algorithme du filtre](#algorithme-du-filtre)

## Rappel

On se souvient que l'accès à une variable partagée doit être protégée. soit via:
- Mutex: `lock()` et `unlock()`
- Sémaphore: `wait()` et `post()`

## Problème d'exclusion fondamentale

### Résultat Théorique

il est impossible de réaliser un algorithme d’exclusion mutuelle pour N threads avec moins de N valeurs partagées si on utilise uniquement des opérations de lecture et d’écriture.

### Essais et Erreurs

#### Les bases

Si on a 2 threads qui doivent accéder à une section critique, il faut garantir:
- La *Safety*: ils ne vont pas y accéder de manière conjointe
- La *Liveness*: ils doivent y accéder en un temps fini.

#### Premier Essai

On peut utiliser une variable `turn` qui indique l'envie d'un thread d'arriver en zone critique.

```c
// thread 1 
while (turn!=0) { 
    /* loop */ 
} 
section_critique();
turn=1;

// thread 2 
while (turn!=1) {
    /* loop */ 
} 
section_critique();
turn=0;
```

On respecte bien l'exclusion mutuelle puisque pour que les deux threads s'exécutent simultanément, il faudrait que `turn = 1` et en même temps `turn = 0`.

Mais pour la **liveness**, si le thread 1 a fini de s'exécuter et qu'il veut ré-exécuter il ne pourra pas le faire tant que le premier thread ne s'exécute ce qui est problématique.

- **Safety**: ✅
- **Liveness**: ❌


#### Second Essai

Ici, l'approche consiste à utiliser un tableau de *flag* qui indique si le thread veut s'exécuter. Donc une valeur de `1` à l'emplacement 0 indique que le thread 1 veut s'exécuter.

```c
// Thread A 
while (flag[B]==true) {
    /* loop */ 
}
flag[A]=true;
section_critique();
flag[A]=false;

// Thread B 
while (flag[A]==true) { 
    /* loop */
} 
flag[B]=true;
section_critique();
flag[B]=false;
```

La **Liveness** est bien respecté car le thread A est seulement bloqué si le thread B est dans session critique. 

Les threads peuvent simultanément lire `flag[A]` et `flag[B]` a `false` et sortir de leur zone critique. 

- **Safety**: ❌
- **Liveness**: ✅

#### Corriger le problème

On peut déplacer l'assignation comme suit:

```c
flag[A]=true; 
while (flag[B]==true) { 
    /* loop */ 
} 
section_critique();
flag[A]=false;
```

On perd la propriété de *Liveness* même si on retrouve celle de *safety*. Ils peuvent mutuellement se bloquer s'ils veulent tous les deux s'exécuter.

- **Safety**: ✅
- **Liveness**: ❌

## Algorithme de Peterson

```c
#define A 0 
#define B 1 

int turn = A;
int flag[]; 
flag[A]=false;
flag[B]=false;

// thread A 
flag[A]=true; 
turn=B; 
while((flag[B]==true)&&(turn==B)) { 
    /* loop */ 
} 
section_critique();
flag[A]=false;

// Thread B 
flag[B]=true; 
turn=A; 
while((flag[A]==true)&&(turn==A)) {
    /* loop */
}
section_critique();
flag[B]=false;
```

L'idée ici est d'avoir un système qui montre quel thread a l'intention de s'exécuter via `flag[]` et une variable qui montre c'est à qui le tour `turn`.

**Safety**: Pour que les deux threads soient en section critique, il faut que ``flag[] = {true, true}``. Ce qui peut être probable en même temps car lu à différent moment. Il faut aussi que `turn = A` et `turn = B`, ce qui n'est évidemment pas possible. (pour  `flag[]` ce n'est techniquement pas possible en vrai).

**Liveness**: Si jamais le thread A veut s'exécuter et pas le thread B, on aura des variables de valeur `flag[] = {true, false}` et `turn = A`. Mais on aura pas de *livelock* car `turn = A` ou `turn = B`.

## Algorithme du filtre



___

[↰](../README.md)
