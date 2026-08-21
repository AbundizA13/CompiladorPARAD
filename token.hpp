#ifndef TOKEN_HPP
#define TOKEN_HPP
#include <string>
#include <utility>

enum class TokenType{ //Uso "TokenType::TIPO", usar getTypeString(TIPO) para impresión.
    IDENTIFIER,
    NUMBER,
    STRING,

    PLUS,
    MINUS,
    MULT,
    DIV,
    MODULO,
    POWER,

    ASSIGN,
    PLUS_ASSIGN,
    MINUS_ASSIGN,

    EQUAL, // =
    NOT_EQUAL, // !=
    LESS_THAN, // <
    GREATER_THAN, // >
    LESS_EQUAL, // <=
    GREATER_EQUAL, // >=

    AND, // &&
    OR, // ||
    NOT, // !

    L_PAR,
    R_PAR,
    L_BRACKET,
    R_BRACKET,
    L_BRACE,
    R_BRACE,

    COMMA,
    SEMICOLON,
    DOT,
    COLON,

    IF,
    ELSE,
    ELIF,
    WHILE,
    FOR,
    DEF,
    CLASS,
    RETURN,
    PRINT,
    TRUE,
    FALSE,
    NONE,
    IN,

    NEWLIINE,
    WHITESPACE,
    COMMENT,
    UNKNOWN,
    FDA //Colisión por palabra reservada "EOF" en C++
};

class Token{
    private:
        size_t linea;
        size_t columna;
        std::string valor;
        TokenType tipo;
    public:
        /*Se usa size_t porque almacena solo enteros positivos
        para AHORRO DE MEMORIA*/
        //Constructor de struct
        Token(TokenType(tipo), std::string(val), size_t(l), size_t(c))
            : tipo(tipo), valor(std::move(val)), linea(l), columna(c){}

        /*El const al final de cada método significa que
        no modificará ningun atributo de la clase*/
        TokenType getType() const; 
        const std::string getValueString() const; //TokenType -> String para impresión
        const std::string& getValue() const;
        size_t getLine() const;
        size_t getColumn() const;
        void printContent() const;
        //Falta cuerpo lógico en token.cpp
};

#endif