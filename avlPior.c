#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

struct tnode* new_tnode(int v);
void tInsert(struct tnode **w, struct tnode *f, struct tnode **R);

void tPrintf(struct tnode *w);
void tPrintfH(struct tnode *w);

int max(int a, int b);
void limpaMemoria(struct tnode **w);
int altura(struct tnode **w, struct tnode **R);
void balancear(struct tnode **w, int a, struct tnode **R);
int percorre(struct tnode *w, int n);

void rD(struct tnode **x, struct tnode **R);
void rE(struct tnode **x, struct tnode **R);

int buscaChave(struct tnode *w, int k);

struct tnode{
    int v;
    int h;
    
    struct tnode *p;
    struct tnode *l;
    struct tnode *r;
};

int main(){

    double t=0;
    struct timeval a,b;
    srand(time(NULL));

    for (int i=100;i<=5000;i+=100){
        double t=0; 
        struct tnode *R = NULL;

        //Preenche vetor 
		for (int j = 1; j<i; j++){
			tInsert(&R,new_tnode(i*2),&R);
        }

        for (int j=1; j<=10000;j++){

            gettimeofday(&a,NULL);
            buscaChave(R, rand()*2-1);
            gettimeofday(&b,NULL);
            t+= (b.tv_sec*1000000+b.tv_usec) - (a.tv_sec*1000000+a.tv_usec);    
        }
        printf("%d %f\n", i, t/10000);
   
        limpaMemoria(&R);
    }


    return 0;
}

int buscaChave(struct tnode *w, int k){
    if (w != NULL){
        if (w->v == k){
            return k;
        }
        if (w->v > k){
            return buscaChave(w->l,k);
        }
        else{
            return buscaChave(w->r,k);
        }
    }
    
    return -1;
}


void tPrintfH(struct tnode *w){

    if (w!=NULL){
        tPrintfH(w->l);
        
        tPrintfH(w->r);
        printf("%d\n",w->h);
    }
}

void limpaMemoria(struct tnode **w){

    if (*w!=NULL){
       limpaMemoria(&((*w)->l));
       limpaMemoria(&((*w)->r));
       free((*w));
    }
}

struct tnode* new_tnode(int v){

    struct tnode *w = (struct tnode*)malloc(sizeof(struct tnode));
    w ->v = v;
    w ->h = 0;
    w->p = NULL;
    w ->l = NULL;
    w ->r = NULL;
    return w;
}

void tInsert(struct tnode **w, struct tnode *f, struct tnode **R){

    if (*w == NULL){
        *w = f;
    } 

    else{
        f->p = *w;
        if (f->v < (*w)->v){
            tInsert(&((*w)->l), f, &(*R));
        }
        else{
            tInsert(&((*w)->r), f, &(*R));
        }
    }
    (*w)->h = altura(&(*w), &(*R));
    
}

int altura(struct tnode **w, struct tnode **R){

    if (*w == NULL){
        return 0;
    }    

    else{
        int h_e = altura(&(*w)->l, &(*R));
        int h_d = altura(&(*w)->r, &(*R)); 
        if (((h_d-h_e) < (-1) ) || ((h_d-h_e) > 1)){ 
            balancear(&(*w),h_d-h_e, &(*R));
            return altura(&(*w), &(*R));
        }
        return max(h_e,h_d)+1;  
    }
}

void balancear(struct tnode **w, int a, struct tnode **R){
    int giro;

    if (a > 1){
        int h_e = altura(&(*w)->r->l, &(*R));
        int h_d = altura(&(*w)->r->r, &(*R));
    
        giro = h_d - h_e;

        if (giro >=0){
            rE(&(*w), &(*R));
        }
    
        else{
            rD(&((*w)->r), &(*R));
            rE(&(*w), &(*R));
        }
    }

    else{
        int h_e = altura(&(*w)->l->l, &(*R));
        int h_d = altura(&(*w)->l->r, &(*R));
    
        giro = h_d - h_e;

        if (giro <=0){
            rD(&(*w), &(*R));
        }

        else{
            rE(&((*w)->l), &(*R));
            rD(&(*w), &(*R));
        }
    }   
}

void rD(struct tnode **x, struct tnode **R){

    struct tnode *y = (struct tnode*)malloc(sizeof(struct tnode));
    struct tnode *c = (struct tnode*)malloc(sizeof(struct tnode));
    y = (*x)->l;
    c = y->r;

    (*x)->l = c;
    y->r = (*x);
    y->p = (*x)->p;
    (*x)->p = y;

    if (c != NULL){
        c->p = (*x);
    }

    if( y->p != NULL){
        if(y->p->v > ((*x)->v)){
            y->p->l = y;
        }
        else{
            y->p->r = y;
        }
    }
    else{
        *R = y;
    }
}

void rE(struct tnode **x, struct tnode **R){

    struct tnode *y = (struct tnode*)malloc(sizeof(struct tnode));
    struct tnode *c = (struct tnode*)malloc(sizeof(struct tnode));
    y = (*x)->r;
    c = y->l;

    (*x)->r = c;
    y->l = (*x);
    y->p = (*x)->p;
    (*x)->p = y;

    if (c != NULL){
        c->p = (*x);
    }

    if( y->p != NULL){
        if(y->p->v > ((*x)->v)){
            y->p->l = y;
        } 
        else{
            y->p->r = y;
        }
    }   
    else{
        *R = y;
    }
}

int max(int a, int b){

    if (a > b){
       return a;
    }

    return b;
}

void tPrintf(struct tnode *w){

    if (w!=NULL){
        tPrintf(w->l);
        printf("%d\n",w->v);
        tPrintf(w->r);
    }
}

