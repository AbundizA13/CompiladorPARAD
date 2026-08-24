#include "parser.hpp"
#include "LexerAnalyzerClass.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

// Esta funcion lee archivos
string leerArchivo(const std::string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    
    if (!archivo.is_open()) {
        cerr<<"Error: No se puede abrir el archivo"<<nombreArchivo<<endl;
        return "";
    }
    
    stringstream buffer;
    buffer<<archivo.rdbuf();
    archivo.close();
    
    return buffer.str();
}

int main() {
	/*Vector provisional, mientras falta el lexer y
    procesador de archivos de THIAGO.*/
	// Meter los archivos de texto aqui
    string codigo = leerArchivo("Programa.txt");
    
    // 1. Lexer para leer codigo
    LexicalAnalyzer lexer(codigo);
    lexer.analyze();
    
    // 2. Obtener los tokens del lexer (El vector de tokens)
    const auto& tokens = lexer.getTokens();
    
    // 3. Lista de identificacion
    for (const auto& token : tokens) {
        cout << token.printContent() << endl;
    }

	try {
		Parser parser(tokens);
		const Program program = parser.parse();
		std::cout << program.toString() << '\n';
	} catch (const ParserSyntaxError& error) {
		std::cerr << "Error de sintaxis: " << error.what() << '\n';
	}

    cout<<"\n\nPresiona Enter para salir..."<<endl;
    cin.get();
    
	return 0;
}
