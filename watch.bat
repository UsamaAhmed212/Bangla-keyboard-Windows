:loop
windres rc/resources.rc -o obj/resources.o && windres rc/version.rc -O coff -o obj/version.res && gcc src/bangla_keyboard.c src/splashScreen.c src/keyboard.c src/systemTray.c src/trayActions/programAutoStartup.c src/trayActions/keyboardLayout.c obj/resources.o obj/version.res -o "Bangla Keyboard.exe" -lgdi32 -lole32 && "Bangla Keyboard.exe"
goto loop
