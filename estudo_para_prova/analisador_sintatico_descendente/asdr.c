#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


//[B] --> B ou vazio
//{B} --> uma ou mais vezes a repeticao de B  (while)


//A::= B[*A]
void A (){
    B();
    if (lookhead == "*"){
        consome("*");
        A();
    }
}
//B::= C(>C)
void B(){
    C();
    consome(ABRE_PARENTESES);
    consome(MAIOR);
    C();
    consome(FECHA_PARENTESES);
}
//C::= [~]D
void C(){
    if (lookhead == "~"){
        consome(TIL);
    }
    D();
}
//D::= a|(A)
void D(){
    if (lookhead == 'a'){
        consome("a");
    }else if (lookhead == '('){
        consome(ABRE_PARENTESES);
        A();
        consome(FECHA_PARENTESES);
    }
}




//S::=[~|>]L{(~|>)L}
void S(){
    if(lookhead == '~'){
        consome(TIL);
    }else if(lookhead == '>'){
        consome(MAIOR);
    }
    L();
    while(lookhead == '~' || lookhead == '>'){
        if(lookhead == '~'){
            consome(TIL);
        }else if(lookhead == '>'){
            consome(MAIOR);
        }
        L();
    }
}
//L::= T{(*|<)T}
void L(){
    T();
    while(lookhead == '*' || lookhead == '<'){
        if (lookhead == '*'){
            consome(MULTIPLICACAO)
        }else if(lookhead == '<'){
            consome(MENOR);
        }
        T();
    }
}
//T::= a|%S&
void T(){
    if(lookhead == 'a'){
        consome('a');
    }else if(lookhead == '%'){
        consome('%');
        S();
        consome('&');
    }
}











void S(){
    if (lookhead == 'c'){
        B();
        S();
    }elif(lookhead == 'a' || lookhead == 'e' ||lookhead == 'b' ||lookhead == 'd'){
        A();
        C();
    }
}

void A(){
    if(lookhead == 'a'){
        consome('a')
    }else if(lookhead == 'e'){
        consome('e')
    }
}

void B(){
    if(lookhead == 'c'){
        consome('c')
        C();
        consome('d')
    }
}

void C(){
    if (lookhead == 'b'){
        consome('b');
        C();
    }else if(lookhead == 'd'){
        consome('d');
    }
}







void A(){
    consome('b');
    B();
    consome('b');
}

void B(){
    if (lookhead == 'c'){
        consome('c')
        C()
    }else if(lookhead == 'e'){
        consome('e');
        D();
    }
}

void D(){
    consome('d')
    consome('a')
}

void C(){
    if (lookhead == 'a'){
        consome('a')
        A()
    }    
}


void S(){
    if (lookhead == '~'){
        consome('~');
    }else if(lookhead == '>'){
        consome('>');
    }
    L();
    while(lookhead == '~' || lookhead == '>'){
        if (lookhead == '~'){
            consome('~');
        }else if(lookhead == '>'){
            consome('>');
        }
        L();
    }
}

void L(){
    T();
    while(lookhead == '*' || lookhead == '<'){
        if (lookhead == '*'){
            consome('*');
        }else if(lookhead == '<'){
            consome('<');
        }
        T();
    }
}

void T(){
    if (lookhead == 'a'){
        consome('a')
    }else{
        consome('%')
        S()
        consome('&')
    }
}













