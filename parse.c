/** @file
   Parser wielomianów + funkcja odczytująca kod polecenia - implementacje

   @author Piotr Książek
   @copyright Uniwersytet Warszawski
   @date 2017-04-24, 2017-05-26
*/
//
// Created by HyperWorks on 2017-05-27.
//

#include "parse.h"
#include "poly.h"
#include <stdio.h>
#include "array.h"
#include <limits.h>
#include <string.h>


int getAction(char * s){
    if(strcmp(s,"ZERO")==0) return ZERO;
    if(strcmp(s,"IS_COEFF")==0) return IS_COEFF;
    if(strcmp(s,"IS_ZERO")==0) return IS_ZERO;
    if(strcmp(s,"CLONE")==0) return CLONE;
    if(strcmp(s,"ADD")==0) return ADD;
    if(strcmp(s,"MUL")==0) return MUL;
    if(strcmp(s,"NEG")==0) return NEG;
    if(strcmp(s,"SUB")==0) return SUB;
    if(strcmp(s,"IS_EQ")==0) return IS_EQ;
    if(strcmp(s,"DEG")==0) return DEG;
    if(strcmp(s,"AT")==0) return AT;
    if(strcmp(s,"PRINT")==0) return PRINT;
    if(strcmp(s,"POP")==0) return POP;
    if(strcmp(s,"DEG_BY")==0) return DEG_BY;
    return UNRECOGNIZED;
}

poly_coeff_t parseCoeff(char *first, bool *error, rctype * column){
    long long idx = 0;
    bool ujemne = false;
    char c = *first;
    if(c=='-'){
        ujemne = true;
        c = getchar();
        *column += 1;
    }
    if(!ujemne) {
        while (c >= '0' && c <= '9' && (c - '0') <= (LONG_MAX - idx * 10)) {
            idx = 10 * idx + c - '0';
            c = getchar();
            *column += 1;
        }
    }
    else{
        while (c >= '0' && c <= '9' && (-c + '0') >= (LONG_MIN - idx * 10)) {
            if (idx > 0) {
                idx = (-1) * (10 * idx + c - '0');
            } else idx = 10 * idx - c + '0';
            c = getchar();
            *column += 1;
        }
    }
    if(c >= '0' && c <= '9' ){
        *error = true;
    }
    *first = c;
    return idx;
}

poly_exp_t parseExp(char *first, bool *error, rctype * column){
    char c = *first;
    poly_coeff_t x = 0;
    bool anythingAtAll=false;
    if(c >= '0' && c <= '9' && !*error ) {
        while (c >= '0' && c <= '9' && !*error && c - '0' <= INT_MAX - 10 * x) {
            x = 10 * x + c - '0';
            c = getchar();
            *column += 1;
            anythingAtAll = true;
        }
    }
    else if( c == '-' && !*error){
        c = getchar();
        *column += 1;
        anythingAtAll=true;
        while(c == '0') {
            c = getchar();
            column += 1;
        }
    }
    if( c>='0' && c<='9'){
        *error = true;
    }
    if(c == '\n' || !anythingAtAll){
        *error = true;
    }
    *first = c;
    return x;
}

void freeAuxArray(unsigned count, Mono * monos){
    unsigned i;
    for(i = 0;i < count;i++){
        MonoDestroy(&monos[i]);
    }
}

Poly tryParsePoly(char *first, bool *error, rctype * row, rctype * column, bool req){
    char c = *first;
    ArrayH T;
    init(&T);
    Poly p;
    poly_coeff_t x;
    poly_exp_t e;
    if( ((c >= '0' && c <= '9') || c == '-' )&& !*error){
        x = parseCoeff(&c, error, column);
        if(c!='\n' && !req){
            *error = true;
        }
        else if(c!=',' && req){
            *error = true;
        }
        else{
            *first = c;
            return PolyFromCoeff(x);
        }
    }
    else if(c == '(' && !*error){
        req = true;
        do {
            if(c == '+' && !*error){
                c = getchar();
                *column += 1;
            }
            if( c == '('  && !*error) {
                c = getchar();
                *column += 1;
                p = tryParsePoly(&c, error, row, column, req);
                if (c == ','  && !*error){
                    c = getchar();
                    *column += 1;
                    e = parseExp(&c, error, column);
                    if (c == ')'  && !*error){
                        c = getchar();
                        *column += 1;
                        append(&T, MonoFromPoly(&p, e));
                    }
                    else{
                        *error = true;
                        PolyDestroy(&p);
                    }
                }
                else{
                    *error = true;
                    PolyDestroy(&p);
                }
            }
            else{
                *error = true;
            }
        } while (c == '+' && !*error);
        if(c!='\n' && c!=','){
            *error=true;
        }
        *first = c;
        if(!*error){
            p = PolyAddMonos(T.lastIndex+1,T.array);
            freeArray(&T);
            return p;
        }
        else{
            freeAuxArray(T.lastIndex+1, T.array);
            freeArray(&T);
            return PolyZero();
        }
    }
    else{
        *error = true;
    }
    *first = c;
    freeArray(&T);
    return PolyZero();
}

