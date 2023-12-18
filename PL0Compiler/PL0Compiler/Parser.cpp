
#include"Parser.h"
#include"PL0Exception.h"

/*
	<����> => <���ʽ><��ϵ�����><���ʽ>

	Condition��ָ��ָ��Condition��һ��
*/
void Parser::Condition() {
	Expression();
	if (token.getType() != TokenType::EQUAL &&
		token.getType() != TokenType::NOT_EQUAL &&
		token.getType() != TokenType::LESS &&
		token.getType() != TokenType::LESS_EQUAL &&
		token.getType() != TokenType::GREATER &&
		token.getType() != TokenType::GREATER_EQUAL
		) {
		throw PL0Exception("�Ƿ��Ĺ�ϵ�����", lexer->getLine(), lexer->getCol());
	}
	Expression();
}

/*
	<�������> => IF <����> THEN <���>
*/
void Parser::ConditionalStat() {
	// ��ʱtoken�Ѿ�ָ��IF
	Condition();
	
	if (token.getType() != TokenType::THEN) {
		throw PL0Exception("��������ʽ����", lexer->getLine(), lexer->getCol());
	}
	getNextToken();
	Statement();
}

/*
	<ѭ�����> => WHILE <����> DO <���>
*/
void Parser::LoopStat() {
	Condition();
	
	if (token.getType() != TokenType::DO) {
		throw PL0Exception("ѭ������ʽ����", lexer->getLine(), lexer->getCol());
	}
	getNextToken();
	Statement();
}


/*
	<����> => <��ʶ��>|<�޷�������>|(<���ʽ>)
*/
void Parser::Factor() {
	
	if (token.getType() == TokenType::IDENTIFIER) {
		// ʶ��Ϊ��ʶ��

	}
	else if (token.getType() == TokenType::NUMBER) {
		// ʶ��Ϊ����

	}
	else if (token.getType() == TokenType::LEFT_PAREN) {
		// ʶ��Ϊ���ʽ


		Expression();
	
		if (token.getType() != TokenType::RIGHT_PAREN) {
			throw PL0Exception("ȱ��������", lexer->getLine(), lexer->getCol());
		}

		

	}
	else {
		throw PL0Exception("ȱ�����ӻ����Ӹ�ʽ����", lexer->getLine(), lexer->getCol());
	}
}

/*
	<��> => <����>|<��><�˷������><����>
	���Եȼ�Ϊ��
	<��> => <����> { <�˷������> <����> }

	Item��ָ��ָ��Item��һ��
*/
void Parser::Item() {
	Factor();
	getNextToken();
	while (true) {
		if (token.getType() == TokenType::MULTIPLY || token.getType() == TokenType::DIVIDE) {
			// ���ں������
			// ���崦��
			getNextToken();
			Factor();
			getNextToken();
		}
		else
			break;
	}


}

/*
	<���ʽ> => [+|-]��|<���ʽ><�ӷ������><��>
	���Եȼ�Ϊ��
	<���ʽ> => [<�ӷ������>] �� { <�ӷ������> �� }

	Expression��ָ��ָ��Expression��һ��
*/
void Parser::Expression() {
	getNextToken();
	if (token.getType() == TokenType::PLUS || token.getType() == TokenType::MINUS) {
		// ���崦��

		getNextToken();
	}
	Item();//Item�����Ѿ���ָ��ָ����Item��һ��
	while (true) {
		if (token.getType() == TokenType::PLUS || token.getType() == TokenType::MINUS) {
			// ���ں����
			// ���崦��
			getNextToken();
			Item();//Item�����Ѿ���ָ��ָ����Item��һ��
		}
		else
			break;
	}
	
}

/*
	<��ֵ���> => <��ʶ��> := <���ʽ>
*/
void Parser::AssignmentStat() {
	// ��ʱtoken�Ѿ�ָ��IDENTIFIER
	std::string sym_name = token.getValue();

	getNextToken();
	if (token.getType() != TokenType::ASSIGN) {
		throw PL0Exception("��ֵ����ʽ����", lexer->getLine(), lexer->getCol());
	}
	Expression();
	// finish ����Ϊ����������м��������
}

