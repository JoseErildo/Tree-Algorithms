#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

struct tnode* new_tnode(int v);
void tInsert(struct tnode **w, struct tnode *f);
void tPrintf(struct tnode *w);
void tPrintfH(struct tnode *w);

int max(int a, int b);
void limpaMemoria(struct tnode **w);
int tamanho(struct tnode **w);

int buscaChave(struct tnode *w, int k);

struct tnode{   //estrutura da arvore
    int v;  //chave armazenada
    int h;  //altura da arvore

    struct tnode *p;    //ponteiro para estrutura pai;
    struct tnode *l;    //ponteiro para a estrutura do filho à esquerda;
    struct tnode *r;    //ponteiro para a estrutura do filho à direita;
};

int main(){

    struct timeval a,b; //armazenam o tempo de execução;
    srand(time(NULL));

    //*Trecho responsável pelo cálculo do tempo*;

    //laço responsável por criar estruturas que irão de 100 à 5000; tamanho +100 a cada loop;
    for (int i=100;i<=5000;i+=100){
        double t=0; //variável responsável por somar o tempo, nestre trecho ele é zerado para iniciar um novo cálculo de um vetor maior;

        struct tnode *R = NULL; //Cria um ponteiro para a variável R do tipo tnode recebendo NULL;

        //laço responsável pela inserção de valores;
        for (int j=1; j<=i; j++){   //estrutura criada de acordo com o tamanho da variável 'i' ;
            tInsert(&R, new_tnode(j));  //chamada da função de inserção, *é passado o endereço da Raíz/e o valor a ser inserido dentro da chamada para a função tnode ;
        }

        //laço responsável pelo cálculo do tempo de execução;
        for (int j =0; j<5000;j++){ //o laço será percorrido 5000 vezes;

            gettimeofday(&a,NULL);  //calcula o tempo decorrido no programa até a chamada dessa função;

            buscaChave(R, i+1); //chamada para a função de busca *recebe conteúdo de R e valor a ser buscado para que ocorra o pior caso;

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
            return buscaChave(w->l,k);  //busca a chave no lado esquerdo da arvore.
        }
        else{
            return buscaChave(w->r,k);  //busca a chave no lado esquerdo da arvore.
        }
    }

    return -1;  //Se a chave não for encontrada, retorna -1;
}

//Função Responsável por Exibir a altura dos nós da Arvore em ordem Crescente.
void tPrintfH(struct tnode *w){

    if (w!=NULL){   //Se houver elemento no nó.

        tPrintfH(w->l);//percorra o lado esquerdo.
        tPrintfH(w->r); //percorra o lado direito.

        printf("%d\n",w->h);    //exiba a altura do nó atual;
    }
}

//Função para liberar memória na arvore;
void limpaMemoria(struct tnode **w){

    if (*w!=NULL){  //Se houver conteúdo;

       limpaMemoria(&((*w)->l));    //percorra o lado esquerdo;
       limpaMemoria(&((*w)->r));    //percorra o lado direito;

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
int tamanho(struct tnode **w){

    if (*w == NULL){    //Caso não exista conteúdo em w, retorna o valor 0;
        return -1;
    }    

    else{

        int h_e = tamanho(&(*w)->l);    //verificar a altura do nó à esquerda;
        int h_d = tamanho(&(*w)->r);    //verificar a altura do nó à direita;

        return max(h_e,h_d)+1;  //retorna a altura maior entre h_e/h_d +1;
    }
}

//Função para inserir um novo nó na àrvore;
void tInsert(struct tnode **w, struct tnode *f){    //*endereço da estrutura/ nó criado em lNode*;

    if (*w == NULL){    //Se não houver conteúdo, w recebe a estrutura de f;
        *w = f;
    } 

    else{
        f->p = *w;//Se não, o pai de f recebe o conteúdo de w;

        //Verifica se a inserção do nó deve ser à esquerda ou direita de w;
        if (f->v < (*w)->v){    //Se o valor de f gor menor que o valor e w;

            tInsert(&((*w)->l), f); //verifica o lado esquerdo da arvore;
        }
        else{   //Se a condição anterior não for obedecida;

            tInsert(&((*w)->r), f); //verifica o lado esquerdo da arvore;
        }
    }
    (*w)->h = tamanho(&(*w));   //altura de h é definido pela função 'tamanho', é passado o endereço do nó atual;
}

//Função para retornar o valor máximo entre as variáveis 'a' e 'b';
int max(int a, int b){

    if (a > b){ //se 'a' for maior que 'b', retorna 'a';
       return a;
    }

    return b;    //se não, retorna b;
}

//Função para exibir todos os valores da arvore em ordem crescente;
void tPrintf(struct tnode *w){

    if (w!=NULL){   //Se houver conteúdo na estrutura;

        tPrintf(w->l);  //percorra o lado esquerdo;
        printf("%d\n",w->v);    //printe o valor do nó;
        tPrintf(w->r);  //percorra o lado direito;
    }
}


