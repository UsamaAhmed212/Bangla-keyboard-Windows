# **Bangla Keyboard Windows**

## **Overview**

This guide explains how to compile a C program into an executable (`.exe`) with a custom icon.

## **Prerequisites**

Ensure you have the following files in your project directory:

- **`bangla_keyboard.c`**: Your main C source file.
- **`bangla_keyboard.ico`**: Your custom icon file.
- **`resource.h`**: Resource header file.
- **`resources.rc`**: Resource script file.

## **Compilation Steps 01**

### 1. Compile the Resource File

Run the following command:

```bash
windres rc/resources.rc -o obj/resources.o
```
## Expected Result:

- **`resources.o`**: that contains machine code.

### 2. Generated Executable File (.exe)
```bash
gcc src/bangla_keyboard.c src/splashScreen.c src/systemTray.c obj/resources.o -o "Bangla Keyboard.exe" -mwindows
```
OR
```bash
gcc src/bangla_keyboard.c src/splashScreen.c src/systemTray.c obj/resources.o -o "Bangla Keyboard.exe" -lgdi32 -lcomctl32 -lshell32
```
OR
```bash
gcc src/bangla_keyboard.c src/splashScreen.c src/systemTray.c obj/resources.o -o "Bangla Keyboard.exe" -mwindows -lgdi32 -lcomctl32 -lshell32
```
#### Behavior of Flags
> **`-mwindows:`** Tells GCC to create a Windows GUI application. This means that the application will not have a console window attached to it when run.

> **`-lcomctl32::`** Links the Common Controls library, which provides support for common Windows UI elements (like toolbars and status bars).

> **`-lshell32::`** Links the Shell library, which provides access to Windows shell functions.

> **`-lgdi32:`** Links the GDI (Graphics Device Interface) library, which is used for graphical output.

## Expected Result:

- **`Bangla Keyboard.exe`**: Executable File (.exe)

### 3. Run Executable File (.exe)

```bash
"Bangla Keyboard.exe"
```
OR
```bash
./"Bangla Keyboard.exe"
```

## **Compilation Steps 02**

### 1. Compile the Resource File
Run the following command : => (if a Makefile is available)

```bash
make run
```
