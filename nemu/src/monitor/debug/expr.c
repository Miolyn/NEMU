#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>
#include <stdio.h>

enum {
	NOTYPE = 256,   // ' '
    NUMBER,         // number
    HEXADECIMAL,    // hex
    REG,            // register
    EQ,             // ==
    NEQ,            // !=
    LAND,           // &&
    LOR,            // ||
    LNOT,           // !
    DEREF,          // *addr
    NEG,            // neg - negative is fu hao

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				                                            // spaces
    {"==", EQ},                                                                 // equal

	{"\\+", '+'},					                                            // plus+
    {"-", '-'},                                                                 // minus-
    {"\\*", '*'},                                                               // multi*
    {"/", '/'},                                                                 // div/
    {"\\(", '('},                                                                 // (
    {"\\)", ')'},                                                                 //)
    {"&&", LAND},                                                               // &&
    {"\\|\\|", LOR},                                                            // ||
    {"!=", NEQ},                                                                // !=
    {"!", LNOT},                                                                // !
    {"\\$([eE]?(ax|cx|dx|bx|sp|bp|si|di))|\\$([a-d][hl])", REG},                   // reg
    {"0[xX][a-fA-F0-9]{1,8}", HEXADECIMAL},                                            // hex
    {"([1-9][0-9]{1,31})|[0-9]", NUMBER},                                       // number
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

bool is_parentheses(int i);
bool is_logic(int i);
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

				// Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
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
                    case EQ:
                    case NEQ:
                    case LNOT:
                    case LAND:
                    case LOR:
                    case HEXADECIMAL:
                    case REG:
                    case NUMBER: {
                        tokens[nr_token].type = rules[i].token_type;
                        strncpy(tokens[nr_token].str, substr_start, substr_len);
                        tokens[nr_token++].str[substr_len] = '\0';
                        break;
                    }
				}

				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}
    // find *addr
    for(i = 0; i < nr_token; i++){
        if((tokens[i].type == '*' || tokens[i].type == '-')&& (i == 0 || (tokens[i - 1].type != NUMBER && tokens[i - 1].type != REG && tokens[i - 1].type != HEXADECIMAL && !is_parentheses(i - 1)))){
            // printf("deref:%d\n", i);
            if (tokens[i].type == '*'){
                tokens[i].type = DEREF;
            } else if(tokens[i].type == '-'){
                // printf("neg pos%d\n", i);
                tokens[i].type = NEG;
            }
        }
    }
	return true; 
}

uint32_t eval(bool *success, uint32_t p, uint32_t q);
bool check_parentheses(int *info, uint32_t p, uint32_t q);
int find_dominant_operator(bool *success, uint32_t p, uint32_t q);
uint32_t expr(char *e, bool *success) {
    *success = true;
	if(!make_token(e)) {
		*success = false;
	    return 0;
	}

	/* TODO: Insert codes to evaluate the expression. */
	// panic("please implement me");
    // printf("nr_token:%d\n", nr_token);
	return eval(success, 0, nr_token - 1);
}


const int ERROR = -1;
const int NO_PARENTHESES = 1;

uint32_t eval(bool *success, uint32_t p, uint32_t q){
    // printf("p:%d,q:%d\n", p, q);
    int info;
    if (p > q || p == -1 || q == -1){
        return 0;
    } else if(p == q){
        uint32_t res = 0;
        // printf("%s ", tokens[p].str);
        if(tokens[p].type == NUMBER){
            // printf("number\n");
            res = atoi(tokens[p].str);
        } else if(tokens[p].type == HEXADECIMAL){
            // printf("hex\n");
            res = strtol(tokens[p].str, NULL, 16);
        } else if(tokens[p].type == REG){
            // printf("reg\n");
            res = get_reg_by_str(success, tokens[p].str + 1);
        } else{
            *success = false;
        }
        return res;
    } else if(check_parentheses(&info, p, q)){
        return eval(success, p + 1, q - 1);
    } else if(info == NO_PARENTHESES){
        uint32_t op = find_dominant_operator(success, p, q);
        // printf("op pos%d type %c %d %d %d\n", op, tokens[op].type, op - 1, p, (tokens[op].type == '/'));
        if (op - 1 == -1 && (is_logic(op) || tokens[op].type == '/')){
            *success = false;
            return 0;
        }
        if(is_logic(op) && op >= q){
            *success = false;
            return 0;
        }
        uint32_t val1 = eval(success, p, op - 1);
        uint32_t val2 = eval(success, op + 1, q);
        // printf("val1:%d,val2:%d\n", val1, val2);
        switch(tokens[op].type){
            case '+': return val1 + val2;
            case '-': return val1 - val2;
            case '*': return val1 * val2;
            case '/': return val1 / val2;
            case DEREF:{
                return swaddr_read(val2, 4);              
            }
            case NEG: return -1 * val2;
            case LNOT: return !val2;
            case EQ: return val1 == val2;
            case NEQ: return val1 != val2;
            case LAND: return val1 && val2;
            case LOR: return val1 || val2;
            default: assert(0);
        }
    }
        
    return 0;
}

bool check_parentheses(int *info, uint32_t p, uint32_t q){
    if (tokens[p].type == NEG || tokens[p].type == DEREF){
        *info = NO_PARENTHESES;
        return false;
    }
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

bool is_parentheses(int i){
    return (tokens[i].type == '(' || tokens[i].type == ')');
}

bool is_logic(int i){
    int tp = tokens[i].type;
    return (tp == LAND || tp == LOR || tp == EQ || tp == NEQ);
}
int find_dominant_operator(bool *success, uint32_t p, uint32_t q){
    int op = p;
    int par = 0;
    int i;
    // find deref
    bool flag = false;
    if(tokens[p].type == DEREF || tokens[p].type == NEG || tokens[p].type == LNOT){
        op = p;
        flag = true;
        if (p + 1 == q){
            return op;
        }
    }
    for (i = p; i <= q; i++){
        if (tokens[i].type > NOTYPE && tokens[i].type < EQ){
           continue;
        }

        if(tokens[i].type == '('){
            ++par;
            continue;
        } else if(tokens[i].type == ')'){
            --par; 
            continue;
        }
        // printf("pos%d,par%d\n", i, par);
        if (par == 0){
            // printf("is:%d\n", is_parentheses(op));
            if (is_logic(i)){
                if(tokens[i].type == EQ || tokens[i].type == NEQ){
                    op = i;
                    flag = true;
                } else if (tokens[op].type != EQ && tokens[op].type != NEQ){
                    flag = true;
                    op = i;
                }
            } if (tokens[i].type == '+' && !is_logic(op)){
                flag = true;
                op = i;
            } else if(tokens[i].type == '-' && !is_logic(op)){
                flag = true;
                op = i;
            } else if (tokens[i].type == '*' && !is_logic(op)){
                if (tokens[op].type == '*' || tokens[op].type == '/' || tokens[op].type > NOTYPE || is_parentheses(op)){
                    flag = true;
                    op = i;
                }
            } else if (tokens[i].type == '/' && !is_logic(op)){
                if (tokens[op].type == '*' || tokens[op].type == '/' || tokens[op].type > NOTYPE || is_parentheses(op)){
                    flag = true;
                    op = i;
                }
            }
        }
    }
    // for(i = p; i <= q; i++){
    //    printf("%s ", tokens[i].str);
    // }
    //printf("\n");
    //printf("p=%d,q=%d,op pos:%d\n", p, q, op);
    if (!flag){
        *success = false;
    }
    return op;
}
