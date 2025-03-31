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
char *buffer; //= "    void main ( void ) { if    12.4\n  111.90234  \rvar1\n\n\n\n\nv vA";
char *read_file(const char *file_name);
    
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



int main( int argc, char *argv[]) {
    if (argc < 2){
        printf("Erro: faltou arquivo!");
        return 0;
    }
    buffer = read_file(argv[1]);
    printf("Inicio do programa\n");
    info_atomo = obter_atomo();
    
    lookahead = info_atomo.atomo;

    program();
    // obter_atomo();
    consome(EOS);
    
    printf("%d linhas analisadas, programa sintaticamente correto\n", contaLinha);
    return 0;
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
    if (*buffer == '\0' || *buffer == -1){

        info_atomo.atomo = EOS;
    } 
    else if (*buffer == '('){
        buffer++;
        info_atomo.atomo = ABRE_PARENTESES;
    }
    else if (*buffer == ')'){
        buffer++;
        info_atomo.atomo = FECHA_PARENTESES;
    }
    else if (*buffer == '{'){
        buffer++;
        info_atomo.atomo = ABRE_CHAVES;
    }
    else if (*buffer == '}'){
        buffer++;
        info_atomo.atomo = FECHA_CHAVES;
    }
    else if (*buffer == ','){
        buffer++;
        info_atomo.atomo = VIRGULA;
    }
    else if (*buffer == ';'){
        buffer++;
        info_atomo.atomo = PONTO_VIRGULA;
    }    
    else if (*buffer == '=' && (*(buffer+1) == '=')){
        buffer++;
        info_atomo.atomo = IGUAL;
    }
    else if (*buffer == '=' && (*(buffer+1) != '=')){
        buffer++;
        info_atomo.atomo = ATRIBUICAO;
    }
    else if (*buffer == '<' && (*(buffer+1) == '=')){
        buffer++;
        info_atomo.atomo = MENOR_IGUAL;
    }
    else if (*buffer == '<' && (*(buffer+1) != '=')){
        buffer++;
        info_atomo.atomo = MENOR;
    }
    else if (*buffer == '>' && (*(buffer+1) == '=')){
        buffer++;
        info_atomo.atomo = MAIOR_IGUAL;
    }
    else if (*buffer == '>' && (*(buffer+1) != '=')){
        buffer++;
        info_atomo.atomo = MAIOR;
    }
    else if (*buffer == '+'){
        buffer++;
        info_atomo.atomo = MAIS;
    }
    else if (*buffer == '-'){
        buffer++;
        info_atomo.atomo = MENOS;
    }
    else if (*buffer == '*'){
        buffer++;
        info_atomo.atomo = MULTIPLICACAO;
    }
    else if (*buffer == '/' && (*(buffer+1) != '*' && *(buffer+1) != '/')){
        buffer++;
        info_atomo.atomo = DIVISAO;
    }
    else if (*buffer == '&' && (*(buffer+1) == '&')){
        buffer++;
        info_atomo.atomo = AND;
    }
    else if (*buffer == '|' && (*(buffer+1) == '|')){
        buffer++;
        info_atomo.atomo = OR;
    }

    else if(*buffer == '/' && (*(buffer+1) == '*' || *(buffer+1) == '/')){

        printf("# %3d: %s\n", (contaLinha), "comentario");  
        info_atomo = reconhece_comentario();
        info_atomo.linha = contaLinha;
        return obter_atomo();
        
    }else if(*buffer == '\''){
        info_atomo = reconhece_charconst();
    }else if(*buffer == '0'){
        info_atomo = reconhece_intconst();
    }else if(*buffer == '_' || isalpha(*buffer)){
        info_atomo = reconhece_id();
    }

    info_atomo.linha = contaLinha;
    return info_atomo;
}


TInfoAtomo reconhece_charconst(){
    TInfoAtomo info_charconst;
    char c;
    info_charconst.atomo = ERRO;
    
    if (*buffer != '\'')
        return info_charconst;  
    buffer++; 
    goto q1;
    
q1:
    if (*buffer == '\0' || *buffer == '\'')
        return info_charconst;  
    c = *buffer;  
    if ((unsigned char)c >= 128)
        return info_charconst;  
    buffer++;   
    goto q2;
    
q2:
    if (*buffer == '\'') {
        buffer++;  
        info_charconst.atomo = CHARCONST;
        info_charconst.valorChar = c;
        return info_charconst;
    }
    return info_charconst;  
} 

TInfoAtomo reconhece_intconst(){
    TInfoAtomo info_int;
    info_int.atomo = ERRO;
    char str_hex[20];  
    char *ini = buffer;
    int count = 0;     

    if (*buffer != '0')
        return info_int; 
    buffer++;  
    goto q1;

q1:
    if (*buffer != 'x' && *buffer != 'X')
        return info_int;  
    buffer++; 
    goto q2;

q2:
    if (isdigit(*buffer) || (*buffer >= 'A' && *buffer <= 'F')) {
        buffer++;
        count++;
        goto q2;
    }
    if (count == 0)
        return info_int;  
    
    info_int.atomo = INTCONST;
    {
        int len = buffer - ini;
        if (len > 19)
            len = 19;
        strncpy(str_hex, ini, len);
        str_hex[len] = '\0';
        info_int.valorInt = (int) strtol(str_hex, NULL, 16);
    }
    return info_int;
}


