# Introduction
Another 2D engine, but I stop using procedural programming because "too much keystroke has to use" and I suddenly found Entity-Component-System stuff to be...cool and want to implement it
# Disclaimer
This Entity-Component-System stuff that I'm trying to do is NOT implemented with data-oriented design (DOD) in mind. Rather, I will implement for one single goal: make it easy to create new type of entities
# How to build
## Setup
- Install `cmake` version 3.5 or above
## Build step
```bash
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_INSTALL_PREFIX=$(pwd)/../output -DSDL_SHARED=OFF -DSDL_STATIC=ON -DBUILD_SHARED_LIBS=OFF -Dvendored_default=ON
$ make install
```
The installed game locate at `<root>/output`
