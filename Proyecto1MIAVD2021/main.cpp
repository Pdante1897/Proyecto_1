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
using namespace std;

extern int yyparse();
extern int linea;
extern int columna;
extern int yylineno;
extern Node *listNodos; //lista de nodos

//para distinguir el comando ingresado
enum choice{
    MKD=1,
    SIZE=2,
    FIT=3,
    UNIT=4,
    PATH=5
};
//Metodo main
int main()
{
    //MBR mbr;
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
        Node nodito = listNodos->hijos.at(0);
        if(validarMkDisk(&nodito)){
            printf("Comando reconocido correctamente. \n \n");
        }else{

        }
    }
    }
}


