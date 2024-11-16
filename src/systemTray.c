// systemTray.c
#include <windows.h>
#include <shellapi.h>
#include <stdbool.h>
#include "../include/resource.h"  // Include custom resources
#include "../include/trayActions/programAutoStartup.h"  // Include the header file for the menu items actions
#include "../include/shared.h"  // Include shared resources like variables and function prototypes from the shared header file

// Global variables for tray icon and images
NOTIFYICONDATA nid;       // Structure for the notification icon
HICON defaultIcon;        // Current icon displayed in the tray
HICON banglaIcon;         // Bangla keyboard icon
HICON currentIcon;        // Current icon displayed in the tray
HBITMAP exitBitmap;       // Bitmap for the exit menu item
HBITMAP checkedBitmap;    // Bitmap for checked state
HBITMAP uncheckedBitmap;  // Bitmap for unchecked state

bool isChecked; // Track checkbox state (checked or unchecked)

// Enum for menu item identifiers
enum MenuItems {
    ITEM_RUN_AT_STARTUP, // Automatically assigned value 0
    ITEM_OPEN_2, // Automatically assigned value 1
    ITEM_EXIT    // Automatically assigned value 2
};

// Function declarations
HMENU CreateContextMenu();
LRESULT CALLBACK SystemTrayWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Function to toggle the system tray icon based on the current typing mode
void ToggleIcon() {
    HINSTANCE hInstance = GetModuleHandle(NULL); // Get the handle of the current module
    // Check typing mode (Bangla or English) and update the system tray icon
    if (typingMode == 1 ) {
        currentIcon = banglaIcon; // Switch to Bangla icon
    } else {
        currentIcon = defaultIcon; // Switch back to the default icon
    }

    nid.hIcon = currentIcon; // Update the icon in the NOTIFYICONDATA structure
    Shell_NotifyIcon(NIM_MODIFY, &nid); // Modify the tray icon
}


// Function to create the context menu for the tray icon
HMENU CreateContextMenu() {
    HMENU hMenu = CreatePopupMenu(); // Create a new popup menu

    // Append menu items with identifiers and text
    // Add the checkbox item to the menu with custom bitmap
    if (isChecked) {
        AppendMenu(hMenu, MF_STRING | MF_CHECKED, ITEM_RUN_AT_STARTUP, "Run at Startup"); // Item 1
        MENUITEMINFO mii = {0};
        mii.cbSize = sizeof(MENUITEMINFO);
        mii.fMask = MIIM_BITMAP | MIIM_ID; // Specify bitmap and ID
        mii.wID = ITEM_RUN_AT_STARTUP;
        mii.hbmpItem = checkedBitmap; // Set checked bitmap
        SetMenuItemInfo(hMenu, ITEM_RUN_AT_STARTUP, FALSE, &mii);
    } else {
        AppendMenu(hMenu, MF_STRING | MF_UNCHECKED, ITEM_RUN_AT_STARTUP, "Run at Startup"); // Item 1
        MENUITEMINFO mii = {0};
        mii.cbSize = sizeof(MENUITEMINFO);
        mii.fMask = MIIM_BITMAP | MIIM_ID; // Specify bitmap and ID
        mii.wID = ITEM_RUN_AT_STARTUP;
        mii.hbmpItem = uncheckedBitmap; // Set unchecked bitmap
        SetMenuItemInfo(hMenu, ITEM_RUN_AT_STARTUP, FALSE, &mii);
    }

    AppendMenu(hMenu, MF_STRING, ITEM_OPEN_2, "Open Item 2"); // Item 2
    AppendMenu(hMenu, MF_SEPARATOR, 0, NULL); // Separator line
    AppendMenu(hMenu, MF_STRING, ITEM_EXIT, "Exit"); // Exit item

    // Set up menu item information for icons
    MENUITEMINFO mii = { 0 }; // Initialize MENUITEMINFO structure
    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_BITMAP | MIIM_ID; // Specify bitmap and ID

    mii.wID = ITEM_OPEN_2;  // ID for Open Item 2
    mii.hbmpItem = exitBitmap; // Set the bitmap for Item 2
    SetMenuItemInfo(hMenu, ITEM_OPEN_2, FALSE, &mii);

    mii.wID = ITEM_EXIT;  // ID for the Exit item
    mii.hbmpItem = exitBitmap; // Set the bitmap for the Exit item
    SetMenuItemInfo(hMenu, ITEM_EXIT, FALSE, &mii);

    return hMenu; // Return the created menu
}


