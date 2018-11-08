/*
 * tokenizer.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

struct TokenizerT_ {
    char *tokens;
    // toekn 
    int ptr;
    int length;
};

typedef struct TokenizerT_ TokenizerT;

/*
 * TKCreate creates a new TokenizerT object for a given token stream
 * (given as a string).
 *
 * TKCreate should copy the arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate( char * ts ) {
	// Create token for using
    int i = 0;
    TokenizerT * newToken = (TokenizerT*)malloc(sizeof(TokenizerT));
    if (NULL == ts){
    	printf("Invaild Input.\n"); 
        return NULL;
    	}
    	// Check case of null

        newToken->length = strlen(ts);
        newToken->ptr = 0;
        newToken->tokens = (char*)malloc(newToken->length + 1);
        for (i = 0; i < newToken->length; ++i)
            newToken->tokens[i] = ts[i];
        newToken->tokens[i] = '\0';
        return newToken;
    
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy( TokenizerT * tk ) {
    if (tk != NULL){
        free(tk->tokens);
        free(tk);
    }
}

// Help methods for GetNextToken

int isSpace(char d){
    int c = d;
    if (c == 0x02 || c == 0x09 || c == 0x0b || c == 0x0c ||
        c == 0x0a || c == 0x0d || c == ' ')
        return 1;
    else
        return 0;
}

int isNumber(char c){
    if (c >= '0' && c<= '9')
        return 1;
    else
        return 0;
}

int isInHexRange(char c){
    if (isNumber(c))
        return 1;
    if (c >= 'a' && c <= 'f')
        return 1;
    if (c >= 'A' && c <= 'F')
        return 1;
    return 0;
}

int isFloat(char *token){
    char *p = token;
    p++;
    while (*p != '\0'){
        if (*p == '.' || *p == 'e' || *p == 'E' )
            return 1;
        p++;
    }
    return 0;
}

int isHexadecimal(char *token){
    if (strlen(token) >= 3 && token[0] == '0' &&
                (token[1] == 'X' || token[1] == 'x')){
        return 1;
    }
    return 0;

}

int isOctal(char *token){
    if (isHexadecimal(token) != 1 && isFloat(token) != 1 && strlen(token) >= 2 && token[0] == '0'){
        return 1;
    } else
        return 0;
}

int isZero(char *token){
    if (strlen(token) == 1 && *token == '0')
        return 1;
    else
        return 0;

}
int isDecimal(char *token){
    char *p = token;
    if (isOctal(token) == 1)
        return 0;
    if (isZero(token) == 1)
        return 0;
    while (*p != '\0'){
        if (isNumber(*p) != 1)
            return 0;
        p++;
    }
    return 1;
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken( TokenizerT * tk ) {

    char *res = (char *) malloc(100);
    int i = 0;
    while (tk->ptr < tk->length && isSpace(tk->tokens[tk->ptr]) == 1) {
        tk->ptr += 1;
    }

    if (tk->ptr == tk->length) {
        return NULL;
    }

    if (tk->tokens[tk->ptr] == '0') {
        res[i++] = tk->tokens[tk->ptr];

        if (tk->tokens[tk->ptr + 1] == 'X' || tk->tokens[tk->ptr + 1] == 'x' ) {
            //Begin with 0X
            if (isInHexRange(tk->tokens[tk->ptr+2]) == 0) {
                //Case of 0X+error
                res[i] = '\0';
                tk->ptr += 1;
                return res;
            } else {
                res[i++] = tk->tokens[tk->ptr + 1];
                tk->ptr += 2;
                while (tk->ptr < tk->length && isInHexRange(tk->tokens[tk->ptr]) == 1) {
                    res[i++] = tk->tokens[tk->ptr];
                    tk->ptr += 1;
                }
                res[i] = '\0';
                return res;
            }
        } else if (1 == isNumber(tk->tokens[tk->ptr + 1])) {
            //0+ number octal
            tk->ptr += 1;
            while (tk->ptr < tk->length && 1 == isNumber(tk->tokens[tk->ptr]) && tk->tokens[tk->ptr] <= '7') {
                res[i++] = tk->tokens[tk->ptr];
                tk->ptr += 1;
            }
            res[i++] = '\0';
            return res;
        } else if ('e' == tk->tokens[tk->ptr + 1] || 'E' == tk->tokens[tk->ptr + 1]){
            if (tk->tokens[tk->ptr + 2] == '-' || tk->tokens[tk->ptr + 2] == '+'){
                if (isNumber(tk->tokens[tk->ptr + 3]) != 1){
                    res[i] = '\0';
                    tk->ptr += 1;
                    return res;
                } else{
                    tk->ptr += 1;
                    res[i++] = tk->tokens[tk->ptr];
                    tk->ptr += 1;
                    res[i++] = tk->tokens[tk->ptr];
                    tk->ptr += 1;
                    while (tk->ptr < tk->length && 1 == isNumber(tk->tokens[tk->ptr])) {
                        res[i++] = tk->tokens[tk->ptr];
                        tk->ptr += 1;
                    }
                    res[i++] = '\0';
                    return res;
                }
            } else{
                if (isNumber(tk->tokens[tk->ptr + 2]) != 1){
                    res[i] = '\0';
                    tk->ptr += 1;
                    return res;
                } else{
                    tk->ptr += 1;
                    res[i++] = tk->tokens[tk->ptr];
                    tk->ptr += 1;
                    while (tk->ptr < tk->length && 1 == isNumber(tk->tokens[tk->ptr])) {
                        res[i++] = tk->tokens[tk->ptr];
                        tk->ptr += 1;
                    }
                    res[i++] = '\0';
                    return res;

                }

            }
        } else if ('.' == tk->tokens[tk->ptr + 1]){
            if (isNumber(tk->tokens[tk->ptr + 2]) != 1){
                res[i] = '\0';
                tk->ptr += 1;
                return res;
            } else{
                tk->ptr += 1;
                res[i++] = tk->tokens[tk->ptr];
                tk->ptr += 1;
                while (tk->ptr < tk->length && 1 == isNumber(tk->tokens[tk->ptr])) {
                    res[i++] = tk->tokens[tk->ptr];
                    tk->ptr += 1;
                }
                res[i] = '\0';
            }

            if ('e' == tk->tokens[tk->ptr] || 'E' == tk->tokens[tk->ptr]) {
                if (tk->tokens[tk->ptr + 1] == '-' || tk->tokens[tk->ptr + 1] == '+') {
                    if (isNumber(tk->tokens[tk->ptr + 2]) != 1) {
                        return res;
                    } else {
                        res[i++] = tk->tokens[tk->ptr];
                        tk->ptr += 1;
                        res[i++] = tk->tokens[tk->ptr];
                        tk->ptr += 1;
                        while (tk->ptr < tk->length && 1 == isNumber(tk->tokens[tk->ptr])) {
                            res[i++] = tk->tokens[tk->ptr];
                            tk->ptr += 1;
                        }
                        res[i++] = '\0';
                        return res;
                    }
                } else {
                    if (isNumber(tk->tokens[tk->ptr + 1]) != 1) {
                        res[i] = '\0';
                        return res;
                    } else {
                        res[i++] = tk->tokens[tk->ptr];
                        tk->ptr += 1;
                        while (tk->ptr < tk->length && 1 == isNumber(tk->tokens[tk->ptr])) {
                            res[i++] = tk->tokens[tk->ptr];
                            tk->ptr += 1;
                        }
                        res[i++] = '\0';
                        return res;
                    }
                }
            } else{
                return res;
            }
        }else {
            tk->ptr += 1;
            res[i++] = '\0';
            return res;
        }
    } else if (isNumber(tk->tokens[tk->ptr]) == 1) {
        //decimal
        while (tk->ptr < tk->length && 1 == isNumber(tk->tokens[tk->ptr])) {
            res[i++] = tk->tokens[tk->ptr];
            tk->ptr += 1;
        }

        res[i] = '\0';
        if (tk->tokens[tk->ptr] == '.'){
            if (isNumber(tk->tokens[tk->ptr + 1]) != 1){
                res[i] = '\0';
                return res;
            } else{
                res[i++] = tk->tokens[tk->ptr];
                tk->ptr += 1;
                while (tk->ptr < tk->length && 1 == isNumber(tk->tokens[tk->ptr])) {
                    res[i++] = tk->tokens[tk->ptr];
                    tk->ptr += 1;
                }
                res[i] = '\0';
            }

            if ('e' == tk->tokens[tk->ptr] || 'E' == tk->tokens[tk->ptr]) {
                if (tk->tokens[tk->ptr + 1] == '-' || tk->tokens[tk->ptr + 1] == '+') {
                    if (isNumber(tk->tokens[tk->ptr + 2]) != 1) {
                        return res;
                    } else {
                        res[i++] = tk->tokens[tk->ptr];
                        tk->ptr += 1;
                        res[i++] = tk->tokens[tk->ptr];
                        tk->ptr += 1;
                        while (tk->ptr < tk->length && 1 == isNumber(tk->tokens[tk->ptr])) {
                            res[i++] = tk->tokens[tk->ptr];
                            tk->ptr += 1;
                        }
                        res[i++] = '\0';
                        return res;
                    }
                } else {
                    if (isNumber(tk->tokens[tk->ptr + 1]) != 1) {
                        res[i] = '\0';
                        return res;
                    } else {
                        res[i++] = tk->tokens[tk->ptr];
                        tk->ptr += 1;
                        while (tk->ptr < tk->length && 1 == isNumber(tk->tokens[tk->ptr])) {
                            res[i++] = tk->tokens[tk->ptr];
                            tk->ptr += 1;
                        }
                        res[i++] = '\0';
                        return res;
                    }
                }
            }
            return res;
        }
        // float
        if ('e' == tk->tokens[tk->ptr] || 'E' == tk->tokens[tk->ptr]) {
            tk->ptr -= 1;
            if (tk->tokens[tk->ptr + 2] == '-' || tk->tokens[tk->ptr + 2] == '+') {
                if (isNumber(tk->tokens[tk->ptr + 3]) != 1) {
                    res[i] = '\0';
                    tk->ptr += 1;
                    return res;
                } else {
                    tk->ptr += 1;
                    res[i++] = tk->tokens[tk->ptr];
                    tk->ptr += 1;
                    res[i++] = tk->tokens[tk->ptr];
                    tk->ptr += 1;
                    while (tk->ptr < tk->length && 1 == isNumber(tk->tokens[tk->ptr])) {
                        res[i++] = tk->tokens[tk->ptr];
                        tk->ptr += 1;
                    }
                    res[i++] = '\0';
                    return res;
                }
            } else {
                if (isNumber(tk->tokens[tk->ptr + 2]) != 1) {
                    res[i] = '\0';
                    tk->ptr += 1;
                    return res;
                } else {
                    tk->ptr += 1;
                    res[i++] = tk->tokens[tk->ptr];
                    tk->ptr += 1;
                    while (tk->ptr < tk->length && 1 == isNumber(tk->tokens[tk->ptr])) {
                        res[i++] = tk->tokens[tk->ptr];
                        tk->ptr += 1;
                    }
                    res[i++] = '\0';
                    return res;

                }

            }
        }
        return res;
    } else if (tk->ptr < tk->length) {
        res[i++] = tk->tokens[tk->ptr];
        res[i] = '\0';
        tk->ptr += 1;
        return res;
    } else {
        return NULL;
    }
}





/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) {
    //char data[] = "0e123 0e123.123 123.123e123 123.123e-123 123.123e+123 23.4 0.23e1234f 0.23e+ 0.23e- 0.23e-1234e 0e123 0e-123 0ef 0X123 1231 02133 0Xfff";
    TokenizerT * totalt;
    char *token;

    if (argc != 2){
        printf("Usage is not correct.\n");
        return 0;
    }

    totalt = TKCreate(argv[1]);
    while (1){
    	// Print 
        token = TKGetNextToken(totalt);
        if (token == NULL)
            break;
        if (isHexadecimal(token)){
            printf("Hexadecimal: %s\n", token);
        } else if (isOctal(token)){
            printf("Octal: %s\n", token);
        } else if (isDecimal(token)){
            printf("Decimal: %s\n", token);
        } else if (isZero(token)){
            printf("Zero: %s\n", token);
        } else if (isFloat(token)){
            printf("Float: %s\n", token);
        } else {
            printf("Invalid : [0x%02x]\n", token[0]);
        }
        free(token);
    }
    TKDestroy(totalt);
    return 0;
}
