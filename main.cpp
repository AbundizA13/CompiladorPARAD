#include "parser.hpp"

#include <iostream>
#include <vector>

int main() {
	/*Vector provisional, mientras falta el lexer y
    procesador de archivos de THIAGO.*/
	const std::vector<Token> tokens = {
		Token(TokenType::IDENTIFIER, "x", 1, 1),
		Token(TokenType::ASSIGN, "=", 1, 3),
		Token(TokenType::NUMBER, "5", 1, 5),
		Token(TokenType::PLUS, "+", 1, 7),
		Token(TokenType::NUMBER, "3", 1, 9),
		Token(TokenType::MULT, "*", 1, 11),
		Token(TokenType::NUMBER, "2", 1, 13),

		Token(TokenType::PRINT, "print", 2, 1),
		Token(TokenType::L_PAR, "(", 2, 6),
		Token(TokenType::IDENTIFIER, "x", 2, 7),
		Token(TokenType::R_PAR, ")", 2, 8),

		Token(TokenType::IF, "if", 3, 1),
		Token(TokenType::IDENTIFIER, "x", 3, 4),
		Token(TokenType::GREATER_THAN, ">", 3, 6),
		Token(TokenType::NUMBER, "10", 3, 8),
		Token(TokenType::L_BRACE, "{", 3, 11),
		Token(TokenType::PRINT, "print", 3, 13),
		Token(TokenType::L_PAR, "(", 3, 18),
		Token(TokenType::IDENTIFIER, "x", 3, 19),
		Token(TokenType::R_PAR, ")", 3, 20),
		Token(TokenType::R_BRACE, "}", 3, 22),
		Token(TokenType::ELSE, "else", 3, 24),
		Token(TokenType::L_BRACE, "{", 3, 29),
		Token(TokenType::PRINT, "print", 3, 31),
		Token(TokenType::L_PAR, "(", 3, 36),
		Token(TokenType::STRING, "\"x es pequeno\"", 3, 37),
		Token(TokenType::R_PAR, ")", 3, 52),
		Token(TokenType::R_BRACE, "}", 3, 54),

		Token(TokenType::WHILE, "while", 4, 1),
		Token(TokenType::IDENTIFIER, "x", 4, 7),
		Token(TokenType::LESS_THAN, "<", 4, 9),
		Token(TokenType::NUMBER, "20", 4, 11),
		Token(TokenType::L_BRACE, "{", 4, 14),
		Token(TokenType::IDENTIFIER, "x", 4, 16),
		Token(TokenType::ASSIGN, "=", 4, 18),
		Token(TokenType::IDENTIFIER, "suma", 4, 20),
		Token(TokenType::L_PAR, "(", 4, 24),
		Token(TokenType::IDENTIFIER, "x", 4, 25),
		Token(TokenType::COMMA, ",", 4, 26),
		Token(TokenType::NUMBER, "1", 4, 28),
		Token(TokenType::R_PAR, ")", 4, 29),
		Token(TokenType::R_BRACE, "}", 4, 31),

		Token(TokenType::FDA, "", 5, 1)
	};

	try {
		Parser parser(tokens); //THIAGO AQUÍ DEBES METER EL VECTOR DE TOKENS QUE ARROJA TU LEXER.
		const Program program = parser.parse();
		std::cout << program.toString() << '\n';
	} catch (const ParserSyntaxError& error) {
		std::cerr << "Error de sintaxis: " << error.what() << '\n';
		return 1;
	}

	return 0;
}
