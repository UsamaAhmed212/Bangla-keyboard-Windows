# **(Bangla Keyboard Windows) Compilation with Custom Icon**

## **Overview**

This guide explains how to compile a C program into an executable (`.exe`) with a custom icon.

## **Prerequisites**

Ensure you have the following files in your project directory:

- **`keyboard.c`**: Your main C source file.
- **`keyboard.ico`**: Your custom icon file.
- **`resource.h`**: Resource header file.
- **`resources.rc`**: Resource script file.

## **Compilation Steps**

### 1. Compile the Resource File

Run the following command:

```bash
windres resources.rc -o resource.o
```
## Expected Result:

- **`resource.o`**: that contains machine code.

### 2. Generated Executable File (.exe)
```bash
gcc keyboard.c resource.o -o keyboard.exe
```
## Expected Result:

- **`keyboard.exe`**: Executable File (.exe)

### 3. Run Executable File (.exe)

```bash
keyboard.exe
```
OR
```bash
./keyboard.exe
```
