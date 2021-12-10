#include "particion.h"
#include <QString>
#include <string>
#include <iostream>
#include "estructuras.h"
using namespace std;

Particion::Particion()
{

}

bool Particion::existePart(QString dir, QString name){
    bool partext = true;
    int ext;
    FILE *archivo;
    if((archivo = fopen(dir.toStdString().c_str(),"rb+"))){
        MBR mbr;
        fseek(archivo,0,SEEK_SET);
        fread(&mbr, sizeof(MBR),1,archivo);
        for(int i = 0; i < 4; i++){
            if(strcmp(mbr.mbr_partitions[i].part_name,name.toStdString().c_str()) == 0){
                fclose(archivo);
                return true;
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
                    return false;
                }
                if(strcmp(ebr.part_name,name.toStdString().c_str()) == 0){
                    fclose(archivo);
                    return true;
                }
            }
        }
    }
    fclose(archivo);
    return false;
}

void Particion::crearPartPri(QString direccion, QString name, int size, char fit, char unit){
    char auxU = 0;
    char auxF = 0;
    string auxPath = direccion.toStdString();
    int sizeBytes;
    char buff = '1';
    if(fit != 0)
        auxF = fit;
    else
        auxF = 'W';

    if(unit != 0){
        auxU = unit;
        if(auxU == 'b'){
            sizeBytes = size;
        }else if(auxU == 'k'){
            sizeBytes = size * 1024;
        }else{
            sizeBytes = size*1024*1024;
        }
    }else{
        sizeBytes = size * 1024;
    }
    FILE *filep;
    MBR mbr;
    if((filep = fopen(auxPath.c_str(), "rb+"))){
        int numPart = 0;
        bool flagPart = false;
        fseek(filep,0,SEEK_SET);
        fread(&mbr,sizeof(MBR),1,filep);
        for(int i = 0; i < 4; i++){
            if(mbr.mbr_partitions[i].part_start == -1 || (mbr.mbr_partitions[i].part_status == '1' && mbr.mbr_partitions[i].part_size>=sizeBytes)){
                flagPart = true;
                numPart = i;
                break;
            }
        }
        if(flagPart){
            int espUtilizado = 0;
            for(int i = 0; i < 4; i++){
                if(mbr.mbr_partitions[i].part_status!='1'){
                    espUtilizado += mbr.mbr_partitions[i].part_size;
                }
            }
            cout << "Espacio necesario:  " << sizeBytes << " Bytes" << endl;
            cout << "Espacio disponible: " << (mbr.mbr_tamanio - espUtilizado) << " Bytes" << endl;
            if((mbr.mbr_tamanio - espUtilizado) >= sizeBytes){
                if(!existePart(direccion,name)){
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
                    fseek(filep,0,SEEK_SET);
                    fwrite(&mbr,sizeof(MBR),1,filep);
                    fseek(filep,mbr.mbr_partitions[numPart].part_start,SEEK_SET);
                    for(int i = 0; i < sizeBytes; i++){
                        fwrite(&buff,1,1,filep);
                    }
                    cout << "...\n" << "La particion primaria fue creada con exito" <<  endl;
                }else{
                    cout << "ERROR: Ya existe una particion con el mismo nombre" << endl;
                }

            }else{
                cout << "ERROR: La particion que desea crear excede el espacio libre" << endl;
            }
        }else{
            cout << "ERROR: Ya se llego al limite de particiones (4 particiones por Disco)" << endl;
            cout << "Para crear una nueva debera eliminar al menos una." << endl;
        }
    fclose(filep);
    }else{
        cout << "ERROR: no existe el Disco especificado" << endl;
    }
}
