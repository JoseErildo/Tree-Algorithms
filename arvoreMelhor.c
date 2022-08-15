#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

struct tnode* new_tnode(int v);
void tInsert(struct tnode **w, struct tnode *f,struct tnode **R);
void tPrintf(struct tnode *w);
void tPrintfH(struct tnode *w);

int max(int a, int b);
void limpaMemoria(struct tnode **w);
int tamanho(struct tnode **w,struct tnode **R);

int buscaChave(struct tnode *w, int k);

void melhorCaso(struct tnode **R, int s, int e,struct tnode **T);

void balancear(struct tnode **w, int a, struct tnode **R);
void rD(struct tnode **x, struct tnode **R);
void rE(struct tnode **x, struct tnode **R);

struct tnode{   //estrutura da arvore
    int v;  //chave armazenada
    int h;  //altura da arvore

    struct tnode *p;    //ponteiro para estrutura pai;
    struct tnode *l;    //ponteiro para a estrutura do filho à esquerda;
    struct tnode *r;    //ponteiro para a estrutura do filho à direita;
};

int main(){

    struct timeval a,b;//armazenam o tempo de execução
    srand(time(NULL));

    //*Trecho responsável pelo cálculo do tempo*;

    //laço responsável por criar estruturas que irão de 100 à 5000; tamanho +100 a cada loop;
    for (int i=100;i<=5000;i+=100){
        double t=0; //variável responsável por somar o tempo, nestre trecho ele é zerado para iniciar um novo cálculo de um vetor maior;

        struct tnode *R = NULL; //Cria um ponteiro para a variável R do tipo tnode recebendo NULL;

        //laço responsável pela inserção de valores; 
        for (int j=0;j<i;j++){ //estrutura criada de acordo com o tamanho da variável 'i';
            tInsert(&R, new_tnode(j),&R);   //chamada da função de inserção, *é passado o endereço da Raíz/o valor a ser inserido dentro da chamada para a função tnode/ e o endereço da Raíz*;
        }

        //laço responsável pelo cálculo do tempo de execução;
        for(int j =0; j<5000;j++){
            gettimeofday(&a,NULL);  //calcula o tempo decorrido no programa até a chamada dessa função;

            buscaChave(R, R->v);    //chamada para a função de busca *recebe conteúdo de R e valor da raíz*;

            gettimeofday(&b,NULL);  //calcula o tempo decorrido no programa desde a chamada da função gettime anterior, até está;

            //somatório dos tempos sendo armazenado em t;
            t+= (b.tv_sec*1000000+b.tv_usec) - (a.tv_sec*1000000+a.tv_usec);   
        }      

        //exibe o tamanho da estrutura percorridca e a media de seu tempo;
        printf("%d %f\n", i, t/5000);

        //chamada para a função que libera espaço da árvore;
        limpaMemoria(&R);   
    }
    return 0;
}

//Função Para Buscar a Chave
int buscaChave(struct tnode *w, int k){

    //Verifica se o nó atual é NULL
    if (w != NULL){
        if (w->v == k){ //Verifica se o nó contém a chave
            return k;
        }
        //Se o nó atual não conter a chave, dependendo do tamanho dela, será percorrido o lado esquerdo/direito da arvore.
        if (w->v > k){
            buscaChave(w->l,k); //busca a chave no lado esquerdo da arvore.
        }
        else{
            buscaChave(w->r,k); //busca a chave no lado direito da arvore.
        }
    }
}

//Função Responsável por Exibir a altura dos nós da Arvore em ordem Crescente.
void tPrintfH(struct tnode *w){

    if (w!=NULL){   //Se houver elemento no nó.

        tPrintfH(w->l);//percorra o lado esquerdo.
        tPrintfH(w->r); //percorra o lado direito.

        printf("%d\n",w->h);   //exiba a altura do nó atual;
    }
}

//Função para liberar memória na arvore;
void limpaMemoria(struct tnode **w){

    if (*w!=NULL){  //Se houver conteúdo;
       limpaMemoria(&((*w)->l));    //percorre o lado esquerdo;
       limpaMemoria(&((*w)->r));    //percorre o lado direito;

       free((*w));  //libera memória na linha atual;
    }
}

//Função para criar um novo nó;
struct tnode* new_tnode(int v){ //valor a ser inserido no nó

    struct tnode *w = (struct tnode*)malloc(sizeof(struct tnode));  //Alocação de memória do tamanho tnode para 'w'

