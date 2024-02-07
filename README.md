# wordle
Copy of web-based word guessing game written in C/C++

Two versions of interconnection:
- UDP (User Datagram Protocol) version
- IPC (Inter Process Communication) version

## UDP version
<img src="res/wordle_udp_version.png">

## IPC version
<img src="res/wordle_ipc_version.png">

## Installation
First clone this repository:
```console
git clone https://github.com/alkuzin/wordle.git
```
Before building the project you need to select interconnection mode *(default: UDP)*
by editing __`client_main.cpp`__ and __`server_main.cpp`__ files:

For UDP version:
```cpp
enum transfer_mode mode = UDP;
```

For IPC version:
```cpp
enum transfer_mode mode = IPC;
```

Then build project by running:
```console
make all
```

And run those commands in separate terminals:
```console
clear && ./server
```
```console
clear && ./client
```

In order to return to original state run:
```console
make fclean
```
