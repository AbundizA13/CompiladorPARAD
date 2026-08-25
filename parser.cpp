#include "parser.hpp"
#include "token.hpp"

#include <sstream>

namespace {

std::string joinStatements(const std::vector<std::unique_ptr<Statement>>& statements) {
    std::string result;
    for (std::size_t index = 0; index < statements.size(); ++index) {
        if (index > 0) {
            result += "; ";
        }
        result += statements[index]->toString();
    }
    return result;
}

}

BinaryOperation::BinaryOperation(std::unique_ptr<Expression> left,
                                 std::string operation,
                                 std::unique_ptr<Expression> right)
    : left(std::move(left)), operation(std::move(operation)), right(std::move(right)) {}

std::string BinaryOperation::toString() const {
    return "BinaryOp(" + left->toString() + " " + operation + " " +
           right->toString() + ")";
}

Literal::Literal(std::string value, TokenType type)
    : value(std::move(value)), type(type) {}

std::string Literal::toString() const {
    return "Literal(" + value + ")";
}

Identifier::Identifier(std::string name)
    : name(std::move(name)) {}

std::string Identifier::toString() const {
    return "Identifier(" + name + ")";
}

FunctionCall::FunctionCall(
    std::string name,
    std::vector<std::unique_ptr<Expression>> arguments)
    : name(std::move(name)), arguments(std::move(arguments)) {}

std::string FunctionCall::toString() const {
    std::string result = "FunctionCall(" + name + "(";
    for (std::size_t index = 0; index < arguments.size(); ++index) {
        if (index > 0) {
            result += ", ";
        }
        result += arguments[index]->toString();
    }
    return result + "))";
}

Assignment::Assignment(std::string target, std::unique_ptr<Expression> value)
    : target(std::move(target)), value(std::move(value)) {}

std::string Assignment::toString() const {
    return "Assignment(" + target + " = " + value->toString() + ")";
}

IfStatement::IfStatement(
    std::unique_ptr<Expression> condition,
    std::vector<std::unique_ptr<Statement>> thenBody,
    std::vector<std::unique_ptr<Statement>> elseBody)
    : condition(std::move(condition)),
      thenBody(std::move(thenBody)),
      elseBody(std::move(elseBody)) {}

std::string IfStatement::toString() const {
    return "If(" + condition->toString() + ") then [" +
           joinStatements(thenBody) + "] else [" + joinStatements(elseBody) + "]";
}

WhileStatement::WhileStatement(
    std::unique_ptr<Expression> condition,
    std::vector<std::unique_ptr<Statement>> body)
    : condition(std::move(condition)), body(std::move(body)) {}

std::string WhileStatement::toString() const {
    return "While(" + condition->toString() + ") [" + joinStatements(body) + "]";
}

PrintStatement::PrintStatement(std::unique_ptr<Expression> expression)
    : expression(std::move(expression)) {}

std::string PrintStatement::toString() const {
    return "Print(" + expression->toString() + ")";
}

Program::Program(std::vector<std::unique_ptr<Statement>> statements)
    : statements(std::move(statements)) {}

std::string Program::toString() const {
    return "Program([\n  " + joinStatements(statements) + "\n])";
}

Parser::Parser(const std::vector<Token>& tokens)
    : tokens(tokens), position(0) {}

const Token* Parser::currentToken() const {
    if (position >= tokens.size()) {
        return nullptr;
    }
    return &tokens[position];
}

const Token* Parser::peekToken(std::size_t offset) const {
    const std::size_t nextPosition = position + offset;
    if (nextPosition >= tokens.size()) {
        return nullptr;
    }
    return &tokens[nextPosition];
}

bool Parser::match(TokenType type) const {
    const Token* token = currentToken();
    return token != nullptr && token->getType() == type;
}

const Token& Parser::advance() {
    const Token* token = currentToken();
    if (token == nullptr) {
        throw ParserSyntaxError("Se intentó consumir un token al final del código");
    }
    ++position;
    return *token;
}

const Token& Parser::consume(TokenType expectedType) {
    const Token* token = currentToken();
    if (token == nullptr || token->getType() == TokenType::FDA) {
        throw ParserSyntaxError("Se esperaba " + tokenTypeName(expectedType) +
                                ", pero se llegó al final del código");
    }
    if (token->getType() != expectedType) {
        std::ostringstream message;
        message << "Se esperaba " << tokenTypeName(expectedType)
                << ", se encontró " << token->getTypeString()
                << " en línea " << token->getLine()
                << ", columna " << token->getColumn();
        throw ParserSyntaxError(message.str());
    }
    ++position;
    return *token;
}

bool Parser::isAtEnd() const {
    return currentToken() == nullptr || match(TokenType::FDA);
}

std::string Parser::tokenTypeName(TokenType type) {
    Token token(type, "", 0, 0);
    return token.getTypeString();
}

bool Parser::hasStatementSeparator() const {
    if (match(TokenType::SEMICOLON) || match(TokenType::NEWLINE)) {
        return true;
    }
    if (position == 0 || currentToken() == nullptr) {
        return false;
    }
    //Si la línea del token actual es mayor a la del token pasado, hubo salto de línea
    return currentToken()->getLine() > tokens[position - 1].getLine();
}

void Parser::consumeStatementSeparators() {
    while (match(TokenType::SEMICOLON) || match(TokenType::NEWLINE)) {
        advance();
    }
}

Program Parser::parse() {
    std::vector<std::unique_ptr<Statement>> statements;
    while (!isAtEnd()) {
        statements.push_back(parseStatement());
        if (!isAtEnd()) {
            if (!hasStatementSeparator()) {
                throw ParserSyntaxError(
                    "Se esperaba un salto de línea o ';' entre sentencias");
            }
            consumeStatementSeparators();
        }
    }
    return Program(std::move(statements));
}

