Bangla Keyboard/
├── src/
│   ├── bangla_keyboard.c                    # Main C program combining all functionalities
│   ├── keyboard.c                           # Keyboard handling code
│   ├── splashScreen.c                       # Splash screen handling code
│   ├── systemTray.c                         # System tray handling code
│
├── include/                                 # Header files
│   ├── keyboard.h                           # Header for keyboard-related code
│   ├── splashScreen.h                       # Header for splash screen code
│   ├── systemTray.h                         # Header for system tray-related code
│   ├── resource.h                           # Header for resource definitions (icons, bitmaps)
│
├── rc/                                      # Resource scripts
│   ├── resources.rc                         # Main resource script file for icons, bitmaps, etc.
│
├── resources/                               # Resource directory for icons and bitmaps
│   ├── icons/                               # Icons and other images
│   │   ├── bangla_keyboard.ico              # Icon file for the application
│   │   ├── keyboard.ico                     # Additional icon file
│   ├── images/                              # Other image formats (if needed)
│   ├── bitmaps/                             # Bitmap (.bmp) files
│   │   ├── exit.bmp                         # BMP file for the exit button
│   │   ├── avro_keyboard_splash_screen.bmp  # BMP file for the splash screen
│
├── obj/                                     # Compiled object files from C sources and resources
│   ├── resources.o                          # Object file compiled from resources.rc
│
├── build/                                   # Directory for final build outputs
│   ├── debug/                               # Debug build outputs
│   │   ├── Bangla_Keyboard.exe              # Debug version of the compiled Windows executable
│   ├── release/                             # Release build outputs
│
├── docs/                                    # Project documentation
│   ├── README.md                            # Project overview and setup instructions
│
└── Makefile                                 # Optional Makefile to automate the build process
