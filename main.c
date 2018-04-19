/** @file
   Implementacja kalkulatora.

   @author Piotr Książek
   @copyright Uniwersytet Warszawski
   @date 2017-04-24, 2017-05-26
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "poly.h"
#include <limits.h>
#include "array.h"
#include "stack.h"
#include "parse.h"

typedef int rctype; ///<typ zmiennych liczących wiersze i kolumny
typedef long long poly_arg; ///<typ argumentu do poleceń

/**
 * Funkcja wykonuje polecenie o kodzie command i argumencie arg (jeśli potrzebny)
 * @param command : kod instrukcji
 * @param s : stos
 * @param arg : argument do funkcji AT, DEG_BY
 * @param row : obecnie przetwarzany rząd
 */
void executeCommand(int command, stack *s, poly_arg arg, rctype * row) {
    Poly aux;
    Poly auxTwo;
    Poly res;
    switch (command) {
        case ZERO:
            push(s, PolyZero());
            break;
        case IS_COEFF:
            if (isEmpty(*s)) {
                fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", *row-1);
            } else {
                aux = peek(*s);
                printf("%d\n", 1 * (PolyIsCoeff(&aux)));
            }
            break;
        case IS_ZERO:
            if (isEmpty(*s)) {
                fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", *row-1);
            } else {
                aux = peek(*s);
                printf("%d\n", 1 * PolyIsZero(&aux));
            }
            break;
        case CLONE:
            if (isEmpty(*s)) {
                fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", *row-1);
            } else {
                aux = peek(*s);
                auxTwo = PolyClone(&aux);
                push(s, auxTwo);
            }
            break;
        case ADD:
            if(isEmpty(*s)){
                fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", *row-1);
            }
            else {
                aux = pop(s);
                if (isEmpty(*s)) {
                    fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", *row-1);
                    push(s, aux);
                } else {
                    auxTwo = pop(s);
                    res = PolyAdd(&aux, &auxTwo);
                    PolyDestroy(&aux);
                    PolyDestroy(&auxTwo);
                    push(s, res);
                }
            }
            break;
        case MUL:
            if(isEmpty(*s)){
                fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", *row-1);
            }
            else {
                aux = pop(s);
                if (isEmpty(*s)) {
                    fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", *row-1);
                    push(s, aux);
                } else {
                    auxTwo = pop(s);
                    res = PolyMul(&aux, &auxTwo);
                    PolyDestroy(&aux);
                    PolyDestroy(&auxTwo);
                    push(s, res);
                }
            }
            break;
        case NEG:
            if(isEmpty(*s)){
                fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", *row-1);
            }
            else {
                aux = pop(s);
                res = PolyNeg(&aux);
                PolyDestroy(&aux);
                push(s, res);
            }
            break;
        case SUB:
            if(isEmpty(*s)){
                fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", *row-1);
            }
            else {
                aux = pop(s);
                if (isEmpty(*s)) {
                    fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", *row-1);
                    push(s, aux);
                } else {
                    auxTwo = pop(s);
                    res = PolySub(&aux, &auxTwo);
                    PolyDestroy(&aux);
                    PolyDestroy(&auxTwo);
                    push(s, res);
                }
            }
            break;
        case IS_EQ:
            if(isEmpty(*s)){
                fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", *row-1);
            }
            else {
                aux = pop(s);
                if (isEmpty(*s)) {
                    fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", *row-1);
                    push(s, aux);
                } else {
                    auxTwo = pop(s);
                    printf("%d\n", 1 * PolyIsEq(&aux, &auxTwo));
                    push(s, auxTwo);
                    push(s, aux);
                }
            }
            break;
        case DEG:
            if (isEmpty(*s)) {
                fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", *row-1);
            } else {
                aux = peek(*s);
                printf("%d\n", PolyDeg(&aux));
            }
            break;
        case DEG_BY:
            if (isEmpty(*s)) {
                fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", *row-1);
            } else {
                aux = peek(*s);
                printf("%d\n", PolyDegBy(&aux, arg));
            }
            break;
        case AT:
            if (isEmpty(*s)) {
                fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", *row-1);
            } else {
                aux = pop(s);
                auxTwo = PolyAt(&aux, arg);
                PolyDestroy(&aux);
                push(s, auxTwo);
            }
            break;
        case PRINT:
            if(!isEmpty(*s)) {
                aux = peek(*s);
                printPoly(&aux);
            }
            else{
                fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", *row-1);
            }
            break;
        case POP:
            if (isEmpty(*s)) {
                fprintf(stderr,"ERROR %d STACK UNDERFLOW\n", *row-1);
            } else {
                aux = pop(s);
                PolyDestroy(&aux);
            }
            break;
        default:;
    }
}

/**
 * Funkcja główna.
 */
void calculator(){
    int row = 1;
    int column = 1;
    stack s;
    Poly aux;
    poly_arg arg=0;
    initStack(&s);
    int command=0;
    char c = getchar();
    bool error=false;
    while(c!=EOF) {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')){
            command = parseCommand(&c, &arg, &error, &row, &column);
            if(!error) executeCommand(command, &s, arg, &row);
            else{
                while(c!='\n'){
                    c=getchar();
                    if(c=='\n'){
                        row+=1;
                        column=1;
                    }
                }
                column=1;
                switch(command){
                    case UNRECOGNIZED:
                        fprintf(stderr, "ERROR %d WRONG COMMAND\n", row-1);
                        break;
                    case WRONG_VALUE:
                        fprintf(stderr, "ERROR %d WRONG VALUE\n", row-1);
                        break;
                    case WRONG_VARIABLE:
                        fprintf(stderr, "ERROR %d WRONG VARIABLE\n", row-1);
                        break;
                }
                error = false;
            }
        }
        else {
            Poly q = tryParsePoly(&c, &error, &row, &column, false);
            if(!error){
                push(&s, q);
                if(c=='\n'){
                    row+=1;
                    column=1;
                }
            }
            else{
                fprintf(stderr, "ERROR %d %d\n", row, column);
                column=1;
                error = false;
                while(c!='\n'){
                    c=getchar();
                }
                if(c=='\n'){
                    row+=1;
                    column=1;
                }
            }
        }
        c=getchar();
    }
    while(!isEmpty(s)){
        aux = pop(&s);
        PolyDestroy(&aux);
    }
}

/**
 * Główna funkcja programu, uruchamia kalkulator.
 * @return exit status
 */
int main() {
    calculator();
    return 0;
}