# Elemental Game and Custom-built Game Engine

Welcome to Elemental Game, a game and custom-built game engine designed for creating simple, top-down strategy games. This README provides an overview of the project, its design principles, goals, and instructions for building and testing.

## Getting Started
### Build Tools 
- CMake (version 3.21 or higher)
- C++20-compatible compiler

### Project Structure
- src/: Contains the engine logic, defines a libray target
- apps/: Contains the game source, as well as source for resource editors, all  consumers of the engine target
- test/: Contains unit tests

#### Library Dependencies

- [Catch2](https://github.com/catchorg/Catch2) v3: Test framework for C++ (can be fetched using FetchContent).
- [FakeIt](https://github.com/eranpeer/FakeIt): Mocking framework for C++ (fetched using FetchContent).
- [SDL2](https://www.libsdl.org/): A cross-platform development library providing low-level access to audio, keyboard, mouse, and display functions.
- [SDL2_Image](https://www.libsdl.org/projects/SDL_image/): An extension library for SDL2, offering support for loading various image file formats.
- [SDL2_Gfx](https://sourceforge.net/projects/sdl2gfx/): A graphics primitive extension library for SDL2, providing additional functionality for efficient graphics rendering.

### Building the Project
1. Clone the repository:
    ```
    git clone <URL>
    ```

2. Navigate to the project directory:
    ```
    cd  elemental-game
    ```
3. Configure your build system:
    ```bash
    cmake -B debug -G Unix Makefiles
    # or
    cmake -B debug -G Ninja   # this is faster and more modern
    ```
4. Invoke your build system
    ```
    cmake --build debug
    ```

### Running Unit Tests

1. After building the project, you can run the unit tests:

    ``` 
    cmake --build --target ctest
    ```
This will execute the Catch2 test suite.

## License
This project is licensed under the MPL 2.0 License.

Catch2, which is used for unit-testing is © Catch2 authors, and is licensed under the Boost Software License v1.0
FakeIt, is also used for unit-testing and is © Eran Pe'er and is licensed under the MIT License
