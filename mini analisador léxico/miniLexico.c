/*
Implementa uma função que recebe um ponteiro para vetor de caractere por
parâmetro (somente uma ponteiro), veja a declaração da função:

int miniLexico(char *entrada);

Para compilar no vscode use:
gcc miniLexico.c -Wall -Og -g -o miniLexico
*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

//Definições dos átomos
typedef enum{
    ERRO,
    IDENTIFICADOR,
    NUMERO,
    EOS
    }TAtomo;

typedef struct{
    TAtomo atomo;
    int linha;
    float atributo_numero;
    char atributo_ID[16];
}TInfoAtomo;


    //declaração de var globais
char *strAtomo[]={"ERRO","IDENTIFICADOR","NUMERO","EOS"};
char *entrada = "    \nvarx     12.4\n  111.90234  \rvar1\n\n\n\n\nv vA";
int contaLinha = 1;


// declaracao da funcao
TInfoAtomo obter_atomo();
TInfoAtomo reconhece_num();
TInfoAtomo reconhece_id();

int main(void){
    TInfoAtomo info_atm;
    do{
        info_atm = obter_atomo();
        printf("%03d# %s | ", info_atm.linha, strAtomo[info_atm.atomo]);
        if(info_atm.atomo == NUMERO){
            printf("%.2f", info_atm.atributo_numero);
        }
        printf("\n");
    }while (info_atm.atomo != ERRO && info_atm.atomo != EOS);
    printf("fim de análise léxica\n"); 

}

TInfoAtomo obter_atomo(void){
    TInfoAtomo info_atomo;
    info_atomo.atomo = ERRO;
    //eliminar delimitadores
    while(*entrada == ' ' || *entrada == '\n' || *entrada == '\r' || *entrada == '\t'){
        if(*entrada=='\n'){
            contaLinha++;
        }
        entrada++;
    }
    if (*entrada == '\0'){
        info_atomo.atomo = EOS;
    }
    if(isdigit(*entrada)){
        info_atomo = reconhece_num();
    }
    else if(islower(*entrada)){
        info_atomo = reconhece_id();
    }
    info_atomo.linha = contaLinha;
    return info_atomo;
}


// implementacao da funcao
//NUMERO -> DIGITO+ .DIGITO+ 

TInfoAtomo reconhece_num(){
    TInfoAtomo info_num;
    char str_num[20];
    char *ini_num;
    info_num.atomo = ERRO;

    ini_num = entrada;
// qo:
    if(isdigit(*entrada)){
        entrada++; 
        goto q1;
    }
    return info_num;
q1:
    if(isdigit(*entrada)){
        entrada++; 
        goto q1;
    }
    else if(*entrada == '.'){
        entrada++; 
        goto q2;
    }
    return info_num;
q2:
    if(isdigit(*entrada)){
        entrada++;
        goto q3;
    }
    return info_num;
q3:
    if(isdigit(*entrada)){
        entrada++;
        goto q3;
    }
    else if(isalpha(*entrada)){
        return info_num;
    }
    info_num.atomo = NUMERO;

    //man strncpy
    strncpy(str_num, ini_num, entrada - ini_num);
    str_num[entrada - ini_num]='\0';
    info_num.atributo_numero = atof(str_num);
    return info_num;

}



// implementacao da funcao
//IDENTIFICADOR -> LETRA_MINUSCULA(LETRA_MINUSCULA|DIGITO)

TInfoAtomo reconhece_id(){
    TInfoAtomo info_id;
    info_id.atomo = ERRO;

    
    if (islower(*entrada)){
        entrada++;
        goto q1;
    }
q1:
    if (islower(*entrada) || isdigit(*entrada)){
        entrada ++;
        goto q1;
    }
    if (isupper(*entrada)){
        return info_id;
    }
    info_id.atomo = IDENTIFICADOR;
    return info_id;

}