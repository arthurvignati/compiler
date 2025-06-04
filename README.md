Projeto Parte 1 e 2 - Compilador TINY-C  
Integrante: [Arthur Vignati Moscardi - 10409688]

---

## 1. Descrição Geral  
Este trabalho tem como objetivo implementar as fases de análise léxica, sintática e semântica de um compilador para a linguagem TINY-C (versão simplificada de C), bem como a geração de código intermediário MEPA. A linguagem TINY-C suporta tipos primitivos `int` e `char`, comandos condicionais (`if`) e de repetição (`while`), além das funções de entrada (`readint`) e saída (`writeint`).  

O fluxo completo do compilador agora contempla:

1. **Análise Léxica**: Reconhecimento de tokens a partir do código-fonte.
2. **Análise Sintática**: Validação estrutural do programa conforme a gramática de TINY-C.
3. **Análise Semântica**: Verificação de tipos, escopo e detecção de erros semânticos (por exemplo, uso de variáveis não declaradas, incompatibilidade de tipos, chamadas de função incorretas).
4. **Geração de Código Intermediário (MEPA)**: Tradução do programa sintaticamente e semanticamente correto para instruções MEPA, que podem ser interpretadas por uma máquina virtual ou traduzidas posteriormente para código de máquina.

Na saída padrão, o compilador imprimirá:
- **Tokens reconhecidos** (com linha e descrição de cada token).  
- **Mensagens de erro léxico** (se houver).  
- **Mensagens de erro sintático** (se o programa não estiver sintaticamente correto).  
- **Mensagens de erro semântico** (se o programa passar na sintaxe, mas violar regras semânticas).  
- **Código MEPA gerado**, caso nenhuma etapa anterior apresente erros.

---

## 2. Estrutura do Código  

O compilador foi dividido em quatro módulos principais:

### 2.1 Analisador Léxico  
- **Função `obter_atomo()`**:  
  - Lê o arquivo-fonte (via ponteiro global `buffer`) e retorna um token do tipo `TInfoAtomo`.  
  - Utiliza autômatos (implementados com `goto`) para reconhecer os diferentes tokens.  
- **Funções de Reconhecimento de Tokens**:  
  - `reconhece_id()`: Reconhece identificadores e palavras reservadas. Impõe limite de 15 caracteres para identificadores (gera erro se exceder).  
  - `reconhece_intconst()`: Reconhece constantes inteiras (decimal e hexadecimal) e converte para valor decimal.  
  - `reconhece_charconst()`: Reconhece constantes de caractere, garantindo um único caractere ASCII entre aspas simples.  
  - `reconhece_comentario()`: Reconhece comentários de linha (`//...`) e bloco (`/* ... */`). Todos os comentários são reportados e descartados antes de continuar a análise.  
- **Vetor de Tokens (`strAtomo`)**:  
  - Array de strings contendo os nomes de todos os tokens reconhecidos.  
  - Usado para exibir, em texto, cada token reconhecido (linha e descrição) e para montar mensagens de erro sintático (“esperado X, encontrado Y”).

### 2.2 Analisador Sintático  
- **Função `consome(token esperado)`**:  
  - Compara o token atual (lookahead) com o token esperado.  
  - Se coincidir, invoca `obter_atomo()` para avançar para o próximo token.  
  - Caso contrário, sinaliza erro sintático (mostrando token esperado vs encontrado).  
  - Ignora tokens de comentário, pois estes já foram descartados no léxico.  
- **Implementação Recursiva da Gramática**:  
  Para cada não-terminal da gramática de TINY-C, existe uma função recursiva correspondente:
  - `program()`  
  - `compound_stmt()`  
  - `var_decl()`  
  - `type_specifier()`  
  - `var_decl_list()`  
  - `variable_id()`  
  - `stmt()`  
  - `assig_stmt()`  
  - `cond_stmt()`  
  - `while_stmt()`  
  - `expr()`  
  - `conjunction()`  
  - `comparison()`  
  - `relation()`  
  - `sum()`  
  - `term()`  
  - `factor()`  

  Todas as chamadas a essas funções usam `consome()` para avançar no stream de tokens.  

---

## 3. Análise Semântica  

Além da análise sintática, implementamos agora uma etapa de **verificação semântica**, responsável por validar regras que não podem ser checadas apenas pela gramática. Os principais aspectos verificados são:

