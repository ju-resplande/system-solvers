#include <stdio.h>
#include <stdlib.h>
#define LINHA 0

int determinante(int ordem, int** matriz);

int main(){
    int i, j, num,**matriz = NULL, fila;

    printf("Insira o tamanho da matriz quadrada: ");
    scanf("%d", &num);

    matriz = (int**) malloc(num*sizeof(int*));
    for (i = 0; i < num; i++)
        matriz[i] = (int*) malloc(num*sizeof(int));


    printf("\nInsira os elementos da matriz\n");

    for (i = 0; i < num; i++)
        for (j = 0; j < num; j++)
            scanf("%d", &matriz[i][j]);
   
    printf("\nTomando sempre a %d linha\npara calcular o cofator.\n", LINHA+1);
    printf("O determinante é %d\n", determinante(num, matriz));

    
    for (i = 0; i < num; i++)
        free(matriz[i]);
    free(matriz);

    return 0;
}

int determinante(int ordem, int** matriz){
    int i, j, k, ***cofatores = NULL, a, b, resultado;

    printf("Matriz de ordem %d\n", ordem);

    for (i = 0; i < ordem; i++){
        for (j = 0; j < ordem; j++){
            printf("%2d ", matriz[i][j]);
        }
        printf("\n");
    }

    printf("\n");

    if (ordem == 1) //Se só tem um elemento retorna eles
        return matriz[0][0];

    /*
    if (ordem == 2) //Só pra aumentar a velocidade -> não-obrigatório
        return matriz[0][0]*matriz[1][1] - matriz[0][1]*matriz[1][0];

    if (ordem == 3){ //Só pra aumentar a velocidade -> não-obrigatório
        return
         matriz[0][0]*matriz[1][1]*matriz[2][2]
        +matriz[0][1]*matriz[1][2]*matriz[2][0]
        +matriz[0][2]*matriz[1][0]*matriz[2][1]
        -matriz[0][2]*matriz[1][1]*matriz[2][0]
        -matriz[0][0]*matriz[1][2]*matriz[2][1]
        -matriz[0][1]*matriz[1][0]*matriz[2][2];
    }
    */
    

    /* Alocacao dos cofatores*/
    cofatores = (int***) malloc(ordem*sizeof(int**));
    for (i = 0; i < ordem; i++){
        cofatores[i] = (int**) malloc((ordem-1)*sizeof(int*));

        for (j = 0; j < ordem-1; j++)
            cofatores[i][j] = (int*) malloc((ordem-1)*sizeof(int));
    }
    /* Alocacao dos cofatores*/

    for (k = 0; k < ordem; k++){
        a = 0;
        for (i = 0; i < ordem; i++){
            b = 0;

            if (i != LINHA){
                for (j = 0; j < ordem; j++){
                    if (j != k){
                        cofatores[k] [a][b] = matriz[i][j];
                        b++;
                    }
                }
                a++;
            }
        }
    }

    printf("Cálculo do cofator: \n");

    //Imprimindo os cofatores    

    for (i = 0; i < ordem-1; i++){
        for (k = 0; k < ordem; k++){
            if (!i){
                if ((k+LINHA)%2)
                    printf("-%d * ", matriz[LINHA][k]);
                else
                    printf("+%d * ", matriz[LINHA][k]);
            }
            else
                printf("     ");

            for (j = 0; j < ordem-1; j++)
                printf("%2d ", cofatores[k] [i][j]);

            printf(" ");
        }
        
        printf("\n");
    }

    printf("\n");


    resultado = 0;
    for (i = 0; i < ordem; i++){
        if  ((i+LINHA)%2)
            resultado -= matriz[LINHA][i]*determinante(ordem-1, cofatores[i]);
        else
            resultado += matriz[LINHA][i]*determinante(ordem-1, cofatores[i]); 
    }


    /* Liberacao dos cofatores*/
    for (i = 0; i < ordem; i++){
        for (j = 0; j < ordem-1; j++)
            free(cofatores[i][j]);

        free(cofatores[i]);
    }

    free(cofatores);
    /* Liberacao dos cofatores*/

    return resultado;
}