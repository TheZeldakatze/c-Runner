# c-Runner
a c portation of my game Runner


## Building
Linux
`gcc -lSDL -o runner-linux.amd64 *.c`
Windows
`i686-w64-mingw32-gcc *.c -lmingw32 -lSDLmain -lSDL -o runner-windows.exe`

## Controls
* Left / Right Arrows -   Move the character
* F                   -    Toggle Fullscreen (Only supported with X11)
* Space               - Press to charge jump
* ESCape              -   Closes the program