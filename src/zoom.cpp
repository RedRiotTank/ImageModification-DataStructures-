//
// Created by albertoplaza on 15/10/21.
//

// Fichero: Zoom.cpp
// Calcula el zoom a una imagen
//

#include <iostream>
#include <cstring>
#include <cstdlib>

#include <image.h>

using namespace std;


int main (int argc, char *argv[]){

    char *origen, *destino; // nombres de los ficheros
    Image image;
    Image NewImage;

    // Comprobar validez de la llamada
    // Comprobar validez de la llamada
    if (argc != 6){
        cerr << "Error: Numero incorrecto de parametros.\n";
        cerr << "Uso: subimagen <fich_orig> <fich_rdo> <fila> <col> <filas_sub> <cols_sub>\n";
        exit (1);
    }

    // Obtener argumentos
    origen  = argv[1];
    destino = argv[2];

    int
            fila = atoi(argv[3]),
            col = atoi(argv[4]),
            lado = atoi(argv[5]);


    // Mostramos argumentos
    cout << endl;
    cout << "Fichero origen: " << origen << endl;
    cout << "Fichero resultado: " << destino << endl;

    // Leer la imagen del fichero de entrada
    if (!image.Load(origen)){
        cerr << "Error: No pudo leerse la imagen." << endl;
        cerr << "Terminando la ejecucion del programa." << endl;
        return 1;
    }

    // Mostrar los parametros de la Imagen
    cout << endl;
    cout << "Dimensiones de " << origen << ":" << endl;
    cout << "   Imagen   = " << image.get_rows()  << " filas x " << image.get_cols() << " columnas " << endl;

    // Calcular el ZOOM
   // NewImage = image.Crop(fila, col ,lado ,lado).Zoom2X();

    // Guardar la imagen resultado en el fichero
    if (image.Crop(fila, col ,lado ,lado).Zoom2X().Save(destino))
        cout  << "La imagen se guardo en " << destino << endl;
    else {
        cerr << "Error: No pudo guardarse la imagen." << endl;
        cerr << "Terminando la ejecucion del programa." << endl;
        return 1;

    }
}


/*
    Image a;
    a.Initialize(3,3);
    a.set_pixel(0,0,10);
    a.set_pixel(0,1,6);
    a.set_pixel(0,2,10);
    a.set_pixel(1,0,6);
    a.set_pixel(1,1,10);
    a.set_pixel(1,2,6);
    a.set_pixel(2,0,10);
    a.set_pixel(2,1,4);
    a.set_pixel(2,2,10);

    for (int i=0; i<a.get_rows(); i++) {
        for (int j = 0; j < a.get_cols(); j++)
            cout << abs(a.img[i][j]) << " ";
        cout << "\n";
    }
    Image zoomA;
    zoomA = a.Zoom2X();
    cout << "\n";
    for (int i=0; i<zoomA.get_rows(); i++) {
        for (int j = 0; j < zoomA.get_cols(); j++)
            cout << abs(zoomA.img[i][j]) << " ";
        cout << "\n";
    }

    return 0;

}
*/
/*
int main (int argc, char *argv[]) {

    char *origen, *destino; // nombres de los ficheros
    Image image;
    Image NewImage;

    // Comprobar validez de la llamada
    // Comprobar validez de la llamada
    if (argc != 3) {
        cerr << "Error: Numero incorrecto de parametros.\n";
        cerr << "Uso: subimagen <fich_orig> <fich_rdo> <fila> <col> <filas_sub> <cols_sub>\n";
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

    if (!NewImage.Load(destino)) {
        cerr << "Error: No pudo leerse la imagen." << endl;
        cerr << "Terminando la ejecucion del programa." << endl;
        return 1;
    }

    for (int i = 0; i < image.get_cols(); i++) {
        if (image.get_pixel(0, i) != NewImage.get_pixel(0, i)) {
            cout << "ERROR: (0," << i << ")" << "\n";
        }
    }

    for (int i = 0; i < image.get_cols(); i++) {
        if (image.get_pixel(NewImage.get_rows() - 1, i) != NewImage.get_pixel(NewImage.get_rows() - 1, i)) {
            cout << "ERROR: (0," << i << ")" << "\n";
        }
    }

    for (int i = 0; i < image.get_rows(); i++) {
        if (image.get_pixel(i, 0) != NewImage.get_pixel(i, 0)) {
            cout << "ERROR: (" << i << ",0 \n";
        }
    }

    for (int i = 0; i < image.get_rows(); i++) {
        if (image.get_pixel(i, NewImage.get_cols() - 1) != NewImage.get_pixel(i, NewImage.get_cols() - 1)) {
            cout << "ERROR: (" << i << ",0 \n";
        }

    }

    for (int i = 1; i < NewImage.get_rows(); i = i + 2) {
        for (int j = 1; j < NewImage.get_cols(); j = j + 2) {
            if (image.get_pixel(i, j) != NewImage.get_pixel(i, j)) {
                cout << "ERROR";
            }

        }

    }

    cout << abs(image.get_pixel(0, 11)) << "\n";
    cout << abs(NewImage.get_pixel(0, 11)) << "\n";

    for(int i=0; i < image.get_rows(); i++) {
        for (int j = 0; j < image.get_cols(); j++) {
            if (image.get_pixel(i, j) == NewImage.get_pixel(i, j))
                cout << "iguales en: (" << i << "," << j << ")" << "\n";
            else
                cout << "NO IGUALES EN: (" << i << "," << j << ")" << "\n";

        }
    }

}
    /*
    int contador = 0;
    for(int i=0; i < image.get_rows(); i++) {
        for (int j = 0; j < image.get_cols(); j++) {
            if (image.get_pixel(i, j) == NewImage.get_pixel(i, j))
                cout << "iguales en: (" << i << "," << j << ")" << "\n";
            else{
                cout << "NO IGUALES EN: (" << i << "," << j << ")" << "\n";
                contador++;
                }
        }
    }
    cout << abs(image.get_pixel(1,5)) << "\n";
    cout << abs(NewImage.get_pixel(1,5)) << "\n";

    cout << abs(image.get_pixel(0,5)) << "\n";
    cout << abs(NewImage.get_pixel(0,5)) << "\n";

    cout << abs(image.get_pixel(2,5)) << "\n";
    cout << abs(NewImage.get_pixel(2,5)) << "\n";

    cout << "\n";



    cout << image.size()<< "\n";
    cout << NewImage.size()<< "\n";
    cout << "num distintos: " << contador <<"\n";

}

*/