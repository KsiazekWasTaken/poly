/** @file
   Plik nagłówkowy do implementacji stosu.

   @author Piotr Książek
   @copyright Uniwersytet Warszawski
   @date 2017-04-24, 2017-05-26
*/

#ifndef KALKULATOR_STACK_H
#define KALKULATOR_STACK_H


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "poly.h"
#include <limits.h>

typedef Poly stack_el; ///< element stosu

/**
 * Element listy.
 */
typedef struct list_d{
    struct list_d * next; ///< wskaźnik na następny element
    stack_el x; ///< zawartość elementu listy
} list;

/**
 * Stos.
 */
typedef struct stack_d{
    list * first; ///<wskaźnik na pierwszy element listy
} stack;

/**
 * Funkcja inicjalizująca stos.
 * @param s : stos
 */
void initStack(stack *s);

/**
 * Funkcja sprawdzająca, czy stos jest pusty.
 * @param s : stos
 * @return Czy stos jest pusty?
 */
bool isEmpty(stack s);

/**
 * Funkcja wstawiająca element na stos.
 * @param s : stos
 * @param x : element
 */
void push(stack *s, stack_el x);

/**
 * Sprawdza pierwszy element stosu nie zdejmując go.
 * @param s : stos
 * @return pierwszy element stosu
 */
stack_el peek(stack s);

/**
 * Zwraca pierwszy element stosu i zdejmuje go.
 * @param s : stos
 * @return pierwszy element stosu
 */
stack_el pop(stack *s);

#endif //KALKULATOR_STACK_H
