#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cstring>

using namespace std;

#define OVERHEAD 20
#define CAPACIDAD_INICIAL 4
#define SIGMA 256
#define REPET 10000

//struct de vector uchar*
struct Solucion1{
    vector<unsigned char*> v;
    int intervalo[SIGMA];
    int capacidad;
    int size_v;

    Solucion1(){
        capacidad = CAPACIDAD_INICIAL;
        size_v = 0;
        for (int i=0; i<SIGMA;i++)intervalo[i] = -1;
    }
};

void construir_vector(Solucion1& sol, string archivo);
void intervalos_indice(Solucion1& sol);
int buscar_elementos(Solucion1& sol, unsigned char* palabra);


int main(){
    Solucion1 sol;
    construir_vector(sol, "D1.txt");
    intervalos_indice(sol);
    //cargar vector D2
    vector<unsigned char*> d2;
    ifstream archivo("D2.txt");
    string linea;
    while (getline(archivo, linea)){
        if (!linea.empty()){
            unsigned char* palabra = new unsigned char [linea.size() + 1];
            for (int i = 0; i <= (int)linea.size(); i++){
                palabra[i] = (unsigned char)linea[i];
            }
            d2.push_back(palabra);
        }
    }
    archivo.close();

    int encontradas = 0;
    for (int k = 0; k < REPET; k++){
        int idx_random = rand() % d2.size();
        int resultado = buscar_elementos(sol, d2[idx_random]);
        if (resultado >= 0) encontradas++;
    }
    cout << "Busquedas realizadas: " << REPET << endl;
    cout << "Encontradas: " << encontradas << endl;

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
            if (linea.back() == '\r') linea.pop_back();

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

void intervalos_indice(Solucion1& sol){
    for (int i = 0; i < SIGMA; i++) sol.intervalo[i] = -1;
    for (int i = 0; i < sol.size_v; i++){
        int index = (unsigned char)sol.v[i][0];
        if (sol.intervalo[index] == -1){
            sol.intervalo[index] = i;
        }
    }
}

int buscar_elementos(Solucion1& sol, unsigned char* palabra){
    int index = (unsigned char)palabra[0];

    //l empieza en el intervalo de la letra pedida
    int l = sol.intervalo[index];
    if (l == -1) return -1;

    //r esta al final de ese intervalo
    int r = sol.size_v - 1;
    for (int sig = index + 1; sig < SIGMA; sig++){
        if (sol.intervalo[sig] != -1){
            r = sol.intervalo[sig] - 1;
            break;
        }
    }

    //Busqueda Binaria
    while (l <= r) {
        int mid = (l + r) / 2;
        int cmp = strcmp((char*)palabra, (char*)sol.v[mid]);
        if      (cmp == 0) return mid;
        else if (cmp  < 0) r = mid - 1;
        else               l = mid + 1;
    }
    return -1;
}
