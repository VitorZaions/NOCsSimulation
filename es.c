#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int status = 0;
int lastthread;
int arrived = 0;

int mensagensdestino[201];
int mensagenstrafegadas[201];
int mensagenscriadas[201];

int first = 0;

struct args
{
    int source;
    int target;
    int nodoschave;
    char msg[100];
    int id;
    char ladogo;
};

void *Thread(void *dados)
{

    if(arrived == 0)
    {
        int source = ((struct args*)dados)->source;
        int target = ((struct args*)dados)->target;
        char msg[100];
        strcpy (msg, ((struct args*)dados)->msg);
        int nodosmax = ((struct args*)dados)->nodoschave;
        char ladoir = ((struct args*)dados)->ladogo;
        int myid = status;

        if(first == 0)
        {
            mensagenscriadas[source]++;
            first = 1;
        }

        if(ladoir == 'r')
        {
            if(myid == target && myid != lastthread)
            {
                mensagensdestino[myid]++;
                printf("[Nodo %d]",myid);
                printf("Mensagem recebida '%s' do nodo %d source = %d target = %d \n",msg,lastthread,source,target);
                arrived = 1;
            }
            else
            {

                if(myid != source)
                {
                    printf("[Nodo %d]",myid);
                    printf("recebida mensagem '%s' do nodo %d \n",msg,lastthread);
                }

                if(myid == nodosmax)
                {
                    status = 1;
                }
                else
                {
                    status++;
                }
                mensagenstrafegadas[myid]++;
                printf("[Nodo %d]",myid);
                printf("enviando mensagem '%s' para o nodo %d \n",msg,status);
            }
        }


        else
        {

            if(myid == target && myid != lastthread)
            {
                mensagensdestino[myid]++;
                printf("[Nodo %d]",myid);
                printf("Mensagem recebida '%s' do nodo %d source = %d target = %d \n",msg,lastthread,source,target);
                arrived = 1;
            }
            else
            {

                if(myid != source)
                {
                    printf("[Nodo %d]",myid);
                    printf("recebida mensagem '%s' do nodo %d \n",msg,lastthread);
                }

                if(myid == 1)
                {
                    status = nodosmax;
                }
                else
                {
                    status--;
                }
                mensagenstrafegadas[myid]++;
                printf("[Nodo %d]",myid);
                printf("enviando mensagem '%s' para o nodo %d \n",msg,status);
            }

        }
        lastthread = myid;
    }
    pthread_exit(NULL);
}

char compare(int src, int trgt, int nodoschave)
{
    if(trgt > src)
    {

        int casasdireita = trgt - src;
        int casasesquerda = 0;

        for (int i = 1; i < 2; src--)
        {
            if(src == 1)
            {
                src = nodoschave + 1;
            }

            if(src == trgt)
            {
                casasesquerda++;
                break;
            }
            else
            {
                casasesquerda++;
            }
        }

        if(casasdireita > casasesquerda)
            return 'l';
        else
            return 'r';
    }

    if(trgt < src)
    {
        int casasesquerda = src - trgt;
        int casasdireita = 0;

        for (int i = 1; i < 2; src++)
        {

            if(src == nodoschave)
            {
                src = 1;
            }

            if(src == trgt)
            {
                casasdireita++;
                break;
            }
            else
            {
                casasdireita++;
            }
        }
        if(casasdireita > casasesquerda)
            return 'l';
        else
            return 'r';
    }
}


int comparenum(int src, int trgt, int nodoschave)
{
    if(trgt > src)
    {
        int casasdireita = trgt - src;
        int casasesquerda = 0;

        for (int i = 1; i < 2; src--)
        {

            if(src == 1)
            {
                src = nodoschave + 1;
            }

            if(src == trgt)
            {
                casasesquerda++;
                break;
            }
            else
            {
                casasesquerda++;
            }
        }

        if(casasdireita > casasesquerda)
            return casasesquerda;
        else
            return casasdireita;
    }

    if(trgt < src)
    {

        int casasesquerda = src - trgt;
        int casasdireita = 0;

        for (int i = 1; i < 2; src++)
        {

            if(src == nodoschave)
            {
                src = 1;
            }

            if(src == trgt)
            {
                casasdireita++;
                break;
            }
            else
            {
                casasdireita++;
            }
        }

        if(casasdireita > casasesquerda)
            return casasesquerda;
        else
            return casasdireita;

    }
}


// Função main do programa
int main(int argc, char *argv[])
{

    freopen("entrada.txt", "r", stdin);

    printf("Digite a quantidade de entradas: \n");
    int qntentradas;
    scanf("%d",&qntentradas);

    for(int i = 0; i < 201; i++)
    {
        mensagenscriadas[i] = 0;
        mensagensdestino[i] = 0;
        mensagenstrafegadas[i] = 0;
    }

    for(int i = 0; i < qntentradas ; i++)
    {
        first = 0;
        struct args *Valores = (struct args *)malloc(sizeof(struct args));
        arrived = 0;

        int nodoschaveamento;
        printf("Digite a quantidade de nodos de chaveamento: \n");
        scanf("%d", &nodoschaveamento);

        pthread_t thread[nodoschaveamento];


        printf("Digite o thread source: \n");
        scanf("%d",&Valores->source);
        printf("Digite o thread target: \n");
        scanf("%d",&Valores->target);
        printf("Digite a msg: \n");
        scanf("%s",&Valores->msg);

        lastthread = Valores->source;
        status = Valores->source;

        int src = Valores->source;
        int trgt = Valores->target;

        char ladoir = compare(src,trgt,nodoschaveamento);

        Valores->ladogo = ladoir;
        Valores->id = src;
        Valores->nodoschave = nodoschaveamento;

        int threadsacriar = comparenum(src,trgt,nodoschaveamento);

        for(int i = 0 ; i < threadsacriar + 1 ; i++)
        {
            pthread_create(&thread[i], NULL, Thread, (void *)Valores);
            pthread_join(thread[i], NULL);
        }

    }

    printf("\n");

    for(int i = 0; i < 201; i++)
    {

        if(mensagenscriadas[i] != 0)
        {
            printf("[N%d] Mensagens Criadas: %d \n",i,mensagenscriadas[i]);
        }

        if (mensagensdestino[i] != 0)
        {
            printf("[N%d] Mensagens Destino: %d\n",i,mensagensdestino[i]);
        }

        if(mensagenstrafegadas[i] != 0)
        {
            printf("[N%d] Mensagens Trafegadas: %d \n",i,mensagenstrafegadas[i]);
        }

    }


    pthread_exit(NULL);
}
