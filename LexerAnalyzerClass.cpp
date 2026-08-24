#include "LexerAnalyzerClass.hpp"
#include <map>
#include <vector>

using namespace std;

LexicalAnalyzer::LexicalAnalyzer(const string& fuente) {
    input = fuente;
    posicion = 0;
    linea = 1;
    columna = 1;
    
    // Diccionario de palabras reservadas
    palabrasReservadas["if"] = TokenType::IF;
    palabrasReservadas["else"] = TokenType::ELSE;
    palabrasReservadas["elif"] = TokenType::ELIF;
    palabrasReservadas["while"] = TokenType::WHILE;
    palabrasReservadas["for"] = TokenType::FOR;
    palabrasReservadas["def"] = TokenType::DEF;
    palabrasReservadas["class"] = TokenType::CLASS;
    palabrasReservadas["return"] = TokenType::RETURN;
    palabrasReservadas["print"] = TokenType::PRINT;
    palabrasReservadas["True"] = TokenType::TRUE;
    palabrasReservadas["False"] = TokenType::FALSE;
    palabrasReservadas["None"] = TokenType::NONE;
    palabrasReservadas["in"] = TokenType::IN;
    palabrasReservadas["and"] = TokenType::AND;
    palabrasReservadas["or"] = TokenType::OR;
    palabrasReservadas["not"] = TokenType::NOT;

    // Diccionario de símbolos
    simbolos["+"] = TokenType::PLUS;
    simbolos["-"] = TokenType::MINUS;
    simbolos["*"] = TokenType::MULT;
    simbolos["/"] = TokenType::DIV;
    simbolos["%"] = TokenType::MODULO;
    simbolos["**"] = TokenType::POWER;
    simbolos["="] = TokenType::ASSIGN;
    simbolos["=="] = TokenType::EQUAL;
    simbolos["!="] = TokenType::NOT_EQUAL;
    simbolos["<"] = TokenType::LESS_THAN;
    simbolos[">"] = TokenType::GREATER_THAN;
    simbolos["<="] = TokenType::LESS_EQUAL;
    simbolos[">="] = TokenType::GREATER_EQUAL;
    simbolos["+="] = TokenType::PLUS_ASSIGN;
    simbolos["-="] = TokenType::MINUS_ASSIGN;
    simbolos["("] = TokenType::L_PAR;
    simbolos[")"] = TokenType::R_PAR;
    simbolos["["] = TokenType::L_BRACKET;
    simbolos["]"] = TokenType::R_BRACKET;
    simbolos["{"] = TokenType::L_BRACE;
    simbolos["}"] = TokenType::R_BRACE;
    simbolos[","] = TokenType::COMMA;
    simbolos[";"] = TokenType::SEMICOLON;
    simbolos["."] = TokenType::DOT;
    simbolos[":"] = TokenType::COLON;
    simbolos["&&"] = TokenType::AND;
    simbolos["||"] = TokenType::OR;
    simbolos["!"] = TokenType::NOT;
}

// Funcion para acceder al diccionario de simbolos
bool LexicalAnalyzer::esSimbolo(char c) {
    for (const auto& par : simbolos) {
        if (par.first[0] == c) {
            return true;
        }
    }
    return false;
}

// Verifica el diccionario
bool LexicalAnalyzer::esSimboloValido(const string& s) {
    return simbolos.find(s) != simbolos.end();
}

// Obtiene otro tipo de simbolo 
TokenType LexicalAnalyzer::obtenerTipoSimbolo(const string& s) {
    if (simbolos.find(s) != simbolos.end()) {
        return simbolos[s];
    }
    return TokenType::UNKNOWN;
}

// Funciones para leer en el analizador
// 1. Leer Numeros (Listo)
// 2. Leer Identificadores (Listo)
// 3. Leer Simbolos (Listo)
// 4. Leer Cadenas (listo)
// 5. Leer Comentario (listo)
// 6. El analizador principal (Listo)

// 1. Leer numeros
void LexicalAnalyzer::leerNumero() {
    size_t inicio = posicion;
    
    // Lee todos los dígitos
    while (posicion < input.length() && isdigit(input[posicion])) {
        posicion++;
        columna++;
    }
    
    // Comprobar que haya puntos decimales 
    if (posicion < input.length() && input[posicion] == '.') {
        posicion++; 
        columna++;
        
        // Lee los dígitos después del punto
        while (posicion < input.length() && isdigit(input[posicion])) {
            posicion++;
            columna++;
        }
    }
    
    // Extrae el número completo
    string valor = input.substr(inicio, posicion - inicio);
    
    // Crea el token
    tokens.push_back(Token(TokenType::NUMBER, valor, linea, columna - valor.length()));
}

