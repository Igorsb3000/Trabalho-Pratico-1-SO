#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

/*
*	Autores: Igor Silva e Wesley Gurgel
*/

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
	int matriz_1[n1][m1];

	for(i=0; i<n1; i++){
		for(j=0; j<m1; j++){
			fscanf(file1, "%d;", &matriz_1[i][j]);
		}
	}
	// Fechando o arquivo
	fclose(file1); 


	// printf("\nMatriz 1:\n\n");
	// for(i=0; i<n1; i++){
	// 	for(j=0; j<m1; j++){
	// 		printf("%d;", matriz_1[i][j]);
	// 	}
	// 	printf("\n");
	// }



	/* Lendo Matriz 2 */
	/* Lendo Matriz 2 */

	// Abrindo os arquivos para leitura das matrizes
	file2 = fopen(argv[2], "r");
	if(file2 == NULL){
		printf("Arquivo %s não encontrado!\n", argv[1]);
		return 0;
	}
	
	// Obtendo as dimensoes da matriz
	fscanf(file2, "%d;%d;", &n2, &m2);
	int matriz_2[n2][m2];

	for(i=0; i<n2; i++){
		for(j=0; j<m2; j++){
			fscanf(file2, "%d;", &matriz_2[i][j]);
		}
	}
	// Fechando o arquivo
	fclose(file2); 


	// printf("\nMatriz 2:\n\n");
	// for(i=0; i<n2; i++){
	// 	for(j=0; j<m2; j++){
	// 		printf("%d;", matriz_2[i][j]);
	// 	}
	// 	printf("\n");
	// }

	// Verificando se a multiplicacao das matriz m1 e m2 eh possivel
	if(m1 != n2){
		printf("\nNão é possivel multiplicar as matrizes m1 e m2!\n");
		return 1;
	}


	/* Multiplicando as Matrizes */ 

	lin_m = n1; // Quantidade de linhas da matriz m1
	col_m = m2; // Quantidade de colunas da matriz m2

	int matriz_resultado[lin_m][col_m];

	// printf("\nMontando matriz resultado...\n");
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
	printf("\nTempo gasto para execução foi de %f ms\n", time_spent);

	// printf("\nMatriz Resultado:\n\n");
	// for(i=0; i<lin_m; i++){
	// 	for(j=0; j<col_m; j++){
	// 		printf("%d;", matriz_resultado[i][j]);
	// 	}
	// 	printf("\n");
	// }


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


	return 0;
}
