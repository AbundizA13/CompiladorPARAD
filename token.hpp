#ifndef TOKEN_HPP
#define TOKEN_HPP
#include <string>
#include <utility>

/*
- No cambiar los nombres ni el orden de TokenType.
- El lexer debe crear tokens usando el constructor:
- El parser solo consulta datos de la clase token mediante métodos .get()
- FDA representa EOF (Fin Del Archivo).
*/

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

    NEWLINE,
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
            : linea(l), columna(c), valor(std::move(val)), tipo(tipo){}

        /*El const al final de cada método significa que
        no modificará ningun atributo de la clase*/
        TokenType getType() const; 
        const std::string getTypeString() const; //TokenType -> String para impresión
        const std::string& getValue() const;
        size_t getLine() const;
        size_t getColumn() const;
        const std::string printContent() const;
        //Falta cuerpo lógico en token.cpp
};

#endif