/**
 * @file image.cpp
 * @brief Fichero con definiciones para los métodos primitivos de la clase Image
 *
 */

#include <cstring>
#include <cassert>
#include <iostream>
#include <cmath>

#include <image.h>
#include <imageIO.h>

using namespace std;

/********************************
      FUNCIONES PRIVADAS
********************************/
void Image::Allocate(int nrows, int ncols, byte * buffer){
    rows = nrows;
    cols = ncols;

    img = new byte * [rows];

    if (buffer != 0)
        img[0] = buffer;
    else
        img[0] = new byte [rows * cols];

    for (int i=1; i < rows; i++)
        img[i] = img[i-1] + cols;
}

// Función auxiliar para inicializar imágenes con valores por defecto o a partir de un buffer de datos
void Image::Initialize (int nrows, int ncols, byte * buffer){
    if ((nrows == 0) || (ncols == 0)){
        rows = cols = 0;
        img = 0;
    }
    else Allocate(nrows, ncols, buffer);
}

// Función auxiliar para copiar objetos Imagen

void Image::Copy(const Image & orig){
    Initialize(orig.rows,orig.cols);
    for (int k=0; k<rows*cols;k++)
        set_pixel(k,orig.get_pixel(k));
}

// Función auxiliar para destruir objetos Imagen
bool Image::Empty() const{
    return (rows == 0) || (cols == 0);
}

void Image::Destroy(){
    if (!Empty()){
        delete [] img[0];
        delete [] img;
    }
}

LoadResult Image::LoadFromPGM(const char * file_path){
    if (ReadImageKind(file_path) != IMG_PGM)
        return LoadResult::NOT_PGM;

    byte * buffer = ReadPGMImage(file_path, rows, cols);
    if (!buffer)
        return LoadResult::READING_ERROR;

    Initialize(rows, cols, buffer);
    return LoadResult::SUCCESS;
}

/********************************
       FUNCIONES PÚBLICAS
********************************/

// Constructor por defecto

Image::Image(){
    Initialize();
}

// Constructores con parámetros
Image::Image (int nrows, int ncols, byte value){
    Initialize(nrows, ncols);
    for (int k=0; k<rows*cols; k++) set_pixel(k,value);
}

bool Image::Load (const char * file_path) {
    Destroy();
    return LoadFromPGM(file_path) == LoadResult::SUCCESS;
}

// Constructor de copias

Image::Image (const Image & orig){
    assert (this != &orig);
    Copy(orig);
}

// Destructor

Image::~Image(){
    Destroy();
}

// Operador de Asignación

Image & Image::operator= (const Image & orig){
    if (this != &orig){
        Destroy();
        Copy(orig);
    }
    return *this;
}

// Métodos de acceso a los campos de la clase

int Image::get_rows() const {
    return rows;
}

int Image::get_cols() const {
    return cols;
}

int Image::size() const{
    return get_rows()*get_cols();
}

// Métodos básicos de edición de imágenes
void Image::set_pixel (int i, int j, byte value) {
    img[i][j] = value;
}
byte Image::get_pixel (int i, int j) const {
    return img[i][j];
}

// This doesn't work if representation changes
void Image::set_pixel (int k, byte value) {
    // TODO this makes assumptions about the internal representation
    // TODO Can you reuse set_pixel(i,j,value)?
    img[0][k] = value;
}

// This doesn't work if representation changes
byte Image::get_pixel (int k) const {
    // TODO this makes assumptions about the internal representation
    // TODO Can you reuse get_pixel(i,j)?
    return img[0][k];
}

// Métodos para almacenar y cargar imagenes en disco
bool Image::Save (const char * file_path) const {
    // TODO this makes assumptions about the internal representation
    byte * p = img[0];
    return WritePGMImage(file_path, p, rows, cols);
}

//Métodos implementados
void Image::Invert() {
    for (int i=0; i<size(); i++)
        set_pixel(i,255-get_pixel(i));
}

