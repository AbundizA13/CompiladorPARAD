#include "token.hpp"
#include <string>

TokenType Token::getType() const{
    return tipo;
}
const std::string& Token::getValue() const{
    return valor;
}
size_t Token::getLine() const{
    return linea;
}
size_t Token::getColumn() const{
    return columna;
}
//PARECE HARDCODING PERO ES LA ÚNICA MANERA (O UN DICCIONARIO)
const std::string Token::getTypeString() const{
    switch(tipo){
        case TokenType::IDENTIFIER:
            return "IDENTIFICADOR";
        case TokenType::NUMBER:
            return "NUMERO";
        case TokenType::STRING:
            return "CADENA";
        case TokenType::PLUS:
            return "SUMA";
        case TokenType::MINUS:
            return "RESTA";
        case TokenType::MULT:
            return "MULTIPLICACION";
        case TokenType::DIV:
            return "DIVISION";
        case TokenType::MODULO:
            return "MODULO";
        case TokenType::POWER:
            return "POTENCIA";
        case TokenType::ASSIGN:
            return "ASIGNACION";
        case TokenType::PLUS_ASSIGN:
            return "SUMA_ASIGNACION";
        case TokenType::MINUS_ASSIGN:
            return "RESTA_ASIGNACION";
        case TokenType::EQUAL:
            return "IGUAL";
        case TokenType::NOT_EQUAL:
            return "NO_IGUAL";
        case TokenType::LESS_THAN:
            return "MENOR_QUE";
        case TokenType::GREATER_THAN:
            return "MAYOR_QUE";
        case TokenType::LESS_EQUAL:
            return "MENOR_IGUAL";
        case TokenType::GREATER_EQUAL:
            return "MAYOR_IGUAL";
        case TokenType::AND:
            return "Y_LOGICO";
        case TokenType::OR:
            return "O_LOGICO";
        case TokenType::NOT:
            return "NO_LOGICO";
        case TokenType::L_PAR:
            return "PARENTESIS_IZQ";
        case TokenType::R_PAR:
            return "PARENTESIS_DER";
        case TokenType::L_BRACKET:
            return "CORCHETE_IZQ";
        case TokenType::R_BRACKET:
            return "CORCHETE_DER";
        case TokenType::L_BRACE:
            return "LLAVE_IZQ";
        case TokenType::R_BRACE:
            return "LLAVE_DER";
        case TokenType::COMMA:
            return "COMA";
        case TokenType::SEMICOLON:
            return "PUNTO_COMA";
        case TokenType::DOT:
            return "PUNTO";
        case TokenType::COLON:
            return "DOS_PUNTOS";
        case TokenType::IF:
            return "SI";
        case TokenType::ELSE:
            return "SINO";
        case TokenType::ELIF:
            return "SINO_SI";
        case TokenType::WHILE:
            return "MIENTRAS";
        case TokenType::FOR:
            return "PARA";
        case TokenType::DEF:
            return "FUNCION";
        case TokenType::CLASS:
            return "CLASE";
        case TokenType::RETURN:
            return "RETORNO";
        case TokenType::PRINT:
            return "IMPRIMIR";
        case TokenType::TRUE:
            return "VERDADERO";
        case TokenType::FALSE:
            return "FALSO";
        case TokenType::NONE:
            return "NULO";
        case TokenType::IN:
            return "EN";
        case TokenType::NEWLIINE:
            return "NUEVA_LINEA";
        case TokenType::WHITESPACE:
            return "ESPACIO";
        case TokenType::COMMENT:
            return "COMENTARIO";
        case TokenType::UNKNOWN:
            return "DESCONOCIDO";
        case TokenType::FDA:
            return "FIN_DE_ARCHIVO";
        default:
            return "DESCONOCIDO";
    }
}