1. **Tabela de Símbolos e Escopo**  
   - Cada vez que uma variável ou função é declarada, seu nome, tipo e, se aplicável, parâmetros são inseridos em uma tabela de símbolos associada a um escopo.  
   - O escopo global engloba as declarações de variáveis e funções fora de qualquer bloco.  
   - Cada `compound_stmt` (bloco entre `{ ... }`) cria um novo escopo que pode aninhar variáveis locais.  
   - Ao finalizar um bloco, todas as variáveis desse escopo são removidas da tabela (pop de escopo).  

2. **Verificação de Declarações**  
   - **Variáveis**:  
     - Certificar que toda variável usada (em atribuição ou expressão) foi previamente declarada.  
     - Garantir que não existam declarações duplicadas no mesmo escopo (mesmo identificador e mesmo tipo).  
   - **Funções** (`readint`, `writeint` e rotinas definidas pelo usuário):  
     - Garantir que a assinatura (nome + número de parâmetros + tipos) corresponda ao uso.  
     - Verificar se chamadas a funções não definidas (exceto `readint` e `writeint`) são sinalizadas como erro.  
     - As funções `readint()` e `writeint(expr)` têm assinaturas fixas:  
       - `readint()` → retorna `int`, não aceita argumentos.  
       - `writeint(expr)` → retorna `void`, aceita exatamente uma expressão de tipo `int`.  

3. **Verificação de Tipos**  
   - **Atribuições**:  
     - Em `ID = expr;`, verificar se o tipo de `expr` é compatível com o tipo da variável `ID`.  
     - Não permitir atribuição `int = char` ou `char = int` sem conversão explícita (no TINY-C não há conversão implícita de `char` para `int` ou vice-versa).  
   - **Operações Aritméticas e Relacionais**:  
     - Operandos de `+`, `-`, `*`, `/` devem ser `int`. Gerar erro se usar `char` diretamente em expressões aritméticas.  
     - Operadores relacionais (`==`, `!=`, `<`, `>`, `<=`, `>=`) exigem operandos de mesmo tipo (`int` ou `char`).  
     - Operadores lógicos (`&&`, `||`, `!`) operam sobre expressões do tipo `int` (0 = false, ≠0 = true).  
   - **Expressões de Controle** (`if`, `while`):  
     - A condição deve ser uma expressão `int` (valor zero ou não-zero).  
   - **Parâmetros de Função**:  
     - Em definições e chamadas, verificar correspondência no número de parâmetros e tipos.

4. **Detecção de Erros Semânticos**  
   O compilador emite mensagem de erro semântico com linha e descrição caso encontre:  
   - Uso de variável/função não declarada.  
   - Declaração duplicada no mesmo escopo.  
   - Atribuição com tipos incompatíveis.  
   - Chamadas a funções com número ou tipo de parâmetros incorretos.  
   - Uso de expressão inválida em condição (`if`, `while`).  
   - Qualquer outra violação de regra de tipo ou escopo conforme especificado pela linguagem TINY-C.

5. **Implementação da Análise Semântica**  
   - Durante a análise sintática, cada vez que uma declaração é reconhecida (`var_decl`, assinatura de `function_decl`), adiciona-se um registro na tabela de símbolos do escopo atual.  
   - Ao processar instruções (atribuições, chamadas de função, expressões), consulta-se a tabela de símbolos para recuperar tipo, verificar existência e compatibilidade.  
   - Utilizamos funções auxiliares como:  
     - `insere_tabela(nome_identificador, tipo, atributos…)`  
     - `busca_tabela(nome_identificador)` → retorna informações do símbolo ou `NULL` caso não exista.  
     - `abre_escopo()` / `fecha_escopo()` → gerencia pilha de escopos.  

---

## 4. Geração de Código Intermediário (MEPA)  

Após a validação semântica, o compilador traduz o código-fonte para um conjunto de instruções MEPA (Máquina de Pilha para Execução de Programas), que inclui:

1. **Estrutura Geral do Código MEPA**  
   - **Prólogo e Epílogo**:  
     - No início do programa, aloca-se espaço para variáveis globais e inicializa-se pilha.  
     - Ao final, emite instruções para encerrar a execução e liberar memória.  
   - **Rotina de Funções**:  
     - Cada função (inclusive `main`) recebe um rótulo único.  
     - Ao entrar na função, ajusta-se o `MP` (marcador de pilha) para o novo quadro de ativação.  
     - Ao retornar, limpa o quadro de ativação e retorna endereço de chamada.

