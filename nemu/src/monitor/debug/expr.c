#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>
#include <stdio.h>

enum {
	NOTYPE = 256,
    EQ,
    NUMBER,
    HEXADECIMAL,
    REG,
    DEREF,

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// spaces
    {"==", EQ},                     // equal

	{"\\+", '+'},					// plus
    {"-", '-'},                     // minux
    {"\\*", '*'},                   // multi
    {"/", '/'},                     // /
    {"0[xX][a-fA-F0-9]{1,8}", HEXADECIMAL},  // hex
    {"\\$[eE]?(ax|cx|dx|bx|sp|bp|si|di)", REG},                   // reg
    {"([1-9][0-9]{1,31})|[0-9]", NUMBER},   // number
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */
                
                if (substr_len > 32){
                    return false;
                }
                
                
				switch(rules[i].token_type) {
                    case '+':
                    case '-':
                    case '*':
                    case '/':
                    case '(':
                    case ')':
                    case HEXADECIMAL:
                    case REG:
                    case NUMBER: {
                        tokens[nr_token].type = rules[i].token_type;
                        strncpy(tokens[nr_token].str, substr_start, substr_len);
                        tokens[nr_token++].str[substr_len] = '\0';

                    }
				    //	default: panic("please implement me");
				}

				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}

uint32_t eval(bool *success, uint32_t p, uint32_t q);
bool check_parentheses(int *info, uint32_t p, uint32_t q);
int find_dominant_operator(uint32_t p, uint32_t q);
uint32_t expr(char *e, bool *success) {
    *success = true;
	if(!make_token(e)) {
		*success = false;
	    return 0;
	}

	/* TODO: Insert codes to evaluate the expression. */
	// panic("please implement me");
    printf("nr_token:%d\n", nr_token);
	return eval(success, 0, nr_token - 1);
}


const int ERROR = -1;
const int NO_PARENTHESES = 1;

uint32_t eval(bool *success, uint32_t p, uint32_t q){
    int info;
    if (p > q){
        *success = false;
        return 0;
    } else if(p == q){
        uint32_t res = 0;
        if(tokens[p].type == NUMBER){
            res = atoi(tokens[p].str);
        } else if(tokens[p].type == HEXADECIMAL){
            res = strtol(tokens[p].str, NULL, 16);
        } else if(tokens[p].type == REG){
            res = get_reg_by_str(success, tokens[p].str + 1);
        } else{
            *success = false;
        }
        return res;
    } else if(check_parentheses(&info, p, q)){
        return eval(success, p + 1, q - 1);
    } else if(info == NO_PARENTHESES){
        uint32_t op = find_dominant_operator(p, q);
        uint32_t val1 = eval(success, p, op - 1);
        uint32_t val2 = eval(success, op + 1, q);
        switch(tokens[op].type){
            case '+': return val1 + val2;
            case '-': return val1 - val2;
            case '*': return val1 * val2;
            case '/': return val1 / val2;
            default: assert(0);
        }
    }
        
    return 0;
}

bool check_parentheses(int *info, uint32_t p, uint32_t q){
    int i;
    int suc = 0;
    if (tokens[p].type == '(' && tokens[q].type == ')'){
        for (i = p + 1; i <= q - 1; i++){
            if (tokens[i].type == '('){
                ++suc;
            } else if(tokens[i].type == ')'){
                --suc;
            }

            if (suc < 0){
                break;
            }
        }
        if (i == q){
            return (suc == 0);
        }
        suc = 0;
        for (i = p; i <= q; i++){
            if (tokens[i].type == '('){
                ++suc;
            } else if (tokens[i].type == '('){
                --suc;
            }

            if (suc < 0){
                return false;
            }
        }
        if (i == q + 1 && suc == 0){
            *info = NO_PARENTHESES;
            return false;
        }
        return false;
    }
    for (i = p; i <= q; i++){
        if (tokens[i].type == '('){
            ++suc;
        } else if (tokens[i].type == ')'){
            --suc;
        }

        if (suc < 0){
            return false;
        }
    }
    if (i == q + 1 && suc == 0){
        *info = NO_PARENTHESES;
        return false;
    }
    return false;
}

int find_dominant_operator(uint32_t p, uint32_t q){
    int op = p;
    int par = 0;
    int i;
    for (i = p; i <= q; i++){
        if (tokens[i].type > NOTYPE){
           continue;
        }

        if(tokens[i].type == '('){
            ++par;
        } else if(tokens[i].type == ')'){
           ++par; 
        }

        if (par == 0){
            if (tokens[i].type == '+'){
                op = i;
            } else if(tokens[i].type == '-'){
                op = i;
            } else if (tokens[i].type == '*'){
                if (tokens[op].type == '*' || tokens[op].type == '/'){
                    op = i;
                }
            } else if (tokens[i].type == '/'){
                if (tokens[op].type == '*' || tokens[op].type == '/'){
                    op = i;
                }
            }
        }
    }
    printf("p=%d,q=%d,op pos:%d\n", p, q, op);
    return op;
}
