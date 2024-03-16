%{
#include <iostream>
#include <cstring>
#include <stack>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern FILE *yyin;
extern int yylineno;
extern char *yytext;
extern int yylex();
bool err_found = false;
char err[100001];

int number_variables_global = 0;
int number_functions_global = 0;
int number_variables_local = 0;
bool global_declarations = false;
bool main_program_entry_found = false;

struct vars_entry
{
    char *name_of_id; 
    char *var_type;
    char *dims;
    int *val;
    bool global;

} local_symbols_table[51], global_symbols_table[51];

struct func_entry
{
    char *func_name;
    char *ret_type;
    char *parameters;

} functions_table[11];

void yyerror(const char *str)
{
    std::cout << "ERROR : " << str <<  " " << yylineno << "\n";
}

struct Node
{
    char val[101];
    struct Node *left, *right;
};

Node *create_new_node(char *val)
{
    Node *node = new Node;
    strcpy(node -> val, val);
    node -> left = node -> right = NULL;
    return node;
}

struct Stack
{
    Node *array[10001];
    int top;
};

Node *top(Stack *stack)
{
    if (stack -> top == -1) return NULL;
    return stack -> array[stack -> top];
}

Node *pop(Stack *stack)
{
    if (stack -> top == -1) return NULL;
    return stack -> array[stack -> top--];
}

void push(Stack *stack, Node *node)
{
    stack -> array[++stack -> top] = node;
}

bool is_empty(Stack *stack)
{
    return stack -> top == -1;
}

int op_pred(char *opr)
{
    if (strstr(opr, "/") != NULL || strstr(opr, "*") != NULL) return 4;
    if (strstr(opr, "-") != NULL || strstr(opr, "+") != NULL) return 3;
    return 0;
}

bool is_string_number(char *str)
{
    if (strlen(str) == 1 && str[0] == '-') return 0;
    for (int i = 0; str[i] != '\0'; i++)
        if (!isdigit(str[i]) && str[i] != '-') return 0;
    return 1;
}

Node *AST_create(char **tokens, int nr_of_tokens)
{
    Stack operator_stk, operand_stk;
    operator_stk.top = -1; operand_stk.top = -1;

    for (int i = 0; i < nr_of_tokens; ++i)
    {
        if (tokens[i][0] == '(') push(& operator_stk, create_new_node(tokens[i]));
        else if (tokens[i][0] == ')')
        {
            while (!is_empty(& operator_stk) && top(& operator_stk) -> val[0] != '(')
            {
                Node *opr = pop(& operator_stk);
                opr -> right = pop(& operand_stk);
                opr -> left = pop(& operand_stk);
                push(& operand_stk, opr);
            }
            pop(& operator_stk);
        }
        else if (!is_string_number(tokens[i])) 
        {
            while (!is_empty(& operator_stk) && op_pred(top(& operator_stk) -> val) >= op_pred(tokens[i]))
            {
                Node *opr = pop(& operator_stk);
                opr -> right = pop(& operand_stk);
                opr -> left = pop(& operand_stk);
                push(& operand_stk, opr);
            }
            push(& operator_stk, create_new_node(tokens[i]));
        }
        else push(& operand_stk, create_new_node(tokens[i]));
    }

    while (!is_empty(& operator_stk))
    {
        Node *opr = pop(& operator_stk);
        opr -> right = pop(& operand_stk);
        opr -> left = pop(& operand_stk);
        push(& operand_stk, opr);
    }
    return pop(& operand_stk);
}

int AST_eval(Node *node)
{
    if (node == NULL) return 0;

    if (is_string_number(node -> val)) return atoi(node -> val);

    int left = AST_eval(node -> left);
    int right = AST_eval(node -> right);

    if (strstr(node -> val, "*") != NULL)
        return left * right;
    if (strstr(node -> val, "/") != NULL)
        return left / right;
    if (strstr(node -> val, "%") != NULL)
        return left % right;
    if (strstr(node -> val, "+") != NULL)
        return left + right;
    if (strstr(node -> val, "-") != NULL)
        return left - right;

    return 0;
}

bool is_op(char chr)
{
    return chr == '*' || chr == '/' || chr == '%' || chr == '+' || chr == '-';
}

bool is_round_bracket(char chr)
{
    return chr == '(' || chr == ')';
}

void display_variable_info(vars_entry *var)
{
    std::cout << "Variable name : " << var -> name_of_id << " , variable type : " << var -> var_type << "\n";

    if (var -> dims != NULL) std::cout << "Dimensions : " << var -> dims << "\n";
    else std::cout << "Variable value : " << (*(int *)var -> val) << "\n \n";
}

void display_local_variables()
{
    for (int i = 0; i < number_variables_local; ++i) display_variable_info(& local_symbols_table[i]);
}

void display_function()
{
    for (int i = 0; i < number_functions_global; ++i)
    {
        std::cout << "Function name : " << functions_table[i].func_name << "\n";
        std::cout << "Return type name : " << functions_table[i].ret_type << "\n";
        std::cout << "Parameters name : " << functions_table[i].parameters << "\n\n";
    }
}

void display_global_variables()
{
    for (int i = 0; i < number_variables_global; ++i) display_variable_info(&global_symbols_table[i]);
}

bool declared_variable(char *var)
{
    for (int i = 0; i < number_variables_global; ++i)
        if (strcmp(global_symbols_table[i].name_of_id, var) == 0) return true;

    for (int i = 0; i < number_variables_local; ++i)
        if (strcmp(local_symbols_table[i].name_of_id, var) == 0) return true;

    return false;
}

bool already_declared_variable(char *str)
{
    for (int i = 0; i < number_variables_global; ++i)
        if (strcmp(global_symbols_table[i].name_of_id, str) == 0)
        {
            strcpy(err, "Global variable redeclaration, line : ");
            char lin[11];
            sprintf(lin, "%d", yylineno);
            strcat(err, lin);
            strcat(err, "\n Redeclared global variable name : ");
            strcat(err, str);
            err_found = true;
            return false;
        }

    for (int i = 0; i < number_variables_local; ++i)
        if (strcmp(local_symbols_table[i].name_of_id, str) == 0)
        {
            strcpy(err, "Local variable redeclaration, line : ");
            char lin[11];
            sprintf(lin, "%d", yylineno);
            strcat(err, lin);
            strcat(err, "\n Redeclared local variable name : ");
            strcat(err, str);
            err_found = true;
            return false;
        }

    return true;
}

void create_local_variable(char *decl)
{
    char *tok = strtok(decl, " ");
    char var_type[10001] = {};
    char id[10001] = {};
    char dims[10001] = {};

    strcpy(var_type, tok);
    tok = strtok(NULL, " ");
    char *chr = strchr(tok, '[');

    if (chr != NULL)
    {
        int i = 1;
        while (chr[i] != ']') dims[i - 1] = chr[i], i++;
        chr[0] = '\0';
    }

    strcpy(id, tok);

    if (global_declarations || !already_declared_variable(id)) return;

    if (!global_declarations)
    {
        local_symbols_table[number_variables_local].name_of_id = strdup(id);
        local_symbols_table[number_variables_local].var_type = strdup(var_type);

        if (strlen(dims) != 0) local_symbols_table[number_variables_local].dims = strdup(dims);

        local_symbols_table[number_variables_local].global = false;
        int *initialValue = new int;
        (*initialValue) = 0;

        if (strlen(dims) == 0) local_symbols_table[number_variables_local].val = initialValue;
        else local_symbols_table[number_variables_local].val = new int[1001];
        
        number_variables_local++;
    }
    global_declarations = false;
}

void create_global_variable(char *var_type, char *id, char *dims)
{
    if (!global_declarations || !already_declared_variable(id)) return;

    if (global_declarations)
    {
        global_symbols_table[number_variables_global].name_of_id = strdup(id);
        global_symbols_table[number_variables_global].var_type = strdup(var_type);
        
        if (dims != NULL) global_symbols_table[number_variables_global].dims = strdup(dims);
        
        global_symbols_table[number_variables_global].global = true;
        int *initialValue = new int;
        (*initialValue) = 0;

        if (dims == NULL) global_symbols_table[number_variables_global].val = initialValue;
        else global_symbols_table[number_variables_global].val = new int[1001];

        number_variables_global++;
    }

    global_declarations = false;
}

int get_variable_value(char *id)
{
    for (int i = 0; i < number_variables_local; ++i)
        if (strcmp(local_symbols_table[i].name_of_id, id) == 0)
            return *(local_symbols_table[i].val);

    for (int i = 0; i < number_variables_global; ++i)
        if (strcmp(global_symbols_table[i].name_of_id, id) == 0)
            return *(global_symbols_table[i].val);
}

void create_function(char *ret_type, char *nume, char *parameters)
{
    functions_table[number_functions_global].func_name = strdup(nume);
    functions_table[number_functions_global].ret_type = strdup(ret_type);
    functions_table[number_functions_global].parameters = parameters;
    number_functions_global++;
}

int eval(char *evaluate_expr)
{ 
    char expr_normal[100001] = {};

    for (int i = 0; i < strlen(evaluate_expr); ++i)
    {
        if (evaluate_expr[i] == ' ')
            continue;
        else if (is_op(evaluate_expr[i]))
        {
            strcat(expr_normal, " ");
            char str[5] = {};
            str[0] = evaluate_expr[i];
            strcat(expr_normal, str);
            strcat(expr_normal, " ");
        }
        else if (is_round_bracket(evaluate_expr[i]))
        {
            strcat(expr_normal, " ");
            char str[5] = {};
            str[0] = evaluate_expr[i];
            strcat(expr_normal, str);
            strcat(expr_normal, " ");
        }
        else
        {
            strcat(expr_normal, " ");
            while (evaluate_expr[i] != ' ' && i < strlen(evaluate_expr) && !is_round_bracket(evaluate_expr[i]) && !is_op(evaluate_expr[i]))
            {
                char str[5] = {};
                str[0] = evaluate_expr[i];
                strcat(expr_normal, str);
                ++i;
            }
            --i;
            strcat(expr_normal, " ");
        }
    }

    char expr_temp[100001] = {};
    char *token = strtok(expr_normal, " ");

    while (token != NULL)
    {
        if (!is_op(*token) && !is_round_bracket(*token) && !(token[0] >= '0' && token[0] <= '9'))
        {
            if (!declared_variable(token))
            {
                err_found = true;
                strcpy(err, "Undeclared variable cannot have value, line: ");
                char line_nr[11];
                sprintf(line_nr, "%d", yylineno);
                strcat(err, line_nr);
                strcat(err, "\n Undeclared variable name : ");
                strcat(err, token);
                return 0;
            }
            int val = get_variable_value(token);
            char num_str[12];
            sprintf(num_str, "%d", val);
            strcat(expr_temp, num_str);
        }
        else
        {
            strcat(expr_temp, token);
        }
        strcat(expr_temp, " ");
        token = strtok(NULL, " ");
    }

    char *tokens[10001];
    int index = 0;
    token = strtok(expr_temp, " ");

    while (token != NULL)
    {
        tokens[index++] = token;
        token = strtok(NULL, " ");
    }

    Node *root_AST = AST_create(tokens, index);

    int result = AST_eval(root_AST);
    std::cout << "Expression result : " << result << "\n";
    return result;
}

void assert_expression(char *expression)
{
    char expr_normal[100001] = {};

    for (int i = 0; i < strlen(expression); ++i)
    {
        if (expression[i] == ' ') continue;
        else if (is_round_bracket(expression[i]))
        {
            strcat(expr_normal, " ");
            char str[5] = {};
            str[0] = expression[i];
            strcat(expr_normal, str);
            strcat(expr_normal, " ");
        }
        else if (is_op(expression[i]))
        {
            strcat(expr_normal, " ");
            char str[5] = {};
            str[0] = expression[i];
            strcat(expr_normal, str);
            strcat(expr_normal, " ");
        }
        else
        {
            strcat(expr_normal, " ");
            while (expression[i] != ' ' && i < strlen(expression) && !is_round_bracket(expression[i]) && !is_op(expression[i]))
            {
                char str[5] = {};
                str[0] = expression[i];
                strcat(expr_normal, str);
                ++i;
            }
            --i;
            strcat(expr_normal, " ");
        }
    }

    char expr_temp[100001] = {};
    char *tokens[10001];
    int index = 0;
    char *token = strtok(expr_normal, " ");

    while (token != NULL)
    {
        if (token[0] != '(' && token[0] != ')' && token[0] != '+' && token[0] != '-' && token[0] != '*' && token[0] != '/')
        {
            if (isdigit(token[0]))
            {
                if (strchr(token, '.') != NULL)
                {
                    char* str = new char[11];
                    strcpy(str, "float");
                    tokens[index++] = str;
                }
                else 
                {
                    char* str = new char[11];
                    strcpy(str, "int");
                    tokens[index++] = str;
                }
            }
            else 
            {
                if (!declared_variable(token))
                {
                    err_found = true;
                    strcpy(err, "Cannot user undeclared variable, line : ");
                    char line_nr[11];
                    sprintf(line_nr, "%d", yylineno);
                    strcat(err, line_nr);
                    strcat(err, "\n Undeclared variable name : ");
                    strcat(err, token);
                    return ;
                }

                for (int i = 0; i < number_variables_global; ++i)
                    if (strcmp(global_symbols_table[i].name_of_id, token) == 0) tokens[index++] = global_symbols_table[i].var_type;
                    
                for (int i = 0; i < number_variables_local; ++i)
                    if (strcmp(local_symbols_table[i].name_of_id, token) == 0) tokens[index++] = local_symbols_table[i].var_type;
            }
        }
        token = strtok(NULL, " ");
    }

    bool not_good = false;

    for (int i = 0; i < index - 1; ++i)
        if (strcmp(tokens[i], tokens[i + 1]) != 0) not_good = true;

    if (not_good == false) return;
    else
    {
        err_found = true;
        strcpy(err, "Expression type enouctered multiple varibable types, line : ");
        char line_nr[11];
        sprintf(line_nr, "%d", yylineno);
        strcat(err, line_nr);
        return ;
    }
}

int type_eval_define(char *func_type, char *parameters) 
{
    if (strcmp(func_type, "Eval") == 0) return eval(parameters);
    else if (strcmp(func_type, "TypeOf") == 0)
    {
        char expr_normal[100001] = {};

        for (int i = 0; i < strlen(parameters); ++i)
        {
            if (parameters[i] == ' ')
                continue;
            else if (is_round_bracket(parameters[i]))
            {
                strcat(expr_normal, " ");
                char str[5] = {};
                str[0] = parameters[i];
                strcat(expr_normal, str);
                strcat(expr_normal, " ");
            }
            else if (is_op(parameters[i]))
            {
                strcat(expr_normal, " ");
                char str[5] = {};
                str[0] = parameters[i];
                strcat(expr_normal, str);
                strcat(expr_normal, " ");
            }
            else
            {
                strcat(expr_normal, " ");
                while (parameters[i] != ' ' && i < strlen(parameters) && !is_round_bracket(parameters[i]) && !is_op(parameters[i]))
                {
                    char str[5] = {};
                    str[0] = parameters[i];
                    strcat(expr_normal, str);
                    i++;
                }
                i--;
                strcat(expr_normal, " ");
            }
        }

        char expr_temp[100001] = {};
        char *tokens[10001];
        int index = 0;
        char *token = strtok(expr_normal, " ");

        while (token != NULL)
        {
            if (token[0] != '(' && token[0] != ')' && token[0] != '+' && token[0] != '-' && token[0] != '*' && token[0] != '/')
            {
                if (isdigit(token[0]))
                {
                    if (strchr(token, '.') != NULL)
                    {
                        char* str = new char[11];
                        strcpy(str, "float");
                        tokens[index++] = str;
                    }
                    else 
                    {
                        char* str = new char[11];
                        strcpy(str, "int");
                        tokens[index++] = str;
                    }
                }
                else 
                {
                    if (!declared_variable(token))
                    {
                        err_found = true;
                        strcpy(err, "Cannot assing to undeclared variable, line : ");
                        char line_nr[11];
                        sprintf(line_nr, "%d", yylineno);
                        strcat(err, line_nr);
                        strcat(err, "\n Undeclared variable name : ");
                        strcat(err, token);
                        return 0;
                    }

                    int i = 0;   

                    for (int i = 0; i < number_variables_global; ++i)
                        if (strcmp(global_symbols_table[i].name_of_id, token) == 0) tokens[index++] = global_symbols_table[i].var_type;
                        
                    for (int i = 0; i < number_variables_local; ++i)
                        if (strcmp(local_symbols_table[i].name_of_id, token) == 0) tokens[index++] = local_symbols_table[i].var_type;
                }
            }
            token = strtok(NULL, " ");
        }
  
        bool not_good = false;

        for (int i = 0; i < index - 1; ++i)
            if (strcmp(tokens[i], tokens[i + 1]) != 0)
                not_good = true;

        if (not_good == false)
            std::cout << "Expression type : " << tokens[0] << '\n';
        else
        {
            err_found = true;
            strcpy(err, "Expression type enouctered multiple variable types, line : ");
            char line_nr[11];
            sprintf(line_nr, "%d", yylineno);
            strcat(err, line_nr);
            return 0;
        }
    }
}

void assign_expression(char *assing_var, char *assing_val)
{
    if (!declared_variable(assing_var))
    {
        err_found = true;
        strcpy(err, "Cannot assign to undeclared variable, line : ");
        char line_nr[11];
        sprintf(line_nr, "%d", yylineno);
        strcat(err, line_nr);
        strcat(err, "\n Undeclared variable name : ");
        strcat(err, assing_var);
    }

    int val = eval(assing_val);
    
    for (int i = 0; i < number_variables_global; ++i)
        if (strcmp(global_symbols_table[i].name_of_id, assing_var) == 0)
        {
            *((int *)global_symbols_table[i].val) = val;
            return;
        }

    for (int i = 0; i < number_variables_local; ++i)
        if (strcmp(local_symbols_table[i].name_of_id, assing_var) == 0)
        {
            *((int *)local_symbols_table[i].val) = val;
            return;
        }
}

%}

