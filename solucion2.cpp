#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <ctime>

using namespace std;

// Tipo de dato exigido para las claves
typedef unsigned char uchar;

// Parámetros de la Solución 2
#define NIVELES_L 4    // Cantidad de pisos de la grilla
#define SALTO_K 10     // Una de cada K palabras sube al siguiente nivel
#define SIGMA 256      // Alfabeto ASCII

// Estructura del Nodo: Elemento básico de la grilla
struct Nodo {
    uchar* clave;
    Nodo *sig, *ant, *inf, *sup;

    // Constructor: Crea un nodo y duplica la palabra en memoria dinámica
    Nodo(uchar* c) {
        clave = (uchar*)strdup((const char*)c);
        sig = ant = inf = sup = nullptr;
    }

    // Destructor: Libera la memoria de la palabra
    ~Nodo() {
        if (clave) free(clave);
    }
};

class Solucion2 {
private:
    Nodo* cabeceras[NIVELES_L]; // Array de punteros al inicio de cada nivel

    // Comparación manual carácter a carácter (Orden ASCII)
    int comparar(uchar* s1, uchar* s2) {
        if (!s1 || !s2) return 0;
        int i = 0;
        while (s1[i] != '\0' && s2[i] != '\0') {
            if (s1[i] < s2[i]) return -1;
            if (s1[i] > s2[i]) return 1;
            i++;
        }
        if (s1[i] == '\0' && s2[i] == '\0') return 0;
        return (s1[i] == '\0') ? -1 : 1;
    }

public:
    Solucion2() {
        for (int i = 0; i < NIVELES_L; i++) cabeceras[i] = nullptr;
    }

    // Inserción ordenada en el nivel base (Nivel 0)
    void insertar(uchar* clave) {
        Nodo* nuevo = new Nodo(clave);
        if (!cabeceras[0]) {
            cabeceras[0] = nuevo;
        } else {
            Nodo* curr = cabeceras[0];
            Nodo* prev = nullptr;
            while (curr && comparar(curr->clave, clave) < 0) {
                prev = curr;
                curr = curr->sig;
            }
            if (!prev) {
                nuevo->sig = cabeceras[0];
                cabeceras[0]->ant = nuevo;
                cabeceras[0] = nuevo;
            } else {
                nuevo->sig = curr;
                nuevo->ant = prev;
                prev->sig = nuevo;
                if (curr) curr->ant = nuevo;
            }
        }
    }

    // Construcción de la jerarquía de niveles (Grilla)
    void construirNiveles() {
        for (int i = 1; i < NIVELES_L; i++) {
            Nodo* base = cabeceras[i-1];
            Nodo* ultimoSuperior = nullptr;
            int contador = 0;

            while (base) {
                if (contador % SALTO_K == 0) {
                    Nodo* nuevoSup = new Nodo(base->clave);
                    nuevoSup->inf = base;
                    base->sup = nuevoSup;

                    if (!cabeceras[i]) {
                        cabeceras[i] = nuevoSup;
                    } else {
                        ultimoSuperior->sig = nuevoSup;
                        nuevoSup->ant = ultimoSuperior;
                    }
                    ultimoSuperior = nuevoSup;
                }
                base = base->sig;
                contador++;
            }
        }
    }

    // Búsqueda por niveles (Skip Search)
    bool buscar(uchar* clave) {
        if (!cabeceras[0]) return false;

        // Empezar en el nivel más alto disponible
        int nivelActual = NIVELES_L - 1;
        while (nivelActual > 0 && !cabeceras[nivelActual]) nivelActual--;

        Nodo* curr = cabeceras[nivelActual];

        while (curr) {
            // Avanzar horizontalmente
            while (curr->sig && comparar(curr->sig->clave, clave) <= 0) {
                curr = curr->sig;
            }
            // Verificar si es la palabra
            if (comparar(curr->clave, clave) == 0) return true;

            // Bajar verticalmente
            if (curr->inf) curr = curr->inf;
            else break;
        }
        return false;
    }

    // Eliminación en cascada (Borra en todos los niveles)
    void eliminar(uchar* clave) {
        Nodo* curr = cabeceras[0];
        while (curr && comparar(curr->clave, clave) < 0) curr = curr->sig;

        if (curr && comparar(curr->clave, clave) == 0) {
            // Subir al nivel más alto donde esté la palabra
            while (curr->sup) curr = curr->sup;

            // Borrar hacia abajo reconectando vecinos
            while (curr) {
                Nodo* abajo = curr->inf;
                if (curr->ant) curr->ant->sig = curr->sig;
                else {
                    for(int i = 0; i < NIVELES_L; i++) {
                        if(cabeceras[i] == curr) cabeceras[i] = curr->sig;
                    }
                }
                if (curr->sig) curr->sig->ant = curr->ant;

                delete curr;
                curr = abajo;
            }
        }
    }

