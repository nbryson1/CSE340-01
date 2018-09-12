/*
 * Copyright (C) Rida Bazzi, 2017
 *           (C) Raha Moraffah 2018
 * Do not share this file with anyone
 */
#include <iostream>
#include <istream>
#include <cctype>
#include <stdio.h>
#include <sstream>
#include "lexer.h"

using namespace std;

string reserved[] = { "END_OF_FILE",
                      "COMMA", "LPAREN", "RPAREN",
                        "DOT", "ID", "ERROR", "INPUT_TEXT", "HASH",
                      "CHAR", "UNDERSCORE", "OR", "SYMBOL", "STAR" };



void Token::Print()
{
    cout << "{" << this->lexeme << " , "
         << reserved[(int) this->token_type] << " , "
         << this->line_no << "}\n";
}

LexicalAnalyzer::LexicalAnalyzer()
{
    this->line_no = 1;
    tmp.lexeme = "";
    tmp.line_no = 1;
    tmp.token_type = ERROR;
}

bool LexicalAnalyzer::SkipSpace()
{
    char c;
    bool space_encountered = false;

    input.GetChar(c);
    line_no += (c == '\n');

    while (!input.EndOfInput() && isspace(c)) {
        space_encountered = true;
        input.GetChar(c);
        line_no += (c == '\n');
    }

    if (!input.EndOfInput()) {
        input.UngetChar(c);
    }
    return space_encountered;
}


Token LexicalAnalyzer::ScanIdOrChar()
{
    char c;
    input.GetChar(c);

    if (isalpha(c)) {
        tmp.lexeme = "";
        int no = 0;

        while (!input.EndOfInput() && isalnum(c)) {
            tmp.lexeme += c;
            input.GetChar(c);
            no++;
        }
        tmp.line_no = line_no;
        if(no == 1)
            tmp.token_type = CHAR;
        else
            tmp.token_type = ID;


        if (!input.EndOfInput()) {

            input.UngetChar(c);
        }

    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
    }
    return tmp;
}


Token LexicalAnalyzer::ScanInput()
{
    char c;
    input.GetChar(c);
    string lexeme = "";
    if (c == '"') {
        tmp.lexeme = "";
        lexeme += '"';
        Token symbol = ScanSymbol();
        while (symbol.token_type == SYMBOL) {
            lexeme += symbol.lexeme;
            symbol = ScanSymbol();
        }
        if (!input.EndOfInput()) {
            input.GetChar(c);
            if (c == '"') {
                lexeme += c;
                tmp.lexeme += lexeme;
                tmp.token_type = INPUT_TEXT;
            }
            else{
                tmp.lexeme = "";
                tmp.token_type = ERROR;
            }

        }
        else{
            tmp.lexeme = "";
            tmp.token_type = ERROR;
        }

        tmp.line_no = line_no;

    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
    }
    return tmp;
}

Token LexicalAnalyzer::ScanSymbol()
{
    char c;
    input.GetChar(c);
    tmp.lexeme = "";
    if (isspace(c) || isalnum(c)) {
        //tmp.lexeme += c;
        while (!input.EndOfInput() && (isspace(c) || isalnum(c))) {
            line_no += (c == '\n');
            tmp.lexeme += c;
            input.GetChar(c);
        }
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.line_no = line_no;

        tmp.token_type = SYMBOL;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
    }
    return tmp;
}




// you should unget tokens in the reverse order in which they
// are obtained. If you execute
//
//    t1 = lexer.GetToken();
//    t2 = lexer.GetToken();
//    t3 = lexer.GetToken();
//
// in this order, you should execute
//
//    lexer.UngetToken(t3);
//    lexer.UngetToken(t2);
//    lexer.UngetToken(t1);
//
// if you want to unget all three tokens. Note that it does not
// make sense to unget t1 without first ungetting t2 and t3
//
TokenType LexicalAnalyzer::UngetToken(Token tok)
{
    tokens.push_back(tok);;
    return tok.token_type;
}

