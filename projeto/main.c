/******************************************************************************

Welcome to GDB Online.
  GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
  C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, COBOL, HTML, CSS, JS
  Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/

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


//######## DECLARACOES LEXICO
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
char *strAtomo[]={"ERRO","IDENTIFICADOR", "INTCONST","INTCHAR",
    "COMENTARIO",
    "CHAR",
    "ELSE",
    "IF", 
    "INT",
    "MAIN",
    "READINT",
    "VOID",
    "WHILE",
    "WRITEINT",
    "ABRE_PARENTESES",
    "FECHA_PARENTESES",
    "ABRE_CHAVES",
    "FECHA_CHAVES",
    "VIRGULA",
    "PONTO_VIRGULA",  
    "==",
    "<",
    "<=",
    ">=",
    ">",
    "-",
    "*",
    "/",
    "AND",
    "OR","EOS"};
    
    


int contaLinha = 1;
/*aaaa********************/

// declaracao da funcao
TInfoAtomo obter_atomo();
TInfoAtomo reconhece_id();
TInfoAtomo reconhece_comentario();
TInfoAtomo reconhece_charconst();
TInfoAtomo reconhece_intconst();
//######## FIM DAS DECLARACOES LÉXICO 




//######## DECLARACOES SINTATICO 
// variavel global
TAtomo lookahead;
TInfoAtomo info_atomo;


// E ::= a | b | +EE | *EE

// SINTATICO - prototipacao de funcao
// void E(); 
void consome( TAtomo atomo );


int main(void){
    
    info_atomo = obter_atomo();
    lookahead = info_atomo.atomo;
    
    
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
    if(*entrada == '/'){
        info_atomo.atomo = reconhece_comentario();
        
    }else if(*entrada == '_' || isalpha(*entrada))){
        info_atomo = reconhece_id();
    }
    else if(){
        info_atomo = reconhece_intconst();
    }else if(){
        info_atomo = reconhece_charconst()
    }
    info_atomo.linha = contaLinha;
    return info_atomo;
}


// implementacao da funcao
//NUMERO -> DIGITO+ .DIGITO+ 


TInfoAtomo reconhece_charconst(){

} 

TInfoAtomo reconhece_intconst(){
    
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
    if(*buffer == '\n')){
        return info_comentario;
    }
    else{
        buffer++;
        goto q3;
    }

q4:
    if(*buffer == '/'){
        return info_comentario;
    }else{
        buffer++;
        goto q2;
    }
    // return info_comentario;



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
    char str_id[20]; //não sei se é exatamente 20 tem que ver
    char *ini_id;
    info_id.atomo = ERRO;
    ini_id = buffer;


    info_id.atomo = ERRO;

    if(*entrada=="char"){
        info_id.atomo = CHAR;
        return info_id;
    }
    if(*entrada=="else"){
        info_id.atomo = ELSE;
        return info_id;
    }
    if(*entrada=="if"){
        info_id.atomo = IF;
        return info_id;
    }
    if(*entrada=="int"){
        info_id.atomo = INT;
        return info_id;
    }
    if(*entrada=="main"){
        info_id.atomo = MAIN;
        return info_id;
    }
    if(*entrada=="readint"){
        info_id.atomo = READINT;
        return info_id;
    }
    if(*entrada=="void"){
        info_id.atomo = VOID;
        return info_id;
    }
    if(*entrada=="while"){
        info_id.atomo = WHILE;
        return info_id;
    }
    if(*entrada=="writeint"){
        info_id.atomo = WRITEINT;
        return info_id;
    }
    
    
    if (islower(*entrada)){
        entrada++;
        goto q1;
    }else if(isupper(*entrada)){
        entrada++;
        goto q1;
    }else if(*entrada=='_'){
        entrada++;
        goto q1;
    }
q1:
    if (islower(*entrada) || isupper(*entrada) ||isdigit(*entrada) || *entrada =='_'){
        entrada ++;
        goto q1;
    }
    if (*entrada == '\0'){
        return info_id;
    }
    
    
    // return info_id;

    info_id.atomo = IDENTIFICADOR;
    
    
    strncpy(str_id, ini_id, buffer - ini_id);
    str_id[buffer - ini_id]='\0';
    info_id.lexema = atof(str_id);
    return info_id;
    
}