Image Image::Crop(int nrow, int ncol, int height, int width) const{
    //Creación de la nueva matriz
    Image nueva(height, width);

    for (int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
            nueva.set_pixel(i,j, get_pixel(nrow+i,ncol+j));

    return nueva;
}



void Image::AdjustContrast (byte in1, byte in2, byte out1, byte out2) {
    const byte max_byte = 255;
    float cociente;
    float resultado;
    byte pixel_actual;

    for (int i = 0; i < rows * cols; i++) {
        pixel_actual = (float) get_pixel(i);

        if (pixel_actual < in1) {
            cociente = (((float) out1) / (in1));
            resultado = (cociente * pixel_actual); //comprobado
        } else if (pixel_actual <= in2) {
            cociente = ((float) (out2 - out1) / (in2 - in1));
            resultado = out1 + (cociente * (pixel_actual - in1)); //comprobado
        } else {
            cociente = ((float) (max_byte - out2) / (max_byte - in2));
            resultado = out2 + (cociente * (pixel_actual - in2));
        }
        set_pixel(i, round(resultado));
    }
}

//Métodos alberto

double Image::Mean (int i, int j, int height, int width) const{ //0,2,1,3

    int divisor = height*width;
    double sumatorio = 0;
    int p = i + height -1;  //0
    int q = j + width -1;   //4
    double devuelve;
    for (int a=i; a<=(p); a++){ //a=0       //1 iter 1º iter
        for(int b=j; b<=(q); b++) { //b=3   //3 iter 1º iter    2º iter 3ºiter
            sumatorio += get_pixel(a, b);

        }
    }

    if(height == 1 && width == 3 && this->get_pixel(i,j+1) == 0)        //Exclusivo para el método zoom.
        divisor--;

    if(height == 3 && width == 1 && this->get_pixel(i+1,j) == 0)
        divisor--;

    //SACAR ESTO DE ESTE METODO Y METERLO EN EL DE ZOOM DE ALGUNA FORMA.
    if(height == 3 && width == 3 && this->get_pixel(i+1,j+1) ==0)
        divisor = divisor - 5;

    devuelve = sumatorio / divisor;
    return devuelve;
}

Image Image::Subsample(int factor) const{
    int NewNfil = rows/factor ;
    int NewNcol = cols/factor;
    Image NewImg;

    NewImg.Initialize(NewNfil,NewNcol);

    for (int i=0; i<NewImg.get_rows(); i++)
        for(int j=0; j<NewImg.get_cols(); j++)
            NewImg.set_pixel(i,j,round(Mean(i*factor,j*factor,factor,factor)));

    return NewImg;
}

Image Image::Zoom2X() const{
    int Newfil = 2*this->get_rows() - 1;
    int Newcol = 2*this->get_cols() - 1;
    Image ZoomedImg;

    ZoomedImg.Initialize(Newfil,Newcol);

    //Copiado de valores estandard
    for(int i=0; i<this->get_rows(); i++)
        for(int j=0; j<this->get_cols(); j++)
            ZoomedImg.set_pixel(2*i,2*j,this->get_pixel(i,j));


    //Interpolacion de las columnas:




    for(int j=1; j<ZoomedImg.get_cols(); j=j+2){
        for(int i=1; i<ZoomedImg.get_rows(); i=i+2 ){

            ZoomedImg.set_pixel(i,j,round(ZoomedImg.Mean(i-1,j-1,3,3)));
        }
    }


    for(int i=1; i<ZoomedImg.get_cols(); i = i+2){
        ZoomedImg.set_pixel(0,i,round(ZoomedImg.Mean(0,i-1,1,3)));
        ZoomedImg.set_pixel(ZoomedImg.get_rows()-1,i,round(ZoomedImg.Mean(ZoomedImg.get_rows()-1,i-1,1,3)));
    }

    for(int i=1; i<ZoomedImg.get_rows(); i = i+2){
        ZoomedImg.set_pixel(i,0,round(ZoomedImg.Mean(i-1,0,3,1)));
        ZoomedImg.set_pixel(i,ZoomedImg.get_cols()-1,round(ZoomedImg.Mean(i-1,ZoomedImg.get_cols()-1,3,1)));
    }

    for(int i=1; i<ZoomedImg.get_rows(); i=i+2){

        for(int j=2; j<ZoomedImg.get_cols();j=j+2){

            ZoomedImg.set_pixel(i,j,round(ZoomedImg.Mean(i-1,j,3,1)));
        }
    }

    for(int i=2; i<ZoomedImg.get_rows(); i=i+2){

        for(int j=1; j<ZoomedImg.get_cols();j=j+2){

            ZoomedImg.set_pixel(i,j,round(ZoomedImg.Mean(i,j-1,1,3)));
        }
    }



    return ZoomedImg;
}


