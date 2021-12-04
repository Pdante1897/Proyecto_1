#include <iostream>
#include <QString>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <QTextStream>
#include <stdlib.h>
#include "estructuras.h"
#include <QCoreApplication>
#include <stdio.h>

using namespace std;

void verificarRuta(QString direccion){
    string direccionAux = direccion.toStdString();
    string delimitador = "/";
    size_t pos = 0;
    string res = "";
    while((pos = direccionAux.find(delimitador))!=string::npos){
        res += direccionAux.substr(0,pos)+"/";
        direccionAux.erase(0,pos + delimitador.length());
    }
    QString aux2 = QString::fromStdString(res);
    string comando = "sudo mkdir -p \'"+aux2.toStdString()+"\'";
    system(comando.c_str());
    string comando2 = "sudo chmod -R 777 \'"+aux2.toStdString()+"\'";
    system(comando2.c_str());

}

//para crear los archivos binarios
bool makeDisk(QString direccion, int tam , MBR mbr){
    string dir = direccion.toStdString();
    FILE *archivo = fopen(dir.c_str(),"wb");
    if((archivo = fopen(dir.c_str(),"wb")))
        fclose(archivo);
    else
        cout << "Error al crear el archivo Binario" << endl;
    string comand = "dd if=/dev/zero of=\""+direccion.toStdString()+"\" bs=1024 count="+to_string(tam);
    system(comand.c_str());
    archivo = fopen(direccion.toStdString().c_str(), "rb+");
    if (archivo==NULL){
        printf("no jalo");
        return false;
    }
    fseek(archivo,0,SEEK_SET);
    fwrite(&mbr,sizeof(MBR),1, archivo);
    fflush(archivo);
    fclose(archivo);
    printf("si jalo");
    return true;
}
//para validar si los parametros son correctos y no se repiten
void validarMkDisk(){
    bool bandera;
    bool banderaFit;
    bool banderaUnit;
    bool banderaSize;
}
