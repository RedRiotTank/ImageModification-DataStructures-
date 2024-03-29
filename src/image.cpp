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
void Image::Allocate(int nrows, int ncols, byte *buffer) {
    rows = nrows;
    cols = ncols;

    img = new byte *[rows];

    if (buffer != 0)
        img[0] = buffer;
    else
        img[0] = new byte[rows * cols];

    for (int i = 1; i < rows; i++)
        img[i] = img[i - 1] + cols;
}

// Función auxiliar para inicializar imágenes con valores por defecto o a partir de un buffer de datos
void Image::Initialize(int nrows, int ncols, byte *buffer) {
    if ((nrows == 0) || (ncols == 0)) {
        rows = cols = 0;
        img = 0;
    } else Allocate(nrows, ncols, buffer);
}

// Función auxiliar para copiar objetos Imagen
void Image::Copy(const Image &orig) {
    Initialize(orig.rows, orig.cols);
    for (int k = 0; k < rows * cols; k++)
        set_pixel(k, orig.get_pixel(k));
}

// Función auxiliar para destruir objetos Imagen
bool Image::Empty() const {
    return (rows == 0) || (cols == 0);
}

void Image::Destroy() {
    if (!Empty()) {
        delete[] img[0];
        delete[] img;
    }
}

LoadResult Image::LoadFromPGM(const char *file_path) {
    if (ReadImageKind(file_path) != IMG_PGM)
        return LoadResult::NOT_PGM;

    byte *buffer = ReadPGMImage(file_path, rows, cols);
    if (!buffer)
        return LoadResult::READING_ERROR;

    Initialize(rows, cols, buffer);
    return LoadResult::SUCCESS;
}

/********************************
       FUNCIONES PÚBLICAS
********************************/

// Constructor por defecto

Image::Image() {
    Initialize();
}

// Constructores con parámetros
Image::Image(int nrows, int ncols, byte value) {
    Initialize(nrows, ncols);
    for (int k = 0; k < rows * cols; k++) set_pixel(k, value);
}

bool Image::Load(const char *file_path) {
    Destroy();
    return LoadFromPGM(file_path) == LoadResult::SUCCESS;
}

// Constructor de copias
Image::Image(const Image &orig) {
    assert (this != &orig);
    Copy(orig);
}

// Destructor
Image::~Image() {
    Destroy();
}

// Operador de Asignación
Image &Image::operator=(const Image &orig) {
    if (this != &orig) {
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

int Image::size() const {
    return get_rows() * get_cols();
}

// Métodos básicos de edición de imágenes
void Image::set_pixel(int i, int j, byte value) {
    img[i][j] = value;
}

byte Image::get_pixel(int i, int j) const {
    return img[i][j];
}

// This doesn't work if representation changes
void Image::set_pixel (int k, byte value) {
    // TODO this makes assumptions about the internal representation
    // TODO Can you reuse set_pixel(i,j,value)?

    set_pixel(k/cols, k%cols, value);
}

// This doesn't work if representation changes
// This doesn't work if representation changes
byte Image::get_pixel (int k) const {
    // TODO this makes assumptions about the internal representation
    // TODO Can you reuse get_pixel(i,j)?
    return get_pixel(k/cols, k%cols);
}

// Métodos para almacenar y cargar imagenes en disco
bool Image::Save (const char * file_path) const {
    // TODO this makes assumptions about the internal representation
    return WritePGMImage(file_path, Image(*this).img[0], rows, cols);
}

//Métodos implementados
void Image::Invert() {
    int tam = size();

    for (int i = 0; i < tam; i++)
        set_pixel(i, 255 - get_pixel(i));
}

Image Image::Crop(int nrow, int ncol, int height, int width) const {
    //Creación de la nueva matriz
    Image nueva(height, width);

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            nueva.set_pixel(i, j, get_pixel(nrow + i, ncol + j));

    return nueva;
}

void Image::AdjustContrast(byte in1, byte in2, byte out1, byte out2) {
    const byte max_byte = 255;
    float cociente;
    float resultado;
    byte pixel_actual;

    for (int i = 0; i < rows * cols; i++) {
        pixel_actual = (float) get_pixel(i);

        if (pixel_actual < in1) {
            cociente = (((float) out1) / (in1));
            resultado = (cociente * pixel_actual);
        } else if (pixel_actual <= in2) {
            cociente = ((float) (out2 - out1) / (in2 - in1));
            resultado = out1 + (cociente * (pixel_actual - in1));
        } else {
            cociente = ((float) (max_byte - out2) / (max_byte - in2));
            resultado = out2 + (cociente * (pixel_actual - in2));
        }
        set_pixel(i, round(resultado));
    }
}

double Image::Mean(int i, int j, int height, int width) const {

    int divisor = height * width;
    double sumatorio = 0;
    int Topheight = i + height - 1;
    int Topwidth = j + width - 1;

    for (int a = i; a <= Topheight; a++)
        for (int b = j; b <= Topwidth; b++)
            sumatorio += get_pixel(a, b);

    return sumatorio / divisor;
}

Image Image::Subsample(int factor) const {
    int NewNfil = rows / factor;
    int NewNcol = cols / factor;
    Image NewImg;

    NewImg.Initialize(NewNfil, NewNcol);

    for (int i = 0; i < NewImg.get_rows(); i++)
        for (int j = 0; j < NewImg.get_cols(); j++)
            NewImg.set_pixel(i, j, round(Mean(i * factor, j * factor, factor, factor)));

    return NewImg;
}

Image Image::Zoom2X() const {
    int Newfil = 2 * this->get_rows() - 1;
    int Newcol = 2 * this->get_cols() - 1;
    int iOriginalIndex = 0, jOriginalIndex = 0;
    Image ZoomedImg;

    ZoomedImg.Initialize(Newfil, Newcol);

    for (int i = 0; i < ZoomedImg.get_rows(); i++) {
        for (int j = 0; j < ZoomedImg.get_cols(); j++) {

            if (i % 2 == 0 && j % 2 == 0) {
                ZoomedImg.set_pixel(i, j, this->get_pixel(iOriginalIndex, jOriginalIndex));
                jOriginalIndex++;

                if (jOriginalIndex >= this->get_cols())
                    iOriginalIndex++;
            }

            if (i % 2 == 0 && j % 2 != 0)
                ZoomedImg.set_pixel(i, j, round(this->Mean(iOriginalIndex, jOriginalIndex - 1, 1, 2)));

            if (i % 2 != 0 && j % 2 == 0) {
                ZoomedImg.set_pixel(i, j, round(this->Mean(iOriginalIndex - 1, jOriginalIndex, 2, 1)));
                jOriginalIndex++;
            }

            if (i % 2 != 0 && j % 2 != 0)
                ZoomedImg.set_pixel(i, j, round(this->Mean(iOriginalIndex - 1, jOriginalIndex - 1, 2, 2)));
        }

        jOriginalIndex = 0;

    }

    return ZoomedImg;

}

void Image::ShuffleRows() {
    const int p = 9973;
    byte ** bayt = this->img;
    img = new byte*[this->get_rows()];
    int newr;

    for (int r=0; r<this->get_rows(); r++) {
        newr = r * p % this->get_rows();
        img[r] = bayt[newr];
    }
    delete [] bayt;

}



