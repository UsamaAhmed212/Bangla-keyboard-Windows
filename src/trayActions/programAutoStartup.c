#include <windows.h>
#include <shlobj.h>      // For SHGetFolderPath
#include <stdio.h>       // For printf, wprintf


// Global variables for executable path, folder, name, and shortcut name

char exePath[MAX_PATH];
char exeFolderPath[MAX_PATH];

char exeFolderName[MAX_PATH];
char exeName[MAX_PATH];

char startupFolderPath[MAX_PATH];

char shortcutPath[MAX_PATH];
char shortcutName[MAX_PATH];


// Function to initialize global variable Executable Info
void initializeExecutableDetails() {
    // Retrieve the path of the currently running executable
    if (GetModuleFileName(NULL, exeFolderPath, MAX_PATH) == 0) {
        printf("Failed to get the executable path.\n");
        return;
    }
    
    // Extract the folder path from the full executable path
    char* lastBackslash = strrchr(exeFolderPath, '\\');  // Find the last backslash
    if (lastBackslash != NULL) {
        *lastBackslash = '\0';  // Null-terminate the path at the last backslash to get the folder
        strcpy(exeName, lastBackslash + 1); // Store the executable name
        
        // Now, find the second-last backslash for the folder name
        char* secondLastBackslash = strrchr(exeFolderPath, '\\');
        if (secondLastBackslash != NULL) {
            strcpy(exeFolderName, secondLastBackslash + 1); // Copy just the last folder name
        } else {
            strcpy(exeFolderName, exeFolderPath); // If there's no second backslash, use the full path
        }
    }

    // Remove the ".exe" extension from exeName
    char* ext = strrchr(exeName, '.');
    if (ext != NULL) {
        *ext = '\0';  // Remove the extension part
    }

    // Construct the full path to the executable
    snprintf(exePath, MAX_PATH, "%s\\%s.exe", exeFolderPath, exeName);


    // Retrieve the user's Startup folder dynamically
    if (SHGetFolderPathA(NULL, CSIDL_STARTUP, NULL, 0, startupFolderPath) != S_OK) {
        printf("Failed to retrieve Startup folder path.\n");
        return;
    }

    // Copy the value of exeName into shortcutName
    strcpy(shortcutName, exeName);

    // Build the full path to the shortcut
    snprintf(shortcutPath, MAX_PATH, "%s\\%s.lnk", startupFolderPath, shortcutName); // Use the same shortcut name as created in createShortcutToExe
}


// Define IID_IPersistFile if it’s not defined
const IID IID_IPersistFile = {0x0000010b, 0x0000, 0x0000, {0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};

// Function to create a shortcut to the currently running executable in the user's Startup folder
void createShortcutToExe() {
    HRESULT hres;
    IShellLinkA* psl; // Use IShellLinkA for ANSI paths

    // Initialize COM library
    CoInitialize(NULL);

    // Create an instance of IShellLink
    hres = CoCreateInstance(&CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, &IID_IShellLinkA, (LPVOID*)&psl);

    if (SUCCEEDED(hres)) {
        IPersistFile* ppf;
        WCHAR wsz[MAX_PATH];

        // Set the path to the executable
        psl->lpVtbl->SetPath(psl, exePath);

        // Create the description dynamically
        char description[MAX_PATH];
        snprintf(description, MAX_PATH, "Shortcut to (%s.exe)", exeName);
        psl->lpVtbl->SetDescription(psl, description); // Set the dynamic description


        // Query IShellLink for IPersistFile to save the shortcut
        hres = psl->lpVtbl->QueryInterface(psl, &IID_IPersistFile, (void**)&ppf);

        if (SUCCEEDED(hres)) {
            // Convert the target path to a wide string for use with IPersistFile
            mbstowcs(wsz, shortcutPath, MAX_PATH);

            // Save the shortcut
            hres = ppf->lpVtbl->Save(ppf, wsz, TRUE);
            ppf->lpVtbl->Release(ppf);  // Release IPersistFile
        } else {
            printf("Failed to get IPersistFile interface.\n");
        }
        psl->lpVtbl->Release(psl);  // Release IShellLink
    } else {
        printf("Failed to create IShellLink instance.\n");
    }

    // Uninitialize COM library
    CoUninitialize();
}


/**
 * Adds the application to Windows startup by creating a registry entry in the "Run" 
 * key under HKEY_CURRENT_USER. This ensures the application launches automatically 
 * when the user logs in.
 */
void setAutoStartupRegistryKey() {
    HKEY hKey; // Handle for the registry key
    
    // Attempt to open the "Run" registry key under HKEY_CURRENT_USER for writing
    if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {

        // Set a new registry dynamic value exeFolderName variable to enable auto startup
        if (RegSetValueEx(hKey, exeFolderName, 0, REG_SZ, (const BYTE *)exePath, (DWORD)(strlen(exePath) + 1)) == ERROR_SUCCESS) {
            printf("Successfully added to startup.\n"); // Success message
        } else {
            printf("Failed to set registry value.\n"); // Error message if setting value fails
        }

        // Close the registry key handle to release resources
        RegCloseKey(hKey);
    } else {
        printf("Failed to open registry key.\n"); // Error message if unable to open key
    }
}

/**
 * Removes the application from Windows startup by deleting its registry entry 
 * from the "Run" key under HKEY_CURRENT_USER. This stops the application 
 * from launching automatically when the user logs in.
 */
void removeAutoStartupRegistryKey() {
    HKEY hKey; // Handle for the registry key

    // Attempt to open the "Run" registry key under HKEY_CURRENT_USER for writing
    if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {

        // Attempt to delete the registry value associated with exeFolderName to disable auto-startup
        if (RegDeleteValue(hKey, exeFolderName) == ERROR_SUCCESS) {
            printf("Successfully removed from startup.\n"); // Success message if the value is deleted
        } else {
            printf("Failed to delete registry value.\n"); // Error message if deleting the value fails
        }

        // Close the registry key handle to release resources
        RegCloseKey(hKey);
    } else {
        printf("Failed to open registry key.\n"); // Error message if unable to open the key
    }
}


// Function to handle "Open Item 1" action
void programAutoStartup(int isChecked) {
    printf("isChecked: %d\n", isChecked);

    if (isChecked) {
        initializeExecutableDetails();  // Initialize the executable details
        createShortcutToExe();          // If checked, Create the Shortcut
        setAutoStartupRegistryKey();    // If checked, Set Registry Key
    } else {
        removeAutoStartupRegistryKey(); // If Unchecked, Remove Registry Key
    }
}

// Function to handle "Open Item 2" action
void handleOpenItem2() {
    MessageBox(NULL, "You clicked Open Item 2!", "Info", MB_OK); // Info message for Item 2
}