void parseArg(char *first, poly_arg *arg, bool *error){
    long long idx=0;
    bool ujemne = false;
    char c = *first;
    if(c=='-'){
        ujemne = true;
        c = getchar();
    }
    if(!ujemne) {
        while (c >= '0' && c <= '9' && (c - '0') <= (LONG_MAX - idx * 10)) {
            idx = 10 * idx + c - '0';
            c = getchar();
        }
    }
    else{
        while (c >= '0' && c <= '9' && (-c + '0') >= (LONG_MIN - idx * 10)) {
            if (idx > 0) {
                idx = (-1) * (10 * idx + c - '0');
            } else idx = 10 * idx - c + '0';
            c = getchar();
        }
    }
    if(c >= '0' && c <= '9' ){
        *error = true;
    }
    if(!ujemne) *arg = idx;
    else *arg = idx;
    *first = c;
}

void parseVar(char *first, poly_arg *arg, bool *error){
    long long idx = 0;
    char c = *first;
    if(c >= '0' && c <= '9') {
        while (c >= '0' && c <= '9' && (c - '0') <= (UINT_MAX - idx * 10)) {
            idx = 10 * idx + c - '0';
            c = getchar();
        }
    }
    else if(c == '-'){
        c = getchar();
        while(c == '0')
            c = getchar();
    }
    if(c >= '0' && c <= '9' ){
        *error = true;
    }
    *arg = idx;
    *first = c;
}

int parseCommand(char *first, poly_arg *arg, bool *error, rctype * row, rctype * column){
    char temporaryString[25]; //żadne polecenie nie jest dłuższe niż 24 znaki (nie ogranicza to długości argumentu)
    int temporaryStringIndex = 0;
    int res;
    temporaryString[temporaryStringIndex++]=*first;
    char c = getchar();
    while(((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '_')) && temporaryStringIndex<24){
        temporaryString[temporaryStringIndex++] = c;
        c = getchar();
        *column += 1;
    }
    if(temporaryStringIndex == 24){
        *error = true;
    }
    temporaryString[temporaryStringIndex] = '\0' ;
    res = getAction(temporaryString);
    if(c == ' ' && (res== AT) && !*error){
        c = getchar();
        if((c >= '0' && c <= '9') || c == '-') {
            parseArg(&c, arg, error);
            if (*error) res = WRONG_VALUE;
        }
        else{
            *error = true;
            res = WRONG_VALUE;
        }
    }
    else if(c ==' ' && (res == DEG_BY) && !*error){
        c=getchar();
        if((c>='0' && c <='9') || c == '-') {
            parseVar(&c, arg, error);
            if (*error) res = WRONG_VARIABLE;
        }
        else{
            *error = true;
            res = WRONG_VARIABLE;
        }
    }
    else if(res== AT){
        *error = true;
        res = WRONG_VALUE;
    }
    else if(res== DEG_BY){
        *error = true;
        res = WRONG_VARIABLE;
    }
    if(c!='\n'){
        if(!*error) {
            *error = true;
            switch (res){
                case AT:
                    res = WRONG_VALUE;
                    break;
                case DEG_BY:
                    res = WRONG_VARIABLE;
                    break;
                default:
                    res = UNRECOGNIZED;
                    break;
            }
        }
    }
    else{
        *row += 1;
        *column = 1;
    }
    *first=c;
    if(res == 1) *error = true;
    return res;
}