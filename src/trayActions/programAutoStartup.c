#include <windows.h>
#include <shlobj.h>      // For SHGetFolderPath
#include <stdio.h>       // For printf, wprintf

// Define IID_IPersistFile if it’s not defined
const IID IID_IPersistFile = {0x0000010b, 0x0000, 0x0000, {0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};

// Function to create a shortcut to the currently running executable in the user's Startup folder
void createShortcutToExe() {
    HRESULT hres;
    IShellLinkA* psl; // Use IShellLinkA for ANSI paths

    // Buffer to hold the executable path
    char exePath[MAX_PATH];
    char exeFolderPath[MAX_PATH];
    char exeName[MAX_PATH];

    // Retrieve the path of the currently running executable
    if (GetModuleFileName(NULL, exeFolderPath, MAX_PATH) == 0) {
        printf("Failed to get the executable path.\n");
        return;
    }
    
    // Extract the folder path from the full executable path
    char* lastBackslash = strrchr(exeFolderPath, '\\');  // Find the last backslash
    if (lastBackslash != NULL) {
        *lastBackslash = '\0';  // Null-terminate the path at the last backslash to get the folder
    }

    // Extract the executable name (without extension)
    strcpy(exeName, lastBackslash + 1); // Get the part after the last backslash
    // Remove the ".exe" extension
    char* ext = strrchr(exeName, '.');
    if (ext != NULL) {
        *ext = '\0';  // Remove the extension part
    }

    // Construct the full path to the executable dynamically
    snprintf(exePath, MAX_PATH, "%s\\%s.exe", exeFolderPath, exeName);

    // Define the shortcut name dynamically (using the exe name)
    wchar_t shortcutName[MAX_PATH];
    mbstowcs(shortcutName, exeName, MAX_PATH); // Convert the exe name to a wide string for Unicode support

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

// Function to handle "Open Item 1" action
void programAutoStartup(int isChecked) {
    printf("isChecked: %d\n", isChecked);

    if (isChecked) {
        createShortcutToExe(); // If checked, create the shortcut
    } else {
        printf("Action is Unchecked\n");
    }
}

// Function to handle "Open Item 2" action
void handleOpenItem2() {
    MessageBox(NULL, "You clicked Open Item 2!", "Info", MB_OK); // Info message for Item 2
}
