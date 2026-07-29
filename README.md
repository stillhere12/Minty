# Minty

Expression evaluator in C++ and Rust. Shunting-yard algorithm, no deps.

- `Cint/` — C++, works. Parses `-5`, `-(1+2)`, `1 - -2`. Inline tests in `main()`.
- `Mint/` — Rust, WIP. Currently just `Hello, World!`.

```sh
cd Cint && g++ main.cpp -o main && ./main
cd Mint && cargo run
```
