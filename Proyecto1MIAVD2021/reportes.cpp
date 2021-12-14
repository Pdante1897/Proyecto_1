#include "reportes.h"

#include "estructuras.h"
#include <fstream>
#include "mkdisk.h"
#include "mkfs.h"

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
                        if(valorName == "mbr")
                            repmbr(particion.dir, valorId, valorPath);
                        else if(valorName == "disk"){}
                            repDisco(particion.dir, valorPath);
                    }else
                        cout << "ERROR: no se encuentra la particion" << endl;
                }else
                    cout << "ERROR: parametro ID no definido" << endl;
            }else
                cout << "ERROR: parametro NAME no definido" << endl;
        }else
            cout << "ERROR: parametro PATH no definido" << endl;
    }


}


void repmbr(QString path, QString id, QString pathDest){
    FILE *archivo;
    archivo = fopen(path.toStdString().c_str(), "rb+");
    if(archivo == NULL){
        printf(ANSI_COLOR_CYAN"Error: No se existe el disco deseado \n" ANSI_COLOR_RESET);
        return;
    }
    MBR mbr;
    ofstream archivoDot;
    archivoDot.open("/home/bryan/mbr.txt", ios::out);
    if(archivoDot.fail()){
        printf(ANSI_COLOR_CYAN"Error: No se pudo abrir el archivo \n" ANSI_COLOR_RESET);
        return;
    }
    archivoDot << "digraph G {\n"
            << "node [shape=plaintext] \n"
            << "nodo [ \n"
            << " label =< \n"
            << "<table border=\"0\" cellborder=\"1\" cellspacing=\"0\">"
            << "<tr> <td bgcolor=\"#30A6BB\">Nombre</td> <td bgcolor=\"#30A6BB\"> Valor </td> </tr>";
    fseek(archivo, 0, SEEK_SET);
    fread(&mbr, sizeof(MBR), 1, archivo);
    archivoDot << "<tr>\n";
    archivoDot << "<td bgcolor=\"#89D8E8\"> mbr_tamanio </td>";
    archivoDot << "<td bgcolor=\"#C7F6F0\">" << mbr.mbr_tamanio << " </td> \n";
    archivoDot << "</tr>";
    archivoDot << "<tr>\n";
    archivoDot << "<td bgcolor=\"#89D8E8\"> mbr_fecha_creacion </td>";
    archivoDot << "<td bgcolor=\"#C7F6F0\">" << mbr.mbr_fecha_creacion << " </td> \n";
    archivoDot << "</tr>";
    archivoDot << "<tr>\n";
    archivoDot << "<td bgcolor=\"#89D8E8\"> mbr_disk_signature </td>";
    archivoDot << "<td bgcolor=\"#C7F6F0\">" << mbr.mbr_disk_signature << " </td> \n";
    archivoDot << "</tr>";
    for(int i = 0; i < 4; i++){
            archivoDot << "<tr>\n";
            archivoDot << "<td bgcolor=\"#89D8E8\"> Estado de particion: </td>";
            archivoDot << "<td bgcolor=\"#C7F6F0\">" << mbr.mbr_partitions[i].part_status << " </td> \n";
            archivoDot << "</tr>";
            archivoDot << "<tr>\n";
            archivoDot << "<td bgcolor=\"#89D8E8\"> Tipo de particion: </td>";
            archivoDot << "<td bgcolor=\"#C7F6F0\">" << mbr.mbr_partitions[i].part_type << " </td> \n";
            archivoDot << "</tr>";
            archivoDot << "<tr>\n";
            archivoDot << "<td bgcolor=\"#89D8E8\"> Tipo de ajuste: </td>";
            archivoDot << "<td bgcolor=\"#C7F6F0\">" << mbr.mbr_partitions[i].part_fit<< " </td> \n";
            archivoDot << "</tr>";
            archivoDot << "<tr>\n";
            archivoDot << "<td bgcolor=\"#89D8E8\"> Inicio de particion: </td>";
            archivoDot << "<td bgcolor=\"#C7F6F0\">" << mbr.mbr_partitions[i].part_start << " </td> \n";
            archivoDot << "</tr>";
            archivoDot << "<tr>\n";
            archivoDot << "<td bgcolor=\"#89D8E8\"> Tamaño de particion: </td>";
            archivoDot << "<td bgcolor=\"#C7F6F0\">" << mbr.mbr_partitions[i].part_size << " </td> \n";
            archivoDot << "</tr>";
            archivoDot << "<tr>\n";
            archivoDot << "<td bgcolor=\"#89D8E8\"> Nombre de particion: </td>";
            archivoDot << "<td bgcolor=\"#C7F6F0\">" << mbr.mbr_partitions[i].part_name << " </td> \n";
            archivoDot << "</tr>";
    }
    archivoDot << "</table>\n";
    archivoDot << ">\n";
    archivoDot << "];\n";
    archivoDot << "}";
    archivoDot.close();
    fclose(archivo);
    string comando = "dot -Tpng /home/bryan/mbr.txt -o " + pathDest.toStdString();
    system(comando.c_str());

}

