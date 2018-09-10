/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 *
 * Do not post this file or derivatives of
 * of this file online
 *
 */
#include <iostream>
#include <cstdlib>
#include "parser.h"

using namespace std;

void Parser::syntax_error()
{
    cout << "SYNTAX ERROR\n";
    exit(1);
}

// this function gets a token and checks if it is
// of the expected type. If it is, the token is
// returned, otherwise, synatx_error() is generated
// this function is particularly useful to match
// terminals in a right hand side of a rule.
// Written by Mohsen Zohrevandi
Token Parser::expect(TokenType expected_type)
{
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}

// this function simply checks the next token without
// consuming the input
// Written by Mohsen Zohrevandi
Token Parser::peek()
{
    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    return t;
}

// Parsing

void Parser::parse_input()
{
    //input -> tokens_section INPUT_TEXT

    parse_tokens_section();
    Token t = expect(INPUT_TEXT);
    mylexer.setInput_string(t.lexeme);
}


void Parser::parse_tokens_section()
{
    // tokens_section -> token_list HASH
    parse_token_list();
    expect(HASH);
}

void Parser::parse_token_list()
{
    // token_list -> token
    // token_list -> token COMMA token_list

    parse_token();
    Token t = peek();
    if (t.token_type == COMMA)
    {
        // token_list -> token COMMA token_list
        expect(COMMA);
        parse_token_list();
    }
    else if (t.token_type == HASH)
    {
        // token_list -> token
    }
    else
    {
        syntax_error();
    }

}

void Parser::parse_token()
{
    // token -> ID expr

    Token t = expect(ID);
    REG * reg = parse_expr();
    token_reg tok;
    tok.token_name = t.lexeme;
    tok.reg = reg;
    mylexer.setTokens_list(tok);
}


struct REG * Parser::parse_expr()
{
    // expr -> CHAR
    // expr -> LPAREN expr RPAREN DOT LPAREN expr RPAREN
    // expr -> LPAREN expr RPAREN OR LPAREN expr RPAREN
    // expr -> LPAREN expr RPAREN STAR
    // expr -> UNDERSCORE

    Token t = lexer.GetToken();
//    cout<<t.lexeme;
    if(t.token_type == CHAR){
        // expr -> CHAR
        REG * reg = (REG *)malloc(sizeof(* reg));
        RegNode * regNode1 = (RegNode *)malloc(sizeof(* regNode1));
        regNode1->first_label = t.lexeme.at(0);
        RegNode * regNode2 = (RegNode *)malloc(sizeof(* regNode2));
        regNode1->first_neighbor = regNode2;
        reg->start = regNode1;
        reg->accept = regNode2;
        return reg;
    }
    else if (t.token_type == UNDERSCORE){
        // expr -> UNDERSCORE
        REG * reg = (REG *)malloc(sizeof(* reg));
        RegNode * regNode1 = (RegNode *)malloc(sizeof(* regNode1));
        regNode1->first_label = t.lexeme.at(0);
        RegNode * regNode2 = (RegNode *)malloc(sizeof(* regNode2));
        regNode1->first_neighbor = regNode2;
        reg->start = regNode1;
        reg->accept = regNode2;
        return reg;
    }
    else if(t.token_type == LPAREN){
        // expr -> LPAREN expr RPAREN DOT LPAREN expr RPAREN
        // expr -> LPAREN expr RPAREN OR LPAREN expr RPAREN
        // expr -> LPAREN expr RPAREN STAR
        REG * reg1 = parse_expr();
        expect(RPAREN);
        Token t2 = lexer.GetToken();
        if(t2.token_type == DOT || t2.token_type == OR){
            expect(LPAREN);
            REG * reg2 = parse_expr();
            expect(RPAREN);
            if (t2.token_type == DOT){
                reg1->accept->first_neighbor = reg2->start;
                reg1->accept->first_label = '_';
                reg1->accept = reg2->accept;
                return reg1;
            }
            else{
                RegNode * start = (RegNode *)malloc(sizeof(* start));
                start->first_label = '_';
                start->second_label = '_';
                start->first_neighbor = reg1->start;
                start->second_neighbor = reg2->start;
                RegNode * accept = (RegNode *)malloc(sizeof(* accept));
                reg1->accept->first_label = '_';
                reg1->accept->first_neighbor = accept;
                reg2->accept->first_label = '_';
                reg2->accept->first_neighbor = accept;
                reg1->start = start;
                reg1->accept = accept;
                return reg1;
            }
        }
        else if(t2.token_type == STAR)
        {
            RegNode * start = (RegNode *)malloc(sizeof(* start));
            start->first_label = '_';
            start->second_label = '_';
            start->first_neighbor = reg1->start;
            RegNode * accept = (RegNode *)malloc(sizeof(* accept));
            start->second_neighbor = accept;
            reg1->accept->first_label = '_';
            reg1->accept->first_neighbor = accept;
            reg1->accept->second_label = '_';
            reg1->accept->second_neighbor = reg1->start;
            reg1->start = start;
            reg1->accept = accept;
            return reg1;
        }
        else if(t2.token_type == HASH){
            lexer.UngetToken(t2);
        }
        return reg1;
    }
    else
    {
        syntax_error();
    }
}


void Parser::ParseProgram()
{
    parse_input();
    expect(END_OF_FILE);
    mylexer.my_getToken();
}

int main()
{
    Parser parser;
    parser.ParseProgram();
}
