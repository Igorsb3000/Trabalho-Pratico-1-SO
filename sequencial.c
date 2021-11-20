#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

/*
*	Autores: Igor Silva e Wesley Gurgel
*/

int i, j, **matriz_1, **matriz_2, **matriz_resultado;

int main(int argc, char *argv[]){
	struct timeval begin, end;
	double time_spent;
	int n1, m1, n2, m2, lin_m, col_m, i, j, k;
	FILE *file1, *file2, *file3;

	// Verificando se os nomes dos 2 arquivos foram passados na linha de comando
	if(argc < 3){
		printf("Informe os nomes dos 2 arquivos que contém as matrizes m1 e m2!\n");
		return 1;
	}


	/* Lendo Matriz 1 */

	// Abrindo os arquivos para leitura das matrizes
	file1 = fopen(argv[1], "r");
	if(file1 == NULL){
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

	for(i=0; i<n1; i++){
		for(j=0; j<m1; j++){
			fscanf(file1, "%d;", &matriz_1[i][j]);
		}
	}
	// Fechando o arquivo
	fclose(file1); 


	/* Lendo Matriz 2 */

	// Abrindo os arquivos para leitura das matrizes
	file2 = fopen(argv[2], "r");
	if(file2 == NULL){
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

	for(i=0; i<n2; i++){
		for(j=0; j<m2; j++){
			fscanf(file2, "%d;", &matriz_2[i][j]);
		}
	}
	// Fechando o arquivo
	fclose(file2); 


	// Verificando se a multiplicacao das matriz m1 e m2 eh possivel
	if(m1 != n2){
		printf("\nNão é possivel multiplicar as matrizes m1 e m2!\n");
		return 1;
	}


	/* Multiplicando as Matrizes */ 

	lin_m = n1; // Quantidade de linhas da matriz m1
	col_m = m2; // Quantidade de colunas da matriz m2

	//int matriz_resultado[lin_m][col_m];

	matriz_resultado = malloc(lin_m * sizeof(int *));
    for (i = 0; i < col_m; i++)
    {
        matriz_resultado[i] = malloc(col_m * sizeof(int));
    }

	// Tempo inicial de execucao da thread
    gettimeofday(&begin, NULL);

	for(i=0; i<lin_m; i++){
		for(j=0; j<col_m; j++){
			matriz_resultado[i][j] = 0;
			for(k=0; k<m1; k++){
				matriz_resultado[i][j] += matriz_1[i][k] * matriz_2[k][j];
			}
		}
	}

	// Tempo final da execucao da thread
    gettimeofday(&end, NULL);
    time_spent = (end.tv_sec - begin.tv_sec) * 1000.0;
    time_spent += (end.tv_usec - begin.tv_usec) / 1000.0;
	printf("\nTime spend sequencial =  %f ms\n", time_spent);


	/* Escrevendo o Resultado da Multiplicacao */

	file3 = fopen("resultado_sequencial.csv", "w");
	fprintf(file3, "%d %d\n", lin_m, col_m);
	for(int i=0; i<lin_m; i++){
		for(int j=0; j<col_m; j++){
			fprintf(file3, "c%d", i+1);
			fprintf(file3, "%d %d", j+1, matriz_resultado[i][j]);
			fprintf(file3, "\n");
		}
	}

	fprintf(file3, "%fms;", time_spent);
	fclose(file3); 

	// Liberando espacos da memoria alocada dinamicamente
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