%union
{
    char* string;
    int number;
}

%start program

%token
USER_CLASSES
USER_FUNCTIONS
USER_VARIABLES
PROGRAM_START
PROGRAM_END

IF
ELSE
WHILE
FOR
IN_RANGE

OPERATOR
AND
NOT
OR

PLUS
MINUS
TIMES
DIV
MOD

BOOL_TRUE
BOOL_FALSE

KEY_INPUT
KEY_OUTPUT

FUNCTION
RETURN

CLASS
THIS
ARROW

ASSIGN

START_BRACE
END_BRACE

START_SQUARE_BRACKET
END_SQUARE_BRACKET

START_ROUND_BRACKET
END_ROUND_BRACKET

SEMICOLON
COMMA

%token<string> ID VARIABLE_TYPE STRING
%token<number> NUMBER

%type <string> vars_decl
%type <string> func_call
%type <string> func_args
%type <string> func_args_list
%type <string> expression
%type <string> dims_array
%type <string> logical_expr
%type <string> logical_instr
%type <string> func_ret_type_specifier
%type <string> func_params_decl
%type <string> list_of_func_params_decl

%left NOT
%left AND
%left OR
%left OPERATOR
%left PLUS MINUS
%left TIMES DIV MOD

%%

program:  USER_CLASSES user_classes USER_FUNCTIONS user_functions USER_VARIABLES user_variables { main_program_entry_found = true; } main_code;

