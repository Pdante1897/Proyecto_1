#include "reportes.h"
#include <iostream>
#include <string>
#include <string.h>
#include <iostream>
#include <QString>
#include <stdio.h>
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
#include <fstream>
#include "mkdisk.h"
#include "mkfs.h"
#include <stdlib.h>
#include <bitset>
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
    NAME=11,
    ID=18,
    FS=19

};


void validarRep(Node *Raiz)
{
    bool banderaName = true;
    bool banderaPath = true;
    bool banderaId = true;
    bool banderaRuta = true;
    bool bandera = true;
    QString valorName = "";
    QString valorPath = "";
    QString valorId = "";
    QString valorRuta = "";

    for(int i = 0; i < Raiz->hijitos.count(); i++)
    {
        Node nodito = Raiz->hijitos.at(i);
        nodito.asignarTipo();
        switch (nodito.tipo)
        {
        case NAME:{
            if(banderaName){
                valorName = nodito.valor;
                valorName = valorName.replace("\"", "");
                banderaName=false;
                break;
            }else{
                printf(ANSI_COLOR_CYAN "Error! Ya fue definido el parametro -NAME \n" ANSI_COLOR_RESET);
                bandera=false;
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
                bandera = false;
            }
            break;
        }
            break;
        case ID:
        {
            if(banderaId){
                banderaId = false;
                valorId = nodito.valor;
            }else{
                printf(ANSI_COLOR_CYAN"Error! Ya fue definido el parametro -ID \n");
                bandera = false;
                break;
            }

        }

    }
    }

    if(bandera){
        if(!banderaPath){
            if(!banderaName){
                if(!banderaId){
                    ParticionMount particion = getPart(valorId);
                    if(particion.id != "nulo" ){
                        verificarRuta(valorPath);
                        if(valorName == "mbr"){
                            Graficarmbr(particion.dir, valorPath);
                        }
                        else if(valorName == "disk"){
                            GraficarDisco(particion.dir, valorPath);
                        }else if(valorName == "superbloque"){
                            GraficarSB(particion.dir, particion.name,valorPath);
                        }else if(valorName == "bm_inode"){
                            Graficarbm_inode(particion.dir, particion.name,valorPath);
                        }else if(valorName == "bm_block"){
                            Graficarbm_block(particion.dir, particion.name,valorPath);
                        }else if(valorName == "journal"){
                            //Graficar_journal(particion.dir, particion.name,valorPath);
                        }else if(valorName == "inode"){
                            Graficar_inodos(particion.dir, particion.name,valorPath);
                        }else if(valorName == "block"){
                            Grafcar_bloques(particion.dir, particion.name,valorPath);
                        }
                    }else
                        printf(ANSI_COLOR_CYAN"ERROR: no se encuentra la particion \n");

                }else
                    printf(ANSI_COLOR_CYAN"ERROR: parametro ID no definido \n");

            }else
                printf(ANSI_COLOR_CYAN"ERROR: parametro NAME no definido \n");
        }else
            printf(ANSI_COLOR_CYAN"ERROR: parametro PATH no definido \n");
    }


}

