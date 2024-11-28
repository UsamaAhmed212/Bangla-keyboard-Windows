# **Bangla Keyboard Windows**

## **Overview**

This guide explains how to compile a C program into an executable (`.exe`).

## **Compilation Steps 01**

### 1. Compile the Resource File

Run the following command:

```bash
windres rc/resources.rc -o obj/resources.o && windres rc/version.rc -O coff -o obj/version.res
```

### 2. Generated Executable File (.exe)
```bash
gcc src/bangla_keyboard.c src/splashScreen.c src/keyboard.c src/systemTray.c src/trayActions/programAutoStartup.c src/trayActions/keyboardLayout.c obj/resources.o obj/version.res -o "Bangla Keyboard.exe" -lgdi32 -lole32 -mwindows
```
OR
```bash
gcc src/bangla_keyboard.c src/splashScreen.c src/keyboard.c src/systemTray.c src/trayActions/programAutoStartup.c src/trayActions/keyboardLayout.c obj/resources.o obj/version.res -o "Bangla Keyboard.exe" -lgdi32 -lole32 -lgdi32 -lcomctl32 -lshell32
```
OR
```bash
gcc src/bangla_keyboard.c src/splashScreen.c src/keyboard.c src/systemTray.c src/trayActions/programAutoStartup.c src/trayActions/keyboardLayout.c obj/resources.o obj/version.res -o "Bangla Keyboard.exe" -lgdi32 -lole32 -mwindows -lgdi32 -lcomctl32 -lshell32
```

#### Behavior of Flags

> **`-mwindows:`** Tells GCC to create a Windows GUI application. This means that the application will not have a console window attached to it when run.

> **`-lgdi32:`** Links the program with the gdi32.lib library.gdi32 is the Windows Graphics Device Interface (GDI) library, which provides functions for handling graphics, drawing text, handling fonts, and managing devices like printers and monitors.

> **`-lole32:`** Links the program with the ole32.lib library.ole32 is the Object Linking and Embedding (OLE) library, which provides support for COM (Component Object Model) and OLE automation.

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
make build
```
OR
```bash
make run
```

## **Compilation Steps 03**

### Run the following command : => (if you want Monitors changes and automatically recompiles and runs your application when changes are detected.)
```bash
watch.bat
```
OR
```bash
./watch.bat
```
