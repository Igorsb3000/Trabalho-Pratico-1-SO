#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>


// Variaveis globais
int numero_threads, lin_m, col_m;
int **matriz_resultado_global;

// Funcao para imprimir matriz global que foi alocada dinamicamente
void* imprimir_matriz_global(){
	printf("\n");

	for(int i=0; i<lin_m; i++){
		for(int j=0; j<col_m; j++){
			printf("%d ", matriz_resultado_global[i][j]);
		}
		printf("\n");
	}
}

// Funcao principal das Threads
void* multiplica_matrizes(void* indice){
	sleep(1);
	printf("OII");
	printf("NUMERO THREADS = %d\n", numero_threads);
	sleep(5);
	//pthread_exit(NULL);

}


int main(int argc, char *argv[]){

	int n1, m1, n2, m2, i, j, k;
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
    int P = atoi(argv[3]);
    int status;
    void *thread_return;
    lin_m = n1; // Quantidade de linhas da matriz m1
	col_m = m2; // Quantidade de colunas da matriz m2
	int matriz_resultado[lin_m][col_m];

	// Alocando dinamicamente a matriz global
	matriz_resultado_global = malloc(lin_m * sizeof(int*));
	for(i=0; i<col_m; i++){
		matriz_resultado_global[i] = malloc(col_m * sizeof(int));
	}

	// Multiplicando as matrizes e testando se a matriz global esta funcionando
	for(i=0; i<lin_m; i++){
		for(j=0; j<col_m; j++){
			matriz_resultado[i][j] = 0;
			for(k=0; k<m1; k++){
				matriz_resultado[i][j] += matriz_1[i][k] * matriz_2[k][j];
			}
		}
	}

	for(int i=0; i<lin_m; i++){
		for(int j=0; j<col_m; j++){
			matriz_resultado_global[i][j] = matriz_resultado[i][j];
		}
	}

	imprimir_matriz_global();



	// Fazendo o calculo da quantidade de threads necessarias
	int quantidade_threads = (lin_m * col_m)/P; 
	pthread_t threads[quantidade_threads];


	//clock_t begin = clock();
	numero_threads = quantidade_threads;
	printf("THREADS = %d\n", numero_threads);

	for(i=0; i<quantidade_threads; i++){
		printf("Processo (PID=%d) criando thread #%d\n", getpid(), i);
		status = pthread_create(&threads[i], NULL, multiplica_matrizes, (void *)(size_t)i); //

		if(status != 0){
			printf("Erro na criação da thread!\n");
			return 1;
		}
	}





	// Liberando espeço alocado dinamicamente da matriz global
    for(i=0; i<col_m; i++){
		free(matriz_resultado_global[i]);
	}
	free(matriz_resultado_global);

    
    

	return 0;
}