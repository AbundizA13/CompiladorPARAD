#include "LexerAnalyzerClass.hpp"
#include <map>
#include <vector>

using namespace std;

LexicalAnalyzer::LexicalAnalyzer(const string& fuente) {
    input = fuente;
    posicion = 0;
    linea = 1;
    columna = 1;
    
    // Un diccionario de palabras reservadas
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
    
}

// Funciones para leer en el analizador
// 1. Leer Numeros
// 2. Leer Identificadores
// 3. Leer Simbolos
// 4. Leer Cadenas
// 5. Leer Comentario
// 6. El analizador principal 

// Leer numeros
void LexicalAnalyzer::leerNumeros() {
    
}

// Lee identificadores 
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
        Token(TokenType::IDENTIFIER, valor, linea, columna-valor.length);
    );
}

// Leer simbolos
void LexicalAnalyzer::leerSimbolo() {
    string simbolo;
    
    // Leer el primer carácter
    simbolo += input[posicion];
    posicion++;
    columna++;
    
    // 2. VERIFICA si el siguiente forma un símbolo de 2 caracteres
    if (posicion < input.length()) {
        string posibleDoble = simbolo + input[posicion];
        
        // Si existe en el diccionario de símbolos válidos
        if (esSimboloValido(posibleDoble)) {
            simbolo = posibleDoble;
            posicion++;
            columna++;
        }
    }
    
    // 3. OBTÉN el tipo de token
    TokenType tipo = obtenerTipoSimbolo(simbolo);
    
    // 4. CREA el token
    tokens.push_back(Token(tipo, simbolo, linea, columna - simbolo.length()));
}

// Analizador principal
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
        else if (Symbols(c)) {
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

