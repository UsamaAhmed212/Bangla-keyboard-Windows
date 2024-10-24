// bangla_keyboard.c
#include <windows.h>
#include <process.h>  // For _beginthreadex to create a new thread
#include "../include/systemTray.h"      // Include the system tray header
#include "../include/splashScreen.h"    // Include splash screen header

// Thread function to run the splash screen
unsigned __stdcall splashScreenThread(void* params) {
    HINSTANCE hInstance = ((HINSTANCE*)params)[0];
    int nShowCmd = ((int*)params)[1];

    showSplashScreen(hInstance, nShowCmd);  // Call the splash screen function
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    // Create an array to pass parameters to the thread
    void* params[2] = { hInstance, (void*)(intptr_t)nShowCmd };

    // Start a new thread to show the splash screen
    unsigned threadID;
    HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, splashScreenThread, params, 0, &threadID);

    // Call the System TrayInit function (runs in the main thread)
    systemTrayInit(hInstance);

    // Wait for the splash screen thread to finish
    WaitForSingleObject(hThread, INFINITE); // Wait indefinitely until the thread finishes

    // Close the thread handle
    CloseHandle(hThread); // Clean up the handle

    return 0;  // Return to indicate the application exited successfully
}