// 2. Lee identificadores 
void LexicalAnalyzer::leerIdentificador() {
    // Punto de guardado para contabilizar caracteres
    size_t inicio = posicion;
    
    while (posicion < input.length() && isalpha(input[posicion])) {
        posicion++;
        columna++;
    }
    
    string valor = input.substr(inicio, posicion - inicio);
    
    tokens.push_back(
    // 1. Tipo
    // 2. Valor segun texto
    // 3. Linea actual
    // 4. Columna donde inicia
        Token(TokenType::IDENTIFIER, valor, linea, columna-valor.length())
    );
}

// 3. Leer simbolos
void LexicalAnalyzer::leerSimbolo() {
    string simbolo;
    
    // Leer el primer carácter
    simbolo += input[posicion];
    posicion++;
    columna++;
    
    // 2. verificar si el siguiente forma un símbolo de 2 caracteres
    if (posicion < input.length()) {
        string posibleDoble = simbolo + input[posicion];
        
        // Si existe en el diccionario de símbolos válidos
        if (esSimboloValido(posibleDoble)) {
            simbolo = posibleDoble;
            posicion++;
            columna++;
        }
    }
    
    // 3. obtener el tipo de token
    TokenType tipo = obtenerTipoSimbolo(simbolo);
    
    // 4. CREA el token
    tokens.push_back(Token(tipo, simbolo, linea, columna - simbolo.length()));
}

// 4. Leer cadenas 
void LexicalAnalyzer::leerCadena(char delimitador) {
    size_t inicio = posicion;
    
    posicion++;
    columna++;
    
    // Lee caracteres hasta encontrar la comilla de cierre
    while (posicion < input.length() && input[posicion] != delimitador) {
        if (input[posicion] == '\n') {
            linea++;
            columna = 1;
        } else {
            columna++;
        }
        posicion++;
    }
    
    // Salta la comilla de cierre
    if (posicion < input.length()) {
        posicion++;
        columna++;
    }
    
    // Extrae la cadena completa incluyendo comillas
    string valor = input.substr(inicio, posicion - inicio);
    
    // Crea el token
    tokens.push_back(Token(TokenType::STRING, valor, linea, columna - valor.length()));
}

// 5. Leer comentarios
void LexicalAnalyzer::leerComentario() {
    size_t inicio = posicion;
    
    // Lee hasta el final de la línea
    while (posicion < input.length() && input[posicion] != '\n') {
        posicion++;
        columna++;
    }
    
    // Extrae el comentario
    string valor = input.substr(inicio, posicion - inicio);
    
    // Crea el token (opcional porque el compilador ignora comentarios)
    tokens.push_back(Token(TokenType::COMMENT, valor, linea, columna - valor.length()));
}

// 6. Analizador principal
void LexicalAnalyzer::analyze() {
    while (posicion < input.length()) {
        char c = input[posicion];
        
        // Para leer numeros
        if (isdigit(c)) {
            leerNumero();
        }
        // Para leer identificadores y clasificar reservadas
        else if (isalpha(c)) {
            leerIdentificador();
            
            // ← RECLASIFICAR AQUÍ
            // Si el último token es un identificador, comprobar si es palabra reservada
            Token& ultimoToken = tokens.back();
            if (ultimoToken.getType() == TokenType::IDENTIFIER) {
                string valor = ultimoToken.getValue();
                if (palabrasReservadas.find(valor) != palabrasReservadas.end()) {
                    
                    // Es palabra reservada, REEMPLAZARLO
                    TokenType nuevoTipo = palabrasReservadas[valor];
                    tokens.pop_back();
                    tokens.push_back(Token(nuevoTipo, valor, ultimoToken.getLine(), ultimoToken.getColumn()));
                }
            }
        }
        
        // Simbolos
        else if (esSimbolo(c)) {
            leerSimbolo();
        }
        // Cadenas (strings)
        else if (c == '"' || c == '\'') {
            leerCadena(c);
        }
        // Comentarios
        else if (c == '#') {
            leerComentario();
        }
        // Espacios 
        else if (c == ' ' || c == '\t') {
            posicion += 1;
            columna += 1;
        }
        // Salto de linea
        else if (c == '\n') {
            posicion += 1;
            linea += 1;
            columna = 1;  // reiniciar columnas
        }
        else {
            // Para Carácteres desconocidos
            posicion += 1;
            columna += 1;
        }
    }
}

const vector<Token>& LexicalAnalyzer::getTokens() const {
    return tokens;
}