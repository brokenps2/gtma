#include "Util.h"
#include <SDL2/SDL_stdinc.h>
#include <nuklear.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

float glc(float color) {
    float glcolor;
    glcolor = color/255;
    return glcolor;
}

char *ftoa(float num, int precision) {
    char *str = (char *)malloc(32);
    if (!str) return NULL;

    int intPart = (int)num;
    float fracPart = fabs(num - intPart);

    sprintf(str, "%d", intPart);

    if (precision > 0) {
        char *ptr = str;
        while (*ptr) ptr++;
        *ptr++ = '.';

        for (int i = 0; i < precision; i++) {
            fracPart *= 10;
            int digit = (int)fracPart;
            *ptr++ = '0' + digit;
            fracPart -= digit;
        }
        *ptr = '\0';
    }

    return str;
}

float ftoa_reverse(const char *str) {
    if (!str) return 0.0f;

    float result = 0.0f;
    int sign = 1;

    // Handle negative numbers
    if (*str == '-') {
        sign = -1;
        str++;
    }

    // Process integer part
    while (*str && *str != '.') {
        if (*str >= '0' && *str <= '9') {
            result = result * 10 + (*str - '0');
        } else {
            return 0.0f; // Invalid character
        }
        str++;
    }

    // Process fractional part
    if (*str == '.') {
        str++;
        float fraction = 0.0f;
        float divisor = 10.0f;

        while (*str) {
            if (*str >= '0' && *str <= '9') {
                fraction += (*str - '0') / divisor;
                divisor *= 10.0f;
            } else {
                return 0.0f; // Invalid character
            }
            str++;
        }

        result += fraction;
    }

    return result * sign;
}
