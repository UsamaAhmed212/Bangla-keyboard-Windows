// app_data.h
#ifndef APP_DATA_H
#define APP_DATA_H

#include <windows.h>

// Global structure to hold application data
typedef struct {
    HINSTANCE hInstance;
    int nShowCmd;
} AppData;

extern AppData appData; // Declare the global instance

#endif // APP_DATA_H
