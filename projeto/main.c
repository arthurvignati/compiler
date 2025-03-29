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
gcc main.c -Wall -Og -g -o main
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
    CHARCONST,
    COMENTARIO,
    CHAR,
    ELSE,
    IF, 
    INT,
    MAIN,
    READINT,
    VOID,
    WHILE,
    WRITEINT,
    ABRE_PARENTESES,
    FECHA_PARENTESES,
    ABRE_CHAVES,
    FECHA_CHAVES,
    VIRGULA,
    PONTO_VIRGULA,
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
    char *strAtomo[]={"ERRO","IDENTIFICADOR","INTCONST","CHARCONST","COMENTARIO","CHAR","ELSE","IF","INT","MAIN","READINT","VOID","WHILE","WRITEINT","(",")","{","}",",",";","==","<","<=","=","!=",">",">=","+","-","*","/","&&","||","EOS"};

typedef struct{
    TAtomo atomo;
    int linha;
    char atributo_id[16];  // Para identificadores (e possivelmente para palavras reservadas)
    char atributo_comentario[384];
    int valorInt;     // Para constantes inteiras (intconst) – valor em decimal
    char valorChar;     // Para constantes de caractere (charconst)
}TInfoAtomo;


//declaração de var globais
char *buffer = "    void main ( void )     12.4\n  111.90234  \rvar1\n\n\n\n\nv vA";
    
    
int contaLinha = 1;
/*aaaa********************/

// declaracao da funcao
TInfoAtomo obter_atomo();
TInfoAtomo reconhece_id();
TInfoAtomo reconhece_comentario();
// TInfoAtomo reconhece_charconst();
// TInfoAtomo reconhece_intconst();
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
void var_decl();
void type_specifier();
void var_decl_list();
void variable_id();
void stmt();
void assig_stmt();
void cond_stmt();
void while_stmt();
void expr();
void conjunction();
void comparison();
void relation();
void sum();
void term();
void factor();




