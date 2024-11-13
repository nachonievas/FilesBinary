#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
using namespace std;

struct clientes
{
    int numero;
    char nombre[50];
    char localidad[50];
    float saldo;
    char estado[10];
};

void clienteNuevo(clientes &c) {
    ofstream f("clientes.bin", ios::binary | ios::app);
    if (!f) {
    cout << "Error al abrir contenido.dat\n";
    exit(EXIT_FAILURE);
}
    cout << "Ingrese un numero: ";
    cin >> c.numero;

    cin.ignore();
    cout << "Ingrese un nombre: " << endl;
    cin.getline(c.nombre, 50);

    cout << "Ingrese una localidad: ";
    cin.getline(c.localidad, 50);

    cout << "Ingrese el saldo: ";
    cin >> c.saldo;

    strcpy(c.estado, "Activo");

    f.write(reinterpret_cast<char*>(&c), sizeof(clientes));    

    f.close();
};

void borrarCliente(clientes &c) {
    fstream fich("clientes.bin", ios::binary| ios::in | ios::out);
    if (!fich) {
        cout << "Error al abrir clientes.bin\n";
        exit(EXIT_FAILURE);
    }

    int borrar;
    cout<<"Ingrese el cliente que desea borrar ";
    cin>>borrar;
    
    fich.seekg(sizeof(clientes) * borrar, ios::beg);
    fich.read(reinterpret_cast<char*>(&c), sizeof(clientes));

    // Marca el cliente como inactivo
    strcpy(c.estado, "Inactivo");

    // Mueve el cursor de escritura al mismo lugar para sobrescribir el registro
    fich.seekp(sizeof(clientes) * borrar, ios::beg);
    fich.write(reinterpret_cast<char*>(&c), sizeof(clientes));

    cout<<"tu cliente fue borrado exitosamente "<<endl;
    fich.close();
};

void modificarDatos(clientes &c) {
    fstream fich("clientes.bin", ios::binary| ios::in | ios::out);
    if (!fich) {
        cout << "Error al abrir clientes.bin\n";
        exit(EXIT_FAILURE);
    }
    int numeroCliente;
    cout << "Ingrese el número del cliente que desea modificar: ";
    cin >> numeroCliente;

    bool encontrado = false;
    while (fich.read(reinterpret_cast<char*>(&c), sizeof(clientes))) {
        if (c.numero == numeroCliente && strcmp(c.estado, "Activo") == 0) {
            encontrado = true;
            fich.seekp(-static_cast<int>(sizeof(clientes)), ios::cur);

            string campo;
            cout << "Ingrese el campo que desee modificar (Numero, Nombre, Localidad, Saldo): ";
            cin >> campo;
            cin.ignore(); // Ignorar el salto de línea

            if (campo == "Numero") {
                cout << "Ingrese el nuevo número: ";
                cin >> c.numero;
            } else if (campo == "Nombre") {
                cout << "Ingrese el nuevo nombre: ";
                cin.getline(c.nombre, 50);
            } else if (campo == "Localidad") {
                cout << "Ingrese la nueva localidad: ";
                cin.getline(c.localidad, 50);
            } else if (campo == "Saldo") {
                cout << "Ingrese el nuevo saldo: ";
                cin >> c.saldo;
            } else {
                cout << "Campo no válido.\n";
                fich.close();
                return;
            }

            fich.write(reinterpret_cast<char*>(&c), sizeof(clientes));
            cout << "Los datos del cliente han sido actualizados exitosamente.\n";
            break;
        }
    }

    if (!encontrado) {
        cout << "Cliente no encontrado o ya está inactivo.\n";
    }

    fich.close();
}

void consultarDatos(clientes &c) {
    fstream fich("clientes.bin", ios::binary| ios::in);
    if (!fich) {
        cout << "Error al abrir clientes.bin\n";
        exit(EXIT_FAILURE);
    }
    int numeroCliente;
    cout << "Ingrese el número del cliente que desea consultar: ";
    cin >> numeroCliente;

    bool encontrado = false;
    while (fich.read(reinterpret_cast<char*>(&c), sizeof(clientes))) {
        if (c.numero == numeroCliente && strcmp(c.estado, "Activo") == 0) {
            encontrado = true;
            fich.seekp(-static_cast<int>(sizeof(clientes)), ios::cur);
            cout<<"Numero: "<< c.numero<< endl;
            cout<<"Nombre: "<< c.nombre<< endl;
            cout<<"Localidad: "<< c.localidad<< endl;
            cout<<"Saldo: "<< c.saldo<< endl;
            cout<<"Estado: "<< c.estado<< endl;
           break;
        }
    }

    if (!encontrado) {
        cout << "Cliente no encontrado o ya está inactivo.\n";
    }

    fich.close();
}