//metodo para graficar mbr con graphviz
void Graficarmbr(QString path, QString pathDest){
    string comandoGraficar = "dot -Tpng /home/bryan/mbr.txt -o " + pathDest.toStdString();
    FILE *archivo;
    if((archivo = fopen(path.toStdString().c_str(), "rb+"))){

    }else{
        printf(ANSI_COLOR_CYAN"Error: No se existe el disco deseado \n" ANSI_COLOR_RESET);
        return;
    }
    MBR mbr;
    FILE *archivoDot;
    archivoDot=fopen("/home/bryan/mbr.txt", "w");
    fprintf(archivoDot,"digraph G {\n node [shape=plaintext] \n nodo [ \n label =< \n <table border=\"0\" cellborder=\"1\" cellspacing=\"0\">");
    fprintf(archivoDot,"<tr> <td bgcolor=\"#30A6BB\">Nombre</td> <td bgcolor=\"#30A6BB\"> Valor </td> </tr>" );
    fseek(archivo, 0, SEEK_SET);
    fread(&mbr, sizeof(MBR), 1, archivo);
    fprintf(archivoDot,"<tr>\n" );
    fprintf(archivoDot,"<td bgcolor=\"#89D8E8\"> mbr_tamanio </td>");
    fprintf(archivoDot,"<td bgcolor=\"#C7F6F0\"> %i </td> \n",mbr.mbr_tamanio);
    fprintf(archivoDot,"</tr>");
    fprintf(archivoDot,"<tr>\n");
    fprintf(archivoDot,"<td bgcolor=\"#89D8E8\"> mbr_fecha_creacion </td>");
    fprintf(archivoDot,"<td bgcolor=\"#C7F6F0\"> %li </td> \n",mbr.mbr_fecha_creacion);
    fprintf(archivoDot,"</tr>");
    fprintf(archivoDot,"<tr>\n");
    fprintf(archivoDot,"<td bgcolor=\"#89D8E8\"> mbr_disk_signature </td>");
    fprintf(archivoDot,"<td bgcolor=\"#C7F6F0\"> %i </td> \n",mbr.mbr_disk_signature);
    fprintf(archivoDot,"</tr>");
    for(int i = 0; i < 4; i++){
            fprintf(archivoDot,"<tr>\n");
            fprintf(archivoDot,"<td bgcolor=\"#89D8E8\"> Estado de particion: </td>");
            fprintf(archivoDot,"<td bgcolor=\"#C7F6F0\"> %i </td> \n",mbr.mbr_partitions[i].part_status);
            fprintf(archivoDot,"</tr>");
            fprintf(archivoDot,"<tr>\n");
            fprintf(archivoDot,"<td bgcolor=\"#89D8E8\"> Tipo de particion: </td>");
            fprintf(archivoDot,"<td bgcolor=\"#C7F6F0\"> %c </td> \n",mbr.mbr_partitions[i].part_type);
            fprintf(archivoDot,"</tr>");
            fprintf(archivoDot,"<tr>\n");
            fprintf(archivoDot,"<td bgcolor=\"#89D8E8\"> Tipo de ajuste: </td>");
            fprintf(archivoDot,"<td bgcolor=\"#C7F6F0\"> %c </td> \n",mbr.mbr_partitions[i].part_fit);
            fprintf(archivoDot,"</tr>");
            fprintf(archivoDot,"<tr>\n");
            fprintf(archivoDot,"<td bgcolor=\"#89D8E8\"> Inicio de particion: </td>");
            fprintf(archivoDot,"<td bgcolor=\"#C7F6F0\"> %i </td> \n",mbr.mbr_partitions[i].part_start);
            fprintf(archivoDot,"</tr>");
            fprintf(archivoDot,"<tr>\n");
            fprintf(archivoDot,"<td bgcolor=\"#89D8E8\"> Tamaño de particion: </td>");
            fprintf(archivoDot,"<td bgcolor=\"#C7F6F0\"> %i </td> \n",mbr.mbr_partitions[i].part_size);
            fprintf(archivoDot,"</tr>");
            fprintf(archivoDot,"<tr>\n");
            fprintf(archivoDot,"<td bgcolor=\"#89D8E8\"> Nombre de particion: </td>");
            fprintf(archivoDot,"<td bgcolor=\"#C7F6F0\"> %s </td> \n",mbr.mbr_partitions[i].part_name);
            fprintf(archivoDot,"</tr>");
    }
    fprintf(archivoDot,"</table>\n");
    fprintf(archivoDot,">\n");
    fprintf(archivoDot,"];\n");
    fprintf(archivoDot,"}");
    fclose(archivoDot);

    fclose(archivo);
    system(comandoGraficar.c_str());
    printf(ANSI_COLOR_YELLOW"~~~>Reporte Creado! \n");

}

