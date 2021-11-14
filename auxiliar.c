#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
*	Autores: Igor Silva e Wesley Gurgel
*/

int i, j, **matriz_1, **matriz_2;

int main(int argc, char *argv[]){
	FILE *file1, *file2;
	// Criando ou sobrescrevendo os arquivos
	file1 = fopen("m1.csv", "w");
	file2 = fopen("m2.csv", "w"); 

	// Matriz M1
	int n1 = atoi(argv[1]);
	int m1 = atoi(argv[2]);

	// Matriz M2
	int n2 = atoi(argv[3]);
	int m2 = atoi(argv[4]);
	fprintf(file1, "%d;%d;\n", n1, m1);
	fprintf(file2, "%d;%d;\n", n2, m2);

	// Permite que os numeros aleatorios sejam de fato aleatorios
	srand(time(NULL));

	matriz_1 = malloc(n1 * sizeof(int *));
	for (i = 0; i < m1; i++)
	{
		matriz_1[i] = malloc(m1 * sizeof(int));
	}

	printf("\n***Matriz M1***\n");
	for(int i=0; i<n1; i++){
		for(int j=0; j<m1; j++){
			printf("%d ", matriz_1[i][j]=rand()%100);
			fprintf(file1, "%d;", matriz_1[i][j]);
		}
		printf("\n");
		fprintf(file1, "\n");
	}

	matriz_2 = malloc(n2 * sizeof(int *));
	for (i = 0; i < m2; i++)
	{
		matriz_2[i] = malloc(m2 * sizeof(int));
	}

	printf("\n***Matriz M2***\n");
	for(int i=0; i<n2; i++){
		for(int j=0; j<m2; j++){
			printf("%d ", matriz_2[i][j]=rand()%100);
			fprintf(file2, "%d;", matriz_2[i][j]);
		}
		printf("\n");
		fprintf(file2, "\n");
	}

	// Salvando e fechando os arquivos
	fclose(file1); 
	fclose(file2);

	return 0;
}
