#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

#define OVERHEAD 50
#define CAPACIDAD_INICIAL 4
#define SIGMA 26

void construir_vector(string archivo);
void eliminar(vector<string>& v);
void insertar(vector<string>& v);
void buscar(vector<string>& v);

//variables
vector<string> v;
int size_v = 0;
int capacidad = CAPACIDAD_INICIAL;
int overhead = OVERHEAD;
int inicio[SIGMA];

int main(){
    construir_vector("D1.txt");
    for (int i=0; i < size_v-1; i++){
        cout << v[i] << endl;
    }
}

void construir_vector(string archivo){
    ifstream d1(archivo);
    if (!d1){
        cout << "archivo de texto " << archivo << "no encontrado" << endl;
        return;
    }
    //llenar vector "v" con los elementos uchar de D1.txt
    string linea;
    while (getline(d1, linea)){
        if (!linea.empty()) {
            v.push_back(linea);
            size_v++;
        }
    }
    d1.close();

    //ordenar vector "v" de strings
    for (int i=0; i < size_v-1; i++){
        for (int j=0; j < size_v-i-1; j++){
            if (v[j] > v[j+1]){
                string temporal = v[j];
                v[j] = v[j+1];
                v[j+1] = temporal;
            }
        }
    }
    capacidad = size_v;
}
