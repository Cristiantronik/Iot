#pragma once

#include <string>
#include <vector>
#include "sensores.h"

class BaseDeDatos {
public:
    BaseDeDatos(const std::string& nombre_db);
    ~BaseDeDatos();

    bool almacenarDatos(const DatosSensores& datos);
    bool almacenarMinPromedioMax(const std::vector<DatosSensores>& mediciones);
    bool registrarInicioEjecucion(time_t inicio);
    bool mostrarTiempoTranscurrido();

private:
    sqlite3* db;
    time_t tiempo_inicio;
};
