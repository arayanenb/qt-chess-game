# Qt Chess Game

A desktop chess game built with C++, Qt, and CMake.

## Features

- Interactive 2D graphical chessboard (`ChessboardWidget`)
- Standard chess logic and board state management (`Plateau`)
- Clean Qt Widgets UI integration (`MainWindow`)

## Prerequisites

- **C++ Compiler**: GCC, Clang, or MSVC (C++17 or newer)
- **CMake**: Version 3.16 or higher
- **Qt**: Qt 5 or Qt 6 (Widgets module)

## Build & Run

### Using Qt Creator
1. Open Qt Creator.
2. Select **File > Open File or Project...** and choose `CMakeLists.txt`.
3. Configure the project with your installed Qt kit.
4. Press `Ctrl + R` (or `Cmd + R` on macOS) to build and run.

### Using the Command Line
```bash
# Generate build files
cmake -B build -S .

# Build the project
cmake --build build

# Run the executable (Linux/macOS)
./build/chess_game

# Or on Windows
.\build\Debug\chess_game.exe