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
    OR,
    EOS    
    }TAtomo;

typedef struct{
    TAtomo atomo;
    int linha;
    char atributo_id[16];  // Para identificadores (e possivelmente para palavras reservadas)
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
void consome( TAtomo atomo );
void program();
void compound_stmt();


int main(void){
    
    info_atomo = obter_atomo();
    lookahead = info_atomo.atomo;
    

    program();
    consome(EOS);


    printf("fim de análise léxica\n"); 

}


//######## IMPLEMENTACAO LEXICO 
TInfoAtomo obter_atomo(void){
    TInfoAtomo info_atomo;
    info_atomo.atomo = ERRO;
    //eliminar delimitadores
    while(*buffer == ' ' || *buffer == '\n' || *buffer == '\r' || *buffer == '\t'){
        if(*buffer=='\n'){
            contaLinha++;
        }
        buffer++;
    }
    if (*buffer == '\0'){
        info_atomo.atomo = EOS;
    }
    if(*buffer == '/' && (*(buffer+1) == '*' || *(buffer+1) == '/')){
        info_atomo = reconhece_comentario();
        info_atomo.linha = contaLinha;
        
    }else if(*buffer == '_' || isalpha(*buffer)){
        info_atomo = reconhece_id();
    }
    // else if(){
    //     info_atomo = reconhece_intconst();
    // }else if(){
    //     info_atomo = reconhece_charconst()
    // }
    info_atomo.linha = contaLinha;
    return info_atomo;
}


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
    if(*buffer == '\n'){
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
    info_comentario.atomo = COMENTARIO;
    strncpy(str_com, ini_com, buffer - ini_com);
    str_com[buffer - ini_com]='\0';
    strcpy(info_atomo.atributo_comentario, str_com);
    return info_comentario;
}



TInfoAtomo reconhece_id(){

    
    TInfoAtomo info_id;
    char str_id[20]; //não sei se é exatamente 20 tem que ver
    char *ini_id;
    info_id.atomo = ERRO;
    ini_id = buffer;
    
    if (islower(*buffer)){
        buffer++;
        goto q1;
    }else if(isupper(*buffer)){
        buffer++;
        goto q1;
    }else if(*buffer=='_'){
        buffer++;
        goto q1;
    }
q1:
    if (islower(*buffer) || isupper(*buffer) ||isdigit(*buffer) || *buffer =='_'){
        buffer ++;
        goto q1;
    }
    if (*buffer == '\0'){
        return info_id;
    }
    

    info_id.atomo = IDENTIFICADOR;
    
    if (strcmp(str_id, "char") == 0){
        info_id.atomo = CHAR;
    }
    strncpy(str_id, ini_id, buffer - ini_id);
    str_id[buffer - ini_id]='\0';
    strcpy(info_atomo.atributo_id, str_id);
    return info_id;
}
//######## FIM IMPLEMENTACAO LEXICO 


//######## IMPLEMENTACAO SINTATICO 
// <program> ::= void main ‘(‘ void ‘)’ <compound_stmt> 
void program(){
    switch( lookahead ){
        case VOID:
            //consome +
            consome(VOID);
            break;
        case MAIN:
            //consome *
            consome(MAIN);
            break;
        case ABRE_PARENTESES:
            consome(ABRE_PARENTESES);
            break;
        case FECHA_PARENTESES:
            consome(FECHA_PARENTESES);
            break;
        default:
            compound_stmt();
    }
}

void consome( TAtomo atomo ){
    if( lookahead == atomo ){
        info_atomo = obter_atomo();
        lookahead = info_atomo.atomo;
    }
    else{
        printf("\nErro sintatico: esperado [%s] encontrado [%s]\n",strAtomo[atomo],strAtomo[lookahead]);
        exit(1);
    }
}