    //inserção de atributos na estrutura de w;
    w ->v = v;
    w ->h = 0;
    w ->l = NULL;
    w ->r = NULL;
    w ->p = NULL;

    return w;   //retorna a estrutura criada;
}

//Função para definir a altura da arvore;
int tamanho(struct tnode **w, struct tnode **R){    

    if (*w == NULL){ //Caso não exista conteúdo em w, retorna o valor 0;
        return 0;
    }    

    else{
        int h_e = tamanho(&(*w)->l, &(*R)); //verificar a altura do nó à esquerda;
        int h_d = tamanho(&(*w)->r, &(*R)); //verificar a altura do nó à direita;

        //****************************************************************************************************************************************
        //Essa parte é reponsável pelo balanceamento e foi utilizada apenas no Melhor Caso da A.Desbalanceada para gerar uma estrutura balanceada;

        if (((h_d-h_e) < (-1) ) || ((h_d-h_e) > 1)){ //Se não obedecer a regra de balanceamento, deve ser balanceado;
            
            balancear(&(*w),h_d-h_e, &(*R)); //Chamada para balancear; *passa-se o endereço do nó alturar/o valor da diferença entre hd e he/e o endereço da raíz*
            return tamanho(&(*w), &(*R));   //retorna o tamanho da nova arvore balanceada;
        //****************************************************************************************************************************************
        }
        return max(h_e,h_d)+1;  //retorna a altura maior entre h_e/h_d +1;
    }
}

//Função de Inserção 
void tInsert(struct tnode **w, struct tnode *f, struct tnode **R){   //*endereço da estrutura/nó criado em lNode/endereço da raíz*;

    if (*w == NULL){  //Se não houver conteúdo, w recebe a estrutura de f;
        *w = f;
    } 

    else{
        f->p = *w;  //Se não, o pai de f recebe o conteúdo de w;

        //Verifica se a inserção do nó deve ser à esquerda ou direita de w;
        if (f->v < (*w)->v){    //Se o valor de f gor menor que o valor e w;

            tInsert(&((*w)->l), f,&(*R));  //verifica o lado esquerdo da arvore;
        }
        else{   //Se a condição anterior não for obedecida;
            tInsert(&((*w)->r), f,&(*R));  //verifica o lado esquerdo da arvore;
        }
    }
    (*w)->h = tamanho(&(*w), &(*R));  //altura de h é definido pela função 'tamanho', é passado o endereço do nó atual e o da raíz;
    
}

//Função para retornar o valor máximo entre as variáveis 'a' e 'b';
int max(int a, int b){

    if (a > b){ //se 'a' for maior que 'b', retorna 'a';
       return a;
    }

    return b;   //se não, retorna b;
}

//Função para exibir todos os valores da arvore em ordem crescente;
void tPrintf(struct tnode *w){

    if (w!=NULL){   //Se houver conteúdo na estrutura;

        tPrintf(w->l);  //percorra o lado esquerdo;
        printf("%d\n",w->v);    //printe o valor do nó;
        tPrintf(w->r);  //percorra o lado direito;
    }
}

//Função de balanceamcento;
void balancear(struct tnode **w, int a, struct tnode **R){
    int giro;

    if (a > 1){//se o resultado da diferença dos filhos for maior que 1, *então o balanceamento seguirá o lado direito*;

        //armazena o tamanho da altura dos filhos, do filho direito de 'w';
        int h_e = tamanho(&(*w)->r->l, &(*R));
        int h_d = tamanho(&(*w)->r->r, &(*R));
    
        giro = h_d - h_e;   //armazena a diferença de h_d e h_e em 'giro'

        if (giro >=0){  //se a variavel 'giro' for maior ou igual a 0, *então o balanceamencto seguirá o lado direito e será feita uma rotação à esquerda*;
            
            rE(&(*w), &(*R));   //*Passa endereço de w e endereço da raíz*
        }
    
        else{   //Caso não, *o balanceamencto seguirá o lado esquerdo e será feita uma rotação dupla*;

            rD(&((*w)->r), &(*R));  //rotação à direita no filho direito de w;   *Passa endereço da esquerda de w e endereço da raíz*
            rE(&(*w), &(*R));   //rotação à esquerda em w;  *Passa endereço de w e endereço da raíz*
        }
    }

    else{   //*o balanceamento seguirá o lado direito*

        //armazena o tamanho da altura dos filhos, do filho esquerdo de 'w';
        int h_e = tamanho(&(*w)->l->l, &(*R));
        int h_d = tamanho(&(*w)->l->r, &(*R));
    
        giro = h_d - h_e;   //armazena a diferença de h_d e h_e em 'giro'

        if (giro <=0){  //se a variavel 'giro' for menor ou igual a 0, *então o balanceamencto seguirá o lado esquerdo e será feita uma rotação à direita*;

            rD(&(*w), &(*R));   //*Passa endereço de w e endereço da raíz*
        }

        else{   //Caso não, *o balanceamencto seguirá o lado direito e será feita uma rotação dupla*;

            rE(&((*w)->l), &(*R));  //rotação à esquerda no filho esquerdo de w; *Passa endereço da esquerda de w e endereço da raíz*
            rD(&(*w), &(*R));   //rotação à direita em w;   *Passa endereço de w e endereço da raíz*
        }
    }   
}


