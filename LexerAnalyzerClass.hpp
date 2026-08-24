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
    bool esSimbolo(char c);
    bool esSimboloValido(const string& s);
    
    TokenType obtenerTipoSimbolo(const string& s);
    
    void leerCadena(char delimitador);
    void leerComentario();
    
    vector<Token> tokens;
    string input;
    size_t posicion;
    size_t linea;
    size_t columna;
    map<string, TokenType> palabrasReservadas;
    std::map<std::string, TokenType> simbolos;
    
    public:
    
    // Mis métodos y atributos publicos
    LexicalAnalyzer(const string& fuente);
    void leerNumero();
    void leerIdentificador();
    void leerSimbolo();
    void analyze();
    const vector<Token>& getTokens() const;
};

#endif