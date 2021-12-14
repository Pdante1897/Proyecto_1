#include <QCoreApplication>
#include <iostream>
#include <QString>
#include <stdio.h>
#include <string.h>
#include <QTextStream>
#include <QList>
#include "main.h"
#include "Node.h"
#include "mkdisk.h"
#include "parser.h"
#include "scanner.h"
#include "estructuras.h"
#include "fdisk.h"
#include "clases.h"
#include "mkfs.h"

using namespace std;

extern int yyparse();
extern int linea;
extern int columna;
extern int yylineno;
extern Node *listNodos; //lista de nodos
extern QList<ParticionMount> partMontadas=*new QList<ParticionMount>();
extern QList<QString> pathsMontados =*new QList<QString>();;;

//para distinguir el comando ingresado
enum choice{
    MKD=1,
    SIZE=2,
    FIT=3,
    UNIT=4,
    PATH=5,
    RMDK=6,
    FDK=7,
    MOUNT = 12,
    UMOUNT=13,
    MKFS=14,
    EXEC=15,
    PAUSE=16,
    REP=17

};
//Metodo main
int main()
{
    partMontadas=*new QList<ParticionMount>();
    pathsMontados=*new QList<QString>();
    //QString cadena = "/home/bryan/carpeta/disko.disk";
    //verificarRuta(cadena);
    //makeDisk(cadena,10,mbr);
    char encabezado[] = "------------------------------SISTEMA DE ARCHIVOS-----------------------------\n"
             "By: Bryan Gerardo Paez Morales_______________________________________201700945\n"
             "Ingrese un comando: \n";
    printf("%s",encabezado);
    char lcomando[399];
    string exit = "exit";
    QTextStream qtin(stdin);
    while(true){
        printf("~");
        fgets(lcomando,sizeof (lcomando),stdin);
        if (comExit(lcomando)){
            break;
        }
        leerLComando(lcomando);
    }
    return 0;
}
//metodo retorna true si se ingreso exit
bool comExit(char exit[399]){
    if(exit[0]=='e' && exit[1]=='x' && exit[2]=='i' && exit[3]=='t' ){
        return true;
    }
    return false;

}
//metodo para leer comando ingresado y enviarlo a los analizadores
void leerLComando(char lcomando[399]){
    if(lcomando[0] != '#'){
        YY_BUFFER_STATE buffer;
        buffer = yy_scan_string(lcomando);
        if(yyparse() == 0){
            if(listNodos!=nullptr){
                ejecutarComando(*listNodos);
            }
        }else
            printf("Error: Comando no valido \n \n");
    }
}

//manda a llamar los metodos de cada comando utilizando la clase enumerada
void ejecutarComando(Node Lista){
    Node nodo = Lista;
    nodo.asignarTipo();
    switch (nodo.tipo){
    case MKD:{
        Node nodito = nodo.hijos.at(0);
        if(validarMkDisk(&nodito)){
            printf("Comando reconocido correctamente. \n \n");
        }
        break;
    }
    case RMDK:{
        Node nodito = nodo.hijos.at(0);
        ejecutarRMD(&nodo);
        break;
    }
    case FDK:{
        Node nodito = nodo.hijos.at(0);
        if(validarFDk(&nodito)){
            printf("Comando reconocido correctamente. \n \n");
        };
        break;
    }case MOUNT:{
        Node nodito = nodo.hijos.at(0);
        validarMontaje(&nodito);
        break;
    }case PAUSE:{
        printf("~~~>Sistema en pausa! -Presione una tecla para continuar... ");
        std::cin.get();
        break;

    }case EXEC:{
        string path = listNodos->hijos.at(0).valor.toStdString();
        FILE *archivo;
        if((archivo = fopen(path.c_str(),"r"))){
            char line[400]="";
            memset(line,0,sizeof(line));
            while(fgets(line,sizeof line,archivo)){
                if(line[0]!='\n'){
                    printf("~ %s",line);
                    leerLComando(line);
                }
                memset(line,0,sizeof(line));
            }
            fclose(archivo);
        }else{
            printf("ERROR: script no encontrado\n");
        }break;
    }case UMOUNT:{
        QString id = listNodos->hijos.at(0).valor;
        bool eliminado=false;
        for(int i=0; i<partMontadas.length();i++){
            if(partMontadas.at(i).id==id){
                partMontadas.removeAt(i);
                printf("La unidad fue desmontada con exito! \n");
                eliminado = true;
                break;
            }

        }
        if(!eliminado){printf("ERROR: no se encuentra montada la unidad \n");}
        break;
    }case MKFS:{
        Node nodito = nodo.hijos.at(0);
        validarMKFS(&nodito);
    }
    }
}


