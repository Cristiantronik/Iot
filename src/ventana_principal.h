#pragma once

#include <QMainWindow>
#include "base_de_datos.h"

class VentanaPrincipal : public QMainWindow {
    Q_OBJECT

public:
    VentanaPrincipal(QWidget *parent = nullptr);
    ~VentanaPrincipal();

private:
    BaseDeDatos* db;

    // Otros miembros y métodos de la clase VentanaPrincipal
};