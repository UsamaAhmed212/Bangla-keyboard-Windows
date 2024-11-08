// keyboard.h
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>

// Function declarations
int initializeBangleKeyboard(HINSTANCE hInstance);
bool isVowel(wchar_t unicodeChar);
bool isVowelSign(wchar_t unicodeChar);
void push(wchar_t** arr, int* size, wchar_t value);
void pop(wchar_t** arr, int* size);
bool isShiftPressed();
bool isCapsLockOn();
bool isUppercase();

// Vowel and vowel sign constants
extern const wchar_t vowels[];
extern const wchar_t vowel_signs[];

#endif // KEYBOARD_H
