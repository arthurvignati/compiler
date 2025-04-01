#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int reconheceAFD(char *entrada){
    if (*entrada != '/0'){
        goto q0;
    }
    return 0;
q0:
    while(*entrada == 'a'){
        entrada++;
        if((*entrada + 1) == '/0'){
            entrada++;
            return 1;
        }
    }
    if(*entrada == 'b'){
        entrada++;
        goto q1;
    }
    return 0;
q1: 
    if (*entrada == '/0'){
        return 1;
    }
    if(*entrada == 'a'){
        entrada++;
        goto q0;
    }else if(*entrada == 'b'){
        entrada++;
        goto q2;
    }
    return 0;

q2:
    if(*entrada == '/0'){
        return 1;
    }else if(*entrada == 'a'){
        entrada++;
        goto q0;
    }else if(*entrada == 'b'){
        entrada++;
        goto q3;
    }
    return 0;

q3:
    while(*entrada == 'a' || *entrada == 'b'){
        entrada++;
    }
    return 0;
}