![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)
![Status](https://img.shields.io/badge/status-beta-orange)

# UniEngine Project
### A game engine with only one header file

## License
This project licensed under [GPLv3](LICENSE) license.

## Beta
This version is beta 1.0

## Creating Project
I will add a script creates project and configures settings
But now, i can add these lines of script

```bash
git clone https://github.com/omerpa55/UniEngine
cd UniEngine
meson setup build
meson compile -C build
ln -s build/compile_commands.json compile_commands.json
```

### Now you can edit file uniengine.cpp

## Thanks
Meson Build System

LLVM (clang)

## Dependencies
- Meson
- Clang / GCC
- And a GPU supports OpenGL 3.3 Core

### All other dependencies (GLFW, GLM, etc.) are fetched automatically via WrapDB