user_classes:   /*  EPS */
                |   class_declaration user_classes ;

class_declaration: CLASS ID START_BRACE class_content END_BRACE ;

class_content:    /*  EPS */
                |   vars_decl SEMICOLON class_content
                |   func_entry class_content ;

class_access:   ID ARROW ID
                |   ID ARROW ID dims_array
                |   ID ARROW func_call
                |   THIS ARROW ID
                |   class_access ARROW ID ARROW ID dims_array
                |   class_access ID ARROW ID
		        |   class_access ID ARROW func_call
                |   class_access THIS ARROW ID ;

user_variables: /*  EPS */
                |   { global_declarations = true; } vars_decl SEMICOLON user_variables ;

user_functions: /*  EPS */
                |   user_functions func_entry ;

main_code: PROGRAM_START actual_instructions PROGRAM_END ;

func_ret_type_specifier:   VARIABLE_TYPE { $$ = strdup($1); } ;

func_entry:   func_ret_type_specifier FUNCTION ID START_ROUND_BRACKET func_params_decl END_ROUND_BRACKET START_BRACE actual_instructions END_BRACE { create_function($1, $3, $5); };

func_params_decl: /*  EPS */ { strcpy($$, ""); }
                        |   list_of_func_params_decl {$$ = strdup($1); } ;

