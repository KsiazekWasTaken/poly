/** @file
   Interfejs klasy wielomianów - implementacja + funkcje pomocnicze

   @author Jakub Pawlewicz <pan@mimuw.edu.pl>, Piotr Książek
   @copyright Uniwersytet Warszawski
   @date 2017-04-24, 2017-05-13
*/

#include "poly.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

/**
 * Zlicza jednomiany zmiennej zerowego indeksu.
 * @param[in] p : wielomian
 * @return długość listy p.first
 */
static unsigned monoCount(Poly p){
    unsigned result=0;
    Mono * m;
    if(PolyIsCoeff(&p)){
        ;
    }
    else{
        m = p.first;
        while(m){
            result +=1;
            m=m->next;
        }
    }
    return result;
}

void PolyDestroy(Poly *p){
    Mono *aux;
    Mono *next;
    if (!PolyIsCoeff(p)) {
        aux = p->first;
        while (aux) {
            next = aux->next;
            PolyDestroy(&(aux->p));
            free(aux);
            aux=next;
        }
    }
}

Poly PolyClone(const Poly *p){
    Mono * auxP;
    Mono * auxR;
    Poly r;
    if(PolyIsCoeff(p)){
        r.coeff = p->coeff;
        r.first=NULL;
    }
    else{
      r.coeff=0;
        auxP=p->first;
        r.first=malloc(sizeof(Mono));
        assert(r.first!=NULL);
        auxR=r.first;
        auxR->exp=auxP->exp;
        auxR->p=PolyClone(&auxP->p);
        auxP=auxP->next;
        while(auxP){
            auxR->next=malloc(sizeof(Mono));
            assert(auxR->next!=NULL);
            auxR=auxR->next;
            auxR->exp=auxP->exp;
            auxR->p=PolyClone(&auxP->p);
            auxP=auxP->next;
        }
        auxR->next=NULL;
    }
    return r;
}

/**
 * Funkcja pomocnicza do dodawania.
 * Dodaje stałą do wielomianu.
 * Modyfikuje wielomian p.
 * @param[in] p : wielomian
 * @param[in] x : stała
 * @return 'p:=p+x'
 */
static void AddConst(Poly *p, poly_coeff_t x){
    Mono * aux;
    if(PolyIsCoeff(p)){
        p->coeff+=x;
    }
    else if(p->first->exp!=0){
        aux=p->first;
        p->first=malloc(sizeof(Mono));
        assert(p->first!=NULL);
        p->first->next=aux;
        p->first->exp=0;
        p->first->p.coeff=x;
        p->first->p.first=NULL;
    }
    else{
        AddConst(&(p->first->p),x);
        if(PolyIsZero(&(p->first->p))){
            if(p->first->next){
                aux=p->first;
                p->first=aux->next;
                free(aux);
            }
            else{
                p->coeff=0;
                free(p->first);
                p->first=NULL;
            }
        }
        else if(PolyIsCoeff(&(p->first->p))&&p->first->exp==0&&p->first->next==NULL){
            p->coeff=p->first->p.coeff;
            free(p->first);
            p->first=NULL;

        }
    }
}

/**
 * Wewnętrzna funkcja pomocniczna wypisująca wielomian na wyjście standardowe.
 * @param p : wielomian do wypisania
 */
static void printAux(const Poly *p){
    if(PolyIsCoeff(p)){
        printf("%ld", p->coeff);
    }
    else{
        Mono *iterator = p->first;
        while(iterator){
            printf("(");
            printAux(&iterator->p);
            printf(",%d)", iterator->exp);
            if(iterator->next) printf("+");
            iterator=iterator->next;
        }
    }
}

void printPoly(const Poly *p){
    printAux(p);
    printf("\n");
}

