// bangla_keyboard.c
#include <windows.h>
#include <process.h>  // For _beginthreadex to create a new thread
#include "../include/splashScreen.h"    // Include splash screen header
#include "../include/keyboard.h"        // Include keyboard header
#include "../include/systemTray.h"      // Include the system tray header
#include "../include/trayActions/keyboardLayout.h"  // Include keyboard Layout header
#include "../include/resource.h"  // Include custom resources
#include "../include/app_data.h"  // Include shared app data

AppData appData; // Define the global instance

// Thread function to run the splash screen
unsigned __stdcall splashScreenThread(void* params) {
    HINSTANCE hInstance = ((HINSTANCE*)params)[0];
    int nShowCmd = ((int*)params)[1];

    showSplashScreen(hInstance, nShowCmd);  // Call the splash screen function
    return 0;
}

// Thread function to initialize the system tray
unsigned __stdcall systemTrayThread(void* params) {
    HINSTANCE hInstance = ((HINSTANCE*)params)[0];
    
    systemTrayInit(hInstance);  // Call the system tray initialization function
    return 0;
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    appData.hInstance = hInstance;
    appData.nShowCmd = nShowCmd;

    // Create a named mutex to check if another instance is running
    HANDLE hMutex = CreateMutex(NULL, TRUE, "BanglaKeyboardMutex");

    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        // If the mutex already exists, it means the program is already running
        HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_WARNING_SING));
    
        // Create a message box with custom parameters
        MSGBOXPARAMS mbp = {0};
        mbp.cbSize = sizeof(MSGBOXPARAMS);
        mbp.hwndOwner = NULL;
        mbp.hInstance = GetModuleHandle(NULL);
        mbp.lpszText = "Bangla Keyboard is already running on this system and running more than one instance is not allowed.";
        mbp.lpszCaption = "Bangla Keyboard";
        mbp.dwStyle = MB_OK | MB_USERICON;
        mbp.lpszIcon = MAKEINTRESOURCE(IDI_WARNING_SING);  // Use your icon resource ID here

        MessageBoxIndirect(&mbp);
        
        DestroyIcon(hIcon);
        return 0; // Exit the program
    }
    
    
    // Create an array to pass parameters to the threads
    void* params[2] = { hInstance, (void*)(intptr_t)nShowCmd };

    // Start a new thread to show the splash screen
    unsigned threadID1;
    HANDLE hThread1 = (HANDLE)_beginthreadex(NULL, 0, splashScreenThread, params, 0, &threadID1);

    // Start a new thread to initialize the system tray
    unsigned threadID2;
    HANDLE hThread2 = (HANDLE)_beginthreadex(NULL, 0, systemTrayThread, params, 0, &threadID2);

    // Call the Bangla Keyboard function (runs in the main thread)
    initializeBangleKeyboard(hInstance);
    

    // Wait for the splash screen thread to finish
    WaitForSingleObject(hThread1, INFINITE); // Wait indefinitely until the splash screen thread finishes

    // Wait for the system tray thread to finish
    WaitForSingleObject(hThread2, INFINITE); // Wait indefinitely until the system tray thread finishes

    // Close the thread handles
    CloseHandle(hThread1); // Clean up the splash screen thread handle
    CloseHandle(hThread2); // Clean up the system tray thread handle
    
    CloseHandle(hMutex); // Clean up and close the mutex when done

    return 0;  // Return to indicate the application exited successfully
}