list_of_func_params_decl:    vars_decl {strcat($1, " "); $$ = strdup($1); }
                                | vars_decl COMMA list_of_func_params_decl { strcat($1, " "); strcat($1, $3); $$ = strdup($1); } ;

func_call:  FUNCTION ID START_ROUND_BRACKET func_args END_ROUND_BRACKET {
                    if(strcmp($2, "Eval") || strcmp($2, "TypeOf"))
                    {
                        char function[101];
                        int number = type_eval_define($2, $4);
                        sprintf(function, "%d", number);
                        $$ = strdup(function);
                    }
                    else
                        $$ = strdup("0");
                } ;

func_args:    /*  EPS */ { $$ = strdup(""); }
                    |   func_args_list {$$ = strdup($1); }

func_args_list:   expression { $$ = strdup($1); }
                        |   expression COMMA func_args_list { strcat($$, ","); strcat($$, $3); }

return: RETURN expression ;

actual_instructions:    /*  EPS */
                        | actual_instructions vars_decl {if (main_program_entry_found) create_local_variable($2); }  SEMICOLON
                        | actual_instructions assignment SEMICOLON
                        | actual_instructions func_call SEMICOLON
                        | actual_instructions class_access SEMICOLON
                        | actual_instructions if_else_stmt
                        | actual_instructions while_stmt
                        | actual_instructions for_stmt 
                        | actual_instructions return {if (!main_program_entry_found) ;} SEMICOLON
                        | actual_instructions KEY_INPUT ID {if (!main_program_entry_found) ;} SEMICOLON
                        | actual_instructions KEY_INPUT class_access SEMICOLON
                        | actual_instructions KEY_OUTPUT expression {if (!main_program_entry_found) ;} SEMICOLON ;