void repSB(string rutaParticion, string nombreParticion, string rutaDestino){

    FILE *archivo;

    archivo = fopen(rutaParticion.c_str(), "rb+");

    if(archivo == NULL){
        cout << " >> El disco no existe. \n";
        return;
    }

    int inicio_particion = 0;

    MBR mbr;
    fseek(archivo, 0, SEEK_SET);
    fread(&mbr, sizeof(mbr), 1, archivo);
    for(int i = 0; i < 4; i++){
        if(strcmp(mbr.mbr_partitions[i].part_name, nombreParticion.c_str()) == 0){
            //cout << " >> Size: " << mbr_.mbr_particions[i].part_size << " \n";
            inicio_particion = mbr.mbr_partitions[i].part_start;
            //tam_particion = mbr_.mbr_particions[i].part_size;
            break;
        }

    }

    // superbloque auxiliar
    SuperBloque supB;
    fseek(archivo, inicio_particion, SEEK_SET);

    // Escribir el reporte del superbloque
    ofstream archivoDot;

    archivoDot.open("/home/juan/Desktop/superbloque.txt", ios::out);

    if(archivoDot.fail()){
        cout << " >> No se pudo abrir \n";
    }

    archivoDot << "digraph G {\n"
            << "node [shape=plaintext] \n"
            << "nodo [ \n"
            << " label =< \n"
            << "<table border=\"0\" cellborder=\"1\" cellspacing=\"0\">"
            << "<tr> <td bgcolor=\"#30A6BB\">Nombre</td> <td bgcolor=\"#30A6BB\"> Valor </td> </tr>";

    // Empiezo a leer el superbloque
    fread(&supB, sizeof(SuperBloque), 1, archivo);

    archivoDot << "<tr>\n";
    archivoDot << "<td bgcolor=\"#89D8E7\"> s_inodes_count </td>";
    archivoDot << "<td bgcolor=\"#C7F6FF\">" << supB.s_inodes_count << " </td> \n";
    archivoDot << "</tr>";

    archivoDot << "<tr>\n";
    archivoDot << "<td bgcolor=\"#89D8E7\"> s_blocks_count </td>";
    archivoDot << "<td bgcolor=\"#C7F6FF\">" << supB.s_blocks_count << " </td> \n";
    archivoDot << "</tr>";

    archivoDot << "<tr>\n";
    archivoDot << "<td bgcolor=\"#89D8E7\"> s_free_blocks_count </td>";
    archivoDot << "<td bgcolor=\"#C7F6FF\">" << supB.s_free_blocks_count << " </td> \n";
    archivoDot << "</tr>";

    archivoDot << "<tr>\n";
    archivoDot << "<td bgcolor=\"#89D8E7\"> s_free_inodes_count </td>";
    archivoDot << "<td bgcolor=\"#C7F6FF\">" << supB.s_free_inodes_count << " </td> \n";
    archivoDot << "</tr>";

    archivoDot << "<tr>\n";
    archivoDot << "<td bgcolor=\"#89D8E7\"> s_mtime </td>";
    archivoDot << "<td bgcolor=\"#C7F6FF\">" << supB.s_mtime << " </td> \n";
    archivoDot << "</tr>";

    archivoDot << "<tr>\n";
    archivoDot << "<td bgcolor=\"#89D8E7\"> s_umtime </td>";
    archivoDot << "<td bgcolor=\"#C7F6FF\">" << supB.s_umtime << " </td> \n";
    archivoDot << "</tr>";

    archivoDot << "<tr>\n";
    archivoDot << "<td bgcolor=\"#89D8E7\"> s_mnt_count </td>";
    archivoDot << "<td bgcolor=\"#C7F6FF\">" << supB.s_mnt_count << " </td> \n";
    archivoDot << "</tr>";

    archivoDot << "<tr>\n";
    archivoDot << "<td bgcolor=\"#89D8E7\"> s_magic </td>";
    archivoDot << "<td bgcolor=\"#C7F6FF\">" << supB.s_magic << " </td> \n";
    archivoDot << "</tr>";

    archivoDot << "<tr>\n";
    archivoDot << "<td bgcolor=\"#89D8E7\"> s_inode_size </td>";
    archivoDot << "<td bgcolor=\"#C7F6FF\">" << supB.s_inode_size << " </td> \n";
    archivoDot << "</tr>";

    archivoDot << "<tr>\n";
    archivoDot << "<td bgcolor=\"#89D8E7\"> s_block_size </td>";
    archivoDot << "<td bgcolor=\"#C7F6FF\">" << supB.s_block_size << " </td> \n";
    archivoDot << "</tr>";

    archivoDot << "<tr>\n";
    archivoDot << "<td bgcolor=\"#89D8E7\"> s_first_ino </td>";
    archivoDot << "<td bgcolor=\"#C7F6FF\">" << supB.s_first_ino << " </td> \n";
    archivoDot << "</tr>";

    archivoDot << "<tr>\n";
    archivoDot << "<td bgcolor=\"#89D8E7\"> s_first_blo </td>";
    archivoDot << "<td bgcolor=\"#C7F6FF\">" << supB.s_first_blo << " </td> \n";
    archivoDot << "</tr>";

    archivoDot << "<tr>\n";
    archivoDot << "<td bgcolor=\"#89D8E7\"> s_bm_inode_start </td>";
    archivoDot << "<td bgcolor=\"#C7F6FF\">" << supB.s_bm_inode_start << " </td> \n";
    archivoDot << "</tr>";

    archivoDot << "<tr>\n";
    archivoDot << "<td bgcolor=\"#89D8E7\"> s_bm_block_start </td>";
    archivoDot << "<td bgcolor=\"#C7F6FF\">" << supB.s_bm_block_start << " </td> \n";
    archivoDot << "</tr>";

    archivoDot << "<tr>\n";
    archivoDot << "<td bgcolor=\"#89D8E7\"> s_inode_start </td>";
    archivoDot << "<td bgcolor=\"#C7F6FF\">" << supB.s_inode_start << " </td> \n";
    archivoDot << "</tr>";

    archivoDot << "<tr>\n";
    archivoDot << "<td bgcolor=\"#89D8E7\"> s_block_start </td>";
    archivoDot << "<td bgcolor=\"#C7F6FF\">" << supB.s_block_start << " </td> \n";
    archivoDot << "</tr>";

    archivoDot << "</table>\n";
    archivoDot << ">\n";
    archivoDot << "];\n";
    archivoDot << "}";


    archivoDot.close();
    fclose(archivo);
    string comando = "dot -Tpng /home/bryan/superbloque.txt -o " + rutaDestino;
    system(comando.c_str());


}

