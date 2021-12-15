
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
#include "main.h"
#include "clases.h"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
using namespace std;
extern QList<ParticionMount> partMontadas;
extern QList<QString> pathsMontados;
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
    int numNodos = Raiz->hijitos.length();
    MBR mbr;
    int valorSize=0;
    int valorAdd=0;
    int tamanioFinal=0;
    char valorFit=' ';
    char valorUnit=' ';
    char valorType=' ';
    QString valorName;
    QString valorPath;
    QString valorDel;
    for(int i=0; i<numNodos;++i)
    {
        Node nodito = Raiz->hijitos.at(i);
        nodito.asignarTipo();
        switch (nodito.tipo){
        case FIT:{
            if(banderaFit){
                QString fit=nodito.hijitos.at(0).valor.toStdString().c_str();
                if(fit.at(0) == "b" or fit.at(0) == "B"){
                    fit = "B";
                }else if(fit.at(0) == "f" or fit.at(0) == "F"){
                    fit = "F";
                }else if(fit.at(0) == "w" or fit.at(0) == "W"){
                    fit = "W";
                }
                valorFit=fit.toStdString().at(0);
                printf(ANSI_COLOR_RED"~~~>tipo de ajuste = %s\n" ANSI_COLOR_RESET,fit.toStdString().c_str());
                printf(ANSI_COLOR_RED"~~~>tipo de ajuste = %c\n" ANSI_COLOR_RESET,valorFit);
                banderaFit=false;
            }else{
                printf(ANSI_COLOR_CYAN "Error! Ya fue definido el parametro -FIT \n" ANSI_COLOR_CYAN);
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
                    valorUnit=unit[0];
                }else if(unit == "m" || unit == "M"){
                    unit = 'm';
                    valorUnit=unit[0];
                }else if(unit == "b" || unit == "B"){
                    unit = 'b';
                    valorUnit=unit[0];
                }else{
                    printf(ANSI_COLOR_CYAN "ERROR: Valor del parametro -UNIT desconocido " ANSI_COLOR_RESET);
                    banderaFDisk = true;
                    return !banderaFDisk;
                }
            }else{
                printf(ANSI_COLOR_CYAN "Error! Ya fue definido el parametro -UNIT \n" ANSI_COLOR_RESET);
                return !banderaFDisk;
            }
            break;
        }
        case PATH:{
            if(banderaPath){
                valorPath=nodito.valor;
                valorPath=valorPath.replace("\"","");
                banderaPath=false;
                break;
            }else{
                printf(ANSI_COLOR_CYAN "Error! Ya fue definido el parametro -PATH \n" ANSI_COLOR_RESET);
                return !banderaFDisk;
            }
            break;
        }
        case SIZE:{
            if(!primerDelete and !primerAdd){primerSize=true;}
            if(banderaSize){
                if(nodito.valor.toInt()>0){
                    valorSize=nodito.valor.toInt();
                    banderaSize=false;
                }
                else{
                    printf(ANSI_COLOR_CYAN "Error! El parametro -SIZE debe ser un numero mayor a 0 \n" ANSI_COLOR_RESET);
                    return !banderaFDisk;
                }
            }else{
                banderaSize=true;
                printf(ANSI_COLOR_CYAN "Error! Ya fue definido el parametro -SIZE \n" ANSI_COLOR_RESET);
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
                printf(ANSI_COLOR_CYAN "Error! Ya fue definido el parametro -ADD \n" ANSI_COLOR_RESET);
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
                    printf(ANSI_COLOR_CYAN "Error! El valor del parametro TYPE no fue reconocido \n" ANSI_COLOR_RESET);
                    banderaFDisk = true;
                    break;
                }
            }else{
                printf(ANSI_COLOR_CYAN"Error! Ya fue definido el parametro -TYPE \n" ANSI_COLOR_RESET);
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
                printf(ANSI_COLOR_CYAN "Error! Ya fue definido el parametro -DELETE \n" ANSI_COLOR_RESET);
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
                printf(ANSI_COLOR_CYAN "Error! Ya fue definido el parametro -NAME \n" ANSI_COLOR_RESET);
                return !banderaFDisk;
            }
            break;
        }
        default:{
            break;
        }
    }

    }
    if(!banderaAdd){printf(ANSI_COLOR_RED"~~~>Add todo bien \n");}
    if(!banderaDelete){printf(ANSI_COLOR_RED"~~~>Delete todo bien \n");}
    if(!banderaType){ printf(ANSI_COLOR_RED"~~~>Type  todo bien \n");}
    if(!banderaName){ printf(ANSI_COLOR_RED"~~~>Name  todo bien \n");}
    if(!banderaSize){printf(ANSI_COLOR_RED"~~~>Size todo bien \n");}
    if(!banderaPath){printf(ANSI_COLOR_RED"~~~>Path todo bien \n");}
    if(!banderaFit){ printf(ANSI_COLOR_RED"~~~>Fit  todo bien \n");}
    else{valorFit='W';}
    if(!banderaUnit){
        printf(ANSI_COLOR_RED"~~~>Unit todo bien \n");
        if(valorUnit == 'm'){
            valorSize= valorSize*1048576;
            tamanioFinal = valorSize;
        }else if(valorUnit=='b'){
            tamanioFinal=valorSize;
        }
        else{
            valorSize=valorSize  * 1024;
            tamanioFinal= valorSize;
        }
    }
    else{;
         tamanioFinal = valorSize * 1024;
    }

    if(primerSize){
        if (!banderaType){
            if(valorType=='P'){
                crearPartPri(valorPath, valorName, valorSize, valorFit);
            }else if(valorType=='E'){
                crearPartExt(valorPath, valorName, valorSize, valorFit);
            }else if(valorType=='L'){
                crearLogica(valorPath, valorName, valorSize, valorFit);
            }

        }else{
            crearPartPri(valorPath, valorName, valorSize, valorFit);
        }
    }
    if(primerAdd){
        bool montado = false;
        for(int i=0; i<partMontadas.length(); i++){
            if(partMontadas.at(i).name==valorName and partMontadas.at(i).dir==valorPath){
            montado=true;
            }
        }
        if(!montado){addParticion(valorPath,valorName, valorAdd, valorUnit);}
        else{printf(ANSI_COLOR_CYAN"Error! La particion esta montada! Debe desmontarla antes de usar el comando -ADD \n");}



    }
    if(primerDelete){
        bool montado = false;
        for(int i=0; i<partMontadas.length(); i++){
            if(partMontadas.at(i).name==valorName and partMontadas.at(i).dir==valorPath){
            montado=true;
        }

    } if(!montado){
            delParticion(valorPath, valorName, valorDel);
        }
        else{printf(ANSI_COLOR_CYAN"Error! La particion esta montada! Debe desmontarla antes de usar el comando -DELETE \n");}

    }
    return !banderaFDisk;

}

