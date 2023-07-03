# Asteroids

This project was written for the OOP course at the University of Bucharest in 2022-2023 (first year, second semester).
The basic stuff (game loop, menus, ticking, rendering, collision detection, creating and destroying entities) is working (and I got the max grade for the project 😁).

I may come back to it later to make it an actual game (instead of just shapes flying around).

## Resources

### CMake and GitHub Actions

The code used for CMake and CI comes from our lab teacher's [template repository](https://github.com/mcmarius/oop-template), with some modifications, mostly to the CMake setup.

### Libraries

- [SFML](https://github.com/SFML/SFML/tree/aa82ea132b9296a31922772027ad5d14c1fa381b) (Zlib)
- [{fmt}](https://github.com/fmtlib/fmt/tree/a33701196adfad74917046096bf5a2aa0ab0bb50) (MIT with binary exception)
- [atomic_queue](https://github.com/max0x7ba/atomic_queue.git) (MIT)
- [cs_libguarded](https://github.com/copperspice/cs_libguarded.git) (BSD 2-clause)
- [inja](https://github.com/pantor/inja) + inja dependency [json](https://github.com/nlohmann/json) (dev. dependencies, MIT license, not included in game executable)
- [mapbox::eternal](https://github.com/mapbox/eternal) (ISC) - compile-time/constexpr map

### Assets

- [Public Pixel Font by GGBotNet](https://www.fontspace.com/public-pixel-font-f72305) (Public Domain CC0)
- [Piano loops 094](https://freesound.org/people/josefpres/sounds/683841/) (Public Domain CC0)

### Misc. Online Resources

- [Linking SFML into the executable](https://github.com/SFML/cmake-sfml-project#use-static-libraries)
- Embedding data into the executable:
  - [Generating the array literals using CMake](https://stackoverflow.com/a/27206982)
  - I've since written a proper program to do this as [this answer](https://stackoverflow.com/a/11814544) suggests
- [Game Programming Patterns](https://gameprogrammingpatterns.com/contents.html)
  - Most of these are too advanced for this project, but the Game Loop and Update Method chapters were helpful
- [Physics of JellyCar: Soft Body Physics Explained (_Collision Detection_ section)](https://youtu.be/3OmkehAJoyo?t=90)
  - Very useful info about collision detection, though this project doesn't have soft bodies
  - I wasted a lot of time here from a very stupid bug:
    I was starting the loop over edges from P2->P1, P3->P2, ..., but forgot I also needed P1->PN.
    If any ray got into the polygon through the missing edge it'd flip the result. Since this was letting rays into polygons
    without incrementing the intersection count, it completely broke the algorithm.
    Since SFML circles have 20 edges, it would sometimes line up right and work, making it seem like the
    square player had another bug. Wasted a lot of time with theories about floating point errors or the ray hitting a corner and incrementing the count twice.
- [Point in polygon Wikipedia article](https://en.wikipedia.org/wiki/Point_in_polygon)
  - Describes the ray casting algorithm
- [Shoelace formula](https://en.wikipedia.org/wiki/Shoelace_formula)
  - Used to calculate the area of a polygon

### Open Source Licenses

All dependencies are under their own licenses, mentioned above.
They are all permissive licenses (MIT, BSD, Zlib, ISC, Public Domain CC0),
so I can add them into the executable without any issues (besides OpenAL which is dynamically linked).

TODO: add a credits screen with the licenses to the game