std::unique_ptr<Statement> Parser::parseStatement() {
    if (match(TokenType::IDENTIFIER) && peekToken() != nullptr &&
        peekToken()->getType() == TokenType::ASSIGN) {
        return parseAssignment();
    }
    if (match(TokenType::IF)) {
        return parseIfStatement();
    }
    if (match(TokenType::WHILE)) {
        return parseWhileStatement();
    }
    if (match(TokenType::PRINT)) {
        return parsePrintStatement();
    }

    const Token* token = currentToken();
    if (token == nullptr) {
        throw ParserSyntaxError("Se esperaba una sentencia al final del código");
    }
    throw ParserSyntaxError("Sentencia inesperada comenzando con " +
                            token->getTypeString());
}

std::unique_ptr<Statement> Parser::parseAssignment() {
    const std::string target = consume(TokenType::IDENTIFIER).getValue();
    consume(TokenType::ASSIGN);
    return std::make_unique<Assignment>(target, parseComparison());
}

std::vector<std::unique_ptr<Statement>> Parser::parseBlock() {
    consume(TokenType::L_BRACE);
    std::vector<std::unique_ptr<Statement>> statements;
    while (!isAtEnd() && !match(TokenType::R_BRACE)) {
        statements.push_back(parseStatement());
        if (!match(TokenType::R_BRACE)) {
            if (!hasStatementSeparator()) {
                throw ParserSyntaxError(
                    "Se esperaba un salto de línea o ';' entre sentencias");
            }
            consumeStatementSeparators();
        }
    }
    consume(TokenType::R_BRACE);
    return statements;
}

std::unique_ptr<Statement> Parser::parseIfStatement() {
    consume(TokenType::IF);
    auto condition = parseComparison();
    auto thenBody = parseBlock();
    std::vector<std::unique_ptr<Statement>> elseBody;
    if (match(TokenType::ELSE)) {
        consume(TokenType::ELSE);
        elseBody = parseBlock();
    }
    return std::make_unique<IfStatement>(
        std::move(condition), std::move(thenBody), std::move(elseBody));
}

std::unique_ptr<Statement> Parser::parseWhileStatement() {
    consume(TokenType::WHILE);
    auto condition = parseComparison();
    auto body = parseBlock();
    return std::make_unique<WhileStatement>(std::move(condition), std::move(body));
}

std::unique_ptr<Statement> Parser::parsePrintStatement() {
    consume(TokenType::PRINT);
    consume(TokenType::L_PAR);
    auto expression = parseComparison();
    consume(TokenType::R_PAR);
    return std::make_unique<PrintStatement>(std::move(expression));
}

std::unique_ptr<Expression> Parser::parseComparison() {
    auto left = parseExpression();
    while (match(TokenType::EQUAL) || match(TokenType::NOT_EQUAL) ||
           match(TokenType::LESS_THAN) || match(TokenType::GREATER_THAN) ||
           match(TokenType::LESS_EQUAL) || match(TokenType::GREATER_EQUAL) ||
           match(TokenType::AND) || match(TokenType::OR)) {
        const std::string operation = advance().getValue();
        auto right = parseExpression();
        left = std::make_unique<BinaryOperation>(
            std::move(left), operation, std::move(right));
    }
    return left;
}

std::unique_ptr<Expression> Parser::parseExpression() {
    auto left = parseTerm();
    while (match(TokenType::PLUS) || match(TokenType::MINUS)) {
        const std::string operation = advance().getValue();
        auto right = parseTerm();
        left = std::make_unique<BinaryOperation>(
            std::move(left), operation, std::move(right));
    }
    return left;
}

std::unique_ptr<Expression> Parser::parseTerm() {
    auto left = parseFactor();
    while (match(TokenType::MULT) || match(TokenType::DIV) ||
           match(TokenType::MODULO)) {
        const std::string operation = advance().getValue();
        auto right = parseFactor();
        left = std::make_unique<BinaryOperation>(
            std::move(left), operation, std::move(right));
    }
    return left;
}

std::unique_ptr<Expression> Parser::parseFactor() {
    if (match(TokenType::L_PAR)) { //Parentesis
        consume(TokenType::L_PAR);
        auto expression = parseComparison();
        consume(TokenType::R_PAR);
        return expression;
    }
    if (match(TokenType::IDENTIFIER)) { //Identificadores y funciones
        const std::string name = advance().getValue();
        if (match(TokenType::L_PAR)) {
            return parseFunctionCall(name);
        }
        return std::make_unique<Identifier>(name);
    }
    if (match(TokenType::NUMBER) || match(TokenType::STRING) ||
        match(TokenType::TRUE) || match(TokenType::FALSE) ||
        match(TokenType::NONE)) { //Literales
        const Token& token = advance();
        return std::make_unique<Literal>(token.getValue(), token.getType());
    }

    const Token* token = currentToken();
    throw ParserSyntaxError("Factor inesperado: " +
                            (token == nullptr ? "FIN_DE_ARCHIVO" :
                             token->getTypeString()));
}

std::unique_ptr<Expression> Parser::parseFunctionCall(const std::string& name) {
    consume(TokenType::L_PAR);
    std::vector<std::unique_ptr<Expression>> arguments;
    if (!match(TokenType::R_PAR)) {
        arguments.push_back(parseComparison());
        while (match(TokenType::COMMA)) {
            consume(TokenType::COMMA);
            arguments.push_back(parseComparison());
        }
    }
    consume(TokenType::R_PAR);
    return std::make_unique<FunctionCall>(name, std::move(arguments));
}