Poly PolyAdd(const Poly *p, const Poly *q){
    Poly r;
    if(PolyIsCoeff(p) && PolyIsCoeff(q)){
        r.coeff = (poly_coeff_t)p->coeff + (poly_coeff_t)q->coeff;
        r.first = NULL;
    }
    if(PolyIsCoeff(p)){
        r = PolyClone(q);
        if(!PolyIsZero(p)){
            AddConst(&r,p->coeff);
        }
    }
    else if(PolyIsCoeff(q)){
        r = PolyClone(p);
        if(!PolyIsZero(q)){
            AddConst(&r,q->coeff);
        }
    }
    else{
        Mono * m;
        Mono * dummy = malloc(sizeof(Mono));
        assert(dummy!=NULL);
        m=dummy;
        Poly sum;
        Mono * aux;
        Mono * iteratorP = p->first;
        Mono * iteratorQ = q->first;
        while(iteratorP&&iteratorQ){
            if(iteratorP->exp > iteratorQ->exp){
                m->next = malloc(sizeof(Mono));
                assert(m->next!=NULL);
                m=m->next;
                m->p=PolyClone(&iteratorQ->p);
                m->exp=iteratorQ->exp;
                iteratorQ = iteratorQ->next;
            }
            else if(iteratorP->exp < iteratorQ->exp){
                m->next = malloc(sizeof(Mono));
                assert(m->next!=NULL);
                m=m->next;
                m->p=PolyClone(&iteratorP->p);
                m->exp=iteratorP->exp;
                iteratorP = iteratorP->next;
            }
            else{
                sum = PolyAdd(&iteratorP->p, &iteratorQ->p);
                if(!PolyIsCoeff(&sum)){
                    if(PolyIsZero(&(sum.first->p))){
                        if(sum.first->next){
                            aux=sum.first;
                            sum.first=aux->next;
                            free(aux);
                        }
                        else{
                            sum.coeff=0;
                            free(sum.first);
                            sum.first=NULL;
                        }
                    }
                    else if(PolyIsCoeff(&(sum.first->p))&&sum.first->exp==0&&sum.first->next==NULL){
                        sum.coeff=sum.first->p.coeff;
                        free(sum.first);
                        sum.first=NULL;
                    }
                }
                if(!PolyIsZero(&sum)){
                    m->next = malloc(sizeof(Mono));
                    assert(m->next!=NULL);
                    m=m->next;
                    m->exp=iteratorP->exp;
                    m->p = sum;
                }
                iteratorP = iteratorP->next;
                iteratorQ = iteratorQ->next;
            }
        }
        if(iteratorP){
            while(iteratorP){
                m->next = malloc(sizeof(Mono));
                assert(m->next!=NULL);
                m=m->next;
                m->p=PolyClone(&iteratorP->p);
                m->exp=iteratorP->exp;
                iteratorP=iteratorP->next;
            }
        }
        if(iteratorQ){
            while(iteratorQ){
                m->next = malloc(sizeof(Mono));
                assert(m->next!=NULL);
                m=m->next;
                m->p=PolyClone(&iteratorQ->p);
                m->exp=iteratorQ->exp;
                iteratorQ=iteratorQ->next;
            }
        }
        m->next=NULL;
        r.first=dummy->next;
        if(r.first==NULL) r.coeff=0;
        else if(r.first->next==NULL && PolyIsCoeff(&r.first->p) && r.first->exp==0){
            r.coeff=r.first->p.coeff;
            free(r.first);
            r.first=NULL;
        }
        free(dummy);
    }
    return r;
}

/**
 * Funkcja porównująca do qsort.
 * @param[in] a : pierwszy wykładnik
 * @param[in] b : drugi wykładnik
 * @return -1 iff a<b, 0 iff a=b, 1 iff a>b
 */
static int expComp(const void * a, const void * b){
    Mono m = *(Mono*)a;
    Mono n = *(Mono*)b;
    int x = m.exp;
    int y = n.exp;
    if(x < y) return -1;
    else if(x == y) return 0;
    else return 1;
}

/**
 * Przechodzi po pierwszym poziomie wielomianu.
 * Wykonuje ostatni etap upraszczania.
 * @param[in] p : wielomian
 * return 'znormalizowany p' 
 */
static void PolyNormalizeNonReq(Poly *p){
    if(PolyIsCoeff(p)){
        /*jest dobrze!*/;}
    else{
        Mono * dummy = malloc(sizeof(Mono));
        assert(dummy != NULL);
        dummy->next = p->first;
        Mono * aux = p->first;
        Mono * auxLast = dummy;
        while(aux){
            if(PolyIsZero(&aux->p)){
                auxLast->next=aux->next;
                PolyDestroy(&aux->p);
                free(aux);
                aux = auxLast->next;
            }
            else{
                auxLast = aux;
                aux = aux->next;
            }
        }
        p->first=dummy->next;
        free(dummy);
        if(p->first){
            aux = p->first;
            if(aux->next == NULL && PolyIsCoeff(&aux->p) && aux->exp == 0){
                p->coeff = (aux->p.coeff);
                free(aux);
                p->first = NULL;
            }
        }
        else p->coeff = 0;
    }
}