TInfoAtomo reconhece_comentario(){

    TInfoAtomo info_comentario;
    char str_com[384];
    char *ini_com = buffer;
    info_comentario.atomo = ERRO;

    buffer++; 
    goto q1;
q1:
    if(*buffer == '*'){
        buffer++; 
        goto q2;
    }
    else if(*buffer == '/'){
        buffer++; 
        goto q3;
    }
        goto fim;
q2:
    if (*buffer == '\0') {
        goto fim; 
    }
    if(*buffer == '*'){
        buffer++;
        goto q4;
    } 
    if (*buffer == '\n') {
        contaLinha++;
    }
    
    buffer++;
    goto q2;
q3:
    if (*buffer == '\0') {
        goto fim; 
    }
    if(*buffer == '\n'){
        buffer++;
        contaLinha++;
        goto q5;
    }
    buffer++;
    goto q3;

q4:
    if (*buffer == '\0') {
        goto fim; 
    }
    if(*buffer == '/'){
        buffer++;
        goto q5;
    }if (*buffer == '*') {
        buffer++;
        goto q4;
    }if (*buffer == '\n') {
        contaLinha++;
    }
    buffer++;
    goto q2;
q5:
    info_comentario.atomo = COMENTARIO;
    {
        int length = buffer - ini_com;
        if (length > 383) {
            length = 383;  
        }
        strncpy(str_com, ini_com, length);
        str_com[length] = '\0';
        strcpy(info_comentario.atributo_comentario, str_com);
    }
    goto fim;

    info_comentario.atomo = COMENTARIO;
    strncpy(str_com, ini_com, buffer - ini_com);
    str_com[buffer - ini_com]='\0';
    strcpy(info_atomo.atributo_comentario, str_com);
    return info_comentario;

fim:
    return info_comentario;

}



TInfoAtomo reconhece_id(){
    TInfoAtomo info_id;
    char str_id[16]; 
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
    int len = buffer - ini_id;
    if (len > 15) {
        printf("\nErro lexico: na linha [%d] - identificadores so podem ter no maximo 15 caracteres\n",contaLinha);
        exit(1);
    }

    info_id.atomo = IDENTIFICADOR;
    strncpy(str_id, ini_id, buffer - ini_id);
    str_id[buffer - ini_id]='\0';
    strcpy(info_id.atributo_id, str_id);


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
    while(lookahead == CHAR || lookahead == INT) {
        var_decl();
    }
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
    while (lookahead == VIRGULA){
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
    }else if(lookahead == IDENTIFICADOR){
        consome(IDENTIFICADOR);
    }else{
        consome(ABRE_PARENTESES);
        expr();
        consome(FECHA_PARENTESES);        
   }
}


void consome( TAtomo atomo ){
    while(lookahead == COMENTARIO) {
        printf("# %3d: %s | %s\n", info_atomo.linha, strAtomo[lookahead], info_atomo.atributo_comentario);
        info_atomo = obter_atomo();
        lookahead = info_atomo.atomo;
    }
    if( lookahead == atomo ){
        if(lookahead == IDENTIFICADOR) {
            printf("# %3d: %s | %s\n", info_atomo.linha, strAtomo[lookahead], info_atomo.atributo_id);
        } else if(lookahead == INTCONST) {
            printf("# %3d: %s | %d\n", info_atomo.linha, strAtomo[lookahead], info_atomo.valorInt);
        } else {
            printf("# %3d: %s\n", info_atomo.linha, strAtomo[lookahead]);
        }
        info_atomo = obter_atomo();
        lookahead = info_atomo.atomo;
    }
    else{
        printf("\nErro sintatico: esperado [%s] encontrado [%s]\n",strAtomo[atomo],strAtomo[lookahead]);
        exit(1);
    }
}


char *read_file(const char *file_name) {
    FILE *file_ptr = fopen(file_name, "rb");
    if (file_ptr == NULL) {
        fprintf(stderr, "Erro: Não foi possível abrir o arquivo %s\n", file_name);
        exit(1);
    }
    
    fseek(file_ptr, 0, SEEK_END);
    long file_size = ftell(file_ptr);
    rewind(file_ptr);
    
    char *content = malloc(file_size + 1);
    if (content == NULL) {
        fprintf(stderr, "Erro: Falha na alocação de memória.\n");
        fclose(file_ptr);
        exit(1);
    }
    
    size_t bytes_read = fread(content, 1, file_size, file_ptr);
    if (bytes_read != file_size) {
        fprintf(stderr, "Aviso: Nem todos os bytes foram lidos de %s.\n", file_name);
    }
    content[file_size] = '\0';  
    
    fclose(file_ptr);
    return content;
}