Token LexicalAnalyzer::GetToken()
{
    char c;

    // if there are tokens that were previously
    // stored due to UngetToken(), pop a token and
    // return it without reading from input
    if (!tokens.empty()) {
        tmp = tokens.back();
        tokens.pop_back();
        return tmp;
    }

    SkipSpace();
    tmp.lexeme = "";
    tmp.line_no = line_no;
    input.GetChar(c);
    switch (c) {
        case '.':
            tmp.token_type = DOT;
            return tmp;
        case '*':
            tmp.token_type = STAR;
            return tmp;

        case ',':
            tmp.token_type = COMMA;
            return tmp;

        case '(':
            tmp.token_type = LPAREN;
            return tmp;
        case ')':
            tmp.token_type = RPAREN;
            return tmp;

        //UNDERSCORE
        case '_':
            tmp.token_type = UNDERSCORE;
            return tmp;

        //HASH
        case '#':
            tmp.token_type = HASH;
            return tmp;
        //OR
        case '|':
            tmp.token_type = OR;
            return tmp;
        //INPUT
        case '"':
            input.UngetChar(c);
            return ScanInput();
        default:
            if (isdigit(c)) {
                //CHAR
                //input.UngetChar(c);
                //return ScanNumber();
                tmp.token_type = CHAR;
                tmp.lexeme = c;
            } else if (isalpha(c)) {
                input.UngetChar(c);
                return ScanIdOrChar();
            }
            else if (input.EndOfInput())
                tmp.token_type = END_OF_FILE;
            else
                tmp.token_type = ERROR;

            return tmp;
    }
}

set<RegNode> myLexicalAnalyzer::match_one_char(set<RegNode> S, char c){
    set<RegNode> S1 ;
    set<RegNode>::iterator it;
    for (it = S.begin(); it != S.end(); ++it) {
        if(it->first_label == c && !checkNode(S1, it->first_neighbor)){
            S1.insert(*it->first_neighbor);
        }
        if(it->second_label == c && !checkNode(S1, it->second_neighbor)){
            S1.insert(*it->second_neighbor);
        }
    }
    if(S1.empty()){
        return S1;
    }
    bool changed = true;
    std::set<RegNode> S2;
    while(changed){
        changed = false;
        for (it = S1.begin(); it != S1.end(); ++it) {
            S2.insert(*it);
            set<RegNode>::iterator ik;
            for (ik = S2.begin(); ik != S2.end(); ++ik)
            {
//                cout << ik->id << endl;
            }
//            cout << "ID-Label:" << it->id << endl;
            if(it->first_label == '_' && !checkNode(S2, it->first_neighbor)){
                S2.insert(*it->first_neighbor);
//                cout << "ID" << it->first_neighbor->id << endl;
//                cout << "S2 size:" << S2.size() << endl;
                set<RegNode>::iterator ik;
                for (ik = S2.begin(); ik != S2.end(); ++ik)
                {
//                    cout << ik->id << endl;
                }
            }
            if(it->second_label == '_' && !checkNode(S2, it->second_neighbor)){
                S2.insert(*it->second_neighbor);
            }
        }
        if(!compare_set(S1, S2)){
            changed = true;
            S1.insert(S2.begin(), S2.end());
            S2.clear();
        }
    }
//    cout << "S1 size:" << S1.size() << endl;
    return S1;
}

