Projeto Parte 1 - Compilador TINY-C
Integrante: [Arthur Vignati Moscardi - 10409688]

1. Descrição Geral
Este trabalho tem como objetivo implementar as fases de análise léxica e sintática de um compilador para a linguagem TINY-C, que é baseada na linguagem C. A linguagem TINY-C é uma versão simplificada do C, contendo apenas os tipos primitivos int e char, comandos condicionais (if) e de repetição (while), além das funções de entrada (readint) e saída (writeint).

O compilador deve ler um arquivo fonte, identificar os tokens conforme a gramática e as especificações léxicas apresentadas, e então informar na saída padrão a linha e a descrição de cada token reconhecido, juntamente com o número de linhas analisadas se o programa estiver sintaticamente correto.

2. Estrutura do Código
O compilador foi dividido em duas partes principais:

    2.1 Analisador Léxico
        Função obter_atomo(): Responsável por ler o conteúdo do arquivo (através do ponteiro global buffer) e retornar um token do tipo TInfoAtomo. Essa função utiliza autômatos (implementados com goto) para reconhecer tokens específicos, como identificadores, constantes (intconst e charconst) e comentários.

        Funções de Reconhecimento:

        reconhece_id(): Reconhece identificadores e palavras reservadas.

        reconhece_intconst(): Reconhece constantes inteiras na notação hexadecimal, convertendo o valor para decimal.

        reconhece_charconst(): Reconhece constantes de caractere, garantindo que haja exatamente um caractere entre apóstrofos e que ele esteja na tabela ASCII.

        reconhece_comentario(): Reconhece comentários de linha e de bloco. Os comentários são reportados (impressos) e, em seguida, descartados, chamando novamente obter_atomo().

        Vetor de Tokens (strAtomo): Utilizado para exibir os tokens reconhecidos e também para exibir mensagens de erro sintático, mostrando os nomes dos tokens esperados e encontrados.

    2.2 Analisador Sintático
        Função consome(): Essa função é responsável por "consumir" o token esperado (comparando com o lookahead) e chamar obter_atomo() para avançar para o próximo token.
        Ela também ignora os tokens de comentário, que foram previamente reportados pelo léxico.

        Funções Recursivas para Não-Terminiais: São implementadas funções para cada não-terminal da gramática, como:
            program(), compound_stmt(), var_decl(), type_specifier(), var_decl_list(), variable_id(), stmt(), assig_stmt(), cond_stmt(), while_stmt(), expr(), conjunction(), comparison(), relation(), sum(), term(), factor()

        A interação entre os analisadores é feita por meio da função consome(), que realiza chamadas à função obter_atomo() do analisador léxico.

4. Decisões de Projeto
    Uso de Autômatos com goto: Para facilitar o reconhecimento de tokens, foram implementados autômatos com rótulos (goto), seguindo o modelo apresentado em aula. Isso foi utilizado em funções como reconhece_id(), reconhece_intconst(), reconhece_charconst() e reconhece_comentario().

    Integração Léxico/Sintático: A função consome() do analisador sintático chama obter_atomo() para obter o próximo token e ignora os comentários, garantindo que o parser veja apenas os tokens relevantes para a análise sintática.

    Vetor de Tokens: O vetor strAtomo foi criado para representar, em string, todos os tokens reconhecidos pelo compilador, facilitando a depuração e a exibição de mensagens de erro.

    Limite de 15 Caracteres para Identificadores: Implementado na função reconhece_id(), onde se o identificador ultrapassar 15 caracteres, o token retorna como ERRO, conforme exigido pela especificação.

5. Testes e Resultados
Foram testados os seguintes casos:
- Programa de exemplo fornecido no enunciado do projeto.
- Casos com identificadores com mais de 15 caracteres (que geram erro).
- Casos com intconst e charconst.

6. Instruções de Compilação e Execução
Para compilar:
    gcc -Wall -Wno-unused-result -g -Og compilador.c -o compilador
Para executar:
    ./compilador entrada.txt
