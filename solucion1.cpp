#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>

using namespace std;

#define OVERHEAD 20
#define CAPACIDAD_INICIAL 4
#define SIGMA 256
#define REPET 10

//struct de vector uchar*
struct Solucion1{
    vector<unsigned char*> v;
    int inicio[SIGMA];
    int capacidad;
    int size_v;

    Solucion1(){
        capacidad = CAPACIDAD_INICIAL;
        size_v = 0;
        for (int i=0; i<SIGMA;i++)inicio[i] = -1;
    }
};

void construir_vector(Solucion1& sol, string archivo);

int main(){
    Solucion1 sol;
    construir_vector(sol, "D1.txt");
    cout << "empieza programa..." << endl;
    for (int i=0; i<sol.size_v; i++){
        cout << sol.v[i] << endl;
    }
    return 0;
}

void construir_vector(Solucion1& sol, string archivo){
    ifstream d1(archivo);
    if (!d1){
        cout << "archivo de texto " << archivo << "no encontrado" << endl;
        return;
    }
    //leer D1 y llenar vector "v" con uchar*
    string linea;
    while (getline(d1, linea)){
        if (!linea.empty()) {
            unsigned char* palabra = new unsigned char [linea.size() + 1];
            for (int i = 0; i<= (int)linea.size(); i++){
                palabra[i] = (unsigned char)linea[i];
            }
            sol.v.push_back(palabra);
            sol.size_v++;
            }
    }
    d1.close();
}