//Função responsável pela rotação à direita;
void rD(struct tnode **x, struct tnode **R){

    //alocação de espaço para as variáveis 'y' e 'c';
    struct tnode *y = (struct tnode*)malloc(sizeof(struct tnode));
    struct tnode *c = (struct tnode*)malloc(sizeof(struct tnode));

    y = (*x)->l;    //'y' recebe conteúdo da esquerda de 'x';
    c = y->r;   //'c' recebe conteúdo da direita de 'y';
    
    (*x)->l = c;    //esquerda de 'x' recebe conteúdo de 'c';
    y->r = (*x);    //direita de 'y' recebe conteúdo de 'x';
    y->p = (*x)->p; //pai de 'y' recebe conteúdo do pai de 'x';
    (*x)->p = y;    //pai de 'x' recebe conteúdo de 'y'


    if (c != NULL){ //se houver conteúdo em 'c';
        c->p = (*x);    //pai de 'c' recebe conteúdo de 'x'
    }

    if( y->p != NULL){  //se houver conteúdo no pai de 'y';

        //faz verificaçao para saber se o balanceamento está ocorrendo no lado direito ou esquerdo da árvore;
        if(y->p->v > ((*x)->v)){    //se o valor do pai de y for maior que o valor de 'x';
            y->p->l = y;    //esquerda do pai de 'y' recebe conteúdo de y;
        }
        else{   //Caso não;
            y->p->r = y;    //direita do pai de 'y' recebe conteúdo de y;
        }
    }
    else{   //Caso não haja conteúdo no pai de 'y';
        *R = y; //Raíz da arvore recebe conteúdo de 'y';
    }
}

//Função responsável pela rotação à esquerda;
void rE(struct tnode **x, struct tnode **R){    //*endereço do nó e endereço da raíz*;

    //alocação de espaço para as variáveis 'y' e 'c'; 
    struct tnode *y = (struct tnode*)malloc(sizeof(struct tnode));
    struct tnode *c = (struct tnode*)malloc(sizeof(struct tnode));

    y = (*x)->r;    //'y' recebe conteúdo da direita de 'x';
    c = y->l;   //'c' recebe conteúdo da esquerda de 'y';

    (*x)->r = c;    //direita de 'x' recebe conteúdo de 'c';
    y->l = (*x);    //esquerda de 'y' recebe conteúdo de 'x';
    y->p = (*x)->p; //pai de 'y' recebe conteúdo do pai de 'x';
    (*x)->p = y;    //pai de 'x' recebe conteúdo de 'y'

    if (c != NULL){ //se houver conteúdo em 'c';
        c->p = (*x);    //pai de 'c' recebe conteúdo de 'x'
    }

    //se houver conteúdo no pai de 'y';
    if( y->p != NULL){

        //faz verificaçao para saber se o balanceamento está ocorrendo no lado direito ou esquerdo da árvore;
        if(y->p->v > ((*x)->v)){    //se o valor do pai de y for maior que o valor de 'x';
            y->p->l = y;    //esquerda do pai de 'y' recebe conteúdo de y;
        } 
        else{   //Caso não;
            y->p->r = y;    //direita do pai de 'y' recebe conteúdo de y;
        }
    }   
    else{   //Caso não haja conteúdo no pai de 'y';
        *R = y; //Raíz da arvore recebe conteúdo de 'y';
    }
}