void listadoClientes(clientes &c) {
   fstream fich("clientes.bin", ios::binary | ios::in);
    if (!fich) {
        cout << "Error al abrir clientes.bin\n";
        exit(EXIT_FAILURE);
    }
    fich.seekg(0, ios::beg); // Posicionarse al inicio del archivo
    cout << "Lista de clientes:\n";
    while (fich.read(reinterpret_cast<char*>(&c), sizeof(clientes))) {
        if (strcmp(c.estado, "Activo") == 0) { // Solo mostrar clientes activos
            cout << "Número: " << c.numero << endl;
            cout << "Nombre: " << c.nombre << endl;
            cout << "Localidad: " << c.localidad << endl;
            cout << "Saldo: " << c.saldo << endl;
            cout << "Estado: " << c.estado << endl;
            cout << "----------------------\n";
        }
    }

    fich.close();
}
struct movimientos
{
    int numeroMovimiento;
    int numeroCliente;
    int montoPagado;

};

void pagosProducidos(movimientos &m, clientes &c) {
    ofstream f("archivo.txt", ios::app);
    if (!f) {
        cout << "Error al abrir clientes.bin\n";
        exit(EXIT_FAILURE);
    }
    cout << "Ingrese el número del movimiento: ";
    cin >> m.numeroMovimiento;

    cout << "Ingrese el número del cliente: ";
    cin >> m.numeroCliente;
    if (m.numeroCliente != c.numero)
    {
        cout<<"Este cliente es inexistente "<< endl;
    } else {
        cout << "Ingrese el monto pagado: ";
        cin >> m.montoPagado;
    }
    
  

    // Registra el movimiento en el archivo de texto
    f << "Movimiento N°: " << m.numeroMovimiento << ", "
                    << "Cliente N°: " << m.numeroCliente << ", "
                    << "Monto Pagado: " << m.montoPagado << endl;

    f.close();
 
    // Actualiza el saldo en el archivo binario de clientes
    fstream clientesFile("clientes.bin", ios::binary | ios::in | ios::out);
    if (!clientesFile) {
        cout << "Error al abrir clientes.bin\n";
        exit(EXIT_FAILURE);
    }

    bool encontrado = false;
    while (clientesFile.read(reinterpret_cast<char*>(&c), sizeof(clientes))) {
        if (c.numero == m.numeroCliente && strcmp(c.estado, "Activo") == 0) {
            encontrado = true;
            c.saldo -= m.montoPagado; // Actualiza el saldo restando el monto pagado
            clientesFile.seekp(-static_cast<int>(sizeof(clientes)), ios::cur);
            clientesFile.write(reinterpret_cast<char*>(&c), sizeof(clientes));
            cout << "El saldo del cliente ha sido actualizado exitosamente.\n";
            break;
        }
    }

    if (!encontrado) {
        cout << "Cliente no encontrado o ya está inactivo.\n";
    }

    clientesFile.close();
}


int main() {
clientes c;
movimientos m;
int opcion = 1;
while (opcion != 0) {
    cout<<"---------------------------------------------------"<<endl;
    cout<<"Desea Agregar un nuevo cliente presione 1" <<endl;
    cout<<"Desea Borrar un cliente presione 2" << endl;
    cout<<"Desea Modificar un cliente presione 3" << endl;
    cout<<"Desea Consultar un cliente presione 4"<< endl;
    cout<<"Desea mostrar una lista de todos los clientes presione 5" << endl;
    cout<<"Desea mostrar los movimientos presione 6" << endl;
    cout<<"Presione 0 si desea terminar" << endl;
    cout<<"---------------------------------------------------"<<endl;
    cin>>opcion;
    switch (opcion)
    {
        case 1:
            clienteNuevo(c);
            break;

        case 2: 
            borrarCliente(c);
            break;

        case 3:
            modificarDatos(c);
            break;
        case 4:
            consultarDatos(c);
            break;
        case 5:
            listadoClientes(c);
            break;
        case 6:
            pagosProducidos(m, c);
            break;
        default: cout<<"Termino tu programa";
            break; 
    }   
    
    

}




}