Poly PolyAddMonos(unsigned count, const Mono monos[]){
    Poly r;
    Mono * auxR;
    Poly sum;
    unsigned n = count;
    unsigned i;
    Mono * tab = malloc(sizeof(Mono)*count);
    assert(tab != NULL);
    memcpy(tab,monos,count*sizeof(Mono));
    qsort(tab, n, sizeof(Mono), expComp);
    if(n == 0);
    else{
        i = 0;
        r.first = malloc(sizeof(Mono));
        assert(r.first != NULL);
        r.first->p = tab[i].p;
        r.first->exp = tab[i].exp;
        auxR = r.first;
        for(i = 1;i<n;i++){
            if(auxR->exp == tab[i].exp){
                sum = PolyAdd(&auxR->p,&tab[i].p);
                PolyDestroy(&tab[i].p);
                PolyDestroy(&auxR->p);
                auxR->p = sum;
            }
            else{
                auxR->next = malloc(sizeof(Mono));
                assert(auxR->next!=NULL);
                auxR = auxR->next;
                auxR->p = tab[i].p;
                auxR->exp = tab[i].exp;
            }
        }
        auxR->next = NULL;
        if(r.first == NULL) r.coeff = 0;
        else if(r.first->next == NULL && PolyIsCoeff(&r.first->p) && r.first->exp == 0){
            r.coeff = r.first->p.coeff;
            free(r.first);
            r.first = NULL;
        }
        PolyNormalizeNonReq(&r);
    }
    free(tab);
    return r;
}

Poly PolyMul(const Poly *p, const Poly *q){
    Poly r;
    unsigned lengthP;
    unsigned lengthQ;
    unsigned i;
    unsigned j;
    unsigned lengthR;
    Mono * iteratorP;
    Mono * iteratorQ;
    if(PolyIsZero(p)||PolyIsZero(q)){
        r.first = NULL;
        r.coeff = 0;
    }
    else if(PolyIsCoeff(p)&&PolyIsCoeff(q)){
        /*wynikiem jest iloczyn obu współczynników*/
        r.first = NULL;
        r.coeff = p->coeff*q->coeff;
    }
    else if(PolyIsCoeff(p)){
        /*wynikiem jest suma iloczynów współczynnika z jednomianami q*/
        lengthQ = monoCount(*q);
        iteratorQ = q->first;
        Mono mns[lengthQ];
        for(i = 0;i<lengthQ;i++){
            mns[i].p = PolyMul(p,&iteratorQ->p);
            mns[i].exp = iteratorQ->exp;
            iteratorQ = iteratorQ->next;
        }
        r = PolyAddMonos(lengthQ, mns);

    }
    else if(PolyIsCoeff(q)){
        /*wynikiem jest suma iloczynów współczynnika z jednomianami p*/
        lengthP = monoCount(*p);
        iteratorP = p->first;
        Mono mns[lengthP];
        for(i = 0;i<lengthP;i++){
            mns[i].p = PolyMul(q,&iteratorP->p);
            mns[i].exp = iteratorP->exp;
            iteratorP = iteratorP->next;
        }
        r = PolyAddMonos(lengthP, mns);
    }
    else{
        /*iloczyn dwóch wielomianów, które nie są współczynnikami*/
        lengthP = monoCount(*p);
        lengthQ = monoCount(*q);
        lengthR = lengthP*lengthQ; /*tyle maksymalnie będzie współczynników*/
        Mono * mns = malloc(lengthR*sizeof(Mono));
        assert(mns!=NULL);
        iteratorP = p->first;
        iteratorQ = q->first;
        for(i = 0;i<lengthP;i++){
            iteratorQ = q->first;
            for(j = 0;j<lengthQ;j++){
                mns[i*(lengthQ)+j].p = PolyMul(&iteratorP->p,&iteratorQ->p);
                mns[i*(lengthQ)+j].exp = iteratorP->exp+iteratorQ->exp;
                iteratorQ = iteratorQ->next;
            }
            iteratorP = iteratorP->next;
        }
        r = PolyAddMonos(lengthR,mns);
        free(mns);
    }
    return r;
}

/**
 * Modyfikuje wielomian.
 * Nie wykorzystuje mnożenia.
 * @param[in] p : wielomian
 * @return '-p'
 */
