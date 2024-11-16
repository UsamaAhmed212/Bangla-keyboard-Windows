// keyboard.c
#include <stdio.h>
#include <windows.h>
#include <stdbool.h> // Include for bool type

// Define Unicode values for vowels (স্বরবর্ণ)
const wchar_t vowels[] = {
    0x985,  // অ
    0x986,  // আ
    0x987,  // ই
    0x988,  // ঈ
    0x989,  // উ
    0x98A,  // ঊ
    0x98B,  // ঋ
    0x98C,  // ঌ
    0x98F,  // এ
    0x990,  // ঐ
    0x993,  // ও
    0x994   // ঔ
};

const wchar_t vowel_signs[] = {
    0x9BE,  // া
    0x9BF,  // ি
    0x9C0,  // ী
    0x9C1,  // ু
    0x9C2,  // ূ
    0x9C3,  // ৃ
    0x9C7,  // ে
    0x9C8,  // ৈ
    0x9CB,  // ো
    0x9CC   // ৌ
};

// Define Unicode values for consonants (ব্যঞ্জনবর্ণ)
const wchar_t consonants[] = {
    0x995,  // ক
    0x996,  // খ
    0x997,  // গ
    0x998,  // ঘ
    0x999,  // ঙ
    0x99A,  // চ
    0x99B,  // ছ
    0x99C,  // জ
    0x99D,  // ঝ
    0x99E,  // ঞ
    0x99F,  // ট
    0x9A0,  // ঠ
    0x9A1,  // ড
    0x9A2,  // ঢ
    0x9A3,  // ণ
    0x9A4,  // ত
    0x9A5,  // থ
    0x9A6,  // দ
    0x9A7,  // ধ
    0x9A8,  // ন
    0x9AA,  // প
    0x9AB,  // ফ
    0x9AC,  // ব
    0x9AD,  // ভ
    0x9AE,  // ম
    0x9AF,  // য
    0x9B0,  // র
    0x9B2,  // ল
    0x9B6,  // শ
    0x9B7,  // ষ
    0x9B8,  // স
    0x9B9,  // হ
    0x9DC,  // ড়
    0x9DD,  // ঢ়
    0x9DF,  // য়
    0x9F0,  // র‍্য
    0x9F1   // ঱ (used in some dialects)
};


/**
 * Function to check if a given Unicode character is a vowel.
 * Parameters:
 * - @param unicodeChar: The Unicode character to check.
 * Returns:
 * - @return true if the character is a vowel, false otherwise.
 */
bool isVowel(wchar_t unicodeChar) {
    for (int i = 0; i < sizeof(vowels) / sizeof(vowels[0]); i++) {
        if (unicodeChar == vowels[i]) {
            return true;
        }
    }
    return false;
}

/**
 * Function to check if a given Unicode character is a vowel sign.
 * Parameters:
 * - @param unicodeChar: The Unicode character to check.
 * Returns:
 * - @return true if the character is a vowel sign, false otherwise.
 */
bool isVowelSign(wchar_t unicodeChar) {
    for (int i = 0; i < sizeof(vowel_signs) / sizeof(vowel_signs[0]); i++) {
        if (unicodeChar == vowel_signs[i]) {
            return true;
        }
    }
    return false;
}

/**
 * Function to check if a given Unicode character is a consonant.
 * Parameters:
 * - @param unicodeChar: The Unicode character to check.
 * Returns:
 * - @return true if the character is a consonant, false otherwise.
 */
bool isConsonant(wchar_t unicodeChar) {
    for (int i = 0; i < sizeof(consonants) / sizeof(consonants[0]); i++) {
        if (unicodeChar == consonants[i]) {
            return true;
        }
    }
    return false;
}

/**
 * Function to simulate pressing the Backspace key a specified number of times.
 * Parameters:
 * - @param count: The number of times to send the Backspace key input.
 */
void SendBackspace(int count) {
    // printf("SendBackspace() Function run \n");

    INPUT input[4]; // Maximum of 2 backspaces, hence 4 inputs (2 down + 2 up)

    for (int i = 0; i < count; i++) {
        // Key down for Backspace
        input[0].type = INPUT_KEYBOARD;
        input[0].ki.wVk = VK_BACK; // Virtual key code for Backspace
        input[0].ki.dwFlags = 0; // Key down
        SendInput(1, &input[0], sizeof(INPUT));

        // Key up for Backspace
        input[1].type = INPUT_KEYBOARD;
        input[1].ki.wVk = VK_BACK; // Virtual key code for Backspace
        input[1].ki.dwFlags = KEYEVENTF_KEYUP; // Key up
        SendInput(1, &input[1], sizeof(INPUT));
    }
}

/**
 * Function to add a new wide character to the end of a wide character array.
 * Parameters:
 * - @param arr: Pointer to the wide character array.
 * - @param size: Pointer to the integer representing the current size of the array.
 * - @param value: The wide character value to be added to the array.
 */
void push(wchar_t** arr, int* size, wchar_t value) {
    *size += 1;
    *arr = realloc(*arr, *size * sizeof(wchar_t)); // Resize the array
    if (*arr == NULL) {
        fwprintf(stderr, L"Memory allocation failed\n"); // Print an error message if memory allocation fails
        exit(1);
    }
    (*arr)[*size - 1] = value; // Add the new element at the end
}

