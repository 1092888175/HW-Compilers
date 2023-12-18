#include<iostream>
#include<string>
#include<unordered_map>
#include<cctype>
#include <fstream>
#include <sstream>
#include<stdexcept>
#include"Lexer.h"

/*
    �ʷ�������ʵ��
*/



Token Lexer::getNextToken() {
    // ѭ������Դ�����е�ÿ���ַ�
    while (position < source.length()) {
        char current = source[position];

        // ���ݵ�ǰ��״̬�������ַ�
        switch (currentState) {
        case START:
            // ���Կհ��ַ�������״̬��
            if (isspace(current)) {
                handleWhitespace();
            }
            // �������ĸ��ת�Ƶ���ʶ��״̬
            else if (isalpha(current)) {
                currentState = IN_IDENTIFIER;
            }
            // ��������֣�ת�Ƶ�����״̬
            else if (isdigit(current)) {
                currentState = IN_NUMBER;
            }
            // �������������Ϊ���Ż������ַ�
            else {
                return handleSymbol();
            }
            break;

        case IN_IDENTIFIER:
            // �����ʶ����ؼ��ֲ�����Token
            return identifierOrKeyword();

        case IN_NUMBER:
            // �������ֲ�����Token
            return number();

            // ���� ':=' ����
        case IN_ASSIGN:
            if (current == '=') {
                advance(); // ���� '='
                currentState = START;
                return Token(ASSIGN, ":=");
            }
            else {
                currentState = START;
                return Token(UNKNOWN, ":");
            }

            // ���� '<', '<>', '<=' ����
        case IN_LESS:
            if (current == '>') {
                advance(); // ���� '>'
                currentState = START;
                return Token(NOT_EQUAL, "<>");
            }
            else if (current == '=') {
                advance(); // ���� '='
                currentState = START;
                return Token(LESS_EQUAL, "<=");
            }
            else {
                currentState = START;
                return Token(LESS, "<");
            }

            // ���� '>' �� '>=' ����
        case IN_GREATER:
            if (current == '=') {
                advance(); // ���� '='
                currentState = START;
                return Token(GREATER_EQUAL, ">=");
            }
            else {
                currentState = START;
                return Token(GREATER, ">");
            }

        default:
            // ���״̬δ֪���׳��쳣
            throw std::runtime_error("Lexer in unknown state");
        }
    }

    // �ļ�ĩβʱ����END_OF_FILE
    return Token(END_OF_FILE, "");
}

void Lexer::advance() {
    if (source[position] == '\n') {
        line++;
        col = 1;
    }
    else {
        col++;
    }
    position++;
}

void Lexer::handleWhitespace() {
    while (position < source.length() && isspace(source[position])) {
        advance();
    }
    currentState = START;
}

Token Lexer::identifierOrKeyword() {
    size_t start = position;
    while (position < source.length() && isalnum(source[position])) {
        advance();
    }
    std::string value = source.substr(start, position - start);
    currentState = START;
    return Token(determineIdentifierType(value), value);
}

Token Lexer::number() {
    size_t start = position;
    while (position < source.length() && isdigit(source[position])) {
        advance();
    }
    std::string value = source.substr(start, position - start);
    currentState = START;
    return Token(NUMBER, value);
}

TokenType Lexer::determineIdentifierType(const std::string& str) {
    static const std::unordered_map<std::string, TokenType> keywords = {
        {"PROGRAM", PROGRAM}, {"VAR", VAR}, {"CONST", CONST}, {"BEGIN", BEGIN}, {"END", END}, {"WHILE", WHILE}, {"DO", DO}, {"IF", IF}, {"THEN", THEN} };

    auto it = keywords.find(str);
    if (it != keywords.end()) {
        return it->second;
    }
    return IDENTIFIER;
}

Token Lexer::handleSymbol() {
    char current = source[position];
    advance(); // �����ȶ�ȡ��ǰ�ַ�

    switch (current) {
    case '+': return Token(PLUS, "+");
    case '-': return Token(MINUS, "-");
    case '*': return Token(MULTIPLY, "*");
    case '/': return Token(DIVIDE, "/");
    case '=': return Token(EQUAL, "=");
    case '<':
        if (source[position] == '>') {
            advance();
            return Token(NOT_EQUAL, "<>");
        }
        else if (source[position] == '=') {
            advance();
            return Token(LESS_EQUAL, "<=");
        }
        else {
            return Token(LESS, "<");
        }
    case '>':
        if (source[position] == '=') {
            advance();
            return Token(GREATER_EQUAL, ">=");
        }
        else {
            return Token(GREATER, ">");
        }
    case ':':
        if (source[position] == '=') {
            advance();
            return Token(ASSIGN, ":=");
        }
        else {
            throw std::runtime_error("Unexpected character: ':', expected ':='");
        }
    case '(': return Token(LEFT_PAREN, "(");
    case ')': return Token(RIGHT_PAREN, ")");
    case ';': return Token(SEMICOLON, ";");
    case ',': return Token(COMMA, ",");
    default:
        throw std::runtime_error("Unexpected character: " + std::string(1, current));
    }
}