void GraficarSB(QString path, QString particion, QString pathDest){
    string comandoGraficar = "dot -Tpng /home/bryan/superbloque.txt -o " + pathDest.toStdString();
    FILE *archivo;
    if((archivo = fopen(path.toStdString().c_str(), "rb+"))){

    }else{
        printf(ANSI_COLOR_CYAN"Error: No se existe el disco deseado \n" ANSI_COLOR_RESET);
        return;
    }
    int inicioPart = 0;

    MBR mbr;
    fseek(archivo, 0, SEEK_SET);
    fread(&mbr, sizeof(mbr), 1, archivo);
    for(int i = 0; i < 4; i++){
        if(strcmp(mbr.mbr_partitions[i].part_name, particion.toStdString().c_str()) == 0){
            inicioPart = mbr.mbr_partitions[i].part_start;
            break;
        }

    }
    SuperBloque superBloque;
    fseek(archivo, inicioPart, SEEK_SET);
    FILE *archivoDot;
    archivoDot = fopen("/home/bryan/superbloque.txt", "w");
    fprintf(archivoDot,"digraph G {\n node [shape=plaintext] \n nodo [ \n  label =< \n <table border=\"0\" cellborder=\"1\" cellspacing=\"0\"> <tr> <td bgcolor=\"#30A6BB\">Nombre</td> <td bgcolor=\"#30A6BB\"> Valor </td> </tr>");
    fread(&superBloque, sizeof(SuperBloque), 1, archivo);
    fprintf(archivoDot,"<tr>\n");
    fprintf(archivoDot,"<td bgcolor=\"#89D8E8\"> s_inodes_count </td>");
    fprintf(archivoDot,"<td bgcolor=\"#C7F6F0\"> %i </td> \n",superBloque.s_inodes_count);
    fprintf(archivoDot,"</tr>");
    fprintf(archivoDot,"<tr>\n");
    fprintf(archivoDot,"<td bgcolor=\"#89D8E8\"> s_blocks_count </td>");
    fprintf(archivoDot,"<td bgcolor=\"#C7F6F0\">%i </td> \n",superBloque.s_blocks_count);
    fprintf(archivoDot,"</tr>");
    fprintf(archivoDot,"<tr>\n");
    fprintf(archivoDot,"<td bgcolor=\"#89D8E8\"> s_free_blocks_count </td>");
    fprintf(archivoDot,"<td bgcolor=\"#C7F6F0\">%i </td> \n",superBloque.s_free_blocks_count);
    fprintf(archivoDot,"</tr>");
    fprintf(archivoDot,"<tr>\n");
    fprintf(archivoDot,"<td bgcolor=\"#89D8E8\"> s_free_inodes_count </td>");
    fprintf(archivoDot,"<td bgcolor=\"#C7F6F0\">%i </td> \n",superBloque.s_free_inodes_count);
    fprintf(archivoDot,"</tr>");
    fprintf(archivoDot,"<tr>\n");
    fprintf(archivoDot,"<td bgcolor=\"#89D8E8\"> s_mtime </td>");
    fprintf(archivoDot,"<td bgcolor=\"#C7F6F0\">%li </td> \n",superBloque.s_mtime);
    fprintf(archivoDot,"</tr>");
    fprintf(archivoDot,"<tr>\n");
    fprintf(archivoDot,"<td bgcolor=\"#89D8E8\"> s_umtime </td>");
    fprintf(archivoDot,"<td bgcolor=\"#C7F6F0\">%li </td> \n",superBloque.s_umtime);
    fprintf(archivoDot,"</tr>");
    fprintf(archivoDot,"<tr>\n");
    fprintf(archivoDot,"<td bgcolor=\"#89D8E8\"> s_mnt_count </td>");
    fprintf(archivoDot,"<td bgcolor=\"#C7F6F0\">%i </td> \n",superBloque.s_mnt_count);
    fprintf(archivoDot,"</tr>");
    fprintf(archivoDot,"<tr>\n");
    fprintf(archivoDot,"<td bgcolor=\"#89D8E8\"> s_magic </td>");
    fprintf(archivoDot,"<td bgcolor=\"#C7F6F0\">%i </td> \n",superBloque.s_magic);
    fprintf(archivoDot,"</tr>");
    fprintf(archivoDot,"<tr>\n");
    fprintf(archivoDot,"<td bgcolor=\"#89D8E8\"> s_inode_size </td>");
    fprintf(archivoDot,"<td bgcolor=\"#C7F6F0\">%i </td> \n",superBloque.s_inode_size);
    fprintf(archivoDot,"</tr>");
    fprintf(archivoDot,"<tr>\n");
    fprintf(archivoDot,"<td bgcolor=\"#89D8E8\"> s_block_size </td>");
    fprintf(archivoDot,"<td bgcolor=\"#C7F6F0\">%i </td> \n",superBloque.s_block_size);
    fprintf(archivoDot,"</tr>");
    fprintf(archivoDot,"<tr>\n");
    fprintf(archivoDot,"<td bgcolor=\"#89D8E8\"> s_first_ino </td>");
    fprintf(archivoDot,"<td bgcolor=\"#C7F6F0\">%i </td> \n",superBloque.s_first_ino);
    fprintf(archivoDot,"</tr>");
    fprintf(archivoDot,"<tr>\n");
    fprintf(archivoDot,"<td bgcolor=\"#89D8E8\"> s_first_blo </td>");
    fprintf(archivoDot,"<td bgcolor=\"#C7F6F0\">%i </td> \n",superBloque.s_first_blo);
    fprintf(archivoDot,"</tr>");
    fprintf(archivoDot,"<tr>\n");
    fprintf(archivoDot,"<td bgcolor=\"#89D8E8\"> s_bm_inode_start </td>");
    fprintf(archivoDot,"<td bgcolor=\"#C7F6F0\">%i </td> \n",superBloque.s_bm_inode_start);
    fprintf(archivoDot,"</tr>");
    fprintf(archivoDot,"<tr>\n");
    fprintf(archivoDot,"<td bgcolor=\"#89D8E8\"> s_bm_block_start </td>");
    fprintf(archivoDot,"<td bgcolor=\"#C7F6F0\">%i </td> \n",superBloque.s_bm_block_start);
    fprintf(archivoDot,"</tr>");
    fprintf(archivoDot,"<tr>\n");
    fprintf(archivoDot,"<td bgcolor=\"#89D8E8\"> s_inode_start </td>");
    fprintf(archivoDot,"<td bgcolor=\"#C7F6F0\">%i </td> \n",superBloque.s_inode_start);
    fprintf(archivoDot,"</tr>");
    fprintf(archivoDot,"<tr>\n");
    fprintf(archivoDot,"<td bgcolor=\"#89D8E8\"> s_block_start </td>");
    fprintf(archivoDot,"<td bgcolor=\"#C7F6F0\">%i </td> \n",superBloque.s_block_start);
    fprintf(archivoDot,"</tr>");
    fprintf(archivoDot,"</table>\n");
    fprintf(archivoDot,">\n");
    fprintf(archivoDot,"];\n");
    fprintf(archivoDot,"}");
    fclose(archivoDot);

    fclose(archivo);
    system(comandoGraficar.c_str());
    printf(ANSI_COLOR_YELLOW"~~~>Reporte Creado! \n");


}

