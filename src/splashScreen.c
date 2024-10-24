// splashScreen.c
#include <windows.h>
#include "../include/resource.h" // Include your resource header

// Function prototype for window procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Global variable to hold the bitmap
HBITMAP hBitmap;
#define TIMER_ID 1  // Timer identifier

// Function to show the splash screen
void showSplashScreen(HINSTANCE hInstance, int nShowCmd) {
    const char CLASS_NAME[] = "Splash Screen Class";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc; // Set the window procedure
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    
    // Load the icon (optional)
    // wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYICON)); // Set window icon
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Set default cursor

    // Register the window class
    RegisterClass(&wc);

    // Define window dimensions
    const int width = 485;  // Window width
    const int height = 355; // Window height

    // Calculate center position
    int xPos = (GetSystemMetrics(SM_CXSCREEN) - width) / 2; // Center X
    int yPos = (GetSystemMetrics(SM_CYSCREEN) - height) / 2; // Center Y

    // Create the window
    HWND hwnd = CreateWindowEx(
        0,                          // Optional window styles
        CLASS_NAME,                 // Window class
        NULL,                       // Window text
        WS_POPUP,                   // Window style
        xPos, yPos, width, height,  // Size and position
        NULL,                       // Parent window
        NULL,                       // Menu
        hInstance,                  // Instance handle
        NULL                        // Additional application data
    );

    // Check if the window was created successfully
    if (hwnd == NULL) {
        return;
    }

    // Load the bitmap from resources
    hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_SPLASH_SCREEN));

    // Show the window
    ShowWindow(hwnd, nShowCmd);

    // Set a timer to close the splash screen after 2 seconds (2000 milliseconds)
    SetTimer(hwnd, TIMER_ID, 2000, NULL);

    // Run the message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Cleanup
    DeleteObject(hBitmap); // Free the bitmap resource
}

// Window procedure function
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0); // Post a quit message
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Draw the BMP image
            if (hBitmap) {
                HDC hdcMem = CreateCompatibleDC(hdc);
                SelectObject(hdcMem, hBitmap);
                BITMAP bitmap;
                GetObject(hBitmap, sizeof(bitmap), &bitmap);
                BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
                DeleteDC(hdcMem);
            }

            EndPaint(hwnd, &ps);
        }
        return 0;
        case WM_TIMER: // Handle timer events
            if (wParam == TIMER_ID) {
                DestroyWindow(hwnd); // Close the splash screen
            }
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
