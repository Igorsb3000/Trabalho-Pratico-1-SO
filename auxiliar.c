#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
*	Autores: Igor Silva e Wesley Gurgel
*/

int i, j, **matriz_1, **matriz_2;


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

	matriz_1 = AlocaMatriz(n1, m1);

	//printf("\n***Matriz M1***\n");
	for(int i=0; i<n1; i++){
		for(int j=0; j<m1; j++){
			//printf("%d ", matriz_1[i][j]=rand()%100);
			matriz_1[i][j]=rand()%100;
			fprintf(file1, "%d;", matriz_1[i][j]);
		}
		//printf("\n");
		fprintf(file1, "\n");
	}

	matriz_2 = AlocaMatriz(n2, m2);

	//printf("\n***Matriz M2***\n");
	for(int i=0; i<n2; i++){
		for(int j=0; j<m2; j++){
			//printf("%d ", matriz_2[i][j]=rand()%100);
			matriz_2[i][j]=rand()%100;
			fprintf(file2, "%d;", matriz_2[i][j]);
		}
		//printf("\n");
		fprintf(file2, "\n");
	}

	// Salvando e fechando os arquivos
	fclose(file1); 
	fclose(file2);

	// Liberando espacos da memoria alocada dinamicamente
    DesalocarMatriz(matriz_1, n1);
	DesalocarMatriz(matriz_2, n2);

	return 0;
}
