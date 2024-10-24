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
gcc src/bangla_keyboard.c src/splashScreen.c obj/resources.o -o "Bangla Keyboard.exe" -mwindows
```
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
