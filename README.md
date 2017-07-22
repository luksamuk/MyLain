# MyLain
## The very useful, distributed personal assistant for smart homes

As the title says, MyLain is a small experiment for a smart assistant, mostly written in C.
It is supposed to run, primarily, on Unix consoles for both Desktop and
Smartphone devices (preferably Linux and Termux).

## How it works
The plans are to make it operate so that MyLain monitores your network, and detects whenever you join a known network. Once that happens, MyLain will try to integrate with the other relays on the network, in a peer-to-peer way.

Once MyLain is connected to other devices, it may remotely use their public modules (if you have set their permissions right), and possibly use distributed processing to remotely solve tasks.

Those are the actual plans, though. It might be quite audacious, and there are security concerns about that, but I'm currently more concentrated on having a working prototype before asking myself how to improve it. So I strongly advise you to not use this commercially -- and if you do, or if you fork it, please respect the GNU LGPL v3 license.

## Usability
MyLain is actually a small REPL, which is supposed to be extended by configuring submodules that could be loaded dynamically, like daemons. I'm still not concerned about how this will be done, but it's possible that it'll involve some sort of FFI.

The actual REPL is comprised of so little things right now, so just go ahead and type `help` after starting it to see the quick cheatsheet. `help [command]` will also give you more information on said command.

## Building
MyLain is supposed to run fine on Linux and Termux.

Once you clone this repository, make sure you have `libreadline` installed. Otherwise, you may also build it without `libreadline` support, which will be shown later.

MyLain is completely built by using CMake, so just follow these commands on Linux:

```bash
cd /path/to/mylain/folder
mkdir build && cd build
cmake ..
make
```

If you're building it on Termux, things might be a little tricky, since you have to manually setup CMake so it uses Clang instead of GCC (also, make sure you have `clang`, `cmake` and `make` installed!).

On Termux, follow these commands instead:

```bash
cd /path/to/mylain/folder
mkdir build && cd build
cmake .. -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
make
```

Another way to do that is to `export` the variables `CC=clang` and `CXX=clang++` to your environment, if you're using `bash` on Termux.

Also, remember to use MyLain's command `config set interface` to change the network interface to Termux's WiFi interface, which might be `wlan0`. This might be useful advice for setting up connections.

### Building Without Readline support
If you find yourself needing to strip out the `libreadline` support, you can just add another flag to the commands to activate the NO_READLINE mode:

```bash
cmake .. -DLAIN_NO_READLINE=on
```

You may also add this flag to the Termux CMake command.

Keep in mind that this will remove the REPL's editing capabilities, so you'll have no saved history nor will be able to move through the typed characters using the cursor or the arrows.

### Some build and design concerns
Please keep in mind that I'm trying to follow the Unix philosophy while building this. All code is written using POSIX model and WILL NOT work on default Windows targets (hopefully it MAY work under Cygwin or under the Linux subsystem for Windows). I did not rule out Windows support, but since the primary objective is to run it on my own computer and potentially on home servers, this might not be something I'll be looking into, in the near future.

## License
This project is distributed under the GNU LGPLv3 license. See `LICENSE` or the header of source files for details.

Copyright (c) 2017 Lucas Vieira.