if_else_stmt:   if_stmt
                |   if_stmt else_stmt ;

if_stmt:    IF START_ROUND_BRACKET logical_expr END_ROUND_BRACKET START_BRACE actual_instructions END_BRACE ;

else_stmt:  ELSE START_BRACE actual_instructions END_BRACE ;

while_stmt: WHILE START_ROUND_BRACKET logical_expr END_ROUND_BRACKET START_BRACE actual_instructions END_BRACE ;

for_stmt:   FOR ID IN_RANGE START_ROUND_BRACKET ID COMMA NUMBER COMMA NUMBER END_ROUND_BRACKET START_BRACE actual_instructions END_BRACE
            |   FOR ID IN_RANGE START_ROUND_BRACKET NUMBER COMMA NUMBER COMMA NUMBER END_ROUND_BRACKET START_BRACE actual_instructions END_BRACE
            |   FOR ID IN_RANGE START_ROUND_BRACKET ID COMMA ID COMMA NUMBER END_ROUND_BRACKET START_BRACE actual_instructions END_BRACE
            |   FOR ID IN_RANGE START_ROUND_BRACKET NUMBER COMMA ID COMMA NUMBER END_ROUND_BRACKET START_BRACE actual_instructions END_BRACE ;

vars_decl:  VARIABLE_TYPE ID {
                if (global_declarations)
                    create_global_variable($1, $2, NULL);
                else
                {
                    char variable[1001] = {};
                    strcpy(variable, $1);
                    strcat(variable, " ");
                    strcat(variable, $2);
                    $$ = strdup(variable);
                }
            }
            |   VARIABLE_TYPE ID dims_array {
                    if (global_declarations)
                        create_global_variable($1, $2, $3);
                    else
                    {
                        char variable[1001] = {};
                        strcpy(variable, $1);
                        strcat(variable, " ");
                        strcat(variable, $2);
                        strcat(variable, "[");
                        strcat(variable, $3);
                        strcat(variable, "]");
                        $$ = strdup(variable);
                    }
                }
            |   ID ID {
                    if (global_declarations)
                        create_global_variable($1, $2, NULL);
                    else
                    {
                        char variable[1001] = {};
                        strcpy(variable, $1);
                        strcat(variable, " ");
                        strcat(variable, $2);
                        $$ = strdup(variable);
                    }
                } ;

