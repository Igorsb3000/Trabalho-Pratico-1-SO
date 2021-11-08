#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]){

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


	printf("\nMatriz 1:\n\n");
	for(i=0; i<n1; i++){
		for(j=0; j<m1; j++){
			printf("%d;", matriz_1[i][j]);
		}
		printf("\n");
	}



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


	printf("\nMatriz 2:\n\n");
	for(i=0; i<n2; i++){
		for(j=0; j<m2; j++){
			printf("%d;", matriz_2[i][j]);
		}
		printf("\n");
	}

	// Verificando se a multiplicacao das matriz m1 e m2 eh possivel
	if(n1 != m2){
		printf("\nNão é possivel multiplicar as matrizes m1 e m2!\n");
		return 1;
	}


	/* Multiplicando as Matrizes */ 

	//Defining Threads
    int quantidade_threads = atoi(argv[2]);
    pthread_t threads[quantidade_threads];
    int status, i;
    void *thread_return;

    lin_m = n1; // Quantidade de linhas da matriz m1
	col_m = m2; // Quantidade de colunas da matriz m2

	int matriz_resultado[lin_m][col_m];

	printf("\nMontando matriz resultado...\n");
	clock_t begin = clock();

    for(i=0; i<lin_m; i++){
		for(j=0; j<col_m; j++){
			matriz_resultado[i][j] = 0;
			for(k=0; k<m1; k++){
				matriz_resultado[i][j] += matriz_1[i][k] * matriz_2[k][j];
			}
		}
	}
    
    // for(i=0; i<quantidade_threads; i++){
        
        
    // }


    
    

	return 0;
}