# TD 10

- [TD 10](#td-10)
  - [Exclusion mutuelle](#exclusion-mutuelle)
    - [Safety ?](#safety-)
    - [Liveness ?](#liveness-)
    - [Ordre des Opérations Lock ?](#ordre-des-opérations-lock-)


## Exclusion mutuelle

```c
int flag[2] = {0, 0};
int turn = 0;

void lock(int mon_id) {
    flag[mon_id] = 1;
    while (turn != mon_id) {
        while (flag[1 - mon_id] == 1) {}
        turn = mon_id;
    }
}

void unlock(int mon_id) {
    flag[mon_id] = 0;
}
```

### Safety ?

Pour que le `lock` s'effectue, je dois donc arriver à la fin de la fonction et passer 2 boucles `while`.
- *Première*: on la passe si c'est notre tour et pour que les deux locks se réalisent il faut que `turn == 0` et `turn == 1`. Ce qui n'est pas possible !
- *Second*: la condition `flag[1 - mon_id] == 1` check si l'autre thread veut s'exécuter et donc c'est son tour. Pour que `flag[1 - mon_id] == 0` il faut que unlock soit appelé !

--> Thread pas safe. Si le thread 1 appelle unlock donc `flag = {0,0}` et `turn = 1`. Le thread 0 se réveille et arrive jusqu'à arriver au second `while` car `1 != 0` et `0 == 1`. Donc on arrive à la ligne `turn = mon_id`. Le Thread 0 s'endort et on appelle le thread 1 et donc `flag = {1, 1}` mais `turn == 1` donc on peut déjà finir le lock. Le thread 0 se réveille et reprend son exécution à `turn = mon_id` donc `turn = 0`. Les deux threads ont fini ensemble leur `lock`.

### Liveness ?

Est-ce qu'un thread peut ne pas s'exécuter dans un temps fini ?

On a donc 2 boucles while.
- *Première*: la condition `turn != mon_id`. Techniquement on a le thread 0 ou 1 qui va s'exécuter car `turn` n'est jamais indéfini mdr.
- *Second*: on peut avoir `flag = {1,1}` ce qui est possible si on appelle `lock` maisss la première boucle va nous empêcher de boucler dessus.

Il y a 4 cas de figure:
1. $T_0$ et $T_1$ boucle sur le premier `while`. ❌
2. $T_0$ et $T_1$ boucle sur le second `while`. ❌
3. $T_0$ boucle sur le premier `while` et $T_1$ boucle sur le deuxième `while`. ❌
4. $T_0$ boucle sur le deuxième `while` et $T_1$ boucle sur le premier `while`. ❌

Donc la liveness est respectée.


### Ordre des Opérations Lock ?

Est-ce qu'on va libérer dans l'ordre des lock.