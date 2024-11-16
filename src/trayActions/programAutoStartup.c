#include <windows.h>
#include <shlobj.h>     // For SHGetFolderPath
#include <stdbool.h>    // Include the header for using boolean types (true/false)
#include <stdio.h>

// Global variables for executable path, folder, name, and shortcut name
char exePath[MAX_PATH];
char exeFolderPath[MAX_PATH];

char exeFolderName[MAX_PATH];
char exeName[MAX_PATH];

char startupFolderPath[MAX_PATH];

char shortcutPath[MAX_PATH];
char shortcutName[MAX_PATH];

char registryPath[MAX_PATH];

// Function to initialize global variables
void initializeVariables() {
    // Retrieve the path of the currently running executable
    if (GetModuleFileName(NULL, exeFolderPath, MAX_PATH) == 0) return;
    
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

    /**
     * Construct the full path to the executable
     * The executable path is constructed as "{exeFolderPath}\\{exeName}.exe"
     */
    snprintf(exePath, MAX_PATH, "%s\\%s.exe", exeFolderPath, exeName);


    // Retrieve the user's Startup folder dynamically
    if (SHGetFolderPathA(NULL, CSIDL_STARTUP, NULL, 0, startupFolderPath) != S_OK) return;

    // Copy the value of exeName into shortcutName
    strcpy(shortcutName, exeName);

    
    // The path is constructed as "{startupFolderPath}\{shortcutName}.lnk"
    snprintf(shortcutPath, MAX_PATH, "%s\\%s.lnk", startupFolderPath, shortcutName); // Use the same shortcut name as created in createShortcutToExe
    
    // The registry path is constructed  as "Software\{exeFolderName}\Settings"
    snprintf(registryPath, MAX_PATH, "Software\\%s\\Settings", exeFolderName);
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

        /**
         * Create the description dynamically
         * The shortcut description is dynamically set as "Shortcut to ({exeName}.exe)"
         */
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
        }
        psl->lpVtbl->Release(psl);  // Release IShellLink
    }

    // Uninitialize COM library
    CoUninitialize();
}

// Function to remove a shortcut from the user's Startup folder
void removeShortcutFromStartup() {
    // Attempt to delete the shortcut file using DeleteFileA
    DeleteFileA(shortcutPath);
}



/**
 * Adds the application to Windows startup by creating a registry entry in the "Run" 
 * key under HKEY_CURRENT_USER. This ensures the application launches automatically 
 * when the user logs in.
 */
void setAutoStartupRegistryKey() {
    HKEY hKey;  // Handle for the registry key

    // Attempt to open the "Run" registry key under HKEY_CURRENT_USER for writing
    if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        // Set a new registry dynamic value exeFolderName variable to enable auto startup
        RegSetValueEx(hKey, exeFolderName, 0, REG_SZ, (const BYTE *)exePath, (DWORD)(strlen(exePath) + 1));
        RegCloseKey(hKey); // Close the registry key handle to release resources
    }
}

/**
 * Removes the application from Windows startup by deleting its registry entry 
 * from the "Run" key under HKEY_CURRENT_USER. This stops the application 
 * from launching automatically when the user logs in.
 */
void removeAutoStartupRegistryKey() {
    HKEY hKey;  // Handle for the registry key

    // Attempt to open the "Run" registry key under HKEY_CURRENT_USER for writing
    if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        // Attempt to delete the registry value associated with exeFolderName to disable auto-startup
        RegDeleteValue(hKey, exeFolderName);
        RegCloseKey(hKey); // Close the registry key handle to release resources
    }
}


// Function to load the checkbox state from the registry
bool loadCheckboxState() {
    initializeVariables();

    HKEY hKey;                       // Handle for the registry key
    DWORD value = 1;                 // Default value (checked state)
    DWORD valueSize = sizeof(value); // Size of the DWORD variable

    // Open the registry key for reading
    if (RegOpenKeyEx(HKEY_CURRENT_USER, registryPath, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        // Retrieve the stored checkbox state
        RegQueryValueEx(hKey, "RUN_AT_STARTUP_CHECKBOX_STATE", NULL, NULL, (LPBYTE)&value, &valueSize);
        RegCloseKey(hKey);  // Close the registry key handle to release resources
    }
    return value != 0; // Returns true if the checkbox is enabled, false if disabled
}

// Function to save the checkbox state in the registry
void saveCheckboxState(bool state) {
    HKEY hKey;  // Handle for the registry key

    // Attempt to create or open the registry key for writing
    if (RegCreateKeyEx(HKEY_CURRENT_USER, registryPath, 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        DWORD value = state ? 1 : 0; // Set value to 1 if checked, 0 if unchecked
        
        // Store the checkbox state as a DWORD in the registry
        RegSetValueEx(hKey, "RUN_AT_STARTUP_CHECKBOX_STATE", 0, REG_DWORD, (const BYTE*)&value, sizeof(value));
        RegCloseKey(hKey); // Close the registry key handle to release resources
    }
}


// Function to handle program auto-startup behavior based on the checkbox state.
void programAutoStartup(bool isChecked) {
    if (isChecked) {
        // createShortcutToExe();          // If checked, Create the Shortcut
        setAutoStartupRegistryKey();    // If checked, Set Registry Key
    } else {
        // removeShortcutFromStartup();    // If Unchecked, Remove the Shortcut
        removeAutoStartupRegistryKey(); // If Unchecked, Remove Registry Key
    }

    saveCheckboxState(isChecked);   // Save the updated state to the registry
}

// Function to handle "Open Item 2" action
void handleOpenItem2() {
    MessageBox(NULL, "You clicked Open Item 2!", "Info", MB_OK); // Info message for Item 2
}
