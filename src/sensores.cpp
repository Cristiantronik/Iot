#include "sensores.h"
#include <cstdlib>
#include <ctime>
#include <numeric>
#include <limits>

DatosSensores leerDatosSensores() {
    std::srand(std::time(nullptr));

    DatosSensores datos;
    datos.timestamp = std::time(nullptr);
    datos.temperatura = (-10.0 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (45.0 + 10.0))));
    datos.humedad = rand() % 101;
    datos.humedad_suelo = rand() % 101;
    datos.velocidad_viento = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 40.0));
    datos.direccion_viento = (static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 360.0))) - 180.0;
    datos.precipitacion = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 200.0));
    datos.intensidad_luz = rand() % 4001;

    return datos;
}

float calcularMinimo(const std::vector<DatosSensores>& mediciones) {
    float minimo = std::numeric_limits<float>::max();

    for (const auto& medicion : mediciones) {
        if (medicion.temperatura < minimo) {
            minimo = medicion.temperatura;
        }
        if (medicion.humedad < static_cast<float>(minimo)) {
            minimo = static_cast<float>(medicion.humedad);
        }
        if (medicion.humedad_suelo < static_cast<float>(minimo)) {
            minimo = static_cast<float>(medicion.humedad_suelo);
        }
        if (medicion.velocidad_viento < minimo) {
            minimo = medicion.velocidad_viento;
        }
        if (medicion.direccion_viento < minimo) {
            minimo = medicion.direccion_viento;
        }
        if (medicion.precipitacion < minimo) {
            minimo = medicion.precipitacion;
        }
        if (medicion.intensidad_luz < static_cast<float>(minimo)) {
            minimo = static_cast<float>(medicion.intensidad_luz);
        }
    }

    return minimo;
}

float calcularPromedio(const std::vector<DatosSensores>& mediciones) {
    float suma = 0.0;
    int count = 0;

    for (const auto& medicion : mediciones) {
        suma += medicion.temperatura;
        suma += static_cast<float>(medicion.humedad);
        suma += static_cast<float>(medicion.humedad_suelo);
        suma += medicion.velocidad_viento;
        suma += medicion.direccion_viento;
        suma += medicion.precipitacion;
        suma += static_cast<float>(medicion.intensidad_luz);
        count += 7;
    }

    if (count == 0) {
        return 0.0;
    }

    return suma / count;
}

float calcularMaximo(const std::vector<DatosSensores>& mediciones) {
    float maximo = std::numeric_limits<float>::min();

    for (const auto& medicion : mediciones) {
        if (medicion.temperatura > maximo) {
            maximo = medicion.temperatura;
        }
        if (static_cast<float>(medicion.humedad) > maximo) {
            maximo = static_cast<float>(medicion.humedad);
        }
        if (static_cast<float>(medicion.humedad_suelo) > maximo) {
            maximo = static_cast<float>(medicion.humedad_suelo);
        }
        if (medicion.velocidad_viento > maximo) {
            maximo = medicion.velocidad_viento;
        }
        if (medicion.direccion_viento > maximo) {
            maximo = medicion.direccion_viento;
        }
        if (medicion.precipitacion > maximo) {
            maximo = medicion.precipitacion;
        }
        if (static_cast<float>(medicion.intensidad_luz) > maximo) {
            maximo = static_cast<float>(medicion.intensidad_luz);
        }
    }

    return maximo;
}