/**
 * Function to remove the first element from a wide character array by shifting all elements to the left.
 * Parameters:
 * - @param @param arr: Pointer to the wide character array.
 * - @param @param size: Pointer to the integer representing the current size of the array.
 */
void shift(wchar_t** arr, int* size) {
    if (*size == 0) {
        fwprintf(stderr, L"Array is empty, nothing to remove\n");
        return;
    }

    // Shift elements to the left
    for (int i = 1; i < *size; i++) {
        (*arr)[i - 1] = (*arr)[i];
    }

    *size -= 1;
    *arr = realloc(*arr, *size * sizeof(wchar_t)); // Resize the array
    if (*arr == NULL && *size > 0) {
        fwprintf(stderr, L"Memory allocation failed\n");
        exit(1);
    }
}

/**
 * Function to remove the last element from a wide character array and update the array size.
 * Parameters:
 * - @param @param @param arr: Pointer to the wide character array.
 * - @param @param @param size: Pointer to the integer representing the current size of the array.
 */
void pop(wchar_t** arr, int* size) {
    if (*size == 0) {
        fwprintf(stderr, L"Array is empty, nothing to pop\n"); // Print error message if there is nothing to pop
        return; // Return from the function if array is empty
    }

    *size -= 1; // Decrease the array size by 1
    if (*size == 0) {
        free(*arr); // Free the memory allocated to the array
        *arr = NULL; // Set the array pointer to NULL
    } else {
        *arr = realloc(*arr, *size * sizeof(wchar_t)); // Resize the array to the new size
        if (*arr == NULL) {
            fwprintf(stderr, L"Memory allocation failed\n"); // Print error message if memory reallocation failed
            exit(1); // Exit the program in case of memory reallocation failure
        }
    }
}


// Function to determine if Shift is pressed
bool isShiftPressed() {
    return (GetAsyncKeyState(VK_LSHIFT) & 0x8000) || (GetAsyncKeyState(VK_RSHIFT) & 0x8000);
}

