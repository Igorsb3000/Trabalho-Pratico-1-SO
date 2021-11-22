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

int **AlocaMatriz(int linhas, int colunas){
	int i, **matriz;

	matriz = (int **)malloc(sizeof(int *) * linhas);
	if(matriz == NULL){
		printf("Memoria insuficiente para alocar matriz\n");
		exit(-1);
	}

	for(i=0; i<linhas; i++){
		matriz[i] = (int *)malloc(sizeof(int) * linhas);

		if(matriz[i] == NULL){
		printf("Memoria insuficiente para alocar coluna da matriz\n");
		exit(-1);
		}
	}
	return matriz;
}

void DesalocarMatriz(int **matriz, int linhas){
	int i=0;

	for(i=0; i<linhas; i++){
		free(matriz[i]);
	}
	free(matriz);
}

// Funcao para multiplicar P elementos da matriz resultado para cada Thread
void *multiplica_matrizes(void *i)
{
	FILE *file;
	double time_spent;
	char *nome_arquivo = malloc(30 * sizeof(char));
	int linha, coluna, e, indice = (int)(size_t)i;

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

	//Criando o arquivo e escrevendo os elementos multiplicados e o tempo da thread em ms
	sprintf(nome_arquivo, "resultado_thread_%d.csv", (int)(size_t)i);
	file = fopen(nome_arquivo, "w");
	fprintf(file, "%d;%d;\n", lin_m, col_m);

	
	for (e = indice * P; e < ((indice + 1) * P); e++)
	{
		int total_elementos = (lin_m * col_m);

		if (e < total_elementos)
		{
			linha = e / lin_m;
			coluna = e % col_m;
			fprintf(file, "%d;", matriz_resultado_global[linha][coluna]);
		}
	}

	fprintf(file, "\n%fms;", time_spent);
	fclose(file);
	free(nome_arquivo);

	printf("Time spend thread#%d = %f ms\n", (int)(size_t)i, time_spent);
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{

	int n1, m1, n2, m2, i, j, k;
	FILE *file1, *file2, *file3;


	printf("----------------------\n");

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
	matriz_1 = AlocaMatriz(n1, m1);

	for (i = 0; i < n1; i++)
	{
		for (j = 0; j < m1; j++)
		{
			fscanf(file1, "%d;", &matriz_1[i][j]);
		}
	}
	// Fechando o arquivo
	fclose(file1);


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
	matriz_2 = AlocaMatriz(n2, m2);

	for (i = 0; i < n2; i++)
	{
		for (j = 0; j < m2; j++)
		{
			fscanf(file2, "%d;", &matriz_2[i][j]);
		}
	}
	// Fechando o arquivo
	fclose(file2);


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

	// Alocando dinamicamente a matriz global
	matriz_resultado_global = AlocaMatriz(lin_m, col_m);

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
		status = pthread_create(&threads[i], NULL, multiplica_matrizes, (void *)(size_t)i);

		if (status != 0)
		{
			printf("Erro na criação da thread!\n");
			return 1;
		}
	}

	

	for (i = 0; i < quantidade_threads; i++)
	{
		pthread_join(threads[i], &thread_return);
	}

	// Liberando espacos da memoria alocada dinamicamente
	DesalocarMatriz(matriz_resultado_global, lin_m);
	DesalocarMatriz(matriz_1, n1);
	DesalocarMatriz(matriz_2, n2);

	return 0;
}