int myLexicalAnalyzer::match(REG * reg, string input, int position){
    set<RegNode> So;
    So.insert(*reg->start);
//    cout << reg->start << endl;
//    cout << "So size:" << So.size() << endl;
    std::set<RegNode>::iterator it;
    for (it = So.begin(); it != So.end(); ++it)
    {
//        cout << it->id << endl;
    }
//    cout << "Char:_" << endl;
    set<RegNode> Sa;
    Sa = match_one_char(So, '_');
    Sa.insert(So.begin(), So.end());
    So.insert(Sa.begin(), Sa.end());
//    cout << "Comparison:" << compare_set(Sa, So) << endl;
//    cout << "So size:" << So.size() << endl;
    for (it = So.begin(); it != So.end(); ++it)
    {
//        cout << it->id << endl;
    }
    int size = 0;
    int m = 0;
    for (std::string::iterator it = input.begin() + position, end = input.end(); it != end; ++it)
    {
        size ++;
//        cout << "Char:" << *it << endl;
        So = match_one_char(So, *it);
//        cout << "So size:" << So.size() << endl;
        std::set<RegNode>::iterator ik;
        for (ik = So.begin(); ik != So.end(); ++ik)
        {
//            cout << ik->id << endl;
        }
        if(So.find(*reg->accept) != So.end() ){
            m = size;
//            cout << "Return" << endl;
//            return size;
        }
//        if(!(So.find(*reg->accept) != So.end()) && matched){
//            size --;
//            return size;
//        }

    }
    return m;
}

void myLexicalAnalyzer::my_getToken(){
    int start = 0;
    set<token_reg>::iterator it;
    string lex = "";
    vector<string> splits = split(input_string, ' ');
    for(int i =0; i < splits.size(); i++) {
        start = 0;
        while (start != splits[i].size()) {
//            cout << "Split " << i << " " << splits[i] << endl;
            int max = 0;
            for (it = getTokens_list().begin(); it != getTokens_list().end(); ++it) {
//                cout << "Running token:" << it->token_name << endl;
                int lex_size = match(it->reg, splits[i], start);
                if (lex_size > max) {
                    max = lex_size;
                    lex = it->token_name;
                }
            }
            if (max == 0) {
                cout << "ERROR";
                return;
            }
            cout << lex << " , \"" << splits[i].substr(start, max) << "\"" << endl;
            start += max;
        }
    }
}

void myLexicalAnalyzer::setInput_string(const string &input_string) {
    myLexicalAnalyzer::input_string = input_string;
}

void myLexicalAnalyzer::setTokens_list(const token_reg &token) {
    myLexicalAnalyzer::tokens_list.insert(token);
}

const set<token_reg> &myLexicalAnalyzer::getTokens_list() const {
    return tokens_list;
}

vector<string> myLexicalAnalyzer::split(string s, char delimit) {
    vector<string> temp;
    stringstream ss(s); // Turn the string into a stream.
    string t;
    while(getline(ss, t, delimit)) {
        temp.push_back(t);
    }
    return temp;
}

int myLexicalAnalyzer::counter;

int myLexicalAnalyzer::getCounter() const {
    counter ++;
    return counter;
}

void myLexicalAnalyzer::setCounter(int count) {
    counter = count;
}

bool myLexicalAnalyzer::checkNode(set<RegNode> S, RegNode * r){
    set<RegNode>::iterator i;
    for (i = S.begin(); i != S.end(); ++i)
    {
        if(r->id == i->id){
            return true;
        }
    }
    return false;
}

bool myLexicalAnalyzer::compare_set(set<RegNode> S1, set<RegNode> S2){
//    cout << "Compare1:" << S1.size() << endl;
//    cout << "Compare2:" << S2.size() << endl;
    if(S1.size() != S2.size()){
        return false;
    }
    set<RegNode>::iterator i;
    int s1[S1.size()];
    int s2[S2.size()];
    int j = 0, k= 0;
    for (i = S1.begin(); i != S1.end(); ++i)
    {
        s1[j] = i->id;
        j++;
    }
    set<RegNode>::iterator t;
    for (i = S2.begin(); i != S2.end(); ++i)
    {
        s2[k] = i->id;
        k++;
    }
    sort(s1, s1+j);
    sort(s2, s2+k);

    for (int m=0; m<k; m++) {
        if (s1[m] != s2[m]) {
            return false;
        }
    }
    return true;
}