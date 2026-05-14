#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cstring>

using namespace std;

#define OVERHEAD 1
#define CAPACIDAD_INICIAL 1
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
// funciones necesarias
void construir_vector(Solucion1& sol, string archivo);
void intervalos_indice(Solucion1& sol);
int buscar_elementos(Solucion1& sol, unsigned char* palabra);
void insertar_elementos(Solucion1& sol, unsigned char* palabra);
void eliminar_elementos(Solucion1& sol, unsigned char* palabra);
long calcular_memoria(Solucion1& sol);
void randomizar(vector<unsigned char*>& vec);
void experimento(Solucion1& sol);

// construir vector d1 a partir de diccionario D1
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
    // actualizacion de capacidad
    sol.capacidad = sol.size_v;
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
    //index recibe el valor ASCII de la primera letra de la palabra buscada
    int index = (unsigned char)palabra[0];

    //l empieza en el intervalo de la letra pedida segun el valor de index
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

void insertar_elementos(Solucion1& sol, unsigned char* palabra){
    if (sol.size_v == sol.capacidad) {
        int nueva_cap = sol.capacidad + sol.capacidad * OVERHEAD;

        // crear nuevo vector mas grande
        vector<unsigned char*> nuevo_v(nueva_cap, nullptr);

        // copiar uno a uno
        for (int i = 0; i < sol.size_v; i++)
            nuevo_v[i] = sol.v[i];

        // reemplazar
        sol.v = nuevo_v;
        sol.capacidad = nueva_cap;
    }
    // 2.encontrar posicion correcta
    int pos = sol.size_v;
    for (int i = 0; i < sol.size_v; i++) {
        if (strcmp((char*)palabra, (char*)sol.v[i]) <= 0) {
            pos = i;
            break;
        }
    }

    // 3.desplazar elementos a la derecha
    for (int i = sol.size_v; i > pos; i--)
        sol.v[i] = sol.v[i-1];

    // 4.insertar palabra
    sol.v[pos] = palabra;
    sol.size_v++;

    // 5.actualizar intervalo[]
    intervalos_indice(sol);
}

void eliminar_elementos(Solucion1& sol, unsigned char* palabra) {
    // 1. buscar la palabra
    int idx = buscar_elementos(sol, palabra);
    if (idx == -1) {
        return;
    }
    // 3. borrar memoria basura
    delete[] sol.v[idx];
    sol.v[idx] = nullptr;

    // 4. desplazar elementos a la izquierda
    for (int i = idx; i < sol.size_v - 1; i++)
        sol.v[i] = sol.v[i+1];

    // 5. dejar ultima celda en nullptr
    sol.v[sol.size_v - 1] = nullptr;
    sol.size_v--;

    // 6. actualizar intervalo[]
    intervalos_indice(sol);
}

long calcular_memoria(Solucion1& sol) {
    long mem_punteros  = sol.capacidad * sizeof(unsigned char*);
    long mem_palabras  = 0;
    for (int i = 0; i < sol.size_v; i++)
        mem_palabras += strlen((char*)sol.v[i]) + 1;
    long mem_intervalo = SIGMA * sizeof(int);
    long mem_struct    = sizeof(int) * 2;
    return mem_punteros + mem_palabras + mem_intervalo + mem_struct;
}

void randomizar(vector<unsigned char*>& vec) {
    for (int i = vec.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        unsigned char* temp = vec[i];
        vec[i] = vec[j];
        vec[j] = temp;
    }
}

void experimento(Solucion1& sol){
    clock_t t;
        float avgT;

        cout << "\n==EXPERIMENTOS: SOLUCION 1== " << endl;
        // experimento 1
        cout << "\n=Experimento 1: Construccion= " << endl;
        t = clock();
        construir_vector(sol, "D1.txt");
        t = clock() - t;
        cout << "Tiempo construccion: " << (float)t / CLOCKS_PER_SEC << "s" << endl;
        cout << "Memoria construccion vector: " << calcular_memoria(sol) / 1024.0 << " KB" << endl;

        // cargar D2 dentro de experimento
          vector<unsigned char*> d2;
          ifstream archivo("D2.txt");
          string linea;
          while (getline(archivo, linea)){
              if (!linea.empty()){
                  if (linea.back() == '\r') linea.pop_back();
                  unsigned char* palabra = new unsigned char[linea.size() + 1];
                  for (int i = 0; i <= (int)linea.size(); i++)
                      palabra[i] = (unsigned char)linea[i];
                  d2.push_back(palabra);
              }
          }
          archivo.close();

        // experimento 2
        cout << "\n=Experimento 2: Busqueda= " << endl;
        intervalos_indice(sol);
        srand(time(NULL));
        int encontradas = 0;
        t = clock();
        for (int k = 0; k < REPET; k++) {
            if (buscar_elementos(sol, d2[rand() % d2.size()]) >= 0) encontradas++;
        }
        t = clock() - t;
        cout << "Palabras encontradas: " << encontradas << "/" << REPET << endl;
        cout << "Tiempo promedio busqueda(10k): " << (float)t / CLOCKS_PER_SEC / REPET << " s" << endl;
        cout << "Memoria busqueda: " << calcular_memoria(sol) / 1024.0 << " KB (igual que construccion)" << endl;

        // experimento 3
        cout << "\n=Experimento 3: Insercion/Eliminacion= " << endl;
        vector<unsigned char*> primeras(d2.begin(), d2.begin() + 5000);
        vector<unsigned char*> ultimas(d2.end() - 5000, d2.end());
        randomizar(primeras);
        randomizar(ultimas);

        int inserciones = 0, eliminaciones = 0;

        // insercion
        t = clock();
        for (int i = 0; i < 5000; i++) { insertar_elementos(sol, primeras[i]); inserciones++; }
        t = clock() - t;
        cout << "inserciones(5k): " << inserciones << " Tiempo inserciones: " << (float)t / CLOCKS_PER_SEC << "s" << endl;
        cout << "Memoria tras insercion: " << calcular_memoria(sol) / 1024.0 << " KB" << endl;

        // eliminacion
        t = clock();
        for (int i = 0; i < 5000; i++) {
            int antes = sol.size_v;
            eliminar_elementos(sol, ultimas[i]);
            if (sol.size_v < antes) eliminaciones++;
        }
        t = clock() - t;
        cout << "eliminaciones(5k): " << eliminaciones << " Tiempo eliminaciones: " << (float)t / CLOCKS_PER_SEC << "s" << endl;
        cout << "Memoria tras eliminacion: " << calcular_memoria(sol) / 1024.0 << " KB" << endl;
}

int main(){
    Solucion1 sol;
    experimento(sol);
    return 0;
}