void GraficarDisco(QString path, QString pathDest){
    string comandoGraficar = "dot -Tpng /home/bryan/disk.txt -o " + pathDest.toStdString();
    FILE *archivo;
    if((archivo = fopen(path.toStdString().c_str(), "rb+"))){

    }else{
        printf(ANSI_COLOR_CYAN"Error: No se existe el disco deseado \n" ANSI_COLOR_RESET);
        return;
    }
    MBR mbr;
    fseek(archivo, 0, SEEK_END);
    FILE *archivoDot;
    archivoDot =fopen("/home/bryan/disk.txt", "w");
    fprintf(archivoDot," digraph disk { \n concentrate=True; \n rankdir=TB; \n node [shape=record];\n nodo [label=\" ");
    fseek(archivo, 0, SEEK_SET);
    fread(&mbr, sizeof(MBR), 1, archivo);
    fprintf(archivoDot," mbr \\n %i bytes | ",sizeof(MBR));
    for(int i = 0; i < 4; i++){

            if(mbr.mbr_partitions[i].part_type == 'P'){
                fprintf(archivoDot, " Primaria \\n %i bytes | ",mbr.mbr_partitions[i].part_size);
            }
            else if(mbr.mbr_partitions[i].part_type == 'E'){
                fprintf(archivoDot," { Extendida  | {  ");

                FILE *archivo2 = fopen(path.toStdString().c_str(), "rb+");
                fseek(archivo, mbr.mbr_partitions[i].part_start,SEEK_SET);
                EBR ebr;
                while((fread(&ebr,sizeof(EBR),1,archivo))!=0 && (ftell(archivo) < (mbr.mbr_partitions[i].part_size + mbr.mbr_partitions[i].part_start))){
                    fprintf(archivoDot,"%s %i bytes |",ebr.part_name,ebr.part_size);
                    fseek(archivo,ebr.part_next,SEEK_SET);
                    fread(&ebr,sizeof(EBR),1,archivo);

                }
                fprintf(archivoDot," } } |");
                fclose(archivo2);

            }
            else{
                fprintf(archivoDot," Libre |");
        }
    }
    fprintf(archivoDot," \" ]; \n");
    fprintf(archivoDot," } \n");
    fclose(archivoDot);

    fclose(archivo);
    system(comandoGraficar.c_str());
    printf(ANSI_COLOR_YELLOW"~~~>Reporte Creado! \n");

}

