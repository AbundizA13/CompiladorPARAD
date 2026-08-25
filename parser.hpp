#ifndef PARSER_HPP
#define PARSER_HPP

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "token.hpp"

class ParserSyntaxError : public std::runtime_error {
public:
    explicit ParserSyntaxError(const std::string& message)
        : std::runtime_error(message) {}
};

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual std::string toString() const = 0;
};

class Expression : public ASTNode {};
class Statement : public ASTNode {};

class BinaryOperation : public Expression {
private:
    std::unique_ptr<Expression> left;
    std::string operation;
    std::unique_ptr<Expression> right;

public:
    BinaryOperation(std::unique_ptr<Expression> left,
                    std::string operation,
                    std::unique_ptr<Expression> right);

    std::string toString() const override;
};

class Literal : public Expression {
private:
    std::string value;
    TokenType type;

public:
    Literal(std::string value, TokenType type);

    std::string toString() const override;
};

class Identifier : public Expression {
private:
    std::string name;

public:
    explicit Identifier(std::string name);

    std::string toString() const override;
};

class FunctionCall : public Expression {
private:
    std::string name;
    std::vector<std::unique_ptr<Expression>> arguments;

public:
    FunctionCall(std::string name,
                 std::vector<std::unique_ptr<Expression>> arguments);

    std::string toString() const override;
};

class Assignment : public Statement {
private:
    std::string target;
    std::unique_ptr<Expression> value;

public:
    Assignment(std::string target, std::unique_ptr<Expression> value);

    std::string toString() const override;
};

class IfStatement : public Statement {
private:
    std::unique_ptr<Expression> condition;
    std::vector<std::unique_ptr<Statement>> thenBody;
    std::vector<std::unique_ptr<Statement>> elseBody;

public:
    IfStatement(std::unique_ptr<Expression> condition,
                std::vector<std::unique_ptr<Statement>> thenBody,
                std::vector<std::unique_ptr<Statement>> elseBody);

    std::string toString() const override;
};

class WhileStatement : public Statement {
private:
    std::unique_ptr<Expression> condition;
    std::vector<std::unique_ptr<Statement>> body;

public:
    WhileStatement(std::unique_ptr<Expression> condition,
                   std::vector<std::unique_ptr<Statement>> body);

    std::string toString() const override;
};

class PrintStatement : public Statement {
private:
    std::unique_ptr<Expression> expression;

public:
    explicit PrintStatement(std::unique_ptr<Expression> expression);

    std::string toString() const override;
};

class Program : public ASTNode {
private:
    std::vector<std::unique_ptr<Statement>> statements;

public:
    explicit Program(std::vector<std::unique_ptr<Statement>> statements);

    std::string toString() const override;
};

class Parser {
private:
    const std::vector<Token>& tokens;
    std::size_t position;

    static std::string tokenTypeName(TokenType type);

    bool hasStatementSeparator() const;
    void consumeStatementSeparators();

    std::vector<std::unique_ptr<Statement>> parseBlock();
    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<Statement> parseAssignment();
    std::unique_ptr<Statement> parseIfStatement();
    std::unique_ptr<Statement> parseWhileStatement();
    std::unique_ptr<Statement> parsePrintStatement();

    std::unique_ptr<Expression> parseComparison();
    std::unique_ptr<Expression> parseExpression();
    std::unique_ptr<Expression> parseTerm();
    std::unique_ptr<Expression> parseFactor();
    std::unique_ptr<Expression> parseFunctionCall(const std::string& name);

public:
    explicit Parser(const std::vector<Token>& tokens);

    Program parse();

    const Token* currentToken() const;
    const Token* peekToken(std::size_t offset = 1) const;

    bool match(TokenType type) const;
    bool isAtEnd() const;

    const Token& advance();
    const Token& consume(TokenType expectedType);
};

#endif
