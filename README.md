# TALL-E: **T**erminal **A**bstraction **L**ayer **L**ibrary
The 'E' is just a subtle nod to WALL-E.

## Getting Started
Clone the repository with:
```shell
git clone https://github.com/ScriptExec/talle.git
```

### Build Requirements
CMake 3.24+

> [!WARNING]
> 32-bit platforms are **not supported**

#### Install required packages
Ubuntu
```shell
sudo apt install build-essential cmake ninja-build gdb
```

## Building
To build the library use CMake via an IDE or manually, with:
```shell
cmake --preset=<PRESET> && cmake --build --preset=<PRESET>
```
Replace `<PRESET>` with one of:
- `x64-<SYSTEM>-debug`
- `x64-<SYSTEM>-release`

and replace `<SYSTEM>` with a value from:
- windows
- linux
- macos
