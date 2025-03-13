# Space Invaders

Space Invaders is a modern take on the classic arcade game, built using C++ and the SFML library. Relive the nostalgic gameplay of the original while enjoying smooth, responsive graphics and sound!

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Controls](#controls)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgements](#acknowledgements)

## Overview

This project is a reimagined version of the iconic Space Invaders game. Developed in C++ with SFML, it brings together classic arcade excitement and modern programming techniques. The goal is to defend Earth from waves of alien invaders and achieve the highest score possible.

## Features

- **Classic Gameplay:** Experience retro-style shooting and enemy waves.
- **Smooth Animation:** Powered by SFML for crisp graphics and real-time input.
- **Customizable Settings:** Easily adjust game settings and controls to fit your preferences.
- **Score Tracking:** Keep track of your scores as you progress through increasing levels of difficulty.

## Installation

### Prerequisites

- A C++ compiler supporting C++11 (or later).
- [SFML](https://www.sfml-dev.org/) (Simple and Fast Multimedia Library).

### Building from Source

1. **Clone the repository:**
    ```bash
    git clone https://github.com/yourusername/space-invaders.git
    cd space-invaders
    ```

2. **Compile the project:**

   You can compile the project using your preferred method. For example, using `g++`:
    ```bash
    g++ -std=c++11 -I/path/to/SFML/include -L/path/to/SFML/lib -o space_invaders src/*.cpp -lsfml-graphics -lsfml-window -lsfml-system
    ```

   Or, if you prefer using CMake:
    ```bash
    mkdir build && cd build
    cmake ..
    make
    ```

3. **Run the game:**
    ```bash
    ./space_invaders
    ```

## Usage

Once compiled, run the game binary to start playing. Use the controls below to navigate your spaceship, fire at invading aliens, and try to beat your high score.

## Controls

- **Arrow Keys:** Move your spaceship left and right.
- **Spacebar:** Fire your weapon.
- **Escape:** Pause or exit the game.

> _Note: If needed, you can customize these controls by modifying the source code or a configuration file (if provided)._

## Contributing

Contributions are welcome! If you want to contribute to the project, please follow these steps:

1. Fork the repository.
2. Create a feature branch:
    ```bash
    git checkout -b feature/YourFeature
    ```
3. Commit your changes:
    ```bash
    git commit -m 'Add some feature'
    ```
4. Push to your branch:
    ```bash
    git push origin feature/YourFeature
    ```
5. Open a Pull Request describing your changes.

Please ensure that your code adheres to the project's coding style and includes any relevant tests.

## License

This project is licensed under the [MIT License](LICENSE).

## Acknowledgements

- A big thank you to the [SFML](https://www.sfml-dev.org/) team for providing an excellent multimedia library.
- Inspired by the original [Space Invaders](https://en.wikipedia.org/wiki/Space_Invaders) arcade game.

