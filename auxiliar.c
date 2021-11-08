#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Será utilizado na multiplicação das matrizes
/*
typedef struct matriz {
	int **mat_data;
	int linhas;
	int colunas;
} matriz;



matriz* create_mat(int n_linhas, int n_colunas, int mat_raw[n_linhas][n_colunas]){
	matriz* MAT;
	MAT = malloc(sizeof(struct matriz)*1); // Alocando dinamicamente espaço para 1 Matriz

	MAT->mat_data=malloc(sizeof(int*)*n_linhas);

	for(int i=0; i<n_linhas; i++){
		MAT->mat_data[i] = malloc(sizeof(int)*n_colunas);
	}

	for(int i=0; i<n_linhas; i++){
		for(int j=0; j<n_colunas; j++){
			MAT->mat_data[i][j] = mat_raw[i][j];
		}
	}
	MAT->linhas = n_linhas;
	MAT->colunas = n_colunas;

	return MAT;
}*/

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

	int mat_raw_M1[n1][m1];
	int mat_raw_M2[n2][m2];

	// Permite que os numeros aleatorios sejam de fato aleatorios
	srand(time(NULL));

	printf("\n***Matriz M1***\n");
	for(int i=0; i<n1; i++){
		for(int j=0; j<m1; j++){
			printf("%d ", mat_raw_M1[i][j]=rand()%100);
			fprintf(file1, "%d;", mat_raw_M1[i][j]);
		}
		printf("\n");
		fprintf(file1, "\n");
	}


	printf("\n***Matriz M2***\n");
	for(int i=0; i<n2; i++){
		for(int j=0; j<m2; j++){
			printf("%d ", mat_raw_M2[i][j]=rand()%100);
			fprintf(file2, "%d;", mat_raw_M2[i][j]);
		}
		printf("\n");
		fprintf(file2, "\n");
	}

	// Salvando e fechando os arquivos
	fclose(file1); 
	fclose(file2);

	return 0;
}