2. **Conjunto de Instruções MEPA Utilizadas**  
   - `GET`: Obter valor de variável ou constante e empilhar.  
   - `PUT`: Descarregar valor do topo da pilha em variável.  
   - `RMEM`: Reserva `n` posições de memória (para variáveis locais).  
   - `FMEM`: Libera `n` posições de memória (fim de bloco).  
   - `JMP L`: Salta incondicionalmente para o rótulo `L`.  
   - `JMPF L`: Salta para `L` se o valor no topo da pilha for falso (`0`).  
   - `ADD`, `SUB`, `MUL`, `DIV`: Operações aritméticas entre dois topos da pilha.  
   - `EQ`, `NE`, `LT`, `GT`, `LE`, `GE`: Operadores relacionais (comparam dois valores da pilha).  
   - `AND`, `OR`, `NOT`: Operadores lógicos.  
   - `CALL L`: Chama função cujo início está em `L`.  
   - `RET`: Retorna da função (reajusta `MP` e salta para retorno).  
   - `PRI`: Chama rotina interna para imprimir valor inteiro (ligado a `writeint`).  
   - `LEI`: Chama rotina interna para ler valor inteiro (ligado a `readint`).

3. **Tradução de Construções da Linguagem para MEPA**  
   - **Declaração de Variáveis** (`var_decl`):  
     - Para cada variável global: simplesmente reserva espaço em memória estática (offset fixo).  
     - Variáveis locais: gera `RMEM 1` no início do bloco e `FMEM 1` no final.  
   - **Atribuição** (`assig_stmt`):  
     1. Garante que variável foi declarada.  
     2. Gera instrução `GET EndVar` (endereço da variável).  
     3. Gera código para expressão do lado direito (empilha valor).  
     4. Emite `PUT EndVar` (descarrega valor na variável).  
   - **Expressões Aritméticas e Lógicas**:  
     - Cada nó da árvore sintática é traduzido recursivamente.  
     - Exemplo: Em `a + b * 2`, gera-se:  
       ```
       GET end_b      ; empilha valor de b
       GET Const_2    ; empilha valor 2
       MUL            ; multiplica (b * 2)
       GET end_a      ; empilha valor de a
       ADD            ; soma (a + resultado anterior)
       ```  
   - **Comando `if`** (`cond_stmt`):  
     1. Gera código para expressão de condição e deixa resultado no topo da pilha.  
     2. Emite `JMPF L_else` (pular para rótulo do “else” caso condição seja falsa).  
     3. Tradução do bloco “then”.  
     4. Emite `JMP L_end` (pular para o fim).  
     5. Define rótulo `L_else`; se houver bloco “else”, traduz blocos;  
     6. Define rótulo `L_end` no fim.  
   - **Comando `while`** (`while_stmt`):  
     1. Define rótulo `L_start`.  
     2. Gera código para expressão de condição;  
     3. Emite `JMPF L_end` (sair do loop se condição for falsa).  
     4. Tradução do corpo do loop.  
     5. Emite `JMP L_start` (retorna ao início).  
     6. Define rótulo `L_end`.  
   - **Funções de I/O** (`readint`, `writeint`):  
     - `readint()`: gera `LEI` e empilha valor lido;  
     - `writeint(expr)`: gera código da expressão `expr` e, no final, emite `PRI` para imprimir o valor.

4. **Implementação da Geração MEPA**  
   - A geração é feita logo após passar pelas etapas léxico, sintático e semântico, percorrendo novamente a árvore sintática abstrata (AST) construída pelo parser.  
   - Cada nó da AST armazena informações semânticas (tipo, endereço de variável) obtidas na fase anterior.  
   - Funções-chave de tradução:  
     - `gera_mepa_program()`: Cria rótulo `MAIN` e emite código de prólogo.  
     - `gera_mepa_var_globais()`: Para cada variável global, define rótulos `EndVar` e reserva espaço.  
     - `gera_mepa_funcao(nome)`: Ao encontrar declaração de função, emite `LABEL nome`, gera código dos parâmetros, corpo e emite `RET`.  
     - `gera_mepa_stmt(stmt_node)`: Dependendo do tipo de nó (`IF`, `WHILE`, `ASSIGN`, `CALL`), chama a rotina apropriada.  
     - `gera_mepa_expr(expr_node)`: Traduz recursivamente expressões aritméticas e lógicas.  

---

## 5. Decisões de Projeto  

1. **Uso de Autômatos com `goto`** (Análise Léxica):  
   - Facilita a implementação de máquinas de estado finitas para reconhecimento de identificadores, constantes e comentários.  

2. **Integração Léxico/Sintático/Semântico**:  
   - A função `consome()` do parser solicita ao léxico (`obter_atomo()`) o próximo token e, em pontos-chave (declaração de identificadores, tipos, parâmetros), atualiza a tabela de símbolos.  
   - Ao final do `program()`, se não houver erro, invoca-se a geração MEPA.

