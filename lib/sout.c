/*
Copyright (C) 2026 waxodium <waxodium@proton.me>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#include <stdarg.h>

#if defined(_WIN32)
    #include <windows.h>
#else
    #include <unistd.h>
#endif

void _putchar(char character) {
#if defined(_WIN32)
    DWORD written;
    WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), &character, 1, &written, NULL);
#else
    write(1, &character, 1);
#endif
}

void sout(const char *format, ...) {
    va_list arguments;
    va_start(arguments, format);

    for (const char *cursor = format; *cursor != '\0'; cursor++) {
        
        if (*cursor == '%' && *(cursor + 1) != '\0') {
            cursor++;
            
            switch (*cursor) {
                case 's': {
                    char *text = va_arg(arguments, char *);
                    if (text == NULL) {
                        text = "(null)";
                    }
                    while (*text != '\0') {
                        _putchar(*text++);
                    }
                    break;
                }
                
                case 'd': {
                    int number = va_arg(arguments, int);
                    char digits_buffer[12];
                    int position = 0;
                    
                    if (number == 0) {
                        _putchar('0');
                        break;
                    }
                    if (number < 0) {
                        _putchar('-');
                        number = -number; 
                    }
                    
                    while (number > 0) {
                        digits_buffer[position++] = (number % 10) + '0';
                        number = number/10;
                    }
                    
                    while (position > 0) {
                        _putchar(digits_buffer[--position]);
                    }
                    break;
                }
                
                case 'c': {
                    char single_character = (char)va_arg(arguments, int);
                    _putchar(single_character);
                    break;
                }
                
                case '%': {
                    _putchar('%');
                    break;
                }
                
                default: {
                    _putchar('%');
                    _putchar(*cursor);
                    break;
                }
            }
        } else {
            _putchar(*cursor);
        }
    }

    va_end(arguments);
}