void Graficarbm_inode(QString path, QString particion, QString pathDest){
    FILE *archivo;
    if((archivo = fopen(path.toStdString().c_str(), "rb+"))){
        int inicioPart = 0;
        MBR mbr;
        ofstream archivotxt;
        archivotxt.open(pathDest.toStdString(), ios::out);
        SuperBloque superBloqueAux;
        fseek(archivo, 0, SEEK_SET);
        fread(&mbr, sizeof(MBR), 1, archivo);
        for(int i = 0; i < 4; i++){
            if(strcmp(mbr.mbr_partitions[i].part_name, particion.toStdString().c_str()) == 0){
                inicioPart = mbr.mbr_partitions[i].part_start;
                break;
            }
        }
        fseek(archivo, inicioPart, SEEK_SET);
        fread(&superBloqueAux, sizeof(SuperBloque), 1, archivo);
        fseek(archivo, superBloqueAux.s_bm_inode_start, SEEK_SET);
        char bitchar[superBloqueAux.s_inode_size];
        TablaInodos inodoT;
        int aux=0;
        for(int i = 0; i < superBloqueAux.s_inodes_count; i++){
            fseek(archivo, superBloqueAux.s_bm_inode_start+i*sizeof(TablaInodos),SEEK_SET);
            fread(&bitchar,superBloqueAux.s_inode_size,1,archivo);
            std::bitset<8> bit(bitchar);
            for(int j=0;j<8;j++){
                archivotxt << "  " << bit.to_string().at(j) << "  ";
                if((1+aux)%20 == 0){
                    archivotxt << "\n";
                }
                aux++;
            }
        }
        archivotxt.close();
        fclose(archivo);
        printf(ANSI_COLOR_YELLOW"~~~>Reporte Creado! \n");
    }else{
        printf(ANSI_COLOR_CYAN"Error: No se existe el disco deseado \n" ANSI_COLOR_RESET);
        return;
    }



}

void Graficarbm_block(QString path, QString particion, QString pathDest){
    FILE *archivo;
    if((archivo = fopen(path.toStdString().c_str(), "rb+"))){
        SuperBloque superbloqueAux;
        int inicioPart = 0;
        MBR mbr;
        fseek(archivo, 0, SEEK_SET);
        fread(&mbr, sizeof(MBR), 1, archivo);
        for(int i = 0; i < 4; i++){
            if(strcmp(mbr.mbr_partitions[i].part_name, particion.toStdString().c_str()) == 0){
                inicioPart = mbr.mbr_partitions[i].part_start;
                break;
            }
        }
        fseek(archivo, inicioPart, SEEK_SET);
        ofstream archivotxt;
        archivotxt.open(pathDest.toStdString(), ios::out);
        fread(&superbloqueAux, sizeof(SuperBloque), 1, archivo);
        fseek(archivo, superbloqueAux.s_bm_block_start, SEEK_SET);
        char bitchar[superbloqueAux.s_blocks_count];
        TablaInodos inodoT;
        int aux=0;
        for(int i = 0; i < superbloqueAux.s_blocks_count; i++){
            fseek(archivo, superbloqueAux.s_bm_block_start+i*superbloqueAux.s_block_size,SEEK_SET);
            fread(&bitchar,superbloqueAux.s_block_size,1,archivo);
            try {
                std::bitset<8> bit(bitchar);
                for(int j=0;j<8;j++){
                    archivotxt << "  " << bit.to_string().at(j) << "  ";
                    if((1+aux)%20 == 0){
                        archivotxt << "\n";
                    }
                    aux++;

                }
            }  catch (exception) {

            }


        }
        archivotxt.close();
        fclose(archivo);
        printf(ANSI_COLOR_YELLOW"~~~>Reporte Creado! \n");
    }else{
        printf(ANSI_COLOR_CYAN"Error: No se existe el disco deseado \n" ANSI_COLOR_RESET);
        return;
    }


}


