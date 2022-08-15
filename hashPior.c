#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

struct lnode* new_lnode(int v);
struct hash* new_hash(int m);
unsigned long int fHash(struct hash *h, int v);
void hinSert(struct hash **h, int v);

void lPrintV(struct hash *H);

void insertNodehH(struct lnode *h,struct hash **w);

void limpaHash(struct hash **h);
void limpaLinha(struct lnode **h);

void insertNodehH(struct lnode *h,struct hash **w);
struct hash* reHashing(struct hash **h);

void lPrintH(struct hash *h);
void lPrintNode(struct lnode *w);

long int buscaChave(struct hash *w, long int k);
long int buscaNode(struct lnode *w, long int k);

struct lnode{ //Nó/Lista

    long int v; //chavec
    struct lnode *p;    //ponteiro para o próximo;

};

struct hash{ //Tabela

    struct lnode **t;   //vetor;
    unsigned int m; //tamanho do vetor;
    unsigned int n; //quantidade de elementos no vetor;

};

int main(void){
    
    long int c = 8192;
    double t=0;
    struct timeval a,b;
    
    //laço responsável pelo tamanho do vetor;
     for (int i = 25; i<=5000;i+=25){ //vetor é aumentado a cada loop +25vezes até 5000;
        t = 0; //zera a variavel que soma o tempo de execução;

        struct hash *H = new_hash(2); //cria um hash de tamanho 2;

        for (int  j= 1; j<=i; j++){ //estrutura criada de acordo com o tamanho da variável 'i' ;
            hinSert(&H,c); //chamacda da função de inserção, *é passado o endereço do Hash, e o valor a ser inserido;
        }

        //laço responsável pelo cálculo do tempo de execução;        
        for(int j =0; j<5000;j++){  //o laço será percorrido 5000 vezes;
            gettimeofday(&a,NULL);  //calcula o tempo decorrido no programa até a chamada dessa função;

            buscaChave(H, c*2); //chamada para a função de busca *recebe conteúdo de R e valor a ser buscado para que ocorra o melhor caso;

            gettimeofday(&b,NULL);  //calcula o tempo decorrido no programa desde a chamada da função gettime anterior, até está;

            t+= (b.tv_sec*1000000+b.tv_usec) - (a.tv_sec*1000000+a.tv_usec); //somatório dos tempos sendo armazenado em t;
        }
        //exibe o tamanho da estrutura percorridca e a media de seu tempo;      
        printf("%d %f\n", i, t/5000);

        //chamada para a função que libera espaço da árvore;  
        limpaHash(&H);  
    }
    return 0;
}

//Função responsável pela busca da chave;
long int buscaChave(struct hash *w, long int k){    //*estrutura/chave;

    long int p = fHash(w,k);    //p recebe o endereço da posição de k
    if (w->t[p] != NULL){   //se houver conteúdo na posição;
        if (w->t[p]->v == k){   //se o valor da posição t for igual a chave, retorna 'k';
            return k;
        }
        else{ //se não, é buscado a chave pelo próximo nó da estrutura;
            return buscaNode(w->t[p]->p,k); //*proximo nó/chave;
        }
    }   //se a chave não for encontrada, retorna -1;
    return -1;
}

//Responsável pela busca dós nós do vetor;
long int buscaNode(struct lnode *w, long int k){    //*estrutura/chave*

    if (w != NULL){ //se houver conteúdo;
        if (w->v == k){ //se o valor do conteúdo for igual a chave, retorna 'k';
            return k;
        }
        else{//se não, é buscado a chave pela próxima estrutura;
            return buscaNode(w->p,k); //*proximo nó/chave*
        }
    }   //se a chave não for encontrada, retorna -1;
    return -1;
}

//Printa toda a tabela
void lPrintH(struct hash *h){ 

    for (int i = 0; i< h->m; i++){  //percorre um loop do tamanho do vetor 'h->m';
        if (h->t[i] != NULL){   //if houver conteúdo na posição 'i' do vetor;
            lPrintNode(h->t[i]->p); //vai para o próximo nó; 
            printf("\n%ld", h->t[i]->v);    //printa o conteúdo da posição do vetor;
        }        
    }
}

//Printa os Nós do vetor
void lPrintNode(struct lnode *w){

    if(w != NULL){  //se houver conteúdo em 'w';
        lPrintNode(w->p);   //vai para o próximo nó;
        printf("\n%ld",w->v);   //printa o conteúdo de 'w';
    }
}


//Cria uma estrutura do tipo lnode
struct lnode* new_lnode(int v){ //valor a ser inserido;

