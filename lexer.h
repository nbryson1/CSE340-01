/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 */
#ifndef __LEXER__H__
#define __LEXER__H__

#include <vector>
#include <string>

#include "inputbuf.h"
#include <list>

// ------- token types -------------------

typedef enum { END_OF_FILE = 0,
    COMMA, LPAREN, RPAREN,
    DOT, ID, ERROR, INPUT_TEXT, HASH,
    CHAR, UNDERSCORE, OR, SYMBOL, STAR // TODO: Add labels for new token types here
} TokenType;

struct REG_node{
    struct REG_node * first_neighbor;
    char first_label;
    struct REG_node * second_neighbor;
    char second_label;
};

struct REG{
    struct REG_node * start;
    struct REG_node * accept;
};

struct list{
    std::string token_name;
    std::string * reg_node;
};

class Token {
  public:
    void Print();

    std::string lexeme;
    TokenType token_type;
    int line_no;
};

class LexicalAnalyzer {
  public:
    Token GetToken();
    TokenType UngetToken(Token);
    LexicalAnalyzer();

  private:
    std::vector<Token> tokens;
    int line_no;
    Token tmp;
    InputBuffer input;

    bool SkipSpace();
    Token ScanInput();
    Token ScanSymbol();
    Token ScanIdOrChar();

};


class myLexicalAnalyzer {
public:
    Token my_getToken();
    std::string match(REG_node, std::string, std::string);

private:
    std::list<REG_node> tokens;
    std::string input_string;

};

#endif  //__LEXER__H__
