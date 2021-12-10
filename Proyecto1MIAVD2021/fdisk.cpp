
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

using namespace std;
extern QList<ParticionMount> partMontadas;

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
                valorFit=fit[0];
                printf("~~~>tipo de ajuste = %c\n",valorFit);
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
                    valorUnit=unit[0];
                }else if(unit == "m" || unit == "M"){
                    unit = 'm';
                    valorUnit=unit[0];
                }else if(unit == "b" || unit == "B"){
                    unit = 'b';
                    valorUnit=unit[0];
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
                valorPath=nodito.valor;
                valorPath=valorPath.replace("\"","");
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
                    valorSize=nodito.valor.toInt();
                    banderaSize=false;
                }
                else{
                    printf("Error! El parametro -SIZE debe ser un numero mayor a 0 \n");
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
    else{valorFit='W';}
    if(!banderaUnit){
        printf("~~~>Unit todo bien \n");
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
                crearPartPri(valorPath, valorName, valorSize, valorUnit, valorFit);
            }else if(valorType=='E'){
                //metodo particion extendida
            }else if(valorType=='L'){
                //metodo particion logica
            }

        }else{
            crearPartPri(valorPath, valorName, valorSize, valorUnit, valorFit);
        }
    }
    if(primerAdd){
        bool montado = false;
        for(int i=0; i<partMontadas.length(); i++){
            if(partMontadas.at(i).name==valorName and partMontadas.at(i).dir==valorPath){
            montado=true;
        }
            if(montado){addParticion("","", 0, 0);}
            else{printf("Error! La particion esta montada! Debe desmontarla antes de usar el comando -ADD \n");}

        }
    if(primerDelete){
        bool montado = false;
        for(int i=0; i<partMontadas.length(); i++){
            if(partMontadas.at(i).name==valorName and partMontadas.at(i).dir==valorPath){
            montado=true;
        }
            if(montado){delParticion();}
            else{printf("Error! La particion esta montada! Debe desmontarla antes de usar el comando -DELETE \n");}

        }
    }
    return !banderaFDisk;

    }
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
        bool banderaExtend = true;
        fseek(archivo,0,SEEK_SET);
        fread(&mbr,sizeof(MBR),1,archivo);
        for(int i=0; i<4; i++){
            if(mbr.mbr_partitions[i].part_type=='E'){
                indiceExt=i;
                banderaExtend=false;
            }
            else if(mbr.mbr_partitions[i].part_name==name){
                indicePart=i;
                part=true;
                if(mbr.mbr_partitions[i].part_type=='E'){extendida=true; banderaExtend=false;}
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
                            printf("~~~>Se agrego espacio a la particion exitosamente! \n");
                        }else{
                            printf("Error: No es posible agregar espacio a la particion ya que no hay suficiente espacio despues de ella. \n");

                        }
                    }

                }else{//restar
                    if(valorAdd >= mbr.mbr_partitions[indicePart].part_size){
                        printf("Error: No se puede eliminar una cantidad mayor de espacio de la que tiene asignada la particion! \n");
                    }else{
                        mbr.mbr_partitions[indicePart].part_size=mbr.mbr_partitions[indicePart].part_size-valorAdd;
                        fseek(archivo,0,SEEK_SET);
                        fwrite(&mbr,sizeof(MBR),1,archivo);
                        printf("~~~>Se quito espacio a la particion exitosamente! \n");
                    }
                }
            }else{//extendida
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
                            printf("~~~>Se agrego espacio a la particion exitosamente! \n");
                        }else{
                            printf("Error: No es posible agregar espacio a la particion ya que no hay suficiente espacio despues de ella. \n");

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
                            printf("~~~>Se quito espacio a la particion exitosamente! \n");
                        }
                    }else{
                        printf("Error: No se puede eliminar una cantidad mayor de espacio de la que tiene asignada la particion! \n");
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
                                fseek(archivo,ebr.part_start,SEEK_SET);
                                fwrite(&ebr,sizeof(EBR),1, archivo);
                                printf("~~~>Se agrego espacio a la particion exitosamente! \n");
                            }else{
                                printf("Error: No es posible agregar espacio a la particion ya que no hay suficiente espacio despues de ella. \n");

                            }
                        }else{
                            printf("Error: No es posible agregar espacio a la particion ya que no hay suficiente espacio despues de ella. \n");

                        }
                    }else{//tiene una siguiente

                        int fragmento = inSigLogica-finLogica;
                        if(fragmento>0){

                            if(fragmento>valorAdd){
                                fseek(archivo,ebr.part_start,SEEK_SET);
                                fwrite(&ebr,sizeof(EBR),1, archivo);
                                printf("~~~>Se agrego espacio a la particion exitosamente! \n");
                            }else{
                                printf("Error: No es posible agregar espacio a la particion ya que no hay suficiente espacio despues de ella. \n");

                            }
                        }else{
                            printf("Error: No es posible agregar espacio a la particion ya que no hay suficiente espacio despues de ella. \n");

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
                        printf("Error: No se puede eliminar una cantidad mayor de espacio de la que tiene asignada la particion! \n");
                    }else{
                        ebr.part_size = ebr.part_size + valorAdd;
                        fseek(archivo,ebr.part_start,SEEK_SET);
                        fwrite(&ebr,sizeof(EBR),1,archivo);
                        printf("~~~>Se quito espacio a la particion exitosamente! \n");
                    }
                }

            }else{
                printf("Error: No se puede encontrar la Particion Extendida que contiene la Particion Logica deseada! \n");

            }
        }
        fclose(archivo);
    }else{
        printf("ERROR el disco donde se desea agregar/quitar unidades no existe \n");
    }
}
void delParticion(){}

void crearPartPri(QString direccion, QString name, int size, char fit){
    char auxF= fit;
    int sizeBytes = size;
    char buff = '1';
    bool existe=false;
    bool partext = true;
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
    fclose(archivo);

    if(fit != 0)
        auxF = fit;
    else
        auxF = 'W';


    MBR mbr;
    if((archivo = fopen(direccion.toStdString().c_str(), "rb+"))){
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
            printf("~~~>Espacio necesario:  %i Bytes \n",sizeBytes);
            printf("~~~>Espacio disponible:  %i Bytes \n",mbr.mbr_tamanio - utilizado);
            if((mbr.mbr_tamanio - utilizado) >= sizeBytes){
                if(!partext){
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
                    printf("~~~>La particion primaria fue creada con exito \n");
                }else{
                    printf("ERROR: Ya existe una particion con el mismo nombre \n");

                }

            }else{
                printf("ERROR: La particion que desea crear excede el espacio libre \n");

            }
        }else{
            printf("ERROR: Ya se llego al limite de particiones (4 particiones por Disco) \n");
            printf("Para crear una nueva debera eliminar al menos una. \n");
        }
    fclose(archivo);
    }else{
        printf("ERROR: no existe el Disco especificado \n");

    }
}

