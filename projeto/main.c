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
    EOS,
    IDENTIFICADOR,
    INTCONST,
    INTCHAR,
    COMENTARIO,
    //Palavras reservadas
    CHAR,
    ELSE,
    IF, 
    INT,
    MAIN,
    READINT,
    VOID,
    WHILE,
    WRITEINT,
    //Delimitadores
    ABRE_PARENTESES,
    FECHA_PARENTESES,
    ABRE_CHAVES,
    FECHA_CHAVES,
    VIRGULA,
    PONTO_VIRGULA,
    //Operadores
    IGUAL, //==
    MENOR,
    MENOR_IGUAL,
    ATRIBUICAO, //=
    DIFERENTE,
    MAIOR,
    MAIOR_IGUAL,
    MAIS,
    MENOS,
    MULTIPLICACAO,
    DIVISAO,
    AND,
    OR    
    }TAtomo;

typedef struct{
    TAtomo atomo;
    int linha;

    char lexema[16];  // Para identificadores (e possivelmente para palavras reservadas)
    int valorInt;     // Para constantes inteiras (intconst) – valor em decimal
    char valorChar;     // Para constantes de caractere (charconst)
    char atributo_comentario;
}TInfoAtomo;


    //declaração de var globais

char *buffer = "    \nvarx     12.4\n  111.90234  \rvar1\n\n\n\n\nv vA";
// char *strAtomo[]={"ERRO","IDENTIFICADOR","NUMERO","EOS"};
int contaLinha = 1;


/*aaaa********************/

// declaracao da funcao
TInfoAtomo obter_atomo();
TInfoAtomo reconhece_id();
TInfoAtomo reconhece_comentario();
TInfoAtomo reconhece_charconst();
TInfoAtomo reconhece_intconst();

int main(void){
    TInfoAtomo info_atm;
    do{
        info_atm = obter_atomo();
        printf("%03d# %s | ", info_atm.linha, strAtomo[info_atm.atomo]);
        // if(info_atm.atomo == NUMERO){
        //     printf("%.2f", info_atm.atributo_numero);
        // }
        // printf("\n");
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


TInfoAtomo reconhece_char_const(){

} 

TInfoAtomo reconhece_int_const(){
    
}


TInfoAtomo reconhece_comentario(){

    TInfoAtomo info_comentario;
    char str_com[20];
    char *ini_com;
    info_comentario.atomo = ERRO;
    ini_com = buffer;
// qo:
    if(*buffer == '/'){
        buffer++; 
        goto q1;
    }
    return info_comentario;
q1:
    if(*buffer == '*'){
        buffer++; 
        goto q2;
    }
    else if(*buffer == '/'){
        buffer++; 
        goto q3;
    }
    return info_comentario;
q2:
    if(*buffer == '*'){
        buffer++;
        goto q4;
    }else{
        buffer++;
        goto q2;
    }
    return info_comentario;
q3:
    if(isdigit(*buffer)){
        buffer++;
        goto q3;
    }
    else if(isalpha(*buffer)){
        return info_num;
    }

q4:
    if(*buffer == '/'){
        return info_comentario;
    }else{
        buffer++;
        goto q2;
    }
    return info_comentario;



    info_comentario.atomo = COMENTARIO;



    //man strncpy
    strncpy(str_com, ini_com, buffer - ini_com);
    str_com[buffer - ini_com]='\0';
    info_comentario.atributo_comentario = atof(str_com);
    return info_comentario;


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