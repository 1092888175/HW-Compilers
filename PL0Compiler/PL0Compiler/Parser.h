#pragma once

#include "Lexer.h"


/*
	�﷨������
*/

class Parser
{
public:
	Parser(Lexer& lexer) {this->lexer = &lexer;}
	~Parser() {}
	bool BeginParse(); // ����  �����﷨����
	
private:
	Token token;// ��ǰ����
	Lexer* lexer=nullptr;// �ʷ�������

	void getNextToken() { token = lexer->getNextToken(); }

	
	void ProgramHead(); // �����ײ�
	void SubProgram(); //�ֳ���
	void ConstDeclaration();// ��������
	void ConstDefinition();//��������
	void VarDeclaration();//��������

	void Statement();//���
	void AssignmentStat();//��ֵ���
	void ConditionalStat();//�������
	void LoopStat();//ѭ�����
	void CompoundStat();//�������
	

	void Expression();//���ʽ
	void Item();//��
	void Factor();//����

	void Condition();//����


	
};