// Function to determine if Caps Lock is on
bool isCapsLockOn() {
    return (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
}

// Function to determine if the pressed key should be uppercase
bool isUppercase() {
    return isShiftPressed() ^ isCapsLockOn(); // XOR to determine uppercase condition
}



/**
 * Function to track the sequence of key presses for forming the 'ঋ' character.
 * Parameters:
 * - @param virtualKeyCode: The virtual key code representing the key pressed.
 */

int rie_keyPressCount = 0; // track the sequence of key presses 'ঋ'

void rie_keyPressCountProcess(int virtualKeyCode) {
    if (virtualKeyCode == 0x52) {  // 'R' key
        rie_keyPressCount = 1; // Start counting for 'ঋ'
    } else if (virtualKeyCode == 0x49) {  // 'I' key
        if (rie_keyPressCount == 1) {
            rie_keyPressCount = 2; // Increment count to indicate 'ঋ' is in progress
        } else {
            rie_keyPressCount = 0; // Reset if 'I' or 'E' is pressed out of order
        }
    } else {
        rie_keyPressCount = 0; // Reset the rie(ঋ) counter for any other key press
    }
}


/**
 * Function to track the sequence of key presses for forming the 'ঙ' character.
 * Parameters:
 * - @param virtualKeyCode: The virtual key code representing the key pressed.
 */

int umo_keyPressCount = 0; // track the sequence of key presses 'ঙ'

void umo_keyPressCountProcess(int virtualKeyCode) {
    if (virtualKeyCode == 0x55) {  // 'U' key
        umo_keyPressCount = 1; // Start counting for 'ঙ'
    } else if (virtualKeyCode == 0x4D) {  // 'M' key
        if (umo_keyPressCount == 1) {
            umo_keyPressCount = 2; // Increment count to indicate 'ঙ' is in progress
        } else {
            umo_keyPressCount = 0; // Reset if 'M' or 'O' is pressed out of order
        }
    } else {
        umo_keyPressCount = 0; // Reset the umo(ঙ) counter for any other key press
    }
}


/**
 * Function to track the sequence of key presses for forming the 'ঞ' character.
 * Parameters:
 * - @param virtualKeyCode: The virtual key code representing the key pressed.
 */

int nio_keyPressCount = 0; // track the sequence of key presses 'ঞ'
void nio_keyPressCountProcess(int virtualKeyCode) {
    if (virtualKeyCode == 0x4E) {  // 'N' key
        nio_keyPressCount = 1; // Start counting for 'ঞ'
    } else if (virtualKeyCode == 0x49) {  // 'I' key
        if (nio_keyPressCount == 1) {
            nio_keyPressCount = 2; // Increment count to indicate 'ঞ' is in progress
        } else {
            nio_keyPressCount = 0; // Reset if 'I' or 'O' is pressed out of order
        }
    } else {
        nio_keyPressCount = 0; // Reset the nio(ঞ) counter for any other key press
    }
}


/**
 * Function to track the sequence of key presses for forming the 'ঠ' and 'থ' character.
 * Parameters:
 * - @param virtualKeyCode: The virtual key code representing the key pressed.
 */

int to_keyPressCount = 0; // track the sequence of key presses 'ঠ'
int tho_keyPressCount = 0; // track the sequence of key presses 'থ'

void to_and_tho_keyPressCountProcess(int virtualKeyCode) {
    if (virtualKeyCode == 0x54) {  // 'T' or 't' key
        if (isUppercase()) {  // Check if the key pressed is 'T' or 't'
            to_keyPressCount = 1; // Start counting for 'ঠ'
        } else {
            tho_keyPressCount = 1; // Start counting for 'থ'
        }
    } else {
        to_keyPressCount = 0; // Reset the to(ঠ) counter for any other key press
        tho_keyPressCount = 0;  // Reset the tho(থ) counter for any other key press
    }
}


/**
 * Function to track the sequence of key presses for forming the 'ঢ' and 'ধ' character.
 * Parameters:
 * - @param virtualKeyCode: The virtual key code representing the key pressed.
 */

int dho_keyPressCount = 0; // track the sequence of key presses 'ঢ'
int do_keyPressCount = 0; // track the sequence of key presses 'ধ'

void dho_and_do_keyPressCountProcess(int virtualKeyCode) {
    if (virtualKeyCode == 0x44) {  // 'D' or 'd' key
        if (isUppercase()) {  // Check if the key pressed is 'D' or 'd'
            dho_keyPressCount = 1; // Start counting for 'ঢ'
            do_keyPressCount = 0; // Reset the do(ধ) counter
        } else {
            do_keyPressCount = 1; // Start counting for 'ধ'
            dho_keyPressCount = 0; // Reset the dho(ঢ) counter
        }
    } else {
        dho_keyPressCount = 0; // Reset the dho(ঢ) counter for any other key press
        do_keyPressCount = 0;  // Reset the do(ধ) counter for any other key press
    }
}


/**
 * Function to track the sequence of key presses for forming the 'ঢ়' and '্র'(র ফলা) character.
 * Parameters:
 * - @param virtualKeyCode: The virtual key code representing the key pressed.
 */

int rho_keyPressCount = 0; // track the sequence of key presses 'ঢ়'
int roPhala_keyPressCount = 0; // track the sequence of key presses '্র'(র ফলা)
void rho_and_roPhala_keyPressCountProcess(int virtualKeyCode) {
    if (virtualKeyCode == 0x52) {  // 'R' or 'r' key
        if (isUppercase()) {  // Check if the key pressed is 'R' or 'r'
            rho_keyPressCount = 1; // Start counting for 'ঢ়'
            roPhala_keyPressCount = 0; // Reset the'্র'(র ফলা) counter
        } else {
            roPhala_keyPressCount = 1; // Start counting for '্র'(র ফলা)
            rho_keyPressCount = 0; // Reset the dho(ঢ়) counter
        }
    } else {
        rho_keyPressCount = 0; // Reset the rho(ঢ়) counter for any other key press
        roPhala_keyPressCount = 0;  // Reset the '্র'(র ফলা) counter for any other key press
    }
}


/**
 * Function to track the sequence of key presses for forming the 'ঢ়' and (ব ফলা) character.
 * Parameters:
 * - @param virtualKeyCode: The virtual key code representing the key pressed.
 */

int boPhala_keyPressCount = 0; // Track the sequence of key presses (ব ফলা)

void boPhala_keyPressCountProcess(int virtualKeyCode) {
    if (virtualKeyCode == 0x42 || virtualKeyCode == 0x56) {  // 'B'/'b' or 'V'/'v'
        if ((virtualKeyCode == 0x42 && !isUppercase()) || (virtualKeyCode == 0x56 && isUppercase())) {
            boPhala_keyPressCount = 1;  // Start counting for (ব ফলা)
        } else {
            boPhala_keyPressCount = 0;  // Reset the counter for (ব ফলা)
        }
    } else {
        boPhala_keyPressCount = 0;  // Reset the counter for any other key press
    }
}


/**
 * Function to track the sequence of key presses for forming the ':'(Colon) character.
 * Parameters:
 * - @param virtualKeyCode: The virtual key code representing the key pressed.
 */

int colon_keyPressCount = 0; // track the sequence of key presses ':'(Colon)

void colon_keyPressCountProcess(int virtualKeyCode) {
    if (virtualKeyCode == 0xBA) {  // ':'(Colon) key
        colon_keyPressCount = 1; // Start counting for '':'(Colon)
    } else {
        colon_keyPressCount = 0; // Reset the ':'(Colon) counter for any other key press
    }
}










// Track last key press Unicode Character
wchar_t lastKeyPressUnicodeChar = 0;

wchar_t* lastKeyPressUnicodeCharArray = NULL;
int unicodeCharArraySize = 0;


/**
 * Function to send a Unicode character and manage the last key pressed.
 * 
 * Parameters:
 * - @param virtualKeyCode: The virtual key code representing the key pressed.
 * - @param unicodeChar: The Unicode character to be sent as input.
 */
void SendUnicodeChar(int virtualKeyCode, wchar_t unicodeChar) {
    // Check if the character is not Backspace (Unicode 0x8)
    if (unicodeChar != 0x8) {
        lastKeyPressUnicodeChar = unicodeChar; // Update last key press Unicode character
        
        push(&lastKeyPressUnicodeCharArray, &unicodeCharArraySize, unicodeChar); // Add the character to the array and update the array size
    } else {
        if (unicodeCharArraySize > 0) {
            if (lastKeyPressUnicodeCharArray[unicodeCharArraySize - 1] != 0x20 && lastKeyPressUnicodeCharArray[unicodeCharArraySize - 1] != 0x9) {
                pop(&lastKeyPressUnicodeCharArray, &unicodeCharArraySize); // Remove the last element from the array

                if (unicodeCharArraySize > 0) { // Check if the array still has elements
                    lastKeyPressUnicodeChar = lastKeyPressUnicodeCharArray[unicodeCharArraySize - 1]; // Update last key press Unicode character
                } else {
                    lastKeyPressUnicodeChar = 0; // Reset if the array is now empty
                }
            }
        }
    }
    
    
    rie_keyPressCountProcess(virtualKeyCode); // Process the key press count for forming the 'ঋ' character based on the virtual key code

    umo_keyPressCountProcess(virtualKeyCode); // Process the key press count for forming the 'ঙ' character based on the virtual key code

    nio_keyPressCountProcess(virtualKeyCode); // Process the key press count for forming the 'ঞ' character based on the virtual key code
    
    to_and_tho_keyPressCountProcess(virtualKeyCode); // Process the key press count for forming the 'ঠ' and 'থ' character based on the virtual key code
    
    dho_and_do_keyPressCountProcess(virtualKeyCode); // Process the key press count for forming the 'ঢ' and 'ধ' character based on the virtual key code
    
    rho_and_roPhala_keyPressCountProcess(virtualKeyCode); // Process the key press count for forming the 'ঢ়' and '্র'(র ফলা) character based on the virtual key code
    
    boPhala_keyPressCountProcess(virtualKeyCode); // Process the key press count for forming the (ব ফলা) character based on the virtual key code

    colon_keyPressCountProcess(virtualKeyCode); // Process the key press count for forming the ':'(Colon) character based on the virtual key code


    // Check if the key pressed is not Space (0x20), Tab (0x9), or Backspace (0x8)
    if (virtualKeyCode != 0x20 && virtualKeyCode != 0x9 && virtualKeyCode != 0x8) {
        INPUT input[2] = {0}; // Array for sending key input

        // Simulate key press
        input[0].type = INPUT_KEYBOARD;
        input[0].ki.wScan = unicodeChar; // Unicode value of the character
        input[0].ki.dwFlags = KEYEVENTF_UNICODE;
        SendInput(1, &input[0], sizeof(INPUT));

        // Simulate key release
        input[1].type = INPUT_KEYBOARD;
        input[1].ki.wScan = unicodeChar;
        input[1].ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_UNICODE;
        SendInput(1, &input[1], sizeof(INPUT));

    
        // Alternative way

        // INPUT input = {0};

        // // Send the custom Unicode character
        // input.type = INPUT_KEYBOARD;
        // input.ki.wScan = unicodeChar; // Set the Unicode character
        // input.ki.dwFlags = KEYEVENTF_UNICODE; // Indicate Unicode input
        // SendInput(1, &input, sizeof(INPUT));
        
        // // Release the key (if needed)
        // input.ki.dwFlags = KEYEVENTF_KEYUP;
        // SendInput(1, &input, sizeof(INPUT));
    }
}


LRESULT CALLBACK KeyboardHook(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT *pKeyboard = (KBDLLHOOKSTRUCT *)lParam;

        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            bool isCtrlPressed = (GetAsyncKeyState(VK_CONTROL) & 0x8000);  // Check if Ctrl is pressed

            bool isAltPressed = (GetAsyncKeyState(VK_MENU) & 0x8000);  // Check if Alt is pressed
            
            bool isWindowsKeyPressed = (GetAsyncKeyState(VK_LWIN) & 0x8000) || (GetAsyncKeyState(VK_RWIN) & 0x8000);  // Check if either Left Windows key (VK_LWIN) or Right Windows key (VK_RWIN) is pressed
            
            switch (pKeyboard->vkCode) {
                case VK_SPACE:  // Space key
                    SendUnicodeChar(0x20, 0x20);  // Unicode for space
                    return 0; // Allow original space key input
                
                case VK_TAB:  // Tab key
                    SendUnicodeChar(0x9, 0x9);  // Unicode for tab
                    return 0; // Allow original tab key input


                case VK_BACK:  // Backspace key
                    SendUnicodeChar(0x8, 0x8);  // Unicode for backspace (ASCII 8)
                    return 0; // Allow original backspace key input


                /**
                ** Vowel (স্বরবর্ণ) Key Mapping 
                **/
                case 0x41: // 'A' or 'a'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed

                    if (!isUppercase()) {
                        // Handle Lowercase 'a'
                        if (!isConsonant(lastKeyPressUnicodeChar)) {
                            SendUnicodeChar(0x41, 0x985);  // 'অ'
                        } else {
                            SendUnicodeChar(0x41, 0x9be);  // 'া'
                        }
                    } else {
                        // Handle Uppercase 'A'
                        if (!isConsonant(lastKeyPressUnicodeChar)) {
                            SendUnicodeChar(0x41, 0x986);  // 'আ'
                        } else {
                            SendUnicodeChar(0x41, 0x9be);  // 'া'
                        }
                    }
                    return 1; // Block original 'A' or 'a'


                case 0x49: // 'I' or 'i'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed

                    if (!isUppercase()) {
                        // Handle Lowercase 'i'
                        if (!isConsonant(lastKeyPressUnicodeChar)) {
                            SendUnicodeChar(0x49, 0x987);  // 'ই'
                        } else {
                            SendUnicodeChar(0x49, 0x9BF);  // 'ি'
                        }
                    } else {
                        // Handle Uppercase 'I'
                        if (!isConsonant(lastKeyPressUnicodeChar)) {
                            SendUnicodeChar(0x49, 0x988);  // 'ঈ'
                        } else {
                            SendUnicodeChar(0x49, 0x9C0);  // 'ী'
                        }
                    }
                    return 1; // Block original 'I' or 'i'


                case 0x55: // 'U' or 'u'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed

                    if (!isUppercase()) {
                        // Handle Lowercase 'u'
                        if (!isConsonant(lastKeyPressUnicodeChar)) {
                            SendUnicodeChar(0x55, 0x989);  // 'উ'
                        } else {
                            SendUnicodeChar(0x55, 0x9C1);  // 'ু'
                        }
                    } else {
                        // Handle Uppercase 'U'
                        if (!isConsonant(lastKeyPressUnicodeChar)) {
                            SendUnicodeChar(0x55, 0x98A);  // 'ঊ'
                        } else {
                            SendUnicodeChar(0x55, 0x9C2);  // 'ূ'
                        }
                    }
                    return 1; // Block original 'U' or 'u'


                case 0x45: // 'E' or 'e'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed

                    if (rie_keyPressCount == 2) {
                        SendBackspace(2);  // Send two backspaces
                        if (unicodeCharArraySize < 1 || !isConsonant(lastKeyPressUnicodeCharArray[unicodeCharArraySize - 1])) {
                            SendUnicodeChar(0x45, 0x98B);  // 'ঋ'
                        } else {
                            SendUnicodeChar(0x45, 0x9C3);  // 'ৃ'
                        }
                        return 1; // Block original 'E' or 'e'
                    }
                    
                    if (!isUppercase()) {
                        // Handle Lowercase 'e'
                        if (!isConsonant(lastKeyPressUnicodeChar)) {
                        SendUnicodeChar(0x45, 0x98F);  // 'এ'
                        } else {
                            SendUnicodeChar(0x45, 0x9C7);  // 'ে'
                        }
                    } else {
                        // Handle Uppercase 'E'
                        if (!isConsonant(lastKeyPressUnicodeChar)) {
                            SendUnicodeChar(0x45, 0x990);  // 'ঐ'
                        } else {
                            SendUnicodeChar(0x45, 0x9C8);  // 'ৈ'
                        }
                    }
                    return 1; // Block original 'E' or 'e'


                case 0x4F: // 'O' or 'o'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed

                    if (umo_keyPressCount == 2) {
                        SendBackspace(2);  // Send two backspaces
                        SendUnicodeChar(0x4F, 0x999);  // 'ঙ'
                        return 1; // Block original 'O' or 'o'
                    }

                    if (nio_keyPressCount == 2) {
                        SendBackspace(2);  // Send two backspaces
                        SendUnicodeChar(0x4F, 0x99E);  // 'ঞ'
                        return 1; // Block original 'O' or 'o'
                    }
                    
                    if (!isUppercase()) {
                        // Handle Lowercase 'o'
                        if (!isConsonant(lastKeyPressUnicodeChar)) {
                            SendUnicodeChar(0x4F, 0x993);  // 'ও'
                        } else {
                            SendUnicodeChar(0x4F, 0x9CB);  // 'ো'
                        }
                    } else {
                        // Handle Uppercase 'O'
                        if (!isConsonant(lastKeyPressUnicodeChar)) {
                            SendUnicodeChar(0x4F, 0x994);  // 'ঔ'
                        } else {
                            SendUnicodeChar(0x4F, 0x9CC);  // 'ৌ'
                        }
                    }
                    return 1; // Block original 'O' or 'o'



                /**
                ** Consonant (ব্যঞ্জনবর্ণ)	Key Mapping
                **/
                case 0x4B: // 'K' or 'k'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed
               
                    if (!isUppercase()) {
                        // Handle Lowercase 'k'
                        SendUnicodeChar(0x4B, 0x995);  // 'ক'
                    } else {
                        // Handle Uppercase 'K'
                        SendUnicodeChar(0x4B, 0x996);  // 'খ'
                    }
                    return 1; // Block original 'K' or 'k'


                case 0x47: // 'G' or 'g'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed
               
                    if (!isUppercase()) {
                        // Handle Lowercase 'g'
                        SendUnicodeChar(0x47, 0x997);  // 'গ'
                    } else {
                        // Handle Uppercase 'G'
                        SendUnicodeChar(0x47, 0x998);  // 'ঘ'
                    }
                    return 1; // Block original 'G' or 'g'


                case 0x43: // 'C' or 'c'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed

                    if (!isUppercase()) {
                        // Handle Lowercase 'c'
                        SendUnicodeChar(0x43, 0x99A);  // 'চ'
                    } else {
                        // Handle Uppercase 'C'
                        SendUnicodeChar(0x43, 0x99B);  // 'ছ'
                    }
                    return 1; // Block original 'C' or 'c'


                case 0x4A: // 'J' or 'j'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed
               
                    if (!isUppercase()) {
                        // Handle Lowercase 'j'
                        SendUnicodeChar(0x4A, 0x99C);  // 'জ
                    } else {
                        // Handle Uppercase 'J'
                        SendUnicodeChar(0x4A, 0x99D);  // 'ঝ'
                    }
                    return 1; // Block original 'J' or 'j'


                case 0x54: // 'T' or't'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed
               
                    if (!isUppercase()) {
                        // Handle Lowercase 't'
                        if (tho_keyPressCount == 1) {
                            SendBackspace(1);  // Send One backspaces
                            SendUnicodeChar(0x54, 0x9A5);  // 'থ'
                            tho_keyPressCount = 0; // Reset the tho(থ) counter
                        } else {
                            SendUnicodeChar(0x54, 0x9A4);  // 'ত'
                        }
                        
                    } else {
                        // Handle Uppercase 'T'
                        if (to_keyPressCount == 1) {
                            SendBackspace(1);  // Send One backspaces
                            SendUnicodeChar(0x54, 0x9A0);  // 'ঠ'
                            to_keyPressCount = 0; // Reset the to(ঠ) counter
                        } else {
                            SendUnicodeChar(0x54, 0x99F);  // 'ট'
                        }
                    }
                    return 1; // Block original 'T' or 't'


                case 0x44: // 'D' or 'd'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed

                    if (!isUppercase()) {
                        // Handle Lowercase 'd'
                        if (do_keyPressCount == 1) {
                            SendBackspace(1);  // Send One backspaces
                            SendUnicodeChar(0x44, 0x9A7);  // 'ধ'
                            do_keyPressCount = 0; // Reset the do(ধ) counter
                        } else {
                            SendUnicodeChar(0x44, 0x9A6);  // 'দ'
                        }
                    } else {
                        // Handle Uppercase 'D'
                        if (dho_keyPressCount == 1) {
                            SendBackspace(1);  // Send One backspaces
                            SendUnicodeChar(0x44, 0x9A2);  // 'ঢ'
                            dho_keyPressCount = 0; // Reset the dho(ঢ) counter
                        } else {
                            SendUnicodeChar(0x44, 0x9A1);  // 'ড'
                        }
                    }
                    return 1; // Block original 'D' or 'd'


                case 0x4E: // 'N' or 'n'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed

                    if (!isUppercase()) {
                        // Handle Lowercase 'n'
                        SendUnicodeChar(0x4E, 0x9A8);  // 'ন'

                    } else {
                        // Handle Uppercase 'N'
                        SendUnicodeChar(0x4E, 0x9A3);  // 'ণ'
                    }
                    return 1; // Block original 'N' or 'n'


                case 0x50: // 'P' or 'p'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed

                    if (!isUppercase()) {
                        // Handle Lowercase 'p'
                        SendUnicodeChar(0x50, 0x9AA);  // 'প'
                    } else {
                        // Handle Uppercase 'P'
                        SendUnicodeChar(0x50, 0x9AB);  // 'ফ'
                    }
                    return 1; // Block original 'P' or 'p'


                case 0x46: // 'F' or 'f'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed

                    if (!isUppercase()) {
                        // Handle Lowercase 'p'
                        SendUnicodeChar(0x46, 0x9AB);  // 'ফ'
                    } else {
                        // Handle Uppercase 'P'
                        SendUnicodeChar(0x46, 0x9AA);  // 'প'
                    }
                    return 1; // Block original 'F' or 'f'


                case 0x42: // 'B' or 'b'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed

                    if (!isUppercase()) {
                        // Handle Lowercase 'b'
                        if (boPhala_keyPressCount == 1) {
                            /**
                             * Bangla Special Character Letter
                                - "(ব ফলা)"
                            */
                            if (unicodeCharArraySize >= 2 && isConsonant(lastKeyPressUnicodeCharArray[unicodeCharArraySize - 2])) {
                                SendBackspace(1);  // Send One backspaces
                            }
                            SendUnicodeChar(0x42, 0x9CD);  // '্‌'(হসন্ত)
                            SendUnicodeChar(0x42, 0x9AC);  // 'ব'
                        } else {
                            SendUnicodeChar(0x42, 0x9AC);  // 'ব'
                        }
                    } else {
                        // Handle Uppercase 'B'
                        SendUnicodeChar(0x42, 0x9AD);  // 'ভ'
                    }
                    return 1; // Block original 'B' or 'b'


                case 0x56: // 'V' or 'v'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed

                    if (!isUppercase()) {
                        // Handle Lowercase 'v'
                        SendUnicodeChar(0x56, 0x9AD);  // 'ভ'
                    } else {
                        // Handle Uppercase 'V'
                        if (boPhala_keyPressCount == 1) {
                            /**
                             * Bangla Special Character Letter
                                - "(ব ফলা)"
                            */
                            if (unicodeCharArraySize >= 2 && isConsonant(lastKeyPressUnicodeCharArray[unicodeCharArraySize - 2])) {
                                SendBackspace(1);  // Send One backspaces
                            }
                            SendUnicodeChar(0x56, 0x9CD);  // '্‌'(হসন্ত)
                            SendUnicodeChar(0x56, 0x9AC);  // 'ব'
                        } else {
                            SendUnicodeChar(0x56, 0x9AC);  // 'ব'
                        }
                    }
                    return 1; // Block original 'V' or 'v'


                case 0x4D: // 'M' or 'm'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed

                    if (!isUppercase()) {
                        // Handle Lowercase 'm'
                        SendUnicodeChar(0x4D, 0x9AE);  // 'ম'
                    } else {
                        // Handle Uppercase 'M'
                        SendUnicodeChar(0x4D, 0x9B7);  // 'ষ'
                    }
                    return 1; // Block original 'M' or 'm'


                case 0x5A: // 'Z' or 'z'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed
                    
                    if (!isUppercase()) {
                        // Handle Lowercase 'z'
                        SendUnicodeChar(0x5A, 0x9AF);  // 'য'
                    } else {
                        // Handle Uppercase 'Z'
                        /**
                         * Bangla Special Character Letter
                            - "(য ফলা)"
                        */
                        if (unicodeCharArraySize >= 1 && isConsonant(lastKeyPressUnicodeCharArray[unicodeCharArraySize - 1])) {
                            SendUnicodeChar(0x52, 0x9CD);  // '্‌'(হসন্ত)
                        }
                        SendUnicodeChar(0x5A, 0x9AF);  // 'য'
                    }
                    return 1; // Block original 'Z' or 'z'


                case 0x52: // 'R' or'r'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed

                    if (!isUppercase()) {
                        // Handle Lowercase 'r'
                        if (roPhala_keyPressCount == 1) {
                            /**
                             * Bangla Special Character Letter
                                - "(র ফলা)"
                            */
                            if (unicodeCharArraySize >= 2 && isConsonant(lastKeyPressUnicodeCharArray[unicodeCharArraySize - 2])) {
                                SendBackspace(1);  // Send One backspaces
                            }
                            SendUnicodeChar(0x52, 0x9CD);  // '্‌'(হসন্ত)
                            SendUnicodeChar(0x52, 0x9B0);  // 'র'
                            roPhala_keyPressCount = 0; // Reset the'্র'(র ফলা) counter
                        } else {
                            SendUnicodeChar(0x52, 0x9B0);  // 'র'
                        }
                    } else {
                        // Handle Uppercase 'R'
                        if (rho_keyPressCount == 1) {
                            SendBackspace(1);  // Send One backspaces
                            SendUnicodeChar(0x52, 0x9DD);  // 'ঢ়'
                            rho_keyPressCount = 0; // Reset the dho(ঢ়) counter
                            return 1; // Block original 'R' or'r'
                        } else {
                            SendUnicodeChar(0x52, 0x9DC);  // 'ড়'
                        }
                    }
                    return 1; // Block original 'R' or 'r'


                case 0x4C: // 'L' or 'l'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed

                    SendUnicodeChar(0x4C, 0x9B2);  // 'ল'
                    return 1; // Block original 'L' or 'l'


                case 0x53: // 'S' or 's'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed

                    if (!isUppercase()) {
                        // Handle Lowercase 's'
                        SendUnicodeChar(0x53, 0x9B8);  // 'স'
                    } else {
                        // Handle Uppercase 'S'
                        SendUnicodeChar(0x53, 0x9B6);  // 'শ'
                    }
                    return 1; // Block original 'S' or 's'


                case 0x48: // 'H' or 'h'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed

                    SendUnicodeChar(0x48, 0x9B9);  // 'হ'
                    return 1; // Block original 'H' or 'h'


                case 0x59: // 'Y' or 'y'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed

                    SendUnicodeChar(0x59, 0x9DF);  // 'য়'
                    return 1; // Block original 'Y' or 'y'


                case 0x51: // 'Q' or 'q'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed

                    if (!isUppercase()) {
                        // Handle Lowercase 'q'
                        SendUnicodeChar(0x51, 0x9ce);  // 'ৎ'
                    } else {
                        // Handle Uppercase 'Q'
                        SendUnicodeChar(0x51, 0x982);  // 'ং'
                    }
                    return 1; // Block original 'Q' or 'q'


                case 0xBA: // (Colon)':'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed

                    if (isShiftPressed()) {
                        if (colon_keyPressCount == 1) {
                            SendBackspace(1);  // Send One backspaces
                            SendUnicodeChar(0xBA, 0x3A);  //  (Colon) - ":" - কোলন
                            colon_keyPressCount = 0; // Reset the ':'(Colon) counter
                        } else {
                            SendUnicodeChar(0xBA, 0x983);  // 'ঃ'
                        }
                        return 1; // Block original (Colon)':'
                    }
                    colon_keyPressCount = 0; // Reset the ':'(Colon) counter
                    return 0; // Allow original key input if Ctrl key is pressed


                case 0x36: // // '6' Or '^'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed

                    if (isShiftPressed()) {
                        SendUnicodeChar(0xDC, 0x981);  // 'ঁ'
                    } else {
                        SendUnicodeChar(0x36, 0x09EC);  // '৬' (Six)
                    }
                    return 1; // Block original '6'



                /**
                 * Bangla Special Character Letter
                    - "হসন্ত"
                */
                case 0x58: // 'X' or'x'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed

                    if (!isShiftPressed() || unicodeCharArraySize < 1) { // Check if the Shift key is not pressed
                        SendUnicodeChar(0x58, 0x9CD);  // '্‌'(হসন্ত)
                    } else {
                        // Check if the Shift key is pressed thats means the character is (ref/reph)"রেফ"
                        
                        // Track key press Unicode Character (ref/reph)"রেফ"
                        wchar_t ref_PressBeforeKeyUnicodeChar = lastKeyPressUnicodeCharArray[unicodeCharArraySize - 1];
                        
                        if (isConsonant(lastKeyPressUnicodeChar)) {
                            // If the previous character is a consonant, replace it with (ref/reph)"রেফ"
                            SendBackspace(1);  // Send One backspaces
                            SendUnicodeChar(0x52, 0x9B0);  // 'র'
                            SendUnicodeChar(0x58, 0x9CD);  // '্‌'(হসন্ত)
                            SendUnicodeChar(0x58, ref_PressBeforeKeyUnicodeChar);  // Last key Unicode character before pressing (ref/reph)"রেফ"
                        } else {
                            SendUnicodeChar(0x58, 0x9CD);  // '্‌'(হসন্ত)
                        }
                    }
                    return 1; // Block original 'X' or'x'
                    

                /**
                 * Bangla Special Character Letter
                    - "।" - দাড়ি
                */
                case 0xBE: // '.' (Period key)
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed
                    
                    if (!isShiftPressed()) {
                        SendUnicodeChar(0xBE, 0x0964);  // '।' (Bengali Full Stop)
                        return 1;  // Block original '.' key input
                    }
                    return 0; // Allow original key input


                /**
                 * Dot or Period
                    - "." - ডট
                */
                case VK_DECIMAL: // '.' Numeric Keypad
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed
                    
                    if (!isShiftPressed()) {
                        SendUnicodeChar(VK_DECIMAL, 0x002E);  // '.' (Bangla Dot)
                        return 1;  // Block original '.' (Numeric Keypad) key input
                    }
                    return 0; // Allow original key input


                /**
                 * Additional Letters for Currency
                    - "৳" - টাকা
                */
                case 0x34: // '4'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed

                    if (isShiftPressed()) {
                        SendUnicodeChar(0x34, 0x09f3);  // '৳'(টাকা)
                    } else {
                        SendUnicodeChar(0x34, 0x09EA);  // '৪' (Four)
                    }
                    return 1;  // Block original key input



                /**
                 * Number Keys and Numeric Keypad Bengali Digits (সংখ্যা)
                    * - Key Mapping for Bengali Digits (০-৯)
                 */
                case 0x30: // '0'
                case VK_NUMPAD0: // Numeric keypad '0'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed
                    SendUnicodeChar(0x30, 0x09E6);  // '০' (Zero)
                    return 1;  // Block original key input

                case 0x31: // '1'
                case VK_NUMPAD1: // Numeric keypad '1'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed
                    SendUnicodeChar(0x31, 0x09E7);  // '১' (One)
                    return 1;  // Block original key input

                case 0x32: // '2'
                case VK_NUMPAD2: // Numeric keypad '2'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed
                    SendUnicodeChar(0x32, 0x09E8);  // '২' (Two)
                    return 1;  // Block original key input

                case 0x33: // '3'
                case VK_NUMPAD3: // Numeric keypad '3'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed
                    SendUnicodeChar(0x33, 0x09E9);  // '৩' (Three)
                    return 1;  // Block original key input

                // case 0x34: // '4'
                case VK_NUMPAD4: // Numeric keypad '4'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed
                    SendUnicodeChar(0x34, 0x09EA);  // '৪' (Four)
                    return 1;  // Block original key input

                case 0x35: // '5'
                case VK_NUMPAD5: // Numeric keypad '5'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed
                    SendUnicodeChar(0x35, 0x09EB);  // '৫' (Five)
                    return 1;  // Block original key input

                // case 0x36: // '6'
                case VK_NUMPAD6: // Numeric keypad '6'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed
                    SendUnicodeChar(0x36, 0x09EC);  // '৬' (Six)
                    return 1;  // Block original key input

                case 0x37: // '7'
                case VK_NUMPAD7: // Numeric keypad '7'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed
                    SendUnicodeChar(0x37, 0x09ED);  // '৭' (Seven)
                    return 1;  // Block original key input

                case 0x38: // '8'
                case VK_NUMPAD8: // Numeric keypad '8'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed
                    SendUnicodeChar(0x38, 0x09EE);  // '৮' (Eight)
                    return 1;  // Block original key input

                case 0x39: // '9'
                case VK_NUMPAD9: // Numeric keypad '9'
                     if (isCtrlPressed || isAltPressed || isWindowsKeyPressed) return 0; // Allow original key input if (Ctrl/Alt/Windows) key is pressed
                    SendUnicodeChar(0x39, 0x09EF);  // '৯' (Nine)
                    return 1;  // Block original key input

            }
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}


// Function to write Bangla
int initializeBangleKeyboard(HINSTANCE hInstance) {
    HHOOK hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHook, NULL, 0);
    
    if (hKeyboardHook == NULL) {
        MessageBox(NULL, "Failed to set hook!", "Error", MB_ICONERROR);
        return 1; // Exit if the hook couldn't be set
    }

    // Message loop to keep the hook active
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Unhook when done
    UnhookWindowsHookEx(hKeyboardHook);
    return 0;
}
