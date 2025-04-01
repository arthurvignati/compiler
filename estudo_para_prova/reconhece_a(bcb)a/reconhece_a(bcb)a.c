#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int reconhece_expressao_regular(char *entrada){
    if (*entrada == 'a'){
        entrada++;
        goto q1;
    }
q1:
    if (*entrada == 'b'){
        entrada++;
        goto q2;
    }else if(*entrada == 'c'){
        entrada++;
        goto q3;
    }else if(*entrada == 'a'){
        entrada++;
        return 1;
    }
    return 0;
q2:
    while(*entrada == 'b'){
        entrada++;
        goto q2;
    }if (*entrada == 'a'){
        entrada++;
        return 1;
    }
    return 0;
q3:
    if (*entrada == 'b'){
        entrada++;
        goto q4;
    }
    return 0;
q4:
    if (*entrada == 'c'){
        entrada++;
        goto q3;
    }else if(*entrada == 'a'){
        entrada ++;
        return 1;
    }
    return 0;
}