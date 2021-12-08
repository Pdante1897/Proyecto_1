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
#include "Node.h"
#include "main.h"
#include "disk.h"

using namespace std;

enum choice{
    SIZE=2,
    FIT=3,
    UNIT=4,
    PATH=5
};
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
bool validarMkDisk(Node *Raiz){
    bool banderaDisk=true;
    bool banderaPath=true;
    bool banderaFit=true;
    bool banderaUnit=true;
    bool banderaSize=true;
    int numNodos = Raiz->hijos.length();
    Disk disco;
    for(int i=0; i<numNodos;++i)
    {
        Node nodito = Raiz->hijos.at(i);
        nodito.asignarTipo();
        printf("  Tipo  %i  Numero de hijos %i \n",nodito.tipo, numNodos);
        printf("  Tipo  %s  Numero de hijos %i \n",nodito.nombre.toStdString().c_str(), numNodos);

        switch (nodito.tipo){
        case FIT:{
            if(banderaFit){
                string fit=nodito.hijos.at(0).valor.toStdString();
                if(fit == "b" or fit == "B"){
                    fit = "B";
                }else if(fit == "f" or fit == "F"){
                    fit = "F";
                }else if(fit == "w" or fit == "W"){
                    fit = "W";
                }
                disco.fit=fit[0];
                printf("~~~>tipo de ajuste = %c\n",disco.fit);
                banderaFit=false;
            }else{
                banderaDisk=true;
                printf("Error! Ya fue definido el parametro -FIT \n");
                return !banderaDisk;

            }
            break;
        }
        case UNIT:{
            if(banderaUnit){
                string unit=nodito.valor.toStdString();
                banderaUnit=false;
                if(unit == "k"|| unit == "K"){
                    unit = "k";
                    disco.unit=unit[0];
                }else if(unit == "m" || unit == "M"){
                    unit = 'm';
                    disco.unit=unit[0];
                }else{
                    printf("ERROR: Valor del parametro -UNIT desconocido ");
                    banderaDisk = true;
                    return !banderaDisk;
                }
            }else{
                banderaDisk=true;
                printf("Error! Ya fue definido el parametro -UNIT \n");
                return !banderaDisk;
            }
            break;
        }
        case PATH:{
            if(banderaPath){
                disco.path=nodito.valor;
                banderaPath=false;
                break;
            }else{
                banderaPath=true;
                printf("Error! Ya fue definido el parametro -PATH \n");
            }
            break;
        }
        case SIZE:{
            if(banderaSize){
                if(nodito.valor.toInt()>0){
                    disco.size=nodito.valor.toInt();
                    banderaSize=false;
                }
                else{
                    banderaSize=true;
                    printf("Error! El parametro -SIZE debe ser un numero mayor a 0 %i \n", disco.size);
                }
            }else{
                banderaSize=true;
                printf("Error! Ya fue definido el parametro -SIZE \n");
            }
            break;
        }//mkdisk -size~:~10 -path~:~"hola mundo/lacarpeta"
        default:{
            break;
        }
    }

    }
    if(!banderaSize){printf("~~~>Size todo bien \n");}
    if(!banderaPath){printf("~~~>Path todo bien \n");}
    if(!banderaFit){ printf("~~~>Fit  todo bien \n");}
    if(!banderaUnit){printf("~~~>Unit todo bien \n");}
    if (!banderaPath and !banderaSize){banderaDisk=false;}
    return !banderaDisk;

}
