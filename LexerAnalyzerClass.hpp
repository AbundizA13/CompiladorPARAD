#ifndef LEXICALANALYZER_HPP
#define LEXICALANALYZER_HPP

#include <string>
#include <vector>
#include <map>
#include "token.hpp"

class LexicalAnalyzer {
    private:
    // Mis métodos y atributos privados
    bool esSimbolo(char c);
    bool esSimboloValido(const std::string& s);
    
    TokenType obtenerTipoSimbolo(const std::string& s);
    
    void leerCadena(char delimitador);
    void leerComentario();
    
    std::vector<Token> tokens;
    std::string input;
    std::size_t posicion;
    std::size_t linea;
    std::size_t columna;
    std::map<std::string, TokenType> palabrasReservadas;
    std::map<std::string, TokenType> simbolos;
    
    public:
    
    // Mis métodos y atributos publicos
    LexicalAnalyzer(const std::string& fuente);
    void leerNumero();
    void leerIdentificador();
    void leerSimbolo();
    void analyze();
    const std::vector<Token>& getTokens() const;
};

#endif