/*
	<�������> => BEGIN <���>{;<���>}END
*/
void Parser::CompoundStat() {
	// ��ʱTOKENָ��BEGIN
	token = lexer->getNextToken();;
	Statement();
	while (true) {
		if (token.getType() == TokenType::SEMICOLON) {
			//������ֺ�
			getNextToken();
			Statement();
		}
		else if (token.getType() == TokenType::END) {
			//������END
			break;
		}

		else {
			//����������
			getNextToken();
			if (token.getType() == TokenType::SEMICOLON) {
				//��������ֺ�
				getNextToken();
				Statement();
			}
			else if (token.getType() == TokenType::END) {
				//��������END
				break;
			}
			else {
				throw PL0Exception("��������ʽ����", lexer->getLine(), lexer->getCol());
			}
		}
	}
}
/*
	<����˵��> => VAR<��ʶ��>{,<��ʶ��>};
*/
void Parser::VarDeclaration() {
	// ��ʱtoken�Ѿ�ָ��VAR
	getNextToken();
	if (token.getType() != TokenType::IDENTIFIER) {
		throw PL0Exception("����˵����ʽ����", lexer->getLine(), lexer->getCol());
	}
	std::string sym_name = token.getValue();//sym_nameΪ��ʶ�������˴������м����
	getNextToken();
	while (true) {
		if (token.getType() == TokenType::COMMA) {
			getNextToken();
			if (token.getType() != TokenType::IDENTIFIER) {
				throw PL0Exception("����˵����ʽ����", lexer->getLine(), lexer->getCol());
			}
			std::string sym_name = token.getValue();//sym_nameΪ��ʶ�������˴������м����
			getNextToken();
		}
		else break;
	}
	if (token.getType() != TokenType::SEMICOLON) {
		throw PL0Exception("����˵����ʽ����ȱ�ٷֺ�", lexer->getLine(), lexer->getCol());
	}
}

/*
	<��������> => <��ʶ��>=<�޷�������>
*/
void Parser::ConstDefinition() {
	getNextToken();
	if (token.getType() != TokenType::IDENTIFIER) {
		throw PL0Exception("���������ʽ����", lexer->getLine(), lexer->getCol());
	}
	std::string sym_name = token.getValue();
	getNextToken();
	if (token.getType() != TokenType::EQUAL) {
		throw PL0Exception("���������ʽ����", lexer->getLine() , lexer->getCol());
	}
	getNextToken();
	std::string sym_num = token.getValue();
	int value = std::stoi(sym_num);
	if (token.getType() != TokenType::NUMBER) {
		throw PL0Exception("���������ʽ����", lexer->getLine(), lexer->getCol());
	}
	// finish ����Ϊ����������м�������ɣ�sym_name��sym_num����

}

/*
	<����˵��> => CONST <��������>{, <��������>};
*/
void Parser::ConstDeclaration() {
	// ��ʱtoken�Ѿ�ָ��CONST
	ConstDefinition();
	getNextToken();
	while (true) {
		if (token.getType() == TokenType::COMMA) {
			ConstDefinition();
			getNextToken();
		}
		else break;
	}
	if (token.getType() != TokenType::SEMICOLON) {
		throw PL0Exception("����˵����ʽ����ȱ�ٷֺ�", lexer->getLine(), lexer->getCol());
	}
	// finish ����Ϊ����������м��������
}

/*
	<���> => <��ֵ���>|<�������>|<ѭ�����>|<�������>|<�����>
*/
void Parser::Statement() {
	// ��ʱtokenӦָ������һ������
	switch (token.getType()) {
	case TokenType::IDENTIFIER:
		AssignmentStat();
		break;
	case TokenType::IF:
		ConditionalStat();
		break;
	case TokenType::WHILE:
		LoopStat();
		break;
	case TokenType::BEGIN:
		CompoundStat();
		break;
	case TokenType::SEMICOLON:
	case TokenType::END:
		//��������еĿ���䣬��������
		break;
	default:
		throw PL0Exception("�޷�ʶ���������", lexer->getLine(), lexer->getCol());
	}
}
/*
	<�ֳ���> => [<����˵��>][<����˵��>]<���>
	FIRST = {CONST VAR IDENTIFIER IF WHILE BEGIN END_OF_FILE}
	����������䣬END_OF_FILE�ǿ�����FOLLOW
*/
void Parser::SubProgram() {
	getNextToken();
	if (token.getType() == TokenType::CONST) {
		ConstDeclaration();
		getNextToken();
	}
	if (token.getType() == TokenType::VAR) {
		VarDeclaration();
		getNextToken();
	}
	if (token.getType() == TokenType::END_OF_FILE)
		return;//�ֳ����еĿ���䣬ֱ�ӷ���
	Statement();

	// finish ����Ϊ����������м��������

}

/*
	<����> => <�����ײ�><�ֳ���>
	FIRST = {PROGRAM}
*/
bool Parser::BeginParse() {
	try {
		ProgramHead();
		SubProgram();
		return true;
	}
	catch (PL0Exception err) {
		std::cout << err.msg() << std::endl;
	}
	return false;
}

/*
	<�����ײ�> => PROGRAM<��ʶ��>
	FIRST = {PROGRAM}
*/
void Parser::ProgramHead() {
	getNextToken();
	if (token.getType() != TokenType::PROGRAM) {
		throw PL0Exception("�����ײ�ȱ�ٹؼ���PROGRAM", lexer->getLine(), lexer->getCol());
	}
	getNextToken();
	if (token.getType() != TokenType::IDENTIFIER) {
		throw PL0Exception("�����ײ�ȱ�ٱ�ʶ��" , lexer->getLine(), lexer->getCol());
	}
	// finish ����Ϊ����������м��������
	std::string program_name = token.getValue();

}
