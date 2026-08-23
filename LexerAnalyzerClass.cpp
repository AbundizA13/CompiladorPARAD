#include "LexerAnalyzerClass.hpp"
#include <map>

LexicalAnalyzer::LexicalAnalyzer(const std::string& fuente) {
    input = fuente
    posicion = 0;
    linea = 1;
    columna = 1;
}

// Identificadores
void LexicalAnalyzer::leerIdentificador() {
    size_t inicio = posicion;
    
    while (posicion < input.length() && isalnum(input[posicion])) {
        posicion += 1;
        columna += 1;
    }
    
    std::string valor = input.substr(inicio, posicion - inicio);
    tokens.push_back(Token(TokenType::IDENTIFIER, valor, linea, columna - valor.length()));
}

// Numeros
void LexicalAnalyzer::leerNumero() {
    size_t inicio = posicion;
    
    while (posicion < input.length() && isdigit(input[posicion])) {
        posicion += 1;
        columna += 1;
    }
    
    std::string valor = input.substr(inicio, posicion - inicio);
    tokens.push_back(Token(TokenType::NUMBER, valor, linea, columna - valor.length()));
}

void LexicalAnalyzer::analyze() {
    while (posicion < input.length()) {
        char c = input[posicion];
        
        // Números
        if (isdigit(c)) {
            leerNumero();
        }
        // Identificadores
        else if (isalpha(c)) {
            leerIdentificador();
        }
        // Espacios
        else if (c == ' ') {
            posicion += 1;
            columna += 1;
        }
        // Saltos de línea
        else if (c == '\n') {
            posicion++;
            linea += 1;
            columna += 1;
        }
        else {
            posicion += 1;
        }
    }
}