    struct lnode *w = (struct lnode*)malloc(sizeof(struct lnode));  //aloca espaço na memória de 'w' do tipo lnode;

    //atribui valores;    
    w->v = v;
    w->p = NULL;
    
    //retorna o conteúdo de 'w';    
    return w;
}

//Cria uma estrutura do tipo Hash;
struct hash* new_hash(int m){   //tamanho do vetor

    struct hash *h = (struct hash*)malloc(sizeof(struct hash)); //aloca espaço na memória de 'w' do tipo Hash;
    h->m = m;   //atribui à 'h->m' o tamanho do vetor

    h->t = (struct lnode**)malloc(h->m*sizeof(struct lnode*));  //aloca espaço na memória de 't' do tipo lnode de tamanho 'm';

    //percorre o vetor 't' e o preenche com NULL;
    for(int i = 0; i< h->m; i++){
        h->t[i] = NULL;
    }
    
    h->n = 0;   //numero de elementos inseridos igual a 0;

    return h; //retorna o conteúdo de h;
}

//Função para calcular a posição da chave;
unsigned long int fHash(struct hash *h, int v){ //*ponteiro para estrutura/chave*;

    return v % h->m; //retorna o resto da divisão de 'v' pelo tamanho do vetor;
}

//Função para inserir um novo nó na estrutura;
void hinSert(struct hash **h, int v){   //*endereço de hash/chave*;

    //condição de balanceamnto;
    if ((*h)->m <= (*h)->n){   //se o número de elementos contidos no vetor for menor ou igual ao seu tamanho, será feito o balanceamento; 
        (*h) = reHashing(&(*h)); //endereço de h;
    }

    struct lnode *w = new_lnode(v); //criação de um nó para a estrutura, *passa-se a chave*;
    unsigned long int p = fHash((*h),v);    //calcula a posição da chave;
    w->p = (*h)->t[p]; //proximo nó de 'w', recebe o conteúdo da posição de 'p' do vetor;
    (*h)->t[p] = w; //posição 'p' do vetor recebe conteúdo de 'w';
    (*h)->n = (*h)->n +1;   //insere +1 à variável 'n' que guarda o número de elementos da estrutura;
}

//Função de balanceamento;
struct hash* reHashing(struct hash **h){    //endereço do Hash;

    struct hash *w = new_hash((*h)->m*2);   //cria um novo Hash com dobro de tamanho do vetor anterior;
    
    //loop para percorrer o vetor antigo e inserir no novo;
    for (int p = 0; p < (*h)->m; p++){
        if ((*h)->t[p] != NULL){    //se houver conteúdo na posição 'p' do vetor; 
            hinSert(&w, (*h)->t[p]->v); //insere-se o valor da posição 'p' no novo hash criado, *endereço do novo hash/chave*;
            
            if ((*h)->t[p]->p != NULL){ //se houver conteúdo na próxima estrutura da posição 'p' do vetor;
                insertNodehH((*h)->t[p]->p, &w);    //insere nó,*conteúdo do próximo/endereço do novo hash*;
            }
        }
    }

    //libera memória na antiga tabela hash;
    limpaHash(&(*h));

    return w; //retorna o conteúdo de 'w';
}

//Insere o nó do antigo vetor no novo Hash;
void insertNodehH(struct lnode *h,struct hash **w){ //*ponteiro para a estrutura/endereço do novo Hash*;

    hinSert(&(*w), h->v); //insere a chave no novo Hash, *endereço do novo Hash/chave*;

    if (h->p != NULL){  //se houver conteúdo na proxima estrutura;
        insertNodehH(h->p, &(*w));  //insere nó,*conteúdo do próximo/endereço do novo hash*;
    }
}

//Libera a memória da tabela Hash;
void limpaHash(struct hash **h){    //*endereço do Hash*

    //loop para percorrer as posições do vetor de acordo com o seu tamanho;
    for (int p = 0; p < (*h)->m; p++){
        if ((*h)->t[p] != NULL){    //se houver conteúdo na posição 'p' do vetor;
            limpaLinha(&((*h)->t[p]->p)); //limpa os nós daquela posição, *endereço do próximo conteúdo do vetor*;
        }   
        free((*h)->t[p]);   //libera espaço na posição 'p' do vetor;
    }
    free((*h)); //libera espaço em Hash;
}

//Libera espaço nas linhas do vetor;
void limpaLinha(struct lnode **h){  //endereço do nó;

    if (*h != NULL){    //se houver conteúdo em 'h';
        limpaLinha(&((*h)->p)); //vai para a próxima linha, *endereço da próxima linha*;
        free(*h);   //libera espaço no nó;
    }
}