    // Destructor para evitar fugas de memoria
    ~Solucion2() {
        Nodo* curr = cabeceras[0];
        while (curr) {
            Nodo* temp = curr->sig;
            Nodo* vert = curr->sup;
            while (vert) {
                Nodo* sigV = vert->sup;
                delete vert;
                vert = sigV;
            }
            delete curr;
            curr = temp;
        }
    }

    long calcular_memoria() {
        long mem_nodos = 0;
        long mem_palabras = 0;

        // recorrer nivel base (nivel 0)
        Nodo* curr = cabeceras[0];
        while (curr) {
            mem_nodos += sizeof(Nodo);
            mem_palabras += strlen((char*)curr->clave) + 1;
            curr = curr->sig;
        }

        // recorrer niveles superiores
        for (int i = 1; i < NIVELES_L; i++) {
            curr = cabeceras[i];
            while (curr) {
                mem_nodos += sizeof(Nodo);
                mem_palabras += strlen((char*)curr->clave) + 1;
                curr = curr->sig;
            }
        }

        long mem_cabeceras = NIVELES_L * sizeof(Nodo*);
        return mem_nodos + mem_palabras + mem_cabeceras;
    }
};

// --- Bloque de Experimentación ---
void ejecutar_experimentos() {
    Solucion2 grilla;
    string linea;

    cout << "\n==EXPERIMENTOS: SOLUCION 2== " << endl;
    // 1. Carga de D1.txt (Experimento de Construcción)
    ifstream d1("D1.txt");
    if (!d1) { cout << "Error: No existe D1.txt" << endl; return; }

    cout << "\n=Experimento 1: Construccion= " << endl;
    clock_t t1 = clock();
    while (getline(d1, linea)) {
        if (!linea.empty()){
            if (linea.back()== '\r') linea.pop_back();
            grilla.insertar((uchar*)linea.c_str());
        }
    }
    grilla.construirNiveles(); // construir niveles de la grilla
    clock_t t2 = clock();
    d1.close();
    cout << "Tiempo Carga/Construccion: " << (double)(t2 - t1) / CLOCKS_PER_SEC << "s" << endl;
    cout << "Memoria Construccion niveles: " << grilla.calcular_memoria() / 1024.0 << " KB" << endl; // memoria usada en la construccion

    // Operaciones con D2.txt
    ifstream d2("D2.txt");
    if (!d2) { cout << "Error: No existe D2.txt" << endl; return; }
    vector<string> d2_palabras;
    while (getline(d2, linea)){
        if (!linea.empty()){
            if (linea.back() == '\r') linea.pop_back();
            d2_palabras.push_back(linea);
        }
    }
    d2.close();

    // 2. Busqueda de elementos
    cout << "\n=Experinento 2: Busqueda= " << endl;
    int encontradas = 0;
    clock_t t3 = clock();
    for (int i = 0; i < 10000 && i < (int)d2_palabras.size(); i++) {
        if (grilla.buscar((uchar*)d2_palabras[i].c_str())){
            encontradas++;
        }
    }
    cout << "Palabras encontradas " << encontradas << "/" << "10000" << endl;
    cout << "Tiempo promedio busqueda (10k): " << (double)(clock() - t3) / CLOCKS_PER_SEC << "s" << endl;
    cout << "Memoria busqueda: " << grilla.calcular_memoria() / 1024.0 << " KB (igual que construccion)" << endl;

    // Inserción (5000 primeras)
    cout << "\n=Experimento 3: Insercion/Eliminacion= " << endl;
    clock_t t5 = clock();
    for (int i = 0; i < 5000 && i < (int)d2_palabras.size(); i++){
        grilla.insertar((uchar*)d2_palabras[i].c_str());
    }
    cout << "Tiempo insertar (5k): " << (double)(clock() - t5) / CLOCKS_PER_SEC << "s" << endl;
    cout << "Memoria tras insercion: " << grilla.calcular_memoria() / 1024.0 << " KB" << endl;

    // Eliminación (5000 ultimas)
    clock_t t7 = clock();
    int inicio = max(0, (int)d2_palabras.size() - 5000);
    for (int i = inicio; i < (int)d2_palabras.size(); i++){
        grilla.eliminar((uchar*)d2_palabras[i].c_str());
    }
    cout << "Tiempo Eliminar (5k): " << (double)(clock() - t7) / CLOCKS_PER_SEC << "s" << endl;
    cout << "Memoria tras Eliminacion: " << grilla.calcular_memoria() / 1024.0 << " KB" << endl;
}

int main() {
    ejecutar_experimentos();
    return 0;
}
