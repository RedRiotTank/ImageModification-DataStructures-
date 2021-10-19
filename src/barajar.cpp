//
// Created by albertoplaza on 15/10/21.
//

// Fichero: barajar.cpp
// Shuffle de una imagen por filas o columnas
//

#include <iostream>
#include <cstdlib>
#include <image.h>
#include <chrono>

using namespace std;


int main(int argc, char *argv[]) {

    char *origen, *destino; // nombres de los ficheros
    Image image;

    // Comprobar validez de la llamada
    if (argc != 3) {
        cerr << "Error: Numero incorrecto de parametros.\n";
        cerr << "Uso: barajar <fich_orig> <fich_rdo> \n";
        exit(1);
    }

    // Obtener argumentos
    origen = argv[1];
    destino = argv[2];

    // Mostramos argumentos
    cout << endl;
    cout << "Fichero origen: " << origen << endl;
    cout << "Fichero resultado: " << destino << endl;

    // Leer la imagen del fichero de entrada
    if (!image.Load(origen)) {
        cerr << "Error: No pudo leerse la imagen." << endl;
        cerr << "Terminando la ejecucion del programa." << endl;
        return 1;
    }

    // Mostrar los parametros de la Imagen
    cout << endl;
    cout << "Dimensiones de " << origen << ":" << endl;
    cout << "   Imagen   = " << image.get_rows() << " filas x " << image.get_cols() << " columnas " << endl;

    image.ShuffleRows();
    // Calcular el zoom y guardar la imagen resultado en el fichero
    if (image.Save(destino))
        cout << "La imagen se guardo en " << destino << endl;
    else {
        cerr << "Error: No pudo guardarse la imagen." << endl;
        cerr << "Terminando la ejecucion del programa." << endl;
        return 1;

    }
}