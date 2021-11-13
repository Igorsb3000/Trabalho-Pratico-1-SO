#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>

// Variaveis globais
int numero_processos, lin_m, col_m, lin_aux = 0, col_aux = 0;

// 0 1 2 3 4[0,4] 4/5=0 resto 4    12/5=2 resto 2 [2,2]  [4,2] = 22 elemento
int elemento_atual = 0;
int P;

int id;

int **matriz_resultado_global, **matriz_1, **matriz_2;

// Funcao para imprimir matriz global que foi alocada dinamicamente
void *imprimir_matriz_global()
{
    printf("\n");

    for (int i = 0; i < lin_m; i++)
    {
        for (int j = 0; j < col_m; j++)
        {
            printf("%d ", matriz_resultado_global[i][j]);
        }
        printf("\n");
    }
}

void *multiplica_matrizes2(void *i)
{

    int indice = (int)(size_t)i;
    fflush(stdout);
    printf("INDICE = %d\n", indice / P);
    //((indice+1)*P) -1
    int linha = indice / P;
    int coluna = indice % P;

    for (int contador = 0; contador < P; contador++)
    {
        for (int k = 0; k < col_m; k++)
        {
            matriz_resultado_global[linha][coluna] += matriz_1[linha][k] * matriz_2[k][coluna];
        }
        fflush(stdout);
        printf("Matriz = = %d\n", matriz_resultado_global[linha][coluna]);
        coluna += 1; //prox elemento

        if (coluna >= col_m)
        {
            linha += 1;
        }
        //elemento_atual+=1;
    }
    /*else{
		//break;
		elemento_atual = 0;*/
    pthread_exit(NULL);
}

// Funcao principal das Threads
// Indice thread 1 = #0, thread 2 = #1.....
// P = 3, thread 1 vai calcular 1, 2 e 3 elementos 3
void *multiplica_matrizes(void *indice)
{
    sleep(2); //indice *
    fflush(stdout);

    printf("Thread #%d começando valor P=%d...\n", (int)(size_t)indice, P);
    int elementos_total = lin_m * col_m;

    // Multiplicar P elementos da matriz resultado
    for (int i = 0; i < P; i++)
    {

        // Não passar da quantidade que é pra calcular
        if (elemento_atual < P)
        {

            // Calcula matriz
            matriz_resultado_global[lin_aux][col_aux] = 0;
            fflush(stdout);
            printf("LINHA=%d e COLUNA=%d\n", lin_aux, col_aux);

            for (int k = 0; k < col_m; k++)
            {
                matriz_resultado_global[lin_aux][col_aux] += matriz_1[lin_aux][k] * matriz_2[k][col_aux];
            }

            // Incrementa
            elemento_atual += 1;
            col_aux += 1;

            // Se chegar no limite de colunas, pula uma linha e zera as colunas
            if ((col_aux) >= col_m)
            {
                lin_aux += 1;
                col_aux = 0;
            }
        }
        else
        {
            printf("Não tem mais elementos para calcular\n");
            break;
        }
    }
    elemento_atual = 0;
    //fflush(stdout);
    printf("Thread SAINDO....\n");
    pthread_exit(NULL);
    // processo 1 = #0     0,0 - 0,P   0 até ((indice+1)*P) -1
    // processo 2 = #1                 (indice*P) até ((indice+1)*P) -1
    // ((indice+1) *P) -1 pode estourar o total de elementos, verificar
}