assignment: ID ASSIGN expression {if (main_program_entry_found) { char local[1001]; strcpy(local, $1); strcat(local, " "); strcat(local, $3); assert_expression(local); assign_expression($1, $3); } }
            | ID ASSIGN logical_expr { if (main_program_entry_found) { char local[100]; if (strcmp($3, "ADEVARAT") == 0) strcpy(local, "1"); else strcpy(local, "0"); assign_expression($1, local); } }
            | class_access ASSIGN expression 
	        | ID dims_array ASSIGN expression ;

expression:     STRING
                |   ID { if (main_program_entry_found) $$ = strdup($1); }
                |   NUMBER {
                        if (main_program_entry_found)
                        {
                            char local[101];
                            sprintf(local, "%d", $1);
                            $$ = strdup(local);
                        }
                    }
                |   func_call { $$ = strdup($1); }
                |   class_access { $$ = strdup("0"); }
                |   expression PLUS expression {
                        if (main_program_entry_found)
                        {
                            char local[101];
                            strcpy(local, $1);
                            strcat(local, "+");
                            strcat(local, $3);
                            $$ = strdup(local);
                        }
                    }
                |   START_ROUND_BRACKET expression MINUS expression END_ROUND_BRACKET {
                        if (main_program_entry_found)
                        {
                            char local[101];
                            strcpy(local, "(");
                            strcat(local, $2);
                            strcat(local, "-");
                            strcat(local, $4);
                            strcat(local, ")");
                            $$ = strdup(local);
                        }
                    }
                |   expression TIMES expression {
                        if (main_program_entry_found)
                        {
                            char local[101];
                            strcpy(local, $1);
                            strcat(local, "*");
                            strcat(local, $3);
                            $$ = strdup(local);
                        }
                    }
                |   START_ROUND_BRACKET expression PLUS expression END_ROUND_BRACKET {
                        if (main_program_entry_found)
                        {
                            char local[101];
                            strcpy(local, "(");
                            strcat(local, $2);
                            strcat(local, "+");
                            strcat(local, $4);
                            strcat(local, ")");
                            $$ = strdup(local);
                        }
                    }
                |   expression MINUS expression {
                        if (main_program_entry_found)
                        {
                            char local[101];
                            strcpy(local, $1);
                            strcat(local, "-");
                            strcat(local, $3);
                            $$ = strdup(local);
                        }
                    }
                |   START_ROUND_BRACKET expression TIMES expression END_ROUND_BRACKET {
                        if (main_program_entry_found)
                        {
                            char local[101];
                            strcpy(local, "(");
                            strcat(local, $2);
                            strcat(local, "*");
                            strcat(local, $4);
                            strcat(local, ")");
                            $$ = strdup(local);
                        }
                    }
                |   expression DIV expression {
                        if (main_program_entry_found)
                        {
                            char local[101];
                            strcpy(local, $1);
                            strcat(local, "/");
                            strcat(local, $3);
                            $$ = strdup(local);
                        }
                    }
                |   START_ROUND_BRACKET expression DIV expression END_ROUND_BRACKET {
                        if (main_program_entry_found)
                        {
                            char local[101];
                            strcpy(local, "(");
                            strcat(local, $2);
                            strcat(local, "/");
                            strcat(local, $4);
                            strcat(local, ")");
                            $$ = strdup(local);
                        }
                    }
                |   expression MOD expression {
                        if (main_program_entry_found)
                        {
                            char local[101];
                            strcpy(local, $1);
                            strcat(local, "%");
                            strcat(local, $3);
                            $$ = strdup(local);
                        }
                    }
                | START_ROUND_BRACKET expression MOD expression END_ROUND_BRACKET {
                        if(main_program_entry_found)
                        {
                            char local[101];
                            strcpy(local, "(");
                            strcat(local, $2);
                            strcat(local, "%");
                            strcat(local, $4);
                            strcat(local, ")");
                            $$ = strdup(local);
                        }
                    } ;