void Graficar_inodos(QString path, QString part, QString pathDest){
    string comandoGraficar = "dot -Tpng /home/bryan/inode.txt -o " + pathDest.toStdString();
    FILE *archivo;
    if((archivo = fopen(path.toStdString().c_str(), "rb+"))){
    }else{
        printf(ANSI_COLOR_CYAN"Error: No se existe el disco deseado \n" ANSI_COLOR_RESET);
        return;
    }
    int inicioPart = 0;
    TablaInodos inodo;
    FILE *archivoDot;
    SuperBloque superBloqueAux;
    MBR mbr;
    fseek(archivo, 0, SEEK_SET);
    fread(&mbr, sizeof(MBR), 1, archivo);
    for(int i = 0; i < 4; i++){
        if(strcmp(mbr.mbr_partitions[i].part_name, part.toStdString().c_str()) == 0){
            inicioPart = mbr.mbr_partitions[i].part_start;
            break;
        }
    }
    fseek(archivo, inicioPart, SEEK_SET);
    fread(&superBloqueAux, sizeof(SuperBloque), 1, archivo);
    int inicio_inodos = superBloqueAux.s_inode_start;
    archivoDot=fopen("/home/bryan/inode.txt", "w");
    int inodosOc = superBloqueAux.s_inodes_count - superBloqueAux.s_free_inodes_count;
    fprintf(archivoDot,"digraph Inode {\n");
    fprintf(archivoDot,"node [shape=plaintext] \n");
    for(int i = 0; i < inodosOc; i++){
        fseek(archivo, inicio_inodos, SEEK_SET);
        fread(&inodo, sizeof(TablaInodos), 1, archivo);
        fprintf(archivoDot,"nodo%i [\n label =< \n",i);
        fprintf(archivoDot,"<table border=\"0\" cellborder=\"1\" cellspacing=\"0\"> <tr> <td bgcolor=\"#30A6BC\">Nombre</td> <td bgcolor=\"#30A6BB\"> Valor </td> </tr>");
        fprintf(archivoDot,"<tr>\n");
        fprintf(archivoDot,"<td bgcolor=\"#89D8E6\"> i_uid </td>");
        fprintf(archivoDot,"<td bgcolor=\"#C7F6FE\"> %i </td> \n",inodo.i_uid);
        fprintf(archivoDot,"</tr>");

        fprintf(archivoDot,"<tr>\n");
        fprintf(archivoDot,"<td bgcolor=\"#89D8E6\"> i_gid </td>");
        fprintf(archivoDot,"<td bgcolor=\"#C7F6FE\"> %i </td> \n",inodo.i_gid);
        fprintf(archivoDot,"</tr>");

        fprintf(archivoDot,"<tr>\n");
        fprintf(archivoDot,"<td bgcolor=\"#89D8E6\"> i_atime </td>");
        fprintf(archivoDot,"<td bgcolor=\"#C7F6FE\"> %li </td> \n",inodo.i_atime);
        fprintf(archivoDot,"</tr>");
        for(int j = 0; j < 15; j++){
            if(inodo.i_block[j] != -1){
                fprintf(archivoDot,"<tr>\n");
                fprintf(archivoDot,"<td bgcolor=\"#89D8E6\"> i_block_%i </td>",j);
                fprintf(archivoDot,"<td bgcolor=\"#C7F6FE\"> %i </td> \n",inodo.i_block[j]);
                fprintf(archivoDot,"</tr>");
            }
        }
        fprintf(archivoDot,"<tr>\n");
        fprintf(archivoDot,"<td bgcolor=\"#89D8E6\"> i_perm </td>");
        fprintf(archivoDot,"<td bgcolor=\"#C7F6FE\"> %i </td> \n",inodo.i_perm);
        fprintf(archivoDot,"</tr>");
        fprintf(archivoDot,"<tr>\n");
        fprintf(archivoDot,"<td bgcolor=\"#89D8E6\"> i_type </td>");
        fprintf(archivoDot,"<td bgcolor=\"#C7F6FE\"> %c </td> \n",inodo.i_type);
        fprintf(archivoDot,"</tr>");
        fprintf(archivoDot,"</table>\n");
        fprintf(archivoDot,">\n");
        fprintf(archivoDot,"];\n");

    }
    fprintf(archivoDot,"}");
    fclose(archivoDot);

    fclose(archivo);
    system(comandoGraficar.c_str());
    printf(ANSI_COLOR_YELLOW"~~~>Reporte Creado! \n");
}

