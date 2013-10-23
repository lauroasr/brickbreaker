/* biblioteca criada por Lauro Augusto S. Rezende :) */

#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdarg.h>
#include <string.h>

#define BLACK_COLOR 0
#define DARK_BLUE_COLOR 1
#define DARK_GREEN_COLOR 2
#define DARK_CYAN_COLOR 3
#define DARK_RED_COLOR 4
#define DARK_PINK_COLOR 5
#define DARK_YELLOW_COLOR 6
#define LIGHT_GRAY_COLOR 7
#define GRAY_COLOR 8
#define BLUE_COLOR 9
#define GREEN_COLOR 10
#define CYAN_COLOR 11
#define RED_COLOR 12
#define PINK_COLOR 13
#define YELLOW_COLOR 14
#define WHITE_COLOR 15
#define BACKGROUND_COLOR(color) 16 * color

// cursor vai para a posição dos parâmetros
void gotoxy(int x, int y) {
    static HANDLE hStdout = NULL;
    COORD coord;
    coord.X = x;
    coord.Y = y;
    if(!hStdout)
        hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hStdout, coord);
}

void setTextColor(int color) {
    HANDLE hStdout;
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdout, color);
}

int randomColor(int color) {
    int newColor = rand() % 16;
    if (newColor == color) {
        return 15 - color;
    }
    return newColor;
}

// chance informado nos parametros de retornar verdadeiro
bool random(int percent) {
    return (rand() % 100) + 1 <= percent;
}

float decimalPart(float number) {
    return number - (int) number;
}
