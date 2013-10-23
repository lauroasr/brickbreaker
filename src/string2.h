//#include "..\easy.h"
#include <conio.h>
#include <stdio.h>
#include <string.h>

/**
    Remove espaços contidos no início e fim da String.
    Os espaços entre caracteres não são removidos.
**/
void trim(char *s) {
    char *start = 0, *end;

    // os pontos de início e fim da String são encontrados e colocados nos ponteiros start e end
    for (char *i = s; *i; i++) {
        if (!start) {
            if (*i != ' ') {
                start = i;
            }
        } else if (*i != ' ') {
            end = i;
        }
    }

    // copia o conteúdo do start e end para o início da String
    for (char *i = s; i <= s + (end - start); i++) {
        *i = *(start + (i - s));
    }

    // coloca o \0 na String
    *(s + (end - start) + sizeof(char)) = '\0';
}

/**
    Retorna o tamanho da String.
    O tamanho é encontrado contando todos os caracteres da String até seu \0.
**/
int length(char *s) {
    int stringLength = 0;

    // procura pelo \0 na String
    for (char *i = s; *i; i++) {
        stringLength++;
    }

    // retorna o tamanho encontrado
    return stringLength;
}

/**
    Retorna true se todos os caracteres da primeira String dos parâmetros
    forem iguais à segunda. False caso contrário.
**/
bool equals(char *s0, char *s1) {
    // escaneia todos os caracteres das Strings informadas
    for (int i = 0; i < length(s0); i++) {
        if (*(s0 + i * sizeof(char)) != *(s1 + i * sizeof(char))) {
            // retorna falso se um dos caracteres não se coincidirem
            return false;
        }
    }
    return true;
}

/**
    Retorna true se todos os caracteres da primeira String dos parâmetros
    forem iguais à segunda, ignorando maiúsculas e minúsculas. False caso contrário.
**/
bool equalsIgnoreCase(char *s0, char *s1) {
    // escaneia todos os caracteres das Strings informadas
    for (int i = 0; i < length(s0); i++) {
        if (*(s0 + i * sizeof(char)) != *(s1 + i * sizeof(char))) {
            if (*(s0 + i * sizeof(char)) > 'A' && *(s0 + i * sizeof(char)) < 'Z') {
                if (*(s0 + i * sizeof(char)) + 32 != *(s1 + i * sizeof(char)) + 32) {
                    // retorna falso se um dos caracteres não se coincidirem
                    return false;
                }
            }
        }
    }
    return true;
}

/**
    Coloca todos os caracteres de A a Z para maiúsculo.
**/
void toUpperCase(char *s) {
    for (int i = 0; i < length(s); i++) {
        if (*(s + i * sizeof(char)) >= 'a' && *(s + i * sizeof(char)) <= 'z') {
            *(s + i * sizeof(char)) -= 32;
        }
    }
}

/**
    Coloca todos os caracteres de A a Z para minúsculo.
**/
void toLowerCase(char *s) {
    for (int i = 0; i < length(s); i++) {
        if (*(s + i * sizeof(char)) >= 'A' && *(s + i * sizeof(char)) <= 'Z') {
            *(s + i * sizeof(char)) += 32;
        }
    }
}

/**
    Procura pela primeira ocorrência do caractere informado na String dos parâmetros
**/
int indexOf(char *s, char c) {
    for (int i = 0; i < length(s); i++) {
        if (*(s + i * sizeof(char)) == c) {
            return i;
        }
    }
    return - 1;
}

/**
    Procura pela última ocorrência do caractere informado na String dos parâmetros
**/
int lastIndexOf(char *s, char c) {
    for (int i = length(s) - 1; i >= 0; i--) {
        if (*(s + i * sizeof(char)) == c) {
            return i;
        }
    }
    return - 1;
}

/**
    Retorna uma String com os caracteres dentro das posições informadas
**/
char *substring(char *s, int iniPos, int finalPos) {
    static char output[100];
    for (int i = iniPos; i <= finalPos; i++) {
        output[i - iniPos] = *(s + i * sizeof(char));
    }
    output[finalPos - iniPos + 1] = '\0';
    return output;
}

/**
    Análisa a String dos parâmetros e retorna o inteiro que ela contém.
    Caso o número seja inválido, zero será retornado.
**/
int parseInt(char *s) {
    int output = 0, base = 1;

    for (int i = length(s) - 1; i >= 0; i--) {
        if (*(s + i * sizeof(char)) < '0' || *(s + i * sizeof(char)) > '9') {
            if (i == 0 && *(s + i * sizeof(char)) == '-') {
                output *= - 1;
                break;
            } else {
                return 0;
            }
        }
        output += (*(s + i * sizeof(char)) - '0') * base;
        base *= 10;
    }
    return output;
}

float parseFloat(char *s) {
    int output = 0/*, base = 1*/;

    int decimalPoint = - 1;
    for (int i = length(s) - 1; i >= 0; i--) {
        if (*(s + i * sizeof(char)) == '.') {
            decimalPoint = i;
            break;
        }
    }
    if (decimalPoint > 0) {
        for (int i = length(s) - 1; i > decimalPoint; i--) {
            if (*(s + i * sizeof(char)) < '0' || *(s + i * sizeof(char)) > '9') {
                if (i == 0 && *(s + i * sizeof(char)) == '-') {
                    output *= - 1;
                    break;
                } else {

                }
            }
        }
    }
    return 0;
}