void addParticion(QString path, QString name, int valorAdd, char valorUnit){
    int sizeBytes=0;
    int indicePart;
    int indiceExt;
    bool part=false;
    bool extendida=false;
    bool operacion=true;//true agregar, false quitar
    QString type;
    MBR mbr;
    if(valorAdd>0){
       operacion=true;
    }else{
       operacion=false;
    }
    FILE *archivo;
    if((archivo = fopen(path.toStdString().c_str(), "rb+"))){
        bool banderaExtend = false;
        fseek(archivo,0,SEEK_SET);
        fread(&mbr,sizeof(MBR),1,archivo);

        for(int i = 0; i < 4; i++){
            if((strcmp(mbr.mbr_partitions[i].part_name, name.toStdString().c_str()) == 0)){
                indicePart = i;
                part=true;
                if(mbr.mbr_partitions[i].part_type == 'E')
                    extendida = true;
                break;
            }else if(mbr.mbr_partitions[i].part_type == 'E'){
                indiceExt = i;
                extendida=true;
            }
        }
        bool ultimaPart=false;
        if(indicePart==3){ultimaPart=true;}
        if(part){
            if(!extendida){//es primaria
                if(operacion){//sumar
                    int bytePartAc;
                    int bytePartAcF;
                    int bytePartSig;
                    int fragmento;
                    bytePartAc=mbr.mbr_partitions[indicePart].part_start;
                    bytePartAcF=bytePartAc+mbr.mbr_partitions[indicePart].part_size;
                    if(!ultimaPart){
                        bytePartSig=mbr.mbr_partitions[indicePart+1].part_start;
                    }else{
                        bytePartSig=bytePartAcF+ mbr.mbr_tamanio + (int)sizeof(MBR);

                    }
                    fragmento=bytePartSig-bytePartAcF;
                    if(fragmento >0){
                        if(fragmento>=valorAdd){
                            mbr.mbr_partitions[indicePart].part_size=mbr.mbr_partitions[indicePart].part_size+valorAdd;
                            fseek(archivo,0,SEEK_SET);
                            fwrite(&mbr,sizeof(MBR),1,archivo);
                            printf(ANSI_COLOR_RED"~~~>Se agrego espacio a la particion exitosamente! \n");
                        }else{
                            printf(ANSI_COLOR_CYAN"Error: No es posible agregar espacio a la particion ya que no hay suficiente espacio despues de ella. \n");

                        }
                    }

                }else{//restar
                    if(valorAdd >= mbr.mbr_partitions[indicePart].part_size){
                        printf(ANSI_COLOR_CYAN"Error: No se puede eliminar una cantidad mayor de espacio de la que tiene asignada la particion! \n");
                    }else{
                        mbr.mbr_partitions[indicePart].part_size=mbr.mbr_partitions[indicePart].part_size-valorAdd;
                        fseek(archivo,0,SEEK_SET);
                        fwrite(&mbr,sizeof(MBR),1,archivo);
                        printf(ANSI_COLOR_RED"~~~>Se quito espacio a la particion exitosamente! \n");
                    }
                }
            }else{//extendida
                if(operacion){//sumar
                    int bytePartAc;
                    int bytePartAcF;
                    int bytePartSig;
                    int fragmento=0;
                    bytePartAc=mbr.mbr_partitions[indicePart].part_start;
                    bytePartAcF=bytePartAc+mbr.mbr_partitions[indicePart].part_size;
                    if(!ultimaPart){
                        bytePartSig=mbr.mbr_partitions[indicePart+1].part_start;
                    }else{
                        bytePartSig=bytePartAcF+ mbr.mbr_tamanio + (int)sizeof(MBR);

                    }
                    fragmento=bytePartSig-bytePartAcF;
                    if(fragmento >0){
                        if(fragmento>=valorAdd){
                            mbr.mbr_partitions[indicePart].part_size=mbr.mbr_partitions[indicePart].part_size+valorAdd;
                            fseek(archivo,0,SEEK_SET);
                            fwrite(&mbr,sizeof(MBR),1,archivo);
                            printf(ANSI_COLOR_RED"~~~>Se agrego espacio a la particion exitosamente! \n");
                        }else{
                            printf(ANSI_COLOR_CYAN"Error: No es posible agregar espacio a la particion ya que no hay suficiente espacio despues de ella. \n");

                        }
                    }

                }else{//restar
                    if(valorAdd >= mbr.mbr_partitions[indicePart].part_size){
                        EBR ebr;
                        fseek(archivo, mbr.mbr_partitions[indicePart].part_start,SEEK_SET);
                        fread(&ebr,sizeof(EBR),1,archivo);
                        while((ebr.part_next != -1) && (ftell(archivo) < (mbr.mbr_partitions[indicePart].part_size + mbr.mbr_partitions[indicePart].part_start))){
                            fseek(archivo,ebr.part_next,SEEK_SET);
                            fread(&ebr,sizeof(EBR),1,archivo);
                        }
                        int ultLogica= ebr.part_start+ebr.part_size;
                        int byteCalc = mbr.mbr_partitions[indicePart].part_start+mbr.mbr_partitions[indicePart].part_size+valorAdd;
                        if(byteCalc > ultLogica){
                            mbr.mbr_partitions[indicePart].part_size= mbr.mbr_partitions[indicePart].part_size + valorAdd;
                            fseek(archivo,0,SEEK_SET);
                            fwrite(&mbr,sizeof(MBR),1, archivo);
                            printf(ANSI_COLOR_RED"~~~>Se quito espacio a la particion exitosamente! \n");
                        }
                    }else{
                        printf(ANSI_COLOR_CYAN"Error: No se puede eliminar una cantidad mayor de espacio de la que tiene asignada la particion! \n");
                    }
                }
            }
        }
        else{//logica
            if(extendida){
                if(operacion){//suma
                    EBR ebr;
                    fseek(archivo, mbr.mbr_partitions[indicePart].part_start,SEEK_SET);
                    fread(&ebr,sizeof(EBR),1,archivo);
                    while(ebr.part_name!=name and ebr.part_next != -1){
                        fseek(archivo,ebr.part_next,SEEK_SET);
                        fread(&ebr,sizeof(EBR),1,archivo);
                    }
                    int finLogica=ebr.part_size+ebr.part_start;
                    int finExt = mbr.mbr_partitions[indiceExt].part_size+mbr.mbr_partitions[indiceExt].part_start;
                    int inSigLogica = ebr.part_next;
                    if(ebr.part_next==-1){//ultima
                        int fragmento = finExt-finLogica;
                        if(fragmento>0){
                            if(fragmento>valorAdd){
                                ebr.part_size=ebr.part_size+valorAdd;
                                fseek(archivo,ebr.part_start,SEEK_SET);
                                fwrite(&ebr,sizeof(EBR),1, archivo);
                                printf(ANSI_COLOR_RED"~~~>Se agrego espacio a la particion exitosamente! \n");
                            }else{
                                printf(ANSI_COLOR_CYAN"Error: No es posible agregar espacio a la particion ya que no hay suficiente espacio despues de ella. \n");

                            }
                        }else{
                            printf(ANSI_COLOR_CYAN"Error: No es posible agregar espacio a la particion ya que no hay suficiente espacio despues de ella. \n");

                        }
                    }else{//tiene una siguiente

                        int fragmento = inSigLogica-finLogica;
                        if(fragmento>0){

                            if(fragmento>valorAdd){
                                ebr.part_size=ebr.part_size+valorAdd;
                                fseek(archivo,ebr.part_start,SEEK_SET);
                                fwrite(&ebr,sizeof(EBR),1, archivo);
                                printf(ANSI_COLOR_RED"~~~>Se agrego espacio a la particion exitosamente! \n");
                            }else{
                                printf(ANSI_COLOR_CYAN"Error: No es posible agregar espacio a la particion ya que no hay suficiente espacio despues de ella. \n");

                            }
                        }else{
                            printf(ANSI_COLOR_CYAN"Error: No es posible agregar espacio a la particion ya que no hay suficiente espacio despues de ella. \n");

                        }
                    }
                }else{//resta
                    EBR ebr;
                    fseek(archivo, mbr.mbr_partitions[indicePart].part_start,SEEK_SET);
                    fread(&ebr,sizeof(EBR),1,archivo);
                    while(ebr.part_name!=name and ebr.part_next != -1){
                        fseek(archivo,ebr.part_next,SEEK_SET);
                        fread(&ebr,sizeof(EBR),1,archivo);
                    }
                    if(-1*valorAdd >= ebr.part_size){
                        printf(ANSI_COLOR_CYAN"Error: No se puede eliminar una cantidad mayor de espacio de la que tiene asignada la particion! \n");
                    }else{
                        ebr.part_size = ebr.part_size + valorAdd;
                        fseek(archivo,ebr.part_start,SEEK_SET);
                        fwrite(&ebr,sizeof(EBR),1,archivo);
                        printf(ANSI_COLOR_RED"~~~>Se quito espacio a la particion exitosamente! \n");
                    }
                }

            }else{
                printf(ANSI_COLOR_CYAN"Error: No se puede encontrar la Particion Extendida que contiene la Particion Logica deseada! \n");

            }
        }
        fclose(archivo);
    }else{
        printf(ANSI_COLOR_CYAN"ERROR el disco donde se desea agregar/quitar unidades no existe \n");
    }
}
void delParticion(QString direccion, QString name, QString valorType){
    FILE *archivo;
    if((archivo = fopen(direccion.toStdString().c_str(), "rb+"))){
        MBR mbr;
        fseek(archivo,0,SEEK_SET);
        fread(&mbr,sizeof (MBR),1,archivo);
        char ceros = '\0';
        bool part=false;
        bool partExtendida=false;
        bool partLogica=false;
        bool banderaExtend=true;
        int indicePart=0;
        int indiceExt=0;
        bool bandExt = false;

        string letra = "";

        for(int i = 0; i < 4; i++){
            if((strcmp(mbr.mbr_partitions[i].part_name, name.toStdString().c_str()) == 0)){
                indicePart = i;
                part=true;
                if(mbr.mbr_partitions[i].part_type == 'E')
                    banderaExtend = true;
                break;
            }else if(mbr.mbr_partitions[i].part_type == 'E'){
                indiceExt = i;
                partExtendida=true;
            }
        }
        printf(ANSI_COLOR_RED"~~~>¿Esta seguro que desea eliminar la particion? Y/N : ");
        getline(cin, letra);
        if(letra.compare("Y") == 0 || letra.compare("y") == 0){
            if(part){//principal
                if(!banderaExtend){//primaria
                    if(valorType == "full"){
                        mbr.mbr_partitions[indicePart].part_status = '1';
                        strcpy(mbr.mbr_partitions[indicePart].part_name,"");
                        fseek(archivo,0,SEEK_SET);
                        fwrite(&mbr,sizeof(MBR),1,archivo);
                        fseek(archivo,mbr.mbr_partitions[indicePart].part_start,SEEK_SET);
                        fwrite(&ceros,1,mbr.mbr_partitions[indicePart].part_size,archivo);
                    }else{//fast
                        mbr.mbr_partitions[indicePart].part_status = '1';
                        strcpy(mbr.mbr_partitions[indicePart].part_name,"");
                        fseek(archivo,0,SEEK_SET);
                        fwrite(&mbr,sizeof(MBR),1,archivo);
                    }
                    printf(ANSI_COLOR_RED"~~~>La particion primaria fue eliminada con exito \n");

                }else{//ext
                    if(valorType == "full"){
                        mbr.mbr_partitions[indicePart].part_status = '1';
                        mbr.mbr_partitions[indicePart].part_type = ' ';
                        strcpy(mbr.mbr_partitions[indicePart].part_name,"");
                        fseek(archivo,0,SEEK_SET);
                        fwrite(&mbr,sizeof(MBR),1,archivo);
                        fseek(archivo,mbr.mbr_partitions[indicePart].part_start,SEEK_SET);
                        fwrite(&ceros,1,mbr.mbr_partitions[indicePart].part_size,archivo);
                    }else{//full
                        mbr.mbr_partitions[indicePart].part_status = '1';
                        mbr.mbr_partitions[indicePart].part_type = ' ';
                        strcpy(mbr.mbr_partitions[indicePart].part_name,"");
                        fseek(archivo,0,SEEK_SET);
                        fwrite(&mbr,sizeof(MBR),1,archivo);

                    }
                    printf(ANSI_COLOR_RED"~~~>La particion extendida fue eliminada con exito \n");

                }
            }else{//
                if(partExtendida){
                    EBR erb;
                    fseek(archivo,mbr.mbr_partitions[indiceExt].part_start, SEEK_SET);
                    fread(&erb,sizeof(EBR),1,archivo);
                    if(erb.part_size!=0){
                        fseek(archivo, mbr.mbr_partitions[indiceExt].part_start,SEEK_SET);
                        while((fread(&erb,sizeof(EBR),1,archivo))!=0 && (ftell(archivo) < (mbr.mbr_partitions[indiceExt].part_start + mbr.mbr_partitions[indiceExt].part_size))) {
                            if(strcmp(erb.part_name,name.toStdString().c_str()) == 0 && erb.part_status != '1'){//Se encontro
                                partLogica = true;
                                break;
                            }else if(erb.part_next == -1){//No se encontro
                                break;
                            }
                        }
                    }
                    if(partLogica){
                        if(valorType == "full"){//full
                            erb.part_status = '1';
                            strcpy(erb.part_name, "");
                            fseek(archivo, ftell(archivo)-sizeof(EBR),SEEK_SET);
                            fwrite(&erb,sizeof(EBR),1,archivo);
                            fwrite(&ceros,1,erb.part_size,archivo);
                        }else{//fast

                            erb.part_status = '1';
                            strcpy(erb.part_name, "");
                            fseek(archivo, ftell(archivo)-sizeof(EBR),SEEK_SET);
                            fwrite(&erb,sizeof(EBR),1,archivo);
                        }
                        printf(ANSI_COLOR_RED"~~~>La particion logica fue eliminada con exito \n");

                    }else{
                        printf(ANSI_COLOR_CYAN"ERROR: no se encuentra la particion que desea eliminar \n");

                    }
                }else{
                    printf(ANSI_COLOR_CYAN"ERROR: no se encuentra la particion que desea eliminar \n");
                }
            }
        }else if(letra.compare("N") || letra.compare("n") == 0){
            printf(ANSI_COLOR_RED"~~Operacion Cancelada \n");
        }else{
            printf(ANSI_COLOR_RED"~~Letra ingresada incorrecta \n");
        }

        fclose(archivo);
    }else{
        printf(ANSI_COLOR_CYAN"ERROR: el disco que contiene la particion especificada no existe \n");
    }
}
void crearPartPri(QString direccion, QString name, int size, char fit){
    char auxF= fit;
    int sizeBytes = size;
    char buff = '1';
    bool existe=false;
    bool partext = false;
    bool archivoExist=false;
    int ext=0;
    FILE *archivo;
    if((archivo = fopen(direccion.toStdString().c_str(),"rb+"))){
        archivoExist=true;
        MBR mbr;
        fseek(archivo,0,SEEK_SET);
        fread(&mbr, sizeof(MBR),1,archivo);
        for(int i = 0; i < 4; i++){
            if(strcmp(mbr.mbr_partitions[i].part_name,name.toStdString().c_str()) == 0){
                fclose(archivo);
                existe=true;
            }else if(mbr.mbr_partitions[i].part_type == 'E'){
                ext = i;
                partext=true;
            }
        }
        if(partext){
            fseek(archivo, mbr.mbr_partitions[ext].part_start,SEEK_SET);
            EBR ebr;
            while((fread(&ebr,sizeof(EBR),1,archivo))!=0 && (ftell(archivo) < (mbr.mbr_partitions[ext].part_size + mbr.mbr_partitions[ext].part_start))){
                if(ebr.part_next == -1){
                    existe= false;
                }
                if(strcmp(ebr.part_name,name.toStdString().c_str()) == 0){
                    existe= true;
                }
            }
        }
    }

    if(fit != 0)
        auxF = fit;
    else
        auxF = 'W';


    MBR mbr;
    if(archivo = fopen(direccion.toStdString().c_str(),"rb+")){
        int numPart = 0;
        bool flagPart = false;
        fseek(archivo,0,SEEK_SET);
        fread(&mbr,sizeof(MBR),1,archivo);
        for(int i = 0; i < 4; i++){
            if(mbr.mbr_partitions[i].part_start == -1 || (mbr.mbr_partitions[i].part_status == '1' && mbr.mbr_partitions[i].part_size>=sizeBytes)){
                flagPart = true;
                numPart = i;
                break;
            }
        }
        if(flagPart){
            int utilizado = 0;
            for(int i = 0; i < 4; i++){
                if(mbr.mbr_partitions[i].part_status!='1'){
                    utilizado += mbr.mbr_partitions[i].part_size;
                }
            }
            printf(ANSI_COLOR_RED"~~~>Espacio necesario:  %i Bytes \n",sizeBytes);
            printf(ANSI_COLOR_RED"~~~>Espacio disponible:  %i Bytes \n",mbr.mbr_tamanio - utilizado);
            if((mbr.mbr_tamanio - utilizado) >= sizeBytes){
                if(!existe){
                    if(mbr.mbr_disk_fit == 'B'){
                        int bestF = numPart;
                        for(int i = 0; i < 4; i++){
                            if(mbr.mbr_partitions[i].part_start == -1 || (mbr.mbr_partitions[i].part_status == '1' && mbr.mbr_partitions[i].part_size>=sizeBytes)){
                                if(i != numPart){
                                    if(mbr.mbr_partitions[bestF].part_size > mbr.mbr_partitions[i].part_size){
                                        bestF = i;
                                        break;
                                        }
                                    }
                                }
                        }
                        mbr.mbr_partitions[bestF].part_type = 'P';
                        mbr.mbr_partitions[bestF].part_fit = auxF;
                        if(bestF == 0){
                            mbr.mbr_partitions[bestF].part_start = sizeof(mbr);
                        }else{
                            mbr.mbr_partitions[bestF].part_start = mbr.mbr_partitions[bestF-1].part_start + mbr.mbr_partitions[bestF-1].part_size;
                        }
                        mbr.mbr_partitions[bestF].part_size = sizeBytes;
                        mbr.mbr_partitions[bestF].part_status = '0';
                        strcpy(mbr.mbr_partitions[bestF].part_name,name.toStdString().c_str());
                    }else if(mbr.mbr_disk_fit == 'F'){
                        mbr.mbr_partitions[numPart].part_type = 'P';
                        mbr.mbr_partitions[numPart].part_fit = auxF;
                        if(numPart == 0){
                            mbr.mbr_partitions[numPart].part_start = sizeof(mbr);
                        }else{
                            mbr.mbr_partitions[numPart].part_start = mbr.mbr_partitions[numPart-1].part_start + mbr.mbr_partitions[numPart-1].part_size;
                        }
                        mbr.mbr_partitions[numPart].part_size = sizeBytes;
                        mbr.mbr_partitions[numPart].part_status = '0';
                        strcpy(mbr.mbr_partitions[numPart].part_name,name.toStdString().c_str());
                    }else if(mbr.mbr_disk_fit == 'W'){
                        int  worstIndex= numPart;
                        for(int i = 0; i < 4; i++){
                            if(mbr.mbr_partitions[i].part_start == -1 || (mbr.mbr_partitions[i].part_status == '1' && mbr.mbr_partitions[i].part_size>=sizeBytes)){
                                if(i != numPart){
                                    if(mbr.mbr_partitions[worstIndex].part_size < mbr.mbr_partitions[i].part_size){
                                        worstIndex = i;
                                        break;
                                    }
                                }
                            }
                        }
                        mbr.mbr_partitions[worstIndex].part_type = 'P';
                        mbr.mbr_partitions[worstIndex].part_fit = auxF;
                        if(worstIndex == 0){
                            mbr.mbr_partitions[worstIndex].part_start = sizeof(mbr);
                        }else{
                            mbr.mbr_partitions[worstIndex].part_start = mbr.mbr_partitions[worstIndex-1].part_start + mbr.mbr_partitions[worstIndex-1].part_size;
                        }
                        mbr.mbr_partitions[worstIndex].part_size = sizeBytes;
                        mbr.mbr_partitions[worstIndex].part_status = '0';
                        strcpy(mbr.mbr_partitions[worstIndex].part_name,name.toStdString().c_str());
                    }
                    fseek(archivo,0,SEEK_SET);
                    fwrite(&mbr,sizeof(MBR),1,archivo);
                    fseek(archivo,mbr.mbr_partitions[numPart].part_start,SEEK_SET);
                    for(int i = 0; i < sizeBytes; i++){
                        fwrite(&buff,1,1,archivo);
                    }

                    printf(ANSI_COLOR_RED"~~~>La particion primaria fue creada con exito \n");
                }else{
                    printf(ANSI_COLOR_CYAN"ERROR: Ya existe una particion con el mismo nombre \n");

                }

            }else{
                printf(ANSI_COLOR_CYAN"ERROR: La particion que desea crear excede el espacio libre \n");

            }
        }else{
            printf(ANSI_COLOR_CYAN"ERROR: Ya se llego al limite de particiones (4 particiones por Disco) \n");
            printf(ANSI_COLOR_CYAN"Para crear una nueva debera eliminar al menos una. \n");
        }
        fclose(archivo);
    }else{
        printf(ANSI_COLOR_CYAN"ERROR: no existe el Disco especificado \n");

    }

}
void crearPartExt(QString direccion, QString name, int size, char fit){
    char auxF= fit;
    int sizeBytes = size;
    char unos = '1';
    bool existe=false;
    bool partext = false;
    int ext;
    FILE *archivo;
    if((archivo = fopen(direccion.toStdString().c_str(),"rb+"))){
        MBR mbr;
        fseek(archivo,0,SEEK_SET);
        fread(&mbr, sizeof(MBR),1,archivo);
        for(int i = 0; i < 4; i++){
            if(strcmp(mbr.mbr_partitions[i].part_name,name.toStdString().c_str()) == 0){
                existe=true;
            }else if(mbr.mbr_partitions[i].part_type == 'E'){
                ext = i;
                partext=true;
            }
        }
        if(partext){
            fseek(archivo, mbr.mbr_partitions[ext].part_start,SEEK_SET);
            EBR ebr;
            while((fread(&ebr,sizeof(EBR),1,archivo))!=0 && (ftell(archivo) < (mbr.mbr_partitions[ext].part_size + mbr.mbr_partitions[ext].part_start))){
                if(ebr.part_next == -1){
                    fclose(archivo);
                    existe= false;
                }
                if(strcmp(ebr.part_name,name.toStdString().c_str()) == 0){
                    fclose(archivo);
                    existe= true;
                }
            }
        }
    }

    if(fit != 0)
        auxF = fit;
    else
        auxF = 'W';

    EBR ebr;
    MBR mbr;
    if((archivo = fopen(direccion.toStdString().c_str(), "rb+"))){
        int numPart = 0;
        bool banderaPart = false;
        fseek(archivo,0,SEEK_SET);
        fread(&mbr,sizeof(MBR),1,archivo);
        for(int i = 0; i < 4; i++){
            if (mbr.mbr_partitions[i].part_type == 'E'){
                partext = true;
                break;
            }
        }
        for(int i = 0; i < 4; i++){
            if(mbr.mbr_partitions[i].part_start == -1 || (mbr.mbr_partitions[i].part_status == '1' && mbr.mbr_partitions[i].part_size>=sizeBytes)){
                banderaPart = true;
                numPart = i;
                break;
            }
        }
        if(!partext){
            int utilizado = 0;
            for(int i = 0; i < 4; i++){
                if(mbr.mbr_partitions[i].part_status!='1'){
                    utilizado += mbr.mbr_partitions[i].part_size;
                }
            }
            printf(ANSI_COLOR_RED"~~~>Espacio necesario:  %i Bytes \n",sizeBytes);
            printf(ANSI_COLOR_RED"~~~>Espacio disponible:  %i Bytes \n",mbr.mbr_tamanio - utilizado);
            if((mbr.mbr_tamanio - utilizado) >= sizeBytes){
                if(!partext){
                    if(mbr.mbr_disk_fit == 'B'){
                        int bf = numPart;
                        for(int i = 0; i < 4; i++){
                            if(mbr.mbr_partitions[i].part_start == -1 || (mbr.mbr_partitions[i].part_status == '1' && mbr.mbr_partitions[i].part_size>=sizeBytes)){
                                if(i != numPart){
                                    if(mbr.mbr_partitions[bf].part_size > mbr.mbr_partitions[i].part_size){
                                        bf = i;
                                        break;
                                        }
                                    }
                                }
                        }
                        mbr.mbr_partitions[bf].part_type = 'E';
                        mbr.mbr_partitions[bf].part_fit = auxF;
                        if(bf == 0){
                            mbr.mbr_partitions[bf].part_start = sizeof(mbr);
                        }else{
                            mbr.mbr_partitions[bf].part_start = mbr.mbr_partitions[bf-1].part_start + mbr.mbr_partitions[bf-1].part_size;
                        }
                        mbr.mbr_partitions[bf].part_size = sizeBytes;
                        mbr.mbr_partitions[bf].part_status = '0';
                        strcpy(mbr.mbr_partitions[bf].part_name,name.toStdString().c_str());
                        fseek(archivo,0,SEEK_SET);
                        fwrite(&mbr,sizeof(MBR),1,archivo);
                        fseek(archivo, mbr.mbr_partitions[numPart].part_start,SEEK_SET);
                        ebr.part_fit = auxF;
                        ebr.part_status = '0';
                        ebr.part_start = mbr.mbr_partitions[numPart].part_start;
                        ebr.part_size = 0;
                        ebr.part_next = -1;
                        strcpy(ebr.part_name, "");
                    }else if(mbr.mbr_disk_fit == 'F'){
                        mbr.mbr_partitions[numPart].part_type = 'E';
                        mbr.mbr_partitions[numPart].part_fit = auxF;
                        if(numPart == 0){
                            mbr.mbr_partitions[numPart].part_start = sizeof(mbr);
                        }else{
                            mbr.mbr_partitions[numPart].part_start = mbr.mbr_partitions[numPart-1].part_start + mbr.mbr_partitions[numPart-1].part_size;
                        }
                        mbr.mbr_partitions[numPart].part_size = sizeBytes;
                        mbr.mbr_partitions[numPart].part_status = '0';
                        strcpy(mbr.mbr_partitions[numPart].part_name,name.toStdString().c_str());
                        fseek(archivo,0,SEEK_SET);
                        fwrite(&mbr,sizeof(MBR),1,archivo);
                        fseek(archivo, mbr.mbr_partitions[numPart].part_start,SEEK_SET);
                        ebr.part_fit = auxF;
                        ebr.part_status = '0';
                        ebr.part_start = mbr.mbr_partitions[numPart].part_start;
                        ebr.part_size = 0;
                        ebr.part_next = -1;
                        strcpy(ebr.part_name, "");
                    }else if(mbr.mbr_disk_fit == 'W'){
                        int  wInd= numPart;
                        for(int i = 0; i < 4; i++){
                            if(mbr.mbr_partitions[i].part_start == -1 || (mbr.mbr_partitions[i].part_status == '1' && mbr.mbr_partitions[i].part_size>=sizeBytes)){
                                if(i != numPart){
                                    if(mbr.mbr_partitions[wInd].part_size < mbr.mbr_partitions[i].part_size){
                                        wInd = i;
                                        break;
                                    }
                                }
                            }
                        }
                        mbr.mbr_partitions[wInd].part_type = 'E';
                        mbr.mbr_partitions[wInd].part_fit = auxF;
                        if(wInd == 0){
                            mbr.mbr_partitions[wInd].part_start = sizeof(mbr);
                        }else{
                            mbr.mbr_partitions[wInd].part_start = mbr.mbr_partitions[wInd-1].part_start + mbr.mbr_partitions[wInd-1].part_size;
                        }
                        mbr.mbr_partitions[wInd].part_size = sizeBytes;
                        mbr.mbr_partitions[wInd].part_status = '0';
                        strcpy(mbr.mbr_partitions[wInd].part_name,name.toStdString().c_str());
                        fseek(archivo,0,SEEK_SET);
                        fwrite(&mbr,sizeof(MBR),1,archivo);
                        fseek(archivo, mbr.mbr_partitions[numPart].part_start,SEEK_SET);
                        ebr.part_fit = auxF;
                        ebr.part_status = '0';
                        ebr.part_start = mbr.mbr_partitions[numPart].part_start;
                        ebr.part_size = 0;
                        ebr.part_next = -1;
                        strcpy(ebr.part_name, "");
                    }
                    fwrite(&ebr,sizeof(EBR),1,archivo);
                    for(int i = 0; i < (size - (int)sizeof(EBR)); i++){
                        fwrite(&unos,1,1,archivo);
                    }
                    printf(ANSI_COLOR_RED"~~~>La particion Extendida fue creada con exito \n");
                }else{
                    printf(ANSI_COLOR_CYAN"ERROR: Ya existe una particion con el mismo nombre o una particion Extendida \n");

                }

            }else{
                printf(ANSI_COLOR_CYAN"ERROR: La particion que desea crear excede el espacio libre \n");

            }
        }else{
            printf(ANSI_COLOR_CYAN"ERROR: Ya se llego al limite de particiones (4 particiones por Disco) \n");
            printf(ANSI_COLOR_CYAN"Para crear una nueva debera eliminar al menos una. \n");
        }
    fclose(archivo);
    }else{
        printf(ANSI_COLOR_CYAN"ERROR: no existe el Disco especificado \n");

    }
}