logical_expr:   logical_instr
                    | BOOL_FALSE {$$ = strdup("FALS");}
                    | BOOL_TRUE {$$ = strdup("ADEVARAT");}
                    | logical_instr AND logical_expr
                    | logical_instr OR logical_expr
                    | logical_instr AND START_ROUND_BRACKET logical_expr END_ROUND_BRACKET
                    | logical_instr OR START_ROUND_BRACKET logical_expr END_ROUND_BRACKET
                    | START_ROUND_BRACKET logical_expr END_ROUND_BRACKET AND logical_instr {$$ = strdup("0");}
                    | START_ROUND_BRACKET logical_expr END_ROUND_BRACKET OR logical_instr {$$ = strdup("0");}
                    | NOT START_ROUND_BRACKET logical_expr END_ROUND_BRACKET {$$ = strdup("0");};

logical_instr:    expression OPERATOR expression {$$ = strdup("0");} 
                | NOT expression {$$ = strdup("0");}

dims_array:    dims_array START_SQUARE_BRACKET NUMBER END_SQUARE_BRACKET {
                    char local[101];
                    sprintf(local, "%d", $3);
                    strcat($$, ",");
                    strcat($$, local);
                }
                |   START_SQUARE_BRACKET NUMBER END_SQUARE_BRACKET {
                    char local[101];
                    sprintf(local, "%d", $2);
                    $$=strdup(local);
                } ;
%%

int main(int argc, char* argv[])
{
    yyin = fopen(argv[1], "r");
    yyparse();
    if (err_found) yyerror(err);
    else
    {
        std::cout << "\nGLOBAL VARIABLES TABLE : \n \n";
        display_global_variables(); 
        std::cout << "\nLOCAL VARIABLES TABLE : \n \n";
        display_local_variables();
        std::cout << "\nFUNCTIONS TABLE : \n \n";
        display_function();
    }
    return 0;
}