void repDisco(QString path, QString pathDest){
    FILE *archivo;
    archivo = fopen(path.toStdString().c_str(), "rb+");
    if(archivo == NULL){
        printf(ANSI_COLOR_CYAN"Error: No se pudo abrir el archivo \n" ANSI_COLOR_RESET);
        return;
    }
    MBR mbr;
    fseek(archivo, 0, SEEK_END);
    ofstream archivoDot;
    archivoDot.open("/home/bryan/disk.txt", ios::out);
    if(archivoDot.fail()){
        printf(ANSI_COLOR_CYAN"Error: No se pudo abrir el archivo \n" ANSI_COLOR_RESET);
        return;
    }
    archivoDot << " digraph disk { \n"
            << " concentrate=True; \n"
            << " rankdir=TB; \n"
            << " node [shape=record];\n"
            << " nodo [label=\" ";
    fseek(archivo, 0, SEEK_SET);
    fread(&mbr, sizeof(MBR), 1, archivo);
    archivoDot << " mbr \\n "
            << sizeof(MBR) << " bytes | ";
    for(int i = 0; i < 4; i++){

            if(mbr.mbr_partitions[i].part_type == 'P'){
                archivoDot <<  " Primaria \\n "
                        << mbr.mbr_partitions[i].part_size << " bytes | ";
            }
            else if(mbr.mbr_partitions[i].part_type == 'E'){
                archivoDot << " { Extendida  | {  ";

                FILE *archivo2 = fopen(path.toStdString().c_str(), "rb+");
                fseek(archivo, mbr.mbr_partitions[i].part_start,SEEK_SET);
                EBR ebr;
                while((fread(&ebr,sizeof(EBR),1,archivo))!=0 && (ftell(archivo) < (mbr.mbr_partitions[i].part_size + mbr.mbr_partitions[i].part_start))){
                    archivoDot << ebr.part_name  <<" "<<ebr.part_size<<" bytes"<<" |";
                    fseek(archivo,ebr.part_next,SEEK_SET);
                    fread(&ebr,sizeof(EBR),1,archivo);

                }
                archivoDot << " } } |";
                fclose(archivo2);

            }
            else{
                archivoDot << " Libre |";
        }
    }
    archivoDot << " \" ]; \n";
    archivoDot << " } \n";
    archivoDot.close();
    fclose(archivo);
    string comando = "dot -Tpng /home/bryan/disk.txt -o " + pathDest.toStdString();
    system(comando.c_str());
}