void crearLogica(QString direccion, QString name, int size, char fit){
    char auxF= fit;
    int sizeBytes = size;
    char unos = '1';
    bool existe=false;
    bool partext = false;
    int ext;
    FILE *archivo;
    if((archivo = fopen(direccion.toStdString().c_str(),"rb+"))){
        MBR mbr;
        fseek(archivo,0,SEEK_SET);
        fread(&mbr, sizeof(MBR),1,archivo);
        for(int i = 0; i < 4; i++){
            if(strcmp(mbr.mbr_partitions[i].part_name,name.toStdString().c_str()) == 0){
                fclose(archivo);
                existe=true;
            }else if(mbr.mbr_partitions[i].part_type == 'E'){
                ext = i;
                partext=true;
            }
        }
        if(partext){
            fseek(archivo, mbr.mbr_partitions[ext].part_start,SEEK_SET);
            EBR ebr;
            while((fread(&ebr,sizeof(EBR),1,archivo))!=0 && (ftell(archivo) < (mbr.mbr_partitions[ext].part_size + mbr.mbr_partitions[ext].part_start))){
                if(ebr.part_next == -1){
                    existe= false;
                }
                if(strcmp(ebr.part_name,name.toStdString().c_str()) == 0){
                    existe= true;
                }
            }
        }
    }

    if(fit != 0)
        auxF = fit;
    else
        auxF = 'W';

    EBR ebr;
    MBR mbr;
    if((archivo = fopen(direccion.toStdString().c_str(), "rb+"))){
        int numPart = 0;
        bool banderaPart = false;
        fseek(archivo,0,SEEK_SET);
        fread(&mbr,sizeof(MBR),1,archivo);
        for(int i = 0; i < 4; i++){
            if (mbr.mbr_partitions[i].part_type == 'E'){
                partext = true;
                numPart=i;
                break;
            }
        }

        if(!existe){
            int utilizado = 0;
            for(int i = 0; i < 4; i++){
                if(mbr.mbr_partitions[i].part_status!='1'){
                    utilizado += mbr.mbr_partitions[i].part_size;
                }
            }
            printf(ANSI_COLOR_RED"~~~>Espacio necesario:  %i Bytes \n",sizeBytes);
            //printf("~~~>Espacio disponible:  %i Bytes \n",mbr.mbr_tamanio - utilizado);
            if((mbr.mbr_tamanio - utilizado) >= sizeBytes){
                if(!existe){
                    int inicioEx=mbr.mbr_partitions[numPart].part_start;
                    fseek(archivo,inicioEx,SEEK_SET);
                    fread(&ebr,sizeof(EBR),1,archivo);
                    if(ebr.part_size=0){
                        if(mbr.mbr_partitions[numPart].part_size >= size){
                            ebr.part_status = '0';
                            ebr.part_fit = auxF;
                            ebr.part_start = ftell(archivo) - sizeof(EBR); //Para regresar al comienzo de la extendida
                            ebr.part_size = size;
                            ebr.part_next = -1;
                            strcpy(ebr.part_name, name.toStdString().c_str());
                            fseek(archivo, mbr.mbr_partitions[numPart].part_start ,SEEK_SET);
                            fwrite(&ebr,sizeof(EBR),1,archivo);
                            printf(ANSI_COLOR_RED"La particion logica fue creada con exito \n");
                        }else{
                            printf(ANSI_COLOR_CYAN"ERROR la particion logica a crear excede el espacio disponible de la particion extendida \n");
                        }
                    }else{

                        while((ebr.part_next != -1) && (ftell(archivo) < (mbr.mbr_partitions[numPart].part_size + mbr.mbr_partitions[numPart].part_start))){
                        fseek(archivo,ebr.part_next,SEEK_SET);
                        fread(&ebr,sizeof(EBR),1,archivo);
                        }
                        int espNece = ebr.part_start + ebr.part_size + size;
                        if(espNece <= (mbr.mbr_partitions[numPart].part_size + mbr.mbr_partitions[numPart].part_start)){
                            ebr.part_next = ebr.part_start + ebr.part_size;
                            fseek(archivo,ftell(archivo) - sizeof (EBR),SEEK_SET);
                            fwrite(&ebr, sizeof(EBR),1 ,archivo);
                            fseek(archivo,ebr.part_start + ebr.part_size, SEEK_SET);
                            ebr.part_status = 0;
                            ebr.part_fit = auxF;
                            ebr.part_start = ftell(archivo);
                            ebr.part_size = size;
                            ebr.part_next = -1;
                            strcpy(ebr.part_name,name.toStdString().c_str());
                            fwrite(&ebr,sizeof(EBR),1,archivo);
                            printf(ANSI_COLOR_RED"~~~>La particion logica fue creada con exito \n");
                        }

                    }
                }else{
                    printf(ANSI_COLOR_CYAN"ERROR: la particion logica que desea crear excede el \n");
                    printf(ANSI_COLOR_CYAN"espacio disponible de la particion extendida utilizada. \n");

                }

            }else{
                printf(ANSI_COLOR_CYAN"ERROR: Es necesaria una particion extendida para guardar la particion logica \n");

            }
        }else{
            printf(ANSI_COLOR_CYAN"ERROR: Ya existe una particion con el mismo nombre \n");
        }
    fclose(archivo);
    }else{
        printf(ANSI_COLOR_CYAN"ERROR: no existe el Disco especificado \n");

    }
}

