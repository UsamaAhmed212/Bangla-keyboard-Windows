#include <windows.h>
#include <shlobj.h>     // For SHGetFolderPath
#include <stdbool.h>    // Include the header for using boolean types (true/false)
#include <stdio.h>

void saveCheckboxState() {
    HKEY hKey;
    const char* keyPath = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
    const char* appName = "BanglaKeyboard";
    const char* appPath = "C:\\Program Files (x86)\\Bangla Keyboard\\Bangla Keyboard.exe";

    // Inform the user before adding to startup
    printf("Adding Bangla Keyboard to startup...\n");

    if (RegOpenKeyEx(HKEY_CURRENT_USER, keyPath, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        if (RegSetValueEx(hKey, appName, 0, REG_SZ, (const BYTE*)appPath, strlen(appPath) + 1) == ERROR_SUCCESS) {
            printf("Startup entry added successfully.\n");
        } else {
            printf("Failed to add startup entry.\n");
        }
        RegCloseKey(hKey);
    } else {
        printf("Failed to open registry key.\n");
    }
}

// Function to handle program auto-startup behavior based on the checkbox state.
void programAutoStartup(bool isChecked) {

    saveCheckboxState(isChecked);   // Save the updated state to the registry
}

