/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 */
#ifndef __LEXER__H__
#define __LEXER__H__

#include <vector>

#include "inputbuf.h"
#include <set>

// ------- token types -------------------

typedef enum { END_OF_FILE = 0,
    COMMA, LPAREN, RPAREN,
    DOT, ID, ERROR, INPUT_TEXT, HASH,
    CHAR, UNDERSCORE, OR, SYMBOL, STAR // TODO: Add labels for new token types here
} TokenType;

typedef struct RegNode{
    RegNode * first_neighbor;
    char first_label;
    RegNode * second_neighbor;
    char second_label;
    int id;
    RegNode(){
        first_label = '\0';
        second_label = '\0';
        first_neighbor = NULL;
        second_neighbor = NULL;
    }
} RegNode;

typedef struct REG{
    RegNode * start;
    RegNode * accept;
} REG;

typedef struct token_reg{
    std::string token_name;
    REG * reg;
} token_reg;

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

static bool operator<(const RegNode& n1, const RegNode& n2) {
    return n1.id < n2.id;
}

//static bool operator==(const RegNode& n1, const RegNode& n2) {
//    return n1.id == n2.id;
//}

//static bool operator<(const token_reg& n1, const token_reg& n2) {
//    return n1.token_name < n2.token_name;
//}
//
//static bool operator==(const token_reg& n1, const token_reg& n2) {
//    return n1.token_name == n2.token_name;
//}

class myLexicalAnalyzer {
public:
    void my_getToken();
    int match(REG * reg, std::string, int);
    std::set<RegNode> match_one_char(std::set<RegNode> S, char c);
    void setInput_string(const std::string &input_string);
    void setTokens_list(const token_reg &token);
    const std::vector<token_reg> &getTokens_list() const;
    std::vector<std::string> split(std::string str, char delimit);
    int getCounter() const;
    static void setCounter(int counter);
    bool checkNode(std::set<RegNode> S, RegNode * r);
    bool compare_set(std::set<RegNode> S1, std::set<RegNode> S2);


private:
    std::vector<token_reg> tokens_list;
    std::string input_string;
    static int counter;

};

#endif  //__LEXER__H__
