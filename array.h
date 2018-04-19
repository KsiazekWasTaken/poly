/** @file
   Plik nagłówkowy do implementacji tablicy dynamicznej.

   @author Piotr Książek
   @copyright Uniwersytet Warszawski
   @date 2017-04-24, 2017-03-26
*/
//
// Created by pk371141 on 2017-03-26.
//

#ifndef included_ARRAY_H
#define included_ARRAY_H

#include <stdbool.h>
#include "poly.h"

/**
 * Struktura obsługująca dynamiczną tablicę.
 */
typedef struct Array_struct {
    Mono * array; ///<w tablicy przechowywane będą jednomiany
    int maxIndex; ///<maksymalny indeks przy obecnie zaalokowanej ilości pamięci
    int lastIndex;///<lastIndex zawsze trzyma indeks ostatniego wstawionego elementu
} ArrayH;

/**
 * Inicjalizuje tabicę dynamiczną.
 * @param H : tablica
 */
extern void init(ArrayH * H);

/**
 * Wstawia element x do tablciy H.
 * @param H : tablica
 * @param x : jednomian
 */
extern void append(ArrayH * H, Mono x);

/**
 * Usuwa tablicę z pamięci.
 * @param H : tablica
 */
extern void freeArray(ArrayH *H);

#endif //included_ARRAY_H
