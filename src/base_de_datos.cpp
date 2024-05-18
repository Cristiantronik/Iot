#include "base_de_datos.h"
#include <sqlite3.h>
#include <ctime>
#include <iostream>

static int callback(void* /*NotUsed*/, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }
    return 0;
}

BaseDeDatos::BaseDeDatos(const std::string& nombre_db) {
    int rc = sqlite3_open(nombre_db.c_str(), &db);
    if (rc) {
        std::cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    // Crear la base de datos si no existe
    char* error_msg = nullptr;
    rc = sqlite3_exec(db, "CREATE DATABASE IF NOT EXISTS datos_sensores.db", nullptr, nullptr, &error_msg);
    if (rc != SQLITE_OK) {
        std::cerr << "Error al crear la base de datos: " << error_msg << std::endl;
        sqlite3_free(error_msg);
        sqlite3_close(db);
        return;
    }

    // Crear la tabla si no existe
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS datos_sensores ("
                          "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "timestamp INTEGER,"
                          "temperatura REAL,"
                          "humedad INTEGER,"
                          "humedad_suelo INTEGER,"
                          "velocidad_viento REAL,"
                          "direccion_viento REAL,"
                          "precipitacion REAL,"
                          "intensidad_luz INTEGER"
                          ");", nullptr, nullptr, &error_msg);
    if (rc != SQLITE_OK) {
        std::cerr << "Error al crear la tabla: " << error_msg << std::endl;
        sqlite3_free(error_msg);
    }

    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS datos_sensores_min_prom_max ("
                          "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "timestamp INTEGER,"
                          "min_temperatura REAL,"
                          "prom_temperatura REAL,"
                          "max_temperatura REAL,"
                          "min_humedad INTEGER,"
                          "prom_humedad REAL,"
                          "max_humedad INTEGER,"
                          "min_humedad_suelo INTEGER,"
                          "prom_humedad_suelo REAL,"
                          "max_humedad_suelo INTEGER,"
                          "min_velocidad_viento REAL,"
                          "prom_velocidad_viento REAL,"
                          "max_velocidad_viento REAL,"
                          "min_direccion_viento REAL,"
                          "prom_direccion_viento REAL,"
                          "max_direccion_viento REAL,"
                          "min_precipitacion REAL,"
                          "prom_precipitacion REAL,"
                          "max_precipitacion REAL,"
                          "min_intensidad_luz INTEGER,"
                          "prom_intensidad_luz REAL,"
                          "max_intensidad_luz INTEGER"
                          ");", nullptr, nullptr, &error_msg);
    if (rc != SQLITE_OK) {
        std::cerr << "Error al crear la tabla: " << error_msg << std::endl;
        sqlite3_free(error_msg);
    }
}

BaseDeDatos::~BaseDeDatos() {
    sqlite3_close(db);
}

bool BaseDeDatos::almacenarDatos(const DatosSensores& datos) {
    char* error_msg = nullptr;
    std::string query = "INSERT INTO datos_sensores ("
                        "timestamp, temperatura, humedad, humedad_suelo, "
                        "velocidad_viento, direccion_viento, precipitacion, intensidad_luz) "
                        "VALUES (" + std::to_string(datos.timestamp) + ", " +
                        std::to_string(datos.temperatura) + ", " +
                        std::to_string(datos.humedad) + ", " +
                        std::to_string(datos.humedad_suelo) + ", " +
                        std::to_string(datos.velocidad_viento) + ", " +
                        std::to_string(datos.direccion_viento) + ", " +
                        std::to_string(datos.precipitacion) + ", " +
                        std::to_string(datos.intensidad_luz) + ");";

    int rc = sqlite3_exec(db, query.c_str(), callback, nullptr, &error_msg);
    if (rc != SQLITE_OK) {
        std::cerr << "Error al almacenar los datos: " << error_msg << std::endl;
        sqlite3_free(error_msg);
        return false;
    }

    return true;
}

bool BaseDeDatos::almacenarMinPromedioMax(const std::vector<DatosSensores>& mediciones) {
    if (mediciones.empty()) {
        return false;
    }

    time_t timestamp = mediciones.front().timestamp;
    float min_temperatura = calcularMinimo(mediciones, "temperatura");
    float prom_temperatura = calcularPromedio(mediciones, "temperatura");
    float max_temperatura = calcularMaximo(mediciones, "temperatura");
    int min_humedad = static_cast<int>(calcularMinimo(mediciones, "humedad"));
    float prom_humedad = calcularPromedio(mediciones, "humedad");
    int max_humedad = static_cast<int>(calcularMaximo(mediciones, "humedad"));
    int min_humedad_suelo = static_cast<int>(calcularMinimo(mediciones, "humedad_suelo"));
    float prom_humedad_suelo = calcularPromedio(mediciones, "humedad_suelo");
    int max_humedad_suelo = static_cast<int>(calcularMaximo(mediciones, "humedad_suelo"));
    float min_velocidad_viento = calcularMinimo(mediciones, "velocidad_viento");
    float prom_velocidad_viento = calcularPromedio(mediciones, "velocidad_viento");
    float max_velocidad_viento = calcularMaximo(mediciones, "velocidad_viento");
    float min_direccion_viento = calcularMinimo(mediciones, "direccion_viento");
    float prom_direccion_viento = calcularPromedio(mediciones, "direccion_viento");
    float max_direccion_viento = calcularMaximo(mediciones, "direccion_viento");
    float min_precipitacion = calcularMinimo(mediciones, "precipitacion");
    float prom_precipitacion = calcularPromedio(mediciones, "precipitacion");
    float max_precipitacion = calcularMaximo(mediciones, "precipitacion");
    int min_intensidad_luz = static_cast<int>(calcularMinimo(mediciones, "intensidad_luz"));
    float prom_intensidad_luz = calcularPromedio(mediciones, "intensidad_luz");
    int max_intensidad_luz = static_cast<int>(calcularMaximo(mediciones, "intensidad_luz"));

    char* error_msg = nullptr;
    std::string query = "INSERT INTO datos_sensores_min_prom_max ("
                        "timestamp, min_temperatura, prom_temperatura, max_temperatura, "
                        "min_humedad, prom_humedad, max_humedad, "
                        "min_humedad_suelo, prom_humedad_suelo, max_humedad_suelo, "
                        "min_velocidad_viento, prom_velocidad_viento, max_velocidad_viento, "
                        "min_direccion_viento, prom_direccion_viento, max_direccion_viento, "
                        "min_precipitacion, prom_precipitacion, max_precipitacion, "
                        "min_intensidad_luz, prom_intensidad_luz, max_intensidad_luz) "
                        "VALUES (" + std::to_string(timestamp) + ", " +
                        std::to_string(min_temperatura) + ", " +
                        std::to_string(prom_temperatura) + ", " +
                        std::to_string(max_temperatura) + ", " +
                        std::to_string(min_humedad) + ", " +
                        std::to_string(prom_humedad) + ", " +
                        std::to_string(max_humedad) + ", " +
                        std::to_string(min_humedad_suelo) + ", " +
                        std::to_string(prom_humedad_suelo) + ", " +
                        std::to_string(max_humedad_suelo) + ", " +
                        std::to_string(min_velocidad_viento) + ", " +
                        std::to_string(prom_velocidad_viento) + ", " +
                        std::to_string(max_velocidad_viento) + ", " +
                        std::to_string(min_direccion_viento) + ", " +
                        std::to_string(prom_direccion_viento) + ", " +
                        std::to_string(max_direccion_viento) + ", " +
                        std::to_string(min_precipitacion) + ","+
                        std::to_string(prom_precipitacion) + ", " +
                        std::to_string(max_precipitacion) + ", " +
                        std::to_string(min_intensidad_luz) + ", " +
                        std::to_string(prom_intensidad_luz) + ", " +
                        std::to_string(max_intensidad_luz) + ");";

    int rc = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &error_msg);
    if (rc != SQLITE_OK) {
        std::cerr << "Error al almacenar los datos: " << error_msg << std::endl;
        sqlite3_free(error_msg);
        return false;
    }

    return true;
}
                        
