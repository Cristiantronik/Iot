#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include "sensores.h"
#include "base_de_datos.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Uso: " << argv[0] << " <nombre_db> <periodo_adquisicion>" << std::endl;
        return 1;
    }

    std::string nombre_db = argv[1];
    int periodo_adquisicion = std::stoi(argv[2]);

    if (periodo_adquisicion < 5 || periodo_adquisicion > 20) {
        std::cout << "El periodo de adquisición debe estar entre 5 y 20 segundos." << std::endl;
        return 1;
    }

    BaseDeDatos db(nombre_db);

    std::vector<DatosSensores> mediciones_minuto;
    time_t hora_inicio = std::time(nullptr);

    while (true) {
        DatosSensores datos = leerDatosSensores();
        if (!db.almacenarDatos(datos)) {
            std::cerr << "Error al almacenar los datos" << std::endl;
        }

        mediciones_minuto.push_back(datos);

        if (datos.timestamp >= hora_inicio + 60) {
            if (!db.almacenarMinPromedioMax(mediciones_minuto)) {
                std::cerr << "Error al almacenar los datos min/prom/max" << std::endl;
            }
            mediciones_minuto.clear();
            hora_inicio = datos.timestamp;
        }

        std::this_thread::sleep_for(std::chrono::seconds(periodo_adquisicion));
    }

    return 0;
}