int main(void){
    printf("Inicio do programa\n");
    info_atomo = obter_atomo();
    printf("atomo encontrado: %s\n", strAtomo[info_atomo.atomo]);
    lookahead = info_atomo.atomo;
    printf("lookahead encontrado: %s\n", strAtomo[lookahead]);

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
    if (*buffer == '('){
        info_atomo.atomo = ABRE_PARENTESES;
    }
    if (*buffer == ')'){
        info_atomo.atomo = FECHA_PARENTESES;
    }
    if (*buffer == '{'){
        info_atomo.atomo = ABRE_CHAVES;
    }
    if (*buffer == '}'){
        info_atomo.atomo = FECHA_CHAVES;
    }
    if (*buffer == ','){
        info_atomo.atomo = VIRGULA;
    }
    if (*buffer == ';'){
        info_atomo.atomo = PONTO_VIRGULA;
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


// TInfoAtomo reconhece_charconst(){

// } 

// TInfoAtomo reconhece_intconst(){
    
// }


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
    char str_id[15]; 
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
    
    strncpy(str_id, ini_id, buffer - ini_id);
    str_id[buffer - ini_id]='\0';
    strcpy(info_atomo.atributo_id, str_id);


    if (strcmp(str_id, "char") == 0){
        info_id.atomo = CHAR;
    }
    if (strcmp(str_id, "else") == 0){
        info_id.atomo = ELSE;
    }
    if (strcmp(str_id, "if") == 0){
        info_id.atomo = IF;
    }
    if (strcmp(str_id, "int") == 0){
        info_id.atomo = INT;
    }
    if (strcmp(str_id, "main") == 0){
        info_id.atomo = MAIN;
    }
    if (strcmp(str_id, "readint") == 0){
        info_id.atomo = READINT;
    }
    if (strcmp(str_id, "void") == 0){
        info_id.atomo = VOID;
    }
    if (strcmp(str_id, "while") == 0){
        info_id.atomo = WHILE;
    }
    if (strcmp(str_id, "writeint") == 0){
        info_id.atomo = WRITEINT;
    }
    printf("strf_id: [%s]\n", str_id);
    return info_id;
}
//######## FIM IMPLEMENTACAO LEXICO 


//######## IMPLEMENTACAO SINTATICO 
// <program> ::= void main ‘(‘ void ‘)’ <compound_stmt> 
void program(){
    consome(VOID);
    consome(MAIN);
    consome(ABRE_PARENTESES);
    consome(VOID);
    consome(FECHA_PARENTESES);
    compound_stmt();
}

void compound_stmt(){
    consome(ABRE_CHAVES);
    var_decl();
    while(lookahead == ABRE_CHAVES || lookahead == IDENTIFICADOR || lookahead == IF || lookahead == WHILE || lookahead == READINT || lookahead == WRITEINT ){
        stmt();
    }
    consome(FECHA_CHAVES);
}


void var_decl(){
    if (lookahead == CHAR || lookahead == INT /*|| lookahead == WRITEINT*/){
        type_specifier();
        var_decl_list();
        consome(PONTO_VIRGULA);
    }
}

void type_specifier(){
    if (lookahead == INT){
        consome(INT);
    }else{
        consome(CHAR);
    }
}

void var_decl_list(){
    variable_id();
    while (lookahead == IDENTIFICADOR){
        consome(VIRGULA);
        variable_id();
    }
}


void variable_id(){
    consome(IDENTIFICADOR);
    if (lookahead == ATRIBUICAO){
        consome(ATRIBUICAO);
        expr();
    }
}

void stmt(){
    if (lookahead == ABRE_CHAVES){
        compound_stmt();
    } else if(lookahead == IDENTIFICADOR){
        assig_stmt();
    } else if(lookahead == IF){
        cond_stmt();
    } else if(lookahead == WHILE){
        while_stmt();
    } else if(lookahead == READINT){
        consome(READINT);
        consome(ABRE_PARENTESES);
        consome(IDENTIFICADOR);
        consome(FECHA_PARENTESES);
        consome(PONTO_VIRGULA);
    } else if(lookahead == WRITEINT){
        consome(WRITEINT);
        consome(ABRE_PARENTESES);
        expr();
        consome(FECHA_PARENTESES);
        consome(PONTO_VIRGULA);
    } 
}

void assig_stmt(){
    consome(IDENTIFICADOR);
    consome(ATRIBUICAO);
    expr();
    consome(PONTO_VIRGULA);
}

void cond_stmt(){
    consome(IF);
    consome(ABRE_PARENTESES);
    expr();
    consome(FECHA_PARENTESES);
    stmt();
    if (lookahead == ELSE){
        consome(ELSE);
        stmt();
    }
}

void while_stmt(){
    consome(WHILE);
    consome(ABRE_PARENTESES);
    expr();
    consome(FECHA_PARENTESES);
    stmt();
}

void expr(){
    conjunction();
    while (lookahead == OR ){
        consome(OR);
        conjunction();
    }
}

void conjunction(){
    comparison();
    while (lookahead == AND){
        consome(AND);
        comparison();
    }
}

void comparison(){
    sum();
    if (lookahead == MENOR || lookahead == MENOR_IGUAL || lookahead == IGUAL || lookahead == DIFERENTE || lookahead == MAIOR || lookahead == MAIOR_IGUAL){
        relation();
        sum();
    }
}

void relation(){
    switch (lookahead)
    {
    case MENOR:
        consome(MENOR);
        break;
    case MENOR_IGUAL:
        consome(MENOR_IGUAL);
        break;
    case IGUAL:
        consome(IGUAL);
        break; 
    case DIFERENTE:
        consome(DIFERENTE);
        break; 
    case MAIOR:
        consome(MAIOR);
        break; 
    case MAIOR_IGUAL:
        consome(MAIOR_IGUAL);
        break; 
    default:
        break;
    }
}

void sum(){
    term();
    while(lookahead == MAIS || lookahead == MENOS){
        if(lookahead == MAIS){
            consome(MAIS);
        }else{
            consome(MENOS);
        }
        term();
    }
}

void term(){
    factor();
    while(lookahead == MULTIPLICACAO || lookahead == DIVISAO){
        if(lookahead == MULTIPLICACAO){
            consome(MULTIPLICACAO);
        }else{
            consome(DIVISAO);
        }
        factor();
    }
}

void factor(){
    if(lookahead == INTCONST){
        consome(INTCONST);
    }else if(lookahead == CHARCONST){
        consome(CHARCONST);
    }else{
        consome(IDENTIFICADOR);
    }
    consome(ABRE_PARENTESES);
    expr();
    consome(FECHA_PARENTESES);
}


void consome( TAtomo atomo ){
    if( lookahead == atomo ){
        printf("Consumindo [%s]\n",strAtomo[lookahead]);
        info_atomo = obter_atomo();
        lookahead = info_atomo.atomo;
        printf("Novo lookahead [%s]\n",strAtomo[lookahead]);
    }
    else{
        printf("\nErro sintatico: esperado [%s] encontrado [%s]\n",strAtomo[atomo],strAtomo[lookahead]);
        exit(1);
    }
}