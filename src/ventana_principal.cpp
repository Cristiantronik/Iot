

#include "ventana_principal.h"
#include "ui_ventana_principal.h" // Asumiendo que tienes un archivo ventana_principal.ui

VentanaPrincipal::VentanaPrincipal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::VentanaPrincipal)
{
    ui->setupUi(this);

    // Inicialización de la base de datos
    db = new BaseDeDatos("datos_sensores.db");

    // Configuración de la interfaz gráfica
    // ...
}

VentanaPrincipal::~VentanaPrincipal()
{
    delete db;
    delete ui;
}