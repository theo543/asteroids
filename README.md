# Asteroids

### Tema 0

- [x] Nume proiect (poate fi schimbat ulterior)
- [x] Scurtă descriere a temei alese, ce v-ați propus să implementați: jocul Asteroids

## Tema 2

#### Cerințe
- [x] separarea codului din clase în `.h` (sau `.hpp`) și `.cpp`
- [x] moșteniri
  - [x] clasă cu atribut de tip pointer la o clasă de bază cu derivate
  - [x] funcții virtuale (pure) apelate prin pointeri de bază din clasa de mai sus, constructori virtuali (clone)
    - minim o funcție virtuală va fi **specifică temei** (e.g. nu simple citiri/afișări)
  - [x] apelarea constructorului din clasa de bază din constructori din derivate
  - [x] smart pointers
  - [x] `dynamic_cast`
- [ ] suprascris cc/op= pentru copieri/atribuiri corecte, copy and swap
- [x] excepții
  - [x] ierarhie proprie cu baza `std::exception` sau derivată din `std::exception`; minim 2 clase pentru erori specifice
  - [ ] utilizare cu sens: de exemplu, `throw` în constructor, `try`/`catch` în `main` **TODO:display mesaj eroare pe ecran**
- [x] funcții și atribute `static`
- [x] STL
- [x] cât mai multe `const`
- [ ] la sfârșit: commit separat cu adăugarea unei noi derivate fără a modifica restul codului
- [x] tag de `git`: de exemplu `v0.2`

## Tema 3

#### Cerințe
- [x] 2 șabloane de proiectare (design patterns) - Factory în SwitchFactory, Non-Virtual Interface în WorldBase, Singleton în GlobalLoaders
- [x] o funcție șablon (template) cu sens; minim 2 instanțieri - SwitchFactory::push
- [x] o clasă șablon cu sens; minim 2 instanțieri - OnDemandLoader
<!-- - [ ] o specializare pe funcție/clasă șablon -->
- [x] tag de `git`: de exemplu `v0.3` sau `v1.0`

## Resurse

Dependencies (and their dependencies) are under their respective licenses.

### Librarii

- [SFML](https://github.com/SFML/SFML/tree/aa82ea132b9296a31922772027ad5d14c1fa381b) (Zlib)
- [{fmt}](https://github.com/fmtlib/fmt/tree/a33701196adfad74917046096bf5a2aa0ab0bb50) (MIT with binary exception)
- [atomic_queue](https://github.com/max0x7ba/atomic_queue.git)
- [cs_libguarded](https://github.com/copperspice/cs_libguarded.git)

### Grafică

- [Public Pixel Font by GGBotNet](https://www.fontspace.com/public-pixel-font-f72305) (Public Domain CC0)

### Audio

- [Piano loops 094](https://freesound.org/people/josefpres/sounds/683841/) (Public Domain CC0)

### Informații

- https://github.com/SFML/cmake-sfml-project
  - Explained how to static link SFML using BUILD_SHARED_LIBS=OFF
- https://stackoverflow.com/a/27206982/15885837
  - Explained how to embed resources in executable using CMake
- [Game Programming Patterns](https://gameprogrammingpatterns.com/contents.html)
  - Most of these are too advanced for this project, but the Game Loop and Update Method chapters were helpful
