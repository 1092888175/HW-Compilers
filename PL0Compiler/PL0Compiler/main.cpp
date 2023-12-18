#include<iostream>
#include<string>
#include<unordered_map>
#include<cctype>
#include <fstream>
#include <sstream>
#include<stdexcept>
#include"Lexer.h"
#include"Parser.h"
std::unordered_map<TokenType, std::string> tokenNames = {
    {PROGRAM, "PROGRAM"}, {VAR, "VAR"}, {CONST, "CONST"}, {BEGIN, "BEGIN"},
    {END, "END"}, {WHILE, "WHILE"}, {DO, "DO"}, {IF, "IF"}, {THEN, "THEN"},
    {IDENTIFIER, "IDENTIFIER"}, {NUMBER, "NUMBER"},
    {PLUS, "PLUS"}, {MINUS, "MINUS"}, {MULTIPLY, "MULTIPLY"}, {DIVIDE, "DIVIDE"},
    {ASSIGN, "ASSIGN"}, {EQUAL, "EQUAL"}, {NOT_EQUAL, "NOT_EQUAL"},
    {LESS, "LESS"}, {LESS_EQUAL, "LESS_EQUAL"}, {GREATER, "GREATER"},
    {GREATER_EQUAL, "GREATER_EQUAL"},
    {LEFT_PAREN, "LEFT_PAREN"}, {RIGHT_PAREN, "RIGHT_PAREN"},
    {SEMICOLON, "SEMICOLON"}, {COMMA, "COMMA"},
    {END_OF_FILE, "END_OF_FILE"}, {UNKNOWN, "UNKNOWN"}
};
int main() {
    // ָ��Դ�����ļ���·��
    std::string filePath = "source/source.pl0";

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "failed to open: " << filePath << std::endl;
        return 1;
    }

    // ��ȡ�ļ����ݵ��ַ���
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string sourceCode = buffer.str();

    // ����һ���ʷ�������ʵ��
    Lexer lexer(sourceCode);
    Parser parser(lexer);
    bool flag=parser.BeginParse();

    if (flag)std::cout << "�﷨�����ɹ�" << std::endl;

    // �ʷ��������Գ���
    //// ѭ����ȡ���е�Token��ֱ�������ļ�������
    //Token token = lexer.getNextToken();
    //while (token.getType() != END_OF_FILE) {
    //    // ʹ��ӳ�������ȡToken���͵�����
    //    std::string tokenName = tokenNames[token.getType()];

    //    // ��ӡToken���������ơ����ͺ�ֵ
    //    std::cout << "Token: " << tokenName << " (" << token.getType() << "), Value: " << token.getValue() << std::endl;

    //    // ��ȡ��һ��Token
    //    token = lexer.getNextToken();
    //}

    return 0;
}
