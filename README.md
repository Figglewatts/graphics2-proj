# Graphics 2 Project
A university project to make a 3D game that has a spaceship which takes off from one planet and lands on another.

The game has a number of interesting features, but most notably a collision system that uses Gilbert-Johnson-Keerthi and the Expanding Polytope Algorithm to detect and resolve collisions between 2 arbitrary convex hulls, as well as any shape that provides a support function (such as a sphere for the planets). Code for this can be found in `src/framework/physics/GJK.cpp`.