// Window Procedure to handle messages for the application window
LRESULT CALLBACK SystemTrayWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_APP:
            // Handle right-click to show the context menu
            if (lParam == WM_RBUTTONUP) {
                POINT cursorPos;
                GetCursorPos(&cursorPos); // Get current mouse cursor position

                HMENU hMenu = CreateContextMenu(); // Create the context menu
                SetForegroundWindow(hwnd); // Bring the window to the foreground
                TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, cursorPos.x, cursorPos.y, 0, hwnd, NULL); // Show the menu
                DestroyMenu(hMenu); // Clean up the menu after use
            } else if (lParam == WM_LBUTTONUP) {
                typingMode = !typingMode;  // Toggle (typingMode) between English and Bangla modes
                ToggleIcon();  // Toggle the tray icon on left-click
            }
            break;
        case WM_COMMAND:
            // Handle menu item commands
            switch (LOWORD(wParam)) {
                case ITEM_RUN_AT_STARTUP:
                    isChecked = !isChecked; // Toggle checkbox state
                    Shell_NotifyIcon(NIM_MODIFY, &nid); // Modify the tray icon to reflect changes
                    programAutoStartup(isChecked); // Call function for ITEM_RUN_AT_STARTUP
                    break;
                case ITEM_OPEN_2:
                    handleOpenItem2(); // Call function for Item 2
                    break;
                case ITEM_EXIT:
                    Shell_NotifyIcon(NIM_DELETE, &nid); // Remove the icon from the tray
                    // PostQuitMessage(0); // Quit the application
                    ExitProcess(0);  // Immediately terminates the application
                    break;
            }
            break;
        case WM_DESTROY:
            Shell_NotifyIcon(NIM_DELETE, &nid); // Clean up icon on window destroy
            PostQuitMessage(0); // Exit the application
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam); // Default window procedure
    }
    return 0; // Return 0 to indicate message handled
}

// Function to initialize the system tray
void systemTrayInit(HINSTANCE hInstance) {
    // Load icons for the tray application
    defaultIcon = (HICON)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_keyboard));
    banglaIcon = (HICON)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_bangla_keyboard));

    // Load bitmap for exit menu item
    exitBitmap = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_EXIT_BUTTON), IMAGE_BITMAP, 16, 16, LR_LOADTRANSPARENT);

    // Load bitmaps for checked and unchecked states
    checkedBitmap = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_CHECKED), IMAGE_BITMAP, 16, 16, LR_LOADTRANSPARENT);
    uncheckedBitmap = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_UNCHECKED), IMAGE_BITMAP, 16, 16, LR_LOADTRANSPARENT);

    // Register window class
    WNDCLASS wc = {0}; // Initialize WNDCLASS structure
    wc.lpfnWndProc = SystemTrayWindowProc; // Set window procedure
    wc.hInstance = hInstance; // Set application instance
    wc.lpszClassName = "TrayApp"; // Set class name
    RegisterClass(&wc); // Register the window class

    // Create the application window
    HWND hwnd = CreateWindow("TrayApp", "Tray Application", WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                             NULL, NULL, hInstance, NULL);

    // Set up notification icon data
    nid.cbSize = sizeof(NOTIFYICONDATA); // Set size of NOTIFYICONDATA
    nid.hWnd = hwnd; // Set the window handle
    nid.uID = 1; // Set icon ID
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP; // Set icon, message, and tooltip
    nid.hIcon = defaultIcon; // Set the defaultIcon icon
    nid.uCallbackMessage = WM_APP; // Set callback message
    strcpy(nid.szTip, "Bangla Keyboard"); // Set tooltip text

    Shell_NotifyIcon(NIM_ADD, &nid); // Add the icon to the system tray
    ShowWindow(hwnd, SW_HIDE); // Hide the main window
    
    // Load the last saved checkbox state (checked or unchecked)
    isChecked = loadCheckboxState();

    programAutoStartup(isChecked); // Call function for Auto Startup

    // Message loop for processing messages
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg); // Translate keyboard messages
        DispatchMessage(&msg); // Dispatch messages to the window procedure
    }

    // Clean up resources before exiting
    DestroyIcon(defaultIcon); // Destroy current icon
    DestroyIcon(banglaIcon); // Destroy Bangla icon
    DeleteObject(exitBitmap); // Delete exit bitmap
}
