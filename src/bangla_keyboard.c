// bangla_keyboard.c
#include <windows.h>
#include "../include/splashScreen.h"    // Include splash screen header
// #include "../include/systemTray.h"      // Include the system tray header

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    // Call the splash screen function
    showSplashScreen(hInstance, nShowCmd);

    // Initialize the system tray after the splash screen
    // systemTrayInit();  // Assuming you refactor systemTray.c to expose this function

    return 0;  // Return to indicate the application exited successfully
}
