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
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
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
        printf(ANSI_COLOR_CYAN "Error al crear el archivo Binario" ANSI_COLOR_RESET);
    string comand = "dd if=/dev/zero of=\""+direccion.toStdString()+"\" bs=1024 count="+to_string(tam);
    system(comand.c_str());
    archivo = fopen(direccion.toStdString().c_str(), "rb+");
    if (archivo==NULL){
        return false;
    }
    fseek(archivo,0,SEEK_SET);
    fwrite(&mbr,sizeof(MBR),1, archivo);
    fflush(archivo);
    fclose(archivo);
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
    MBR mbr;
    int tamanioFinal;
    for(int i=0; i<numNodos;++i)
    {
        Node nodito = Raiz->hijos.at(i);
        nodito.asignarTipo();
        switch (nodito.tipo){
        case FIT:{
            if(banderaFit){
                QString fit=nodito.hijos.at(0).valor.toStdString().c_str();
                if(fit.at(0) == "b" or fit.at(0) == "B"){
                    fit = "B";
                }else if(fit.at(0) == "f" or fit.at(0) == "F"){
                    fit = "F";
                }else if(fit.at(0) == "w" or fit.at(0) == "W"){
                    fit = "W";
                }
                disco.fit=fit.toStdString().at(0);
                printf(ANSI_COLOR_RED"~~~>tipo de ajuste = %c\n" ANSI_COLOR_RESET,disco.fit);
                banderaFit=false;
            }else{
                banderaDisk=true;
                printf(ANSI_COLOR_CYAN"Error! Ya fue definido el parametro -FIT \n" ANSI_COLOR_RESET);
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
                    printf(ANSI_COLOR_CYAN"ERROR: Valor del parametro -UNIT desconocido \n" ANSI_COLOR_RESET);
                    banderaDisk = true;
                    return !banderaDisk;
                }
            }else{
                banderaDisk=true;
                printf(ANSI_COLOR_CYAN"Error! Ya fue definido el parametro -UNIT \n" ANSI_COLOR_RESET);
                return !banderaDisk;
            }
            break;
        }
        case PATH:{
            if(banderaPath){
                disco.path=nodito.valor;
                disco.path=disco.path.replace("\"","");
                banderaPath=false;
                break;
            }else{
                banderaPath=true;
                printf(ANSI_COLOR_CYAN "Error! Ya fue definido el parametro -PATH \n" ANSI_COLOR_RESET);
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
                    printf(ANSI_COLOR_CYAN "Error! El parametro -SIZE debe ser un numero mayor a 0 %i \n" ANSI_COLOR_RESET, disco.size);
                }
            }else{
                banderaSize=true;
                printf(ANSI_COLOR_CYAN "Error! Ya fue definido el parametro -SIZE \n" ANSI_COLOR_RESET);
            }
            break;
        }//mkdisk -size~:~10 -path~:~/home/bryan/Escritorio/pruebadisco.disk
         //rmdisk -path~:~/home/bryan/Escritorio/pruebadisco.disk
        default:{
            break;
        }
    }

    }
    if(!banderaSize){printf(ANSI_COLOR_RED "~~~>Size todo bien \n" ANSI_COLOR_RESET);}
    if(!banderaPath){printf(ANSI_COLOR_RED "~~~>Path todo bien \n" ANSI_COLOR_RESET);}
    if(!banderaFit){ printf(ANSI_COLOR_RED "~~~>Fit  todo bien \n" ANSI_COLOR_RESET);}
    else{disco.fit='F';}
    if(!banderaUnit){
        printf(ANSI_COLOR_RED"~~~>Unit todo bien \n" ANSI_COLOR_RESET);
        if(disco.unit == 'm'){
            mbr.mbr_tamanio = disco.size*1048576;
            tamanioFinal = disco.size * 1024;
        }else{
            mbr.mbr_tamanio = disco.size * 1024;
            tamanioFinal= disco.size;
        }
    }
    else{
         mbr.mbr_tamanio = disco.size*1048576;
         tamanioFinal = disco.size * 1024;
    }
    if (!banderaPath and !banderaSize){
        banderaDisk=false;
        mbr.mbr_fecha_creacion = time(nullptr);
        mbr.mbr_disk_signature = static_cast<int>(time(nullptr));
        mbr.mbr_disk_fit=disco.fit;
        for(int j=0; j<4;++j){
            mbr.mbr_partitions[j].part_status = '0';
            mbr.mbr_partitions[j].part_type = '0';
            mbr.mbr_partitions[j].part_fit = '0';
            mbr.mbr_partitions[j].part_size = 0;
            mbr.mbr_partitions[j].part_start = -1;
            strcpy(mbr.mbr_partitions[j].part_name,"");
        }
        verificarRuta(disco.path);
        makeDisk(disco.path,tamanioFinal,mbr);
        printf(ANSI_COLOR_RED "Disco %i Creado con Exito! \n" ANSI_COLOR_RESET,mbr.mbr_disk_signature);
    }
    return !banderaDisk;

}

void ejecutarRMD(Node *Raiz){
    QString direccion = Raiz->hijos.at(0).valor;
    direccion = direccion.replace("\"","");
    FILE *archivo;
    if((archivo=fopen(direccion.toStdString().c_str(),"r"))){
        string letra = "";
        printf(ANSI_COLOR_RED"~~~>¿Esta seguro que desea eliminar el disco? Y/N : \n" ANSI_COLOR_RESET );
        printf("~");
        getline(cin,letra);
        if(letra.compare("Y") == 0 || letra.compare("y") == 0){
            string comando = "rm \""+direccion.toStdString()+"\"";
            system(comando.c_str());
            printf(ANSI_COLOR_RED"El disco  fue eliminado con exito! \n \n" ANSI_COLOR_RESET);
        }else if(letra.compare("N") || letra.compare("n") == 0){
            printf(ANSI_COLOR_RED"Se ha cancelado la accion \n" ANSI_COLOR_RESET);
        }else{
            printf(ANSI_COLOR_CYAN"Letra ingresada incorrecta \n" ANSI_COLOR_RESET);
        }
        fclose(archivo);
    }else{
        printf(ANSI_COLOR_CYAN "Error: No existe el archivo que desea eliminar \n" ANSI_COLOR_RESET);
    }
}
