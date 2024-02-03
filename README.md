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
Then move to *udp_version* or *ipc_version* directory and run:
For UDP version:
```console
cd udp_version && make all
```

For IPC version:
```console
cd ipc_version && make all
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