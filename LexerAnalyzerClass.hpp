#ifndef LEXICALANALYZER_HPP
#define LEXICALANALYZER_HPP

#include <string>
#include <vector>
#include <map>
#include "token.hpp"

using namespace std;

class LexicalAnalyzer {
    private:
    // Mis métodos y atributos privados
    vector<Token> tokens;
    string input;
    size_t posicion;
    size_t linea;
    size_t columna;
    map<string, TokenType> palabrasReservadas;
    
    public:
    // Mis métodos y atributos publicos
    LexicalAnalyzer(const string& fuente);
    void analyze();
    void leerIdentificador();
    void leerNumeros();
    const vector<token>& getTokens() const;
};

#endif