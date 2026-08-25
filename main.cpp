#include "parser.hpp"
#include "parser.hpp"
#include "LexerAnalyzerClass.hpp"

#include <windows.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

// Esta funcion lee archivos
std::string leerArchivo(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    
    if (!archivo.is_open()) {
        std::cerr<<"Error: No se puede abrir el archivo"<<nombreArchivo<<std::endl;
        return "";
    }
    
    std::stringstream buffer;
    buffer<<archivo.rdbuf();
    archivo.close();
    
    return buffer.str();
}

int main() {
    SetConsoleOutputCP(CP_UTF8);    
    SetConsoleCP(CP_UTF8);
	// Meter los archivos de texto aqui
    std::string codigo = leerArchivo("Programa.txt");
    
    // 1. Lexer para leer codigo
    LexicalAnalyzer lexer(codigo);
    lexer.analyze();
    
    // 2. Obtener los tokens del lexer (El vector de tokens)
    const auto& tokens = lexer.getTokens();
    
    std::cout<< "TOKENS (Analizador Léxico):\n\n";
    // 3. Lista de identificacion
    for (const auto& token : tokens) {
        std::cout << token.printContent() << std::endl;
    }

    std::cout << "\nAST (Analizador Sintáctico):\n\n";
	try {
		Parser parser(tokens);
		const Program program = parser.parse();
		std::cout << program.toString() << '\n';
	} catch (const ParserSyntaxError& error) {
		std::cerr << "Error de sintaxis: " << error.what() << '\n';
	}

    std::cout<<"\n\nPresiona Enter para salir..."<<std::endl;
    std::cin.get();
    
	return 0;
}
