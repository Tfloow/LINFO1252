# Project 0 - Implementing malloc and free

- [Project 0 - Implementing malloc and free](#project-0---implementing-malloc-and-free)
  - [Key factors](#key-factors)
    - [Free](#free)
    - [Malloc](#malloc)
  - [How to Build it](#how-to-build-it)


In this project we are tasked with building an equivalent to `malloc` and `free`. We will mimic the heap using an array of 64000 bytes (64 kbytes). This work is heavily inspired by the [lesson](https://moodle.uclouvain.be/pluginfile.php/237345/mod_resource/content/5/03-gestion_memoire.pdf) about malloc and free.

## Key factors

For the Heap we have some crucial things to look for:
1. Fragmentation
2. The way to point data area
3. How much useful and meta data

### Free

Important elements of a good free function are listed here:
- Make sure to not have an issue when doing double free
- Properly empty and avoid fragmentation
- Join empty spaces together

### Malloc
Important elements of a good Malloc function are listed here:
- Find the best place in the Heap:
  - Have a sense of locality
  - Not creating too much fragmentation
  - Using the right space
- Properly allocate and change meta data

## How to Build it

We will implement those functions like a double linked list. We are making each empty space in the heap like a possible payload for data. This way we use more meta data (especially if they are a lot of small spot). Like this we can use a simple algorithm implementing a first fit and next fit. By combining the both of them we can ensure a quick and powerful read while maintaining a good access to data and good locality.