int Letra(QString path){
    int letra=97;
    if (pathsMontados.contains(path)){
        for(int i=0; i<partMontadas.length();i++){
            if(partMontadas.at(i).dir==path){
                letra=partMontadas.at(i).letra;
                return letra;
            }
        }
    }
    for(int i=0; i<pathsMontados.length();i++){
        letra=i+97+1;
    }
    return letra;
}

int Numero(QString path, QString name){
    int num=1;
    for(int i=0; i<partMontadas.length();i++){
        if(partMontadas.at(i).dir==path){
            num=i+2;
            if(partMontadas.at(i).name==name){
                return num=-1;
            }
        }

    }
    return num;
}
bool validarMontaje(Node *Raiz){
    bool banderaMount = true;
    bool banderaPath = true;
    bool banderaName = true;
    QString valorPath="";
    QString valorName="";
    for(int i=0; i< Raiz->hijitos.length(); i++){
        Node nodito=Raiz->hijitos.at(i);
        nodito.asignarTipo();
        switch (nodito.tipo) {
        case PATH:{
            if(banderaPath){
                valorPath=nodito.valor;
                valorPath=valorPath.replace("\"","");
                banderaPath=false;
                break;
            }else{
                printf(ANSI_COLOR_CYAN"Error! Ya fue definido el parametro -PATH \n");
                return !banderaMount;
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
                printf(ANSI_COLOR_CYAN"Error! Ya fue definido el parametro -NAME \n");
                return !banderaMount;
            }
            break;
        }
        }
    }
    if(banderaMount){
        if(!banderaPath){//Parametro obligatorio
            if(!banderaName){//Parametro obligtaorio
                int indicePart = 0;
                bool existePartP=false;
                bool existePartE=false;
                bool existePartL=false;
                FILE *archivito;
                if((archivito = fopen(valorPath.toStdString().c_str(),"rb+"))){
                    MBR mbr;
                    fseek(archivito,0,SEEK_SET);
                    fread(&mbr,sizeof(MBR),1,archivito);
                    for(int i = 0; i < 4; i++){
                        if(mbr.mbr_partitions[i].part_status != '1'){
                            if(strcmp(mbr.mbr_partitions[i].part_name,valorName.toStdString().c_str()) == 0){
                                indicePart= i;
                                existePartP=true;
                            }
                        }
                    }

                }
                if(!existePartP and(archivito = fopen(valorPath.toStdString().c_str(),"rb+"))){
                    int ext=0;
                    MBR mbr;
                    EBR ebr;
                    fseek(archivito,0,SEEK_SET);
                    fread(&mbr,sizeof(MBR),1,archivito);
                    for(int i = 0; i < 4; i++){
                        if(mbr.mbr_partitions[i].part_type == 'E'){
                            indicePart = i;
                            ext=i;
                            existePartE= true;
                            break;
                        }
                    }
                    if(existePartE){
                        fseek(archivito, mbr.mbr_partitions[ext].part_start,SEEK_SET);
                        while(fread(&ebr,sizeof(EBR),1,archivito)!=0 && (ftell(archivito) < mbr.mbr_partitions[ext].part_start + mbr.mbr_partitions[ext].part_size)){
                            if(strcmp(ebr.part_name, valorName.toStdString().c_str()) == 0){
                                indicePart = (ftell(archivito) - sizeof(EBR));
                                existePartL=true;
                            }
                        }
                    }
                    fclose(archivito);
                }

                if(existePartP){
                    FILE *archivo;
                    if((archivo = fopen(valorPath.toStdString().c_str(),"rb+"))){
                        MBR mbr;
                        fseek(archivo, 0, SEEK_SET);
                        fread(&mbr, sizeof(MBR),1,archivo);
                        mbr.mbr_partitions[indicePart].part_status = '2';
                        fseek(archivo,0,SEEK_SET);
                        fwrite(&mbr,sizeof(MBR),1,archivo);
                        fclose(archivo);
                        int letra = Letra(valorPath);
                        int num = Numero(valorPath,valorName);
                        if(num == -1){
                            printf(ANSI_COLOR_CYAN"ERROR la particion ya esta montada \n");
                        }else{
                            char auxL = static_cast<char>(letra);
                            string id="vd";
                            id += auxL + to_string(num);
                            ParticionMount *noditoM= new ParticionMount(valorPath,valorName,auxL,num, id.c_str());
                            if(!pathsMontados.contains(valorPath)){
                                pathsMontados.append(valorPath);
                            }
                            partMontadas.append(*noditoM);

                            printf(ANSI_COLOR_RED"Particion montada con exito \n");
                            imprimirLista();
                        }
                    }else{
                        printf(ANSI_COLOR_CYAN"ERROR: No se encuentra el disco \n");

                    }
                }else{//Posiblemente logica
                    if(existePartL){
                        FILE *filep;
                        if((filep = fopen(valorPath.toStdString().c_str(), "rb+"))){
                            EBR extendedBoot;
                            fseek(filep, indicePart, SEEK_SET);
                            fread(&extendedBoot, sizeof(EBR),1,filep);
                            extendedBoot.part_status = '2';
                            fseek(filep,indicePart,SEEK_SET);
                            fwrite(&extendedBoot,sizeof(EBR),1, filep);
                            fclose(filep);

                            int letra = Letra(valorPath);
                            if(letra == -1){
                                printf(ANSI_COLOR_CYAN"ERROR: la particion ya esta montada \n");

                            }else{
                                int num = Numero(valorPath,valorName);
                                char auxL = static_cast<char>(letra);
                                string id="vd";
                                id += auxL + to_string(num);
                                ParticionMount *noditoM = new ParticionMount(valorPath, valorName, auxL, num, id.c_str());
                                partMontadas.append(*noditoM);
                                if(!pathsMontados.contains(valorPath)){
                                    pathsMontados.append(valorPath);
                                }
                                printf(ANSI_COLOR_RED"La Particion fue montada con exito \n");

                                imprimirLista();
                            }
                        }else{
                            printf(ANSI_COLOR_CYAN"ERROR: No se encuentra el disco \n");

                        }
                    }else{
                        printf(ANSI_COLOR_CYAN"ERROR: No se encuentra la particion a montar \n");

                    }
                }
            }else{
                printf(ANSI_COLOR_CYAN"ERROR: Parametro Name no definido \n");

            }
        }else{
            printf(ANSI_COLOR_CYAN"ERROR: Parametro PATH no definido \n");
        }
    }
}

void montarParticion(Node *Raiz){

}

void imprimirLista(){
    printf(ANSI_COLOR_MAGENTA"--------------------------------------------------------------------------------\n");
    printf("|                        Particiones montadas                                  |\n");
    printf("--------------------------------------------------------------------------------\n");
    printf("| Path                      | Nombre                        | ID               |\n");
    printf("--------------------------------------------------------------------------------\n");
    ParticionMount aux = *new ParticionMount("","",' ',0,"");
    for(int i=0; i<partMontadas.length();i++){
        aux=partMontadas.at(i);

        cout << "| "<< aux.dir.toStdString()<<"           " << aux.name.toStdString() << "             " <<aux.id.toStdString() << endl;
        cout << "--------------------------------------------------------------------------------" << endl;
    }
}



