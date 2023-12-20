# TD

- [TD](#td)
  - [1](#1)
  - [2](#2)
  - [3](#3)
    - [a.](#a)
    - [b.](#b)
    - [c.](#c)
    - [d.](#d)
  - [4](#4)

## 1

--> $2^{32} \approx 4 Go$. Le MMU se charge de faire la conversion entre adresse virtuelle et adresse physique.

On sait que chaque page fait `4 Ko`.

## 2

On a $1000000$ de pages car on a des pages de `4 Ko` avec une ram de `4 Go`.

20 bits pour stocker le numéro de page. on a 4 Ko par page donc notre offset est de 12 bits.

## 3

### a.

Il faut 1 bit de validité, 3 bits pour savoir les permissions, 2 bits pour savoir si les données ont été modifiés ou référencés. Il nous reste donc 20 bits pour les offsets des pages.

On compte par 8 bits. On va donc couper au milieu de la mémoire. Donc les 6 derniers sont des bits de padding qui servent à rien pour éviter le découpage des octets.

### b.

$2^{20}$ pages

### c.

$2^{2} \cdot 2^{20} = 2^{22} = 4194304 o \approx 4 Mo$

### d.

16 Mo de RAM --> $\frac{8 Mo}{16 Mo} = \frac{1}{2}$ Donc la moitié est utilisé pour les tables de page.

## 4

On se partage le tableau des pages car on que 12 Mo de mémoire etc on peut s'assurer que les adresses soient bien différentes.