void Grafcar_bloques(QString path, QString particion, QString pathDestino){
    string comandoGraficar = "dot -Tpng /home/bryan/blocks.txt -o " + pathDestino.toStdString();
    FILE *archivo;
    if((archivo = fopen(path.toStdString().c_str(), "rb+"))){
    }else{
        printf(ANSI_COLOR_CYAN"Error: No se existe el disco deseado \n" ANSI_COLOR_RESET);
        return;
    }
    int inicioPart = 0;
    SuperBloque superBloque;
    FILE *archivoDot;
    MBR mbr;
    BloqueCarpeta bloqueCarpeta;
    BloqueDeArchivos bloqueArchivo;
    fseek(archivo, 0, SEEK_SET);
    fread(&mbr, sizeof(MBR), 1, archivo);
    for(int i = 0; i < 4; i++){
        if(strcmp(mbr.mbr_partitions[i].part_name, particion.toStdString().c_str()) == 0){
            inicioPart = mbr.mbr_partitions[i].part_start;
            break;
        }
    }
    fseek(archivo, inicioPart, SEEK_SET);
    fread(&superBloque, sizeof(SuperBloque), 1, archivo);
    int inicio_bloques = superBloque.s_block_start;
    int bloques_usados = superBloque.s_blocks_count - superBloque.s_free_blocks_count;
    archivoDot = fopen("/home/bryan/blocks.txt", "w");
    fprintf(archivoDot,"digraph Journaling { \n");
    fprintf(archivoDot,"node [shape=plaintext] \n");
    for(int i = 0; i < bloques_usados; i++){
        fseek(archivo, inicio_bloques + i*64, SEEK_SET);
        fread(&bloqueCarpeta, 64, 1, archivo);
        fprintf(archivoDot,"nodo%i[\n",i);
        fprintf(archivoDot,"\t label =< \n \t\t <table border=\"0\" cellborder=\"1\" cellspacing=\"0\"> \n \t\t\t <tr> \n \t\t\t\t <td bgcolor=\"#30A6BC\">No. Bloque </td> <td bgcolor=\"#30A6BB\"> %i </td> \n",i);
        fprintf(archivoDot,"\t\t\t </tr> \n");
        fprintf(archivoDot,"\t\t\t <tr> \n \t\t\t\t <td bgcolor=\"#C7F6FE\"> b_name </td> <td bgcolor=\"#C7F6FF\"> b_inodo </td> \n \t\t\t </tr> \n");
        for(int j = 0; j < 4; j++){
            fprintf(archivoDot,"\t\t\t <tr> \n \t\t\t\t <td bgcolor=\"#C7F6FE\"> %i </td> <td bgcolor=\"#C7F6FF\"> %s </td> \n",bloqueCarpeta.b_content[j].b_inodo,bloqueCarpeta.b_content[j].b_name);
            fprintf(archivoDot,"\t\t\t </tr> \n");
        }
        fprintf(archivoDot,"\t\t </table> \n \t > \n ]; \n");
    }

    for(int i = 0; i < bloques_usados; i++){
        fseek(archivo, inicio_bloques + i*64, SEEK_SET);
        fread(&bloqueArchivo, 64, 1, archivo);
        fprintf(archivoDot,"nodoaux%i[\n",i);
        fprintf(archivoDot,"\t label =< \n \t\t <table border=\"0\" cellborder=\"1\" cellspacing=\"0\"> \n \t\t\t <tr> \n \t\t\t\t <td bgcolor=\"#30A6BC\">No. Bloque </td> <td bgcolor=\"#30A6BB\">  %i  </td> \n \t\t\t </tr> \n",i);
        fprintf(archivoDot,"\t\t\t <tr> \n \t\t\t\t <td bgcolor=\"#C7F6FE\"> content </td> <td bgcolor=\"#C7F6FF\"> %s </td> \n \t\t\t </tr> \n",bloqueArchivo.b_content);
        fprintf(archivoDot,"\t\t </table> \n");
        fprintf(archivoDot,"\t > \n");
        fprintf(archivoDot,"]; \n");
    }
    fprintf(archivoDot,"}\n");
    fclose(archivoDot);
    fclose(archivo);
    system(comandoGraficar.c_str());
    printf(ANSI_COLOR_YELLOW"~~~>Reporte Creado! \n");

}
