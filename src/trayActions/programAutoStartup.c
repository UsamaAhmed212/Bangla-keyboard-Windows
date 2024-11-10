#include <windows.h>
#include <shlobj.h>      // For SHGetFolderPath
#include <stdio.h>       // For printf, wprintf


// Global variables for executable path, folder, name, and shortcut name
char exePath[MAX_PATH];
char exeFolderPath[MAX_PATH];
char exeName[MAX_PATH];
wchar_t shortcutName[MAX_PATH];  // Wide string for shortcut name

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
    }

    // Remove the ".exe" extension from exeName
    char* ext = strrchr(exeName, '.');
    if (ext != NULL) {
        *ext = '\0';  // Remove the extension part
    }

    // Construct the full path to the executable
    snprintf(exePath, MAX_PATH, "%s\\%s.exe", exeFolderPath, exeName);

    // Convert exeName to a wide string for shortcut name
    mbstowcs(shortcutName, exeName, MAX_PATH);
}


// Define IID_IPersistFile if it’s not defined
const IID IID_IPersistFile = {0x0000010b, 0x0000, 0x0000, {0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};

// Function to create a shortcut to the currently running executable in the user's Startup folder
void createShortcutToExe() {
    HRESULT hres;
    IShellLinkA* psl; // Use IShellLinkA for ANSI paths

    // Retrieve the user's Startup folder dynamically
    char startupFolderPath[MAX_PATH];
    if (SHGetFolderPathA(NULL, CSIDL_STARTUP, NULL, 0, startupFolderPath) != S_OK) {
        printf("Failed to retrieve Startup folder path.\n");
        return;
    }

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
            mbstowcs(wsz, startupFolderPath, MAX_PATH);

            // Append the shortcut filename dynamically and add ".lnk" extension
            wcscat(wsz, L"\\");
            wcscat(wsz, shortcutName);  // Append the dynamic shortcut name
            wcscat(wsz, L".lnk");  // Append the ".lnk" extension

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
 * Set the application to automatically start with Windows by adding an entry
 * to the "Run" registry key under HKEY_CURRENT_USER. This entry points to the
 * executable path provided in `exePath`.
 */
void setAutoStartupRegistryKey() {
    HKEY hKey; // Handle for the registry key
    
    // Attempt to open the "Run" registry key under HKEY_CURRENT_USER for writing
    if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {

        // Set a new registry dynamic value exeName variable to enable auto startup
        if (RegSetValueEx(hKey, exeName, 0, REG_SZ, (const BYTE *)exePath, (DWORD)(strlen(exePath) + 1)) == ERROR_SUCCESS) {
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

// Function to handle "Open Item 1" action
void programAutoStartup(int isChecked) {
    printf("isChecked: %d\n", isChecked);

    if (isChecked) {
        initializeExecutableDetails();  // Initialize the executable details
        createShortcutToExe();          // If checked, Create the Shortcut
        setAutoStartupRegistryKey();    // If checked, Set Registry Key
    } else {
        printf("Action is Unchecked\n");
    }
}

// Function to handle "Open Item 2" action
void handleOpenItem2() {
    MessageBox(NULL, "You clicked Open Item 2!", "Info", MB_OK); // Info message for Item 2
}
