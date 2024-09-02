# SDL Tic-Tac-Toe

> This project is part of the [🎮 SDL One-Night-Stand collection](https://github.com/stars/guareschiluca/lists/sdl-one-night-stand) *(unhappy name, I know, but that's my sense of humor)*.

This repository contains a **bare-bones** implementation of a [Tic-Tac-Toe](https://en.wikipedia.org/wiki/Tic-tac-toe) game, based on the *SDL2 framework*.

I started this one-night project *(read the [motivation](#motivation) if you're interested in the conditions 🤣)* with the goal to implement a game without images or fonts. You know when you take a leap into the past and start to make text-based game that run on the command line? Well, this was the mood.

For this reason the game itself is really simplistic and, as said before, bare-bones.

## Table of Contents

- [Motivation](#motivation)
- [Installation](#installation)
- [Usage](#usage)
- [Features](#features)
- [Contributing](#contributing)
- [License](#license)
- [Libraries](#libraries)

## Motivation

> Irrelevant information, skip this paragraph. I mean it.

As all the project in this [collection](https://github.com/stars/guareschiluca/lists/sdl2-one-night-stand), this has been developed on my Android tablet, with a rickety IDE, as an evening filler while watching TV from my bed. That's to say none of these projects ever claimed to be serious.

After finishing a couple of these projects, I started wondering what would mean porting them to a web version.
War Plan:

- Move the sources to my **PC**
- Make a decent **refactoring** *(finally I had a keyboard, I couldn't stand that ugly code any further)*
- Widely **comment**, so the dumb future me will understand what the past me did
- Added **Emscripten** support

That's how this collection started, so look at this as something born to be trivial enough not to distract me too much from the TV.

## Installation

You can clone the repository using you favourite git client or via cli:

```bash
git clone https://github.com/guareschiluca/sdl-tictactoe.git
cd sdl-tictactoe
```

## Usage
This repository includes a Visual Studio solution file fully configured and ready to run, if you want to compile it for PC.

### PC Build

To build for PC just open the solution in Visual Studio and build! `Debug` and `Release` configuration differ by initialization and console output. `Release` configuration will build a windows application while `Debug` configuration will show up the console.

In general, the `Debug` configuration has a few tricks to ease debugging, while `Release` configuration is made to look good.

### Web Build

If you want to build the web version you will need a fully configured Emscripten environment [(download)](https://emscripten.org/docs/getting_started/downloads.html), CMake [(download)](https://cmake.org/download/) and Ninja [(download)](https://ninja-build.org/).

With a fully configured environment, and all ports built, all you need to build for web is to run the [build batch file](build-wasm.bat).
```batch
REM Run a web assembly build
build-wasm.bat

REM Run a clean web assembly build (clears cache before building)
build-wasm.bat -c
REM or
build-wasm.bat --clean

REM Run a web assembly build and open output directory (can be combined with -c or --clean)
build-wasm.bat -o
REM or
build-wasm.bat --open
```

#### Web Build Output

Once the build succeeded, a directory called `public-html` should contain the built files plus:

- a [testing html page](public-html/index.html)
- a [testing server batch file](public-html/run_testing_server.bat) *(requires python)*.

> If you run the testing server, you can test the build at [http://localhost:8000/](http://localhost:8000/) *(unless you edit the configuration)*.

## Features
The game is implemented based on:

- 3x3 Game Field
- Two Players
- AI with 3 Different Difficulties *(drafted, actually)*

The repository also contains:

- Web Assembly Building Script
- Sample Web Page to Test
- Python-based Testing Server

## Contributing
This project is for educational purposes and does not accept collaborators. However, feel free to fork the repository and make your own modifications.

## License
Read the license file for more information: [LICENSE.md](LICENSE.md)

## Libraries
This repository is based on the following libraries:

- SDL2 - Simple DirectMedia Layer