// Fichero: Zoom.cpp
// Calcula el zoom a una imagen
//

#include <iostream>
#include <cstdlib>

#include <image.h>

using namespace std;


int main(int argc, char *argv[]) {

    char *origen, *destino; // nombres de los ficheros
    Image image;
    Image NewImage;

    // Comprobar validez de la llamada
    if (argc != 6) {
        cerr << "Error: Numero incorrecto de parametros.\n";
        cerr << "Uso: zoom <fich_orig> <fich_rdo> <fila> <col> <filas_sub> <cols_sub>\n";
        exit(1);
    }

    // Obtener argumentos
    origen = argv[1];
    destino = argv[2];

    int
            fila = atoi(argv[3]),
            col = atoi(argv[4]),
            lado = atoi(argv[5]);


    // Mostramos argumentos
    cout << endl;
    cout << "Fichero origen: " << origen << endl;
    cout << "Fichero resultado: " << destino << endl;
    cout << "" << fila << endl;
    cout << "" << col << endl;
    cout << "" << lado << endl;

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

    // Calcular el zoom y guardar la imagen resultado en el fichero
    if (image.Crop(fila, col, lado, lado).Zoom2X().Save(destino))
        cout << "La imagen se guardo en " << destino << endl;
    else {
        cerr << "Error: No pudo guardarse la imagen." << endl;
        cerr << "Terminando la ejecucion del programa." << endl;
        return 1;

    }
}