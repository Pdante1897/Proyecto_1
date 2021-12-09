#include "fdisk.h"
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
    PATH=5,
    ADD=8,
    TYPE=9,
    DELETE=10,
    NAME=11
};

bool validarFDk(Node *Raiz){
    bool banderaFDisk=true;
    bool banderaPath=true;
    bool banderaFit=true;
    bool banderaUnit=true;
    bool banderaSize=true;
    bool banderaAdd=true;
    bool banderaType=true;
    bool banderaName=true;
    bool banderaDelete=true;
    bool primerAdd=false;
    bool primerSize=false;
    bool primerDelete=false;
    int numNodos = Raiz->hijos.length();
    Disk disco;
    MBR mbr;
    int valorSize;
    int valorAdd;
    int tamanioFinal;
    char valorFit;
    char valorUnit;
    char valorType;
    QString valorName;
    QString valorPath;
    QString valorDel;
    for(int i=0; i<numNodos;++i)
    {
        Node nodito = Raiz->hijos.at(i);
        nodito.asignarTipo();
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
                printf("Error! Ya fue definido el parametro -FIT \n");
                return !banderaFDisk;

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
                }else if(unit == "b" || unit == "B"){
                    unit = 'b';
                    disco.unit=unit[0];
                }else{
                    printf("ERROR: Valor del parametro -UNIT desconocido ");
                    banderaFDisk = true;
                    return !banderaFDisk;
                }
            }else{
                printf("Error! Ya fue definido el parametro -UNIT \n");
                return !banderaFDisk;
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
                printf("Error! Ya fue definido el parametro -PATH \n");
                return !banderaFDisk;
            }
            break;
        }
        case SIZE:{
            if(!primerDelete and !primerAdd){primerSize=true;}
            if(banderaSize){
                if(nodito.valor.toInt()>0){
                    disco.size=nodito.valor.toInt();
                    banderaSize=false;
                }
                else{
                    printf("Error! El parametro -SIZE debe ser un numero mayor a 0 %i \n", disco.size);
                    return !banderaFDisk;
                }
            }else{
                banderaSize=true;
                printf("Error! Ya fue definido el parametro -SIZE \n");
                return !banderaFDisk;

            }
            break;
        }
        case ADD:{
            if(!primerDelete and !primerSize){primerAdd=true;}
            if(banderaAdd){
                banderaAdd=false;
                valorAdd=nodito.valor.toInt();
                break;
            }else{
                printf("Error! Ya fue definido el parametro -ADD \n");
                return !banderaFDisk;

            }
            break;
        }
        case TYPE:{
            if(banderaType){
                banderaType = false;
                string temporal = nodito.valor.toStdString();
                valorType = temporal[0];
                if(valorType == 'P' || valorType == 'p'){
                    valorType = 'P';
                }else if(valorType == 'E' || valorType == 'e'){
                    valorType = 'E';
                }else if(valorType == 'L' || valorType == 'l'){
                    valorType = 'L';
                }else{
                    printf("Error! El valor del parametro TYPE no fue reconocido \n");
                    banderaFDisk = true;
                    break;
                }
            }else{
                printf("Error! Ya fue definido el parametro -TYPE \n");
                return !banderaFDisk;
            }
            break;
        }
        case DELETE:{
            if(!primerAdd and !primerSize){primerDelete=true;}
            if(banderaDelete){
                banderaDelete=false;
                valorDel=nodito.valor;
            }else{
                printf("Error! Ya fue definido el parametro -DELETE \n");
                return !banderaFDisk;

            }
            break;
        }
        case NAME:{
            if(banderaName){
                valorName = nodito.valor;
                valorName = valorName.replace("\"", "");
                banderaName=false;
                break;
            }else{
                printf("Error! Ya fue definido el parametro -NAME \n");
                return !banderaFDisk;
            }
            break;
        }
        default:{
            break;
        }
    }

    }
    if(!banderaSize){printf("~~~>Size todo bien \n");}
    if(!banderaPath){printf("~~~>Path todo bien \n");}
    if(!banderaFit){ printf("~~~>Fit  todo bien \n");}
    else{disco.fit='F';}
    if(!banderaUnit){
        printf("~~~>Unit todo bien \n");
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

    return !banderaFDisk;

}