static void NegAux(Poly *p){
    if(PolyIsCoeff(p)){
        p->first = NULL;
        p->coeff = p->coeff*(-1);
    } else {
        Mono * itR = p->first; //iterator po R
        while(itR){
            NegAux(&itR->p);
            itR = itR->next;
        }   
    }
}

Poly PolyNeg(const Poly *p){
    Poly r; //result
    if(PolyIsCoeff(p)){
        r.first = NULL;
        r.coeff = p->coeff*(-1);
    }
    else{
        r = PolyClone(p);
        NegAux(&r);
    }
    return r;
}

Poly PolySub(const Poly *p, const Poly *q){
    Poly q1 = PolyNeg(q);
    Poly r = PolyAdd(p,&q1);
    PolyDestroy(&q1);
    return r;
}

/**
 * Funkcja pomocnicza do PolyDeg i PolyDegBy.
 * @param[in] a : pierwszy wykładnik
 * @param[in] b : drugi wykładnik
 * @return a iff a>b, else b
 */
static poly_exp_t MaxDeg(poly_exp_t a, poly_exp_t b){
    if(a>b) return a;
    else return b;
}

poly_exp_t PolyDegBy(const Poly *p, unsigned var_idx){
    poly_exp_t deg = 0; //tu będzie trzymany wynik
    if(PolyIsZero(p)) deg=-1;
    else if(var_idx ==0){
        Mono * iterator = p->first;
        while(iterator){
            deg = MaxDeg(iterator->exp, deg);
            iterator = iterator->next;
        }
    }
    else if(var_idx>0){
        Mono * iterator = p->first;
        while(iterator){
            deg = MaxDeg(PolyDegBy(&iterator->p,var_idx-1), deg);
            iterator = iterator->next;
        }
    }
    return deg;
}

poly_exp_t PolyDeg(const Poly *p){
    poly_exp_t deg = 0; //tu będzie trzymany wynik
    if(PolyIsCoeff(p)){
        if(PolyIsZero(p)) deg = -1;
        else deg = 0;
    }
    else{
        Mono * iterator = p->first;
        deg = PolyDeg(&iterator->p)+iterator->exp;
        iterator = iterator->next;
        while(iterator){
            deg = MaxDeg(PolyDeg(&iterator->p)+iterator->exp,deg);
            iterator = iterator->next;
        }
    }
    return deg;
}

bool PolyIsEq(const Poly *p, const Poly *q){
    if(PolyIsCoeff(p)&&PolyIsCoeff(q)){
        return (p->coeff == q->coeff);
    }
    else if(!PolyIsCoeff(p)&&!PolyIsCoeff(q)){
        Mono * itP = p->first; //iterator po P
        Mono * itQ = q->first; //iterator po Q
        bool res = true;
        while(res&&itP&&itQ){
            res = res&&(itP->exp == itQ->exp);
            res = res&&(PolyIsEq(&itP->p,&itQ->p));
            itP = itP->next;
            itQ = itQ->next;
        }
        if((!itP&&!itQ)&&res) return true;
        else return false;
    }
    else return false;
}

/**
 * Funkcja szybkiego potęgowania.
 * Implementacja pochodzi ze Stack Overflow:
 * http://stackoverflow.com/questions/101439/the-most-efficient-way-to-implement-an-integer-based-power-function-powint-int
 * @param[in] base : podstawa
 * @param[in] exp : wykładnik
 * @return podstawa^wykładnik
 */
static poly_coeff_t ipow(poly_coeff_t base, poly_exp_t exp){
    long long result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }

    return result;
}

Poly PolyAt(const Poly *p, poly_coeff_t x){
    Poly r;  //result
    if(PolyIsCoeff(p)) r = PolyClone(p);
    else{
        Poly swapAux;
        Poly coeff;
        Poly aux;
        Mono * mainIterator = p->first;
        coeff = PolyFromCoeff(ipow(x,(mainIterator->exp)));
        r = PolyMul(&mainIterator->p, &coeff);
        mainIterator=mainIterator->next;
        while(mainIterator){
            coeff = PolyFromCoeff(ipow(x,(mainIterator->exp)));
            aux = PolyMul(&mainIterator->p, &coeff);
            swapAux = PolyAdd(&r, &aux);
            PolyDestroy(&r);
            PolyDestroy(&aux);
            r = swapAux;
            mainIterator = mainIterator->next;
        }
    }
    return r;
}