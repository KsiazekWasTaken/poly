/** @file
   Parser wielomianów + funkcja odczytująca kod polecenia.

   @author Piotr Książek
   @copyright Uniwersytet Warszawski
   @date 2017-04-24, 2017-05-26
*/

#ifndef KALKULATOR_PARSE_H
#define KALKULATOR_PARSE_H

#include "poly.h"
#include <stdio.h>
#include "array.h"
#include <limits.h>

#define ZERO 11 ///<kod oznaczający polecenie ZERO
#define IS_COEFF 12 ///<kod oznaczający polecenie IS_COEFF
#define IS_ZERO 13 ///<kod oznaczający polecenie IS_ZERO
#define CLONE 14 ///<kod oznaczający polecenie CLONE
#define ADD 15 ///<kod oznaczający polecenie ADD
#define MUL 16 ///<kod oznaczający polecenie MUL
#define NEG 17 ///<kod oznaczający polecenie NEG
#define SUB 18 ///<kod oznaczający polecenie SUB
#define IS_EQ 19 ///<kod oznaczający polecenie IS_EQ
#define DEG 20 ///<kod oznaczający polecenie DEG
#define DEG_BY 21 ///<kod oznaczający polecenie DEG_BY
#define AT 22 ///<kod oznaczający polecenie AT
#define PRINT 23 ///<kod oznaczający polecenie PRINT
#define POP 24 ///<kod oznaczający polecenie POP
#define UNRECOGNIZED 1 ///<kod mówiący o nierozpoznanym poleceniu
#define WRONG_VALUE 2 ///<kod mówiący o nieprawidłowej wartości argumentu funkcji AT
#define WRONG_VARIABLE 3 ///<kod mówiący o nieprawidłowej wartości indeksu zmiennej dla funkcji DEG_BY

typedef int rctype; ///<typ zmiennych liczących wiersze i kolumny
typedef long long poly_arg; ///<typ argumentu do poleceń


/**
 * Funkcja porównująca tablicę znaków z rozpoznawanymi poleceniami.
 * @param[in] s : instrukcja
 * @return kod instrukcji
 */
int getAction(char * s);

/**
 * Funkcja wczytująca współczynnik (liczbę) z wejścia standardowego.
 * Jeśli podczas wykonywania funkcji wystąpi błąd, error zostanie ustawiony na true.
 * Po zakończeniu na first przypisany zostaje ostatni wczytany znak.
 * @param first : ostatni wczytany znak przed wejściem do funkcji
 * @param error : zmienna informująca o wystąpieniu błędu przy wczytywaniu wielomianu
 * @param column : obecnie przetwarzana kolumna
 * @return współczynnik
 */
poly_coeff_t parseCoeff(char *first, bool *error, rctype * column);

/**
 * Funkcja wczytująca wykładnik jednomianu z wejścia standardowego.
 * Jeśli podczas wykonywania funkcji wystąpi błąd, error zostanie ustawiony na true.
 * Po zakończeniu na first przypisany zostaje ostatni wczytany znak.
 * @param first : ostatni wczytany znak przed wejściem do funkcji
 * @param error : zmienna informująca o wystąpieniu błędu przy wczytywaniu wielomianu
 * @param column : obecnie przetwarzana kolumna
 * @return wykładnik
 */
poly_exp_t parseExp(char *first, bool *error, rctype * column);

/**
 * Funkcja usuwająca z pamięci pomocniczą tablicę jednomianów.
 * @param count : liczba jednomianów
 * @param monos : tablica jednomianów
 */
void freeAuxArray(unsigned count, Mono * monos);

/**
 * Funkcja przetwarzająca wielomian.
 * Jeśli podczas wykonywania funkcji wystąpi błąd, error zostanie ustawiony na true.
 * Po zakończeniu na first przypisany zostaje ostatni wczytany znak.
 * @param first : ostatni wczytany znak przed wejściem do funkcji
 * @param error : zmienna informująca o wystąpieniu błędu przy wczytywaniu wielomianu
 * @param row : obecnie przetwarzany rząd
 * @param column : obecnie przetwarzana kolumna
 * @param req : informacja o tym, czy funkcja została wywołana przez siebie samą
 * @return wczytany wielomian
 */
Poly tryParsePoly(char *first, bool *error, rctype * row, rctype * column, bool req);

/**
 * Funkcja wczytująca argument polecenia AT
 * Jeśli podczas wykonywania funkcji wystąpi błąd, error zostanie ustawiony na true.
 * Po zakończeniu na first przypisany zostaje ostatni wczytany znak.
 * @param first : ostatni wczytany znak przed wejściem do funkcji
 * @param arg : argument polecenia AT
 * @param error : zmienna informująca o wystąpieniu błędu przy wczytywaniu wielomianu
 */
void parseArg(char *first, poly_arg *arg, bool *error);

/**
 * Funkcja wczytująca indeks zmiennej polecenia DEG_BY
 * Jeśli podczas wykonywania funkcji wystąpi błąd, error zostanie ustawiony na true.
 * Po zakończeniu na first przypisany zostaje ostatni wczytany znak.
 * @param first : ostatni wczytany znak przed wejściem do funkcji
 * @param arg : argument polecenia DEG_BY
 * @param error : zmienna informująca o wystąpieniu błędu przy wczytywaniu wielomianu
 */
void parseVar(char *first, poly_arg *arg, bool *error);

/**
 * Funkcja wczytująca polecenie z wejścia standardowego.
 * Jeśli podczas wykonywania funkcji wystąpi błąd, error zostanie ustawiony na true.
 * Po zakończeniu na first przypisany zostaje ostatni wczytany znak.
 * @param first : ostatni wczytany znak przed wejściem do funkcji
 * @param arg : argument do funkcji AT, DEG_BY
 * @param error : zmienna informująca o wystąpieniu błędu przy wczytywaniu wielomianu
 * @param row : obecnie przetwarzany rząd
 * @param column : obecnie przetwarzana kolumna
 * @return kod polecenia
 */
int parseCommand(char *first, poly_arg *arg, bool *error, rctype * row, rctype * column);

#endif //KALKULATOR_PARSE_H
