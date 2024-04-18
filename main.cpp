/**
 * Juan Antonio Figueroa Rodríguez - A01369043
 * Oscar Sergio Orozco Vázquez - A01634257
 * Eric Samuel Miranda Alvarez - A01425360
 * 
 * Compilacion para debug:
 *    g++ -std=c++17 -Wall -g -o main *.cpp
 * Ejecucion con valgrind:
 *    nix-env -iA nixpkgs.valgrind
 *    valgrind --leak-check=full ./main
 *    https://www.youtube.com/watch?v=8JEEYwdrexc 
 *
 * Compilacion para ejecucion:
 *    g++ -std=c++17 -Wall -O3 -o main *.cpp
 * Ejecucion:
 *    ./main
 **/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono> 

using namespace std;

// Clase para representar un registro de la bitácora
class RegistroBitacora {
public:
    string mes;
    int dia;
    string hora;
    string ip;
    string razon;

    // Constructor predeterminado
    RegistroBitacora() : mes(""), dia(0), hora(""), ip(""), razon("") {}

    // Constructor
    RegistroBitacora(string m, int d, string h, string i, string r) : mes(m), dia(d), hora(h), ip(i), razon(r) {}

    // Sobrecarga del operador < para comparar registros por fecha
    bool operator<(const RegistroBitacora& otro) const {
        // Convertir las fechas a timestamps
        string fecha1_str = mes + " " + to_string(dia) + " " + hora;
        string fecha2_str = otro.mes + " " + to_string(otro.dia) + " " + otro.hora;
        tm fecha1_tm = {}, fecha2_tm = {};
        strptime(fecha1_str.c_str(), "%b %d %H:%M:%S", &fecha1_tm);
        strptime(fecha2_str.c_str(), "%b %d %H:%M:%S", &fecha2_tm);
        time_t fecha1_timestamp = mktime(&fecha1_tm);
        time_t fecha2_timestamp = mktime(&fecha2_tm);
        // Comparar timestamps
        return fecha1_timestamp < fecha2_timestamp;
    }
};

// Función para leer la bitácora desde el archivo y almacenarla en un vector
vector<RegistroBitacora> leerBitacora(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    vector<RegistroBitacora> bitacora;
    string mes, hora, ip, razon;
    int dia;
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo" << endl;
        exit(1);
    }
    while (archivo >> mes >> dia >> hora >> ip) {
        getline(archivo, razon);
        bitacora.emplace_back(mes, dia, hora, ip, razon);
    }
    archivo.close();
    return bitacora;
}

// Implementación de Merge Sort (Adaptado de Stack overflow)
// (https://stackoverflow.com/questions/62427451/merge-sort-implementation-in-c-using-vectors)

void merge(vector<RegistroBitacora>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    vector<RegistroBitacora> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0;
    int j = 0;
    int k = l;

    while (i < n1 && j < n2) {
        if (L[i] < R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(vector<RegistroBitacora>& arr, int l, int r) {
    if (l >= r)
        return;
    int m = l + (r - l) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
}

// Función de Quick Sort (Geeks for Geeks)
// (https://www.geeksforgeeks.org/cpp-program-for-quicksort/)

int partition(vector<RegistroBitacora>& arr, int low, int high) {
    RegistroBitacora pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Implementación de Quick Sort
void quickSort(vector<RegistroBitacora>& arr, int low, int high) {
    if (low < high) {
        // Partition the array
        int pivot = partition(arr, low, high);
        // Sort the subarrays
        quickSort(arr, low, pivot - 1);
        quickSort(arr, pivot + 1, high);
    }
}

// Función para buscar un rango de fechas en la bitácora y guardarlos en un archivo
void buscarPorFecha(const vector<RegistroBitacora>& bitacora, const string& fechaInicio, const string& fechaFin) {
    ofstream archivoResultado("resultado_busqueda.txt");
    if (!archivoResultado.is_open()) {
        cerr << "Error al abrir el archivo para escritura" << endl;
        exit(1);
    }

    cout << "Resultado: " << endl;
    int contador = 0;
    // Convertir las fechas de inicio y fin a timestamps
    tm fechaInicio_tm = {}, fechaFin_tm = {};
    strptime(fechaInicio.c_str(), "%b %d %H:%M:%S", &fechaInicio_tm);
    strptime(fechaFin.c_str(), "%b %d %H:%M:%S", &fechaFin_tm);
    time_t fechaInicio_timestamp = mktime(&fechaInicio_tm);
    time_t fechaFin_timestamp = mktime(&fechaFin_tm);
    bool encontrado = false;
    for (const auto& registro : bitacora) {
        // Convertir la fecha del registro a timestamp
        string fechaRegistro_str = registro.mes + " " + to_string(registro.dia) + " " + registro.hora;
        tm fechaRegistro_tm = {};
        strptime(fechaRegistro_str.c_str(), "%b %d %H:%M:%S", &fechaRegistro_tm);
        time_t fechaRegistro_timestamp = mktime(&fechaRegistro_tm);
        // Comparar si la fecha del registro está dentro del rango
        if (fechaRegistro_timestamp >= fechaInicio_timestamp && fechaRegistro_timestamp <= fechaFin_timestamp) {
            cout << fechaRegistro_str << " " << registro.ip << " " << registro.razon << endl;
            archivoResultado << fechaRegistro_str << " " << registro.ip << " " << registro.razon << endl;
            contador++;
            encontrado = true;
        }
    }
    archivoResultado.close();
    if (!encontrado) {
        cout << "No se encontraron registros dentro del rango especificado." << endl;
    } else {
        cout << "Total de registros: " << contador << " Registros" << endl;
    }
}

int main() {

    // Lee la bitácora desde el archivo
    vector<RegistroBitacora> bitacora = leerBitacora("bitacora.txt");

    // Solicita al usuario elegir el algoritmo de ordenamiento
    cout << "Elija el algoritmo de ordenamiento: " << endl;
    cout << "1. Quick Sort" << endl;
    cout << "2. Merge Sort" << endl;
    int opcion;
    cin >> opcion;

    // Variable para almacenar el tiempo de ejecución
    auto start_time = chrono::high_resolution_clock::now();

    // Ordena la bitácora por fecha utilizando el algoritmo seleccionado
    if (opcion == 1) {
        quickSort(bitacora, 0, bitacora.size() - 1);
    } else if (opcion == 2) {
        mergeSort(bitacora, 0, bitacora.size() - 1);
    } else {
        cout << "Opción no válida." << endl;
        return 1;
    }

    // Mide el tiempo de ejecución
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    cout << "Tiempo de ejecución: " << duration.count() << " microsegundos" << endl;

    // Almacena la bitácora ordenada en un archivo
    ofstream archivoOrdenado("bitacora_ordenada.txt");
    if (!archivoOrdenado.is_open()) {
        cerr << "Error al abrir el archivo para escritura" << endl;
        exit(1);
    }
    for (const auto& registro : bitacora) {
        archivoOrdenado << registro.mes << " " << registro.dia << " " << registro.hora << " " << registro.ip << " " << registro.razon << endl;
    }
    archivoOrdenado.close();

    // Solicita al usuario las fechas de inicio y fin de búsqueda
    string fechaInicio, fechaFin;
    cout << "Ingrese la fecha de inicio: ";
    cin.ignore(); // Limpiar el buffer de entrada
    getline(cin, fechaInicio);
    cout << "Ingrese la fecha de fin: ";
    getline(cin, fechaFin);

    // Busca un rango de fechas en la bitácora y guarda los registros en un archivo
    buscarPorFecha(bitacora, fechaInicio, fechaFin);

    return 0;
}