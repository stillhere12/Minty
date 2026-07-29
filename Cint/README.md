# Cint

A tiny C expression parser that builds an AST from infix notation.

Turns `A + B * C ^ D - E` into a tree you can actually read:

```
└── -
    ├── +
    │   ├── A
    │   └── *
    │       ├── B
    │       └── ^
    │           ├── C
    │           └── D
    └── E
```

## Build

```sh
g++ main.cpp -o main && ./main
gcc -Wall -Wextra -o main main.c && ./main
```
