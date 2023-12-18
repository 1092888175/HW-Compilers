#pragma once
#include<iostream>
#include<string>
#include<unordered_map>
#include<cctype>
#include <fstream>
#include <sstream>
#include<stdexcept>

// ���дʷ�����ö������
enum TokenType {
    PROGRAM, VAR, CONST, BEGIN, END, WHILE, DO, IF, THEN, 
    IDENTIFIER, NUMBER, 
    PLUS, MINUS, MULTIPLY, DIVIDE, ASSIGN, EQUAL, NOT_EQUAL,
    LESS, LESS_EQUAL, GREATER, GREATER_EQUAL,
    LEFT_PAREN, RIGHT_PAREN, SEMICOLON, COMMA,
    END_OF_FILE, UNKNOWN
};

// �ʷ�������״̬
enum State {
    START, IN_COMMENT, IN_NUMBER, IN_IDENTIFIER, IN_ASSIGN, IN_LESS, IN_GREATER, IN_ERROR
};


// ������
class Token {
public:
    Token(TokenType type, std::string value) : type(type), value(value) {}

    TokenType getType() const { return type; }
    std::string getValue() const { return value; }

private:
    TokenType type;
    std::string value;
};

// �ʷ�������
class Lexer {
private:
    std::string source;
    size_t position;
    int line, col; // ����кź��кţ��������������Ϣ

    State currentState;
public:
    Lexer(const std::string& source) : 
        source(source), position(0), line(1), col(1), currentState(START) {};
    
    // �����һ������
    Token getNextToken();

private:
    void advance() ;
    void handleWhitespace() ;
    Token identifierOrKeyword() ;
    Token number() ;
    TokenType determineIdentifierType(const std::string &str) ;
    Token handleSymbol() ;

};