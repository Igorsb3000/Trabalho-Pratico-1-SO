#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>

/*
*	Autores: Igor Silva e Wesley Gurgel
*/

// Variaveis globais
int numero_threads, lin_m, col_m, lin_aux = 0, col_aux = 0, P;
int **matriz_resultado_global, **matriz_1, **matriz_2;
struct timeval begin, end;
double time_spent;
double time_total = 0.0;

// Funcao para imprimir matriz global que foi alocada dinamicamente
void *imprimir_matriz(int **matriz, int lin, int col)
{
	printf("\n");

	for (int i = 0; i < lin; i++)
	{
		for (int j = 0; j < col; j++)
		{
			printf("%d;", matriz[i][j]);
		}
		printf("\n");
	}
}

// Funcao para multiplicar P elementos da matriz resultado para cada Thread
void *multiplica_matrizes(void *i)
{
	FILE *file;
	char *nome_arquivo = malloc(30 * sizeof(char));
	sprintf(nome_arquivo, "resultado_thread_%d.csv", (int)(size_t)i);
	file = fopen(nome_arquivo, "w");
	fprintf(file, "%d;%d;\n", lin_m, col_m);

	int linha, coluna, e, indice = (int)(size_t)i;
	//int elemento_inicial = indice*P;

	for (e = indice * P; e < ((indice + 1) * P); e++)
	{
		int total_elementos = (lin_m * col_m);

		if (e < total_elementos)
		{
			linha = e / lin_m;
			coluna = e % col_m;

			matriz_resultado_global[linha][coluna] = 0;
			for (int k = 0; k < col_aux; k++)
			{
				matriz_resultado_global[linha][coluna] += matriz_1[linha][k] * matriz_2[k][coluna];
			}
			fprintf(file, "%d;", matriz_resultado_global[linha][coluna]);
		}
		else
		{
			// Não tem mais elementos para calcular
			break;
		}
	}

	// Tempo final da execucao da thread
	gettimeofday(&end, NULL);
	time_spent = (end.tv_sec - begin.tv_sec) * 1000.0;
	time_spent += (end.tv_usec - begin.tv_usec) / 1000.0;
	time_total += time_spent;

	printf("Time total = %f | time spent = %f\n", time_total, time_spent);
	fprintf(file, "\n%fms;", time_spent);
	fclose(file);
	free(nome_arquivo);
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{

	int n1, m1, n2, m2, i, j, k;
	FILE *file1, *file2, *file3;

	printf("\n");
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

	// printf("\nMatriz 1:\n\n");
	// imprimir_matriz(matriz_1, n1, m1);

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

	// printf("\nMatriz 2:\n\n");
	// imprimir_matriz(matriz_2, n2, m2);

	// Verificando se a multiplicacao das matriz m1 e m2 eh possivel
	if (m1 != n2)
	{
		printf("\nNão é possivel multiplicar as matrizes m1 e m2!\n");
		return 1;
	}
	col_aux = n2;

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
	int quantidade_threads = (lin_m * col_m) / P;
	//Criando outra thread para o restante de elementos que sobrarem
	if ((lin_m * col_m) % P != 0)
	{
		quantidade_threads += 1;
	}

	pthread_t threads[quantidade_threads];
	numero_threads = quantidade_threads;

	FILE *file;
	double time_spent;
	char *nome_arquivo = malloc(30 * sizeof(char));

	// Tempo inicial de execucao da thread
	gettimeofday(&begin, NULL);

	for (i = 0; i < quantidade_threads; i++)
	{
		// printf("Processo (PID=%d) criando thread #%d\n", getpid(), i);
		status = pthread_create(&threads[i], NULL, multiplica_matrizes, (void *)(size_t)i);

		if (status != 0)
		{
			printf("Erro na criação da thread!\n");
			return 1;
		}
	}

	

	for (i = 0; i < quantidade_threads; i++)
	{
		// printf("Esperando Thread #%d finalizar...\n", i);
		pthread_join(threads[i], &thread_return);
		// printf("Thread #%d finalizada\n", i);
	}

	// Liberando espacos da memoria alocada dinamicamente
	for (i = 0; i < col_m; i++)
	{
		free(matriz_resultado_global[i]);
	}
	free(matriz_resultado_global);

	for (i = 0; i < m1; i++)
	{
		free(matriz_1[i]);
	}
	free(matriz_1);

	for (i = 0; i < m2; i++)
	{
		free(matriz_2[i]);
	}
	free(matriz_2);


	return 0;
}