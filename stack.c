/** @file
   Implementacja listowa stosu.

   @author Piotr Książek
   @copyright Uniwersytet Warszawski
   @date 2017-04-24, 2017-05-26
*/

#include "stack.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "poly.h"
#include <limits.h>
#include <assert.h>


void initStack(stack *s){
    s->first = NULL;
}

bool isEmpty(stack s){
    return s.first == NULL;
}

void push(stack *s, stack_el x){
    list * l = malloc(sizeof(list));
    assert(l != NULL);
    l->x = x;
    l->next = s->first;
    s->first = l;
}

stack_el peek(stack s){
    return s.first->x;
}

stack_el pop(stack *s){
    list * leak;
    stack_el ret;
    if(!isEmpty(*s)) {
        ret = s->first->x;
        leak = s->first;
        s->first = s->first->next;
        free(leak);
    }
    return ret;
}