3. **Representação dos Símbolos**:  
   - Estrutura `TSimbolo`: guarda nome, tipo (`INT` ou `CHAR`), categoria (`VAR` ou `FUNC`), endereço (Offset ou rótulo) e, para funções, lista de parâmetros (tipos e nomes).  
   - Pilha de escopos: cada escopo é um vetor/lista de `TSimbolo`; ao abrir um bloco (`compound_stmt`), empilha-se novo escopo; ao fechar, desempilha.  

4. **Limite de 15 Caracteres para Identificadores**:  
   - Implementado em `reconhece_id()`; se string ultrapassar esse tamanho, retorna token de ERRO LÉXICO com mensagem “identificador muito longo”.

5. **Árvore Sintática Abstrata (AST)**:  
   - Cada nó da AST contém:  
     - Tipo de nó (declaração, atribuição, expressão, etc.).  
     - Ponteiros para filhos (subexpressões, lista de declarações, etc.).  
     - Atributos semânticos (tipo de expressão, endereço de variável, lista de parâmetros).

6. **Geração de Rótulos Únicos**:  
   - Para cada `if` e `while`, gera-se um contador global (`contador_rotulos`) e cria-se rótulos do tipo `L_IF1`, `L_ELSE1`, `L_ENDIF1`, `L_WHILE1`, `L_ENDWHILE1`, garantindo unicidade em todo o programa.

---

## 6. Testes e Resultados  

### 6.1 Casos de Teste Léxico/Sintático/Semântico  
- **Programa de Exemplo do Enunciado** (sem erros):  
  - Verifica reconhecimento de palavras reservadas, tokens de operadores, delimitadores, identificadores, constantes.  
  - Gera saída de todos os tokens, sem mensagens de erro.  
  - Traduz corretamente para MEPA e, ao executar na máquina virtual MEPA, produz resultados esperados.  

- **Identificadores com mais de 15 caracteres**:  
  - Gera erro léxico: “linha X: identificador muito longo”.  

- **Constantes Inteiras e Caractere**:  
  - Testes com valores hexadecimais (`0x1A`, `0Xff`) e caracteres (`'a'`, `'\n'`).  
  - Verifica que `reconhece_intconst()` converte hexadecimais corretamente.  
  - `reconhece_charconst()` aceita exatamente um caractere entre apóstrofos e sinaliza erro caso contrário (mais de um caractere ou cadeia vazia).  

- **Declaração Duplicada de Variável no Mesmo Escopo**:  
  - Programa:  
    ```c
    int x;
    int x;    // erro semântico
    ```  
  - Resultado: “linha Y: variável ‘x’ já declarada neste escopo”.

- **Uso de Variável Não Declarada**:  
  - Programa:  
    ```c
    int x;
    x = y + 5;  // y não foi declarada
    ```  
  - Resultado: “linha Z: variável ‘y’ não declarada”.

- **Atribuição de Tipos Incompatíveis**:  
  - Programa:  
    ```c
    char c;
    c = 10 + 3;  // 10+3 é int, não char
    ```  
  - Resultado: “linha W: atribuição inválida: não é possível atribuir ‘int’ a ‘char’”.

- **Chamadas de Função com Parâmetros Incorretos**:  
  - Programa:  
    ```c
    int f(int a, char b) { return a; }
    void main() {
      f(5);       // falta parâmetro b
      f(5, 'x', 10); // parâmetro a mais
    }
    ```  
  - Resultado:  
    - “linha P: chamada de função ‘f’ com número incorreto de argumentos (esperado 2, encontrado 1)”.  
    - “linha Q: chamada de função ‘f’ com número incorreto de argumentos (esperado 2, encontrado 3)”.

- **Expressões em Condições Inválidas**:  
  - Programa:  
    ```c
    char c;
    if (c == 0) { … }   // comparando char com int → tipo compatível? permitido?
    while ('a') { … }   // expressão char em condição → gerado erro semântico (esperado int)
    ```  
  - Resultado:  
    - “linha R: comparação entre ‘char’ e ‘int’ não permitida sem conversão explícita”.  
    - “linha S: condição de ‘while’ deve ser expressão do tipo ‘int’”.

### 6.2 Testes de Geração MEPA  
- **Programa Simples de Soma**:  
  ```c
  int main() {
    int x;
    int y;
    x = readint();
    y = readint();
    writeint(x + y);
    return 0;
  }