int main(int argc, char *argv[])
{

    int n1, m1, n2, m2, i, j, k;
    FILE *file1, *file2, *file3;

    // Verificando se os nomes dos 2 arquivos foram passados na linha de comando
    if (argc < 3)
    {
        printf("Informe os nomes dos 2 arquivos que contém as matrizes m1 e m2!\n");
        return 1;
    }

    /* Lendo Matriz 1 */

    // Abrindo os arquivos para leitura das matrizes
    file1 = fopen(argv[1], "r");
    if (file1 == NULL)
    {
        printf("Arquivo %s não encontrado!\n", argv[1]);
        return 0;
    }

    // Obtendo as dimensoes da matriz
    fscanf(file1, "%d;%d;", &n1, &m1);
    //int matriz_1[n1][m1];
    matriz_1 = malloc(n1 * sizeof(int *));
    for (i = 0; i < m1; i++)
    {
        matriz_1[i] = malloc(m1 * sizeof(int));
    }

    for (i = 0; i < n1; i++)
    {
        for (j = 0; j < m1; j++)
        {
            fscanf(file1, "%d;", &matriz_1[i][j]);
        }
    }
    // Fechando o arquivo
    fclose(file1);

    printf("\nMatriz 1:\n\n");
    for (i = 0; i < n1; i++)
    {
        for (j = 0; j < m1; j++)
        {
            printf("%d;", matriz_1[i][j]);
        }
        printf("\n");
    }

    /* Lendo Matriz 2 */

    // Abrindo os arquivos para leitura das matrizes
    file2 = fopen(argv[2], "r");
    if (file2 == NULL)
    {
        printf("Arquivo %s não encontrado!\n", argv[1]);
        return 0;
    }

    // Obtendo as dimensoes da matriz
    fscanf(file2, "%d;%d;", &n2, &m2);
    //int matriz_2[n2][m2];
    matriz_2 = malloc(n2 * sizeof(int *));
    for (i = 0; i < m2; i++)
    {
        matriz_2[i] = malloc(m2 * sizeof(int));
    }

    for (i = 0; i < n2; i++)
    {
        for (j = 0; j < m2; j++)
        {
            fscanf(file2, "%d;", &matriz_2[i][j]);
        }
    }
    // Fechando o arquivo
    fclose(file2);

    printf("\nMatriz 2:\n\n");
    for (i = 0; i < n2; i++)
    {
        for (j = 0; j < m2; j++)
        {
            printf("%d;", matriz_2[i][j]);
        }
        printf("\n");
    }

    // Verificando se a multiplicacao das matriz m1 e m2 eh possivel
    if (n1 != m2)
    {
        printf("\nNão é possivel multiplicar as matrizes m1 e m2!\n");
        return 1;
    }

    /* Multiplicando as Matrizes */

    //Defining Threads
    P = atoi(argv[3]);
    int status;
    void *thread_return;
    lin_m = n1; // Quantidade de linhas da matriz m1
    col_m = m2; // Quantidade de colunas da matriz m2
    int matriz_resultado[lin_m][col_m];

    // Alocando dinamicamente a matriz global
    matriz_resultado_global = malloc(lin_m * sizeof(int *));
    for (i = 0; i < col_m; i++)
    {
        matriz_resultado_global[i] = malloc(col_m * sizeof(int));
    }

    // Fazendo o calculo da quantidade de threads necessarias
    int quantidade_processos = (lin_m * col_m) / P;
    pthread_t threads[quantidade_processos];

    //clock_t begin = clock();
    numero_processos = ceil(quantidade_processos);
    printf("Processos = %d\n", numero_processos);

    // Calculando Multiplicação das Matrizes
    for (i = 0; i < quantidade_processos; i++)
    {
        id = shmget(IPC_PRIVATE, 100, 0600);

        process = fork();

        int *a;
        a = shmat(id, 0, 0);

        printf("Processo (PID=%d) criando processo #%d\n", getpid(), i);
        status = pthread_create(&threads[i], NULL, multiplica_matrizes2, (void *)(size_t)i);

        if (status != 0)
        {
            printf("Erro na criação da thread!\n");
            return 1;
        }
    }

    for (i = 0; i < quantidade_processos; i++)
    {
        printf("Esperando THread #%d finalizar...\n", i);
        pthread_join(threads[i], &thread_return);
        printf("Thread #%d finalizada\n", i);
    }

    imprimir_matriz_global();

    printf("Liberando espaco da matriz matriz resultado\n");
    // Liberando espeço alocado dinamicamente da matriz global
    for (i = 0; i < col_m; i++)
    {
        free(matriz_resultado_global[i]);
    }
    free(matriz_resultado_global);
    lin_aux = 0;
    col_aux = 0;

    return 0;
}
