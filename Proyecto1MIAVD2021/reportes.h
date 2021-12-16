#ifndef REPORTES_H
#define REPORTES_H

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
using namespace std;

class Reporte
{
public:
    Reporte(QString dir, QString name, char letra, int num, QString id);
    QString dir;
    QString name;
    char letra;
    int num;
    QString id;
};

void Graficar(int reporte,QString path, QString particion, QString pathDest);
void GraficarDisco(QString , QString );
void Graficarmbr(QString path, QString pathDest);
void GraficarSB(QString path, QString part, QString pathDest);
void Graficarbm_inode(QString path, QString particion, QString pathDest);
void Graficarbm_block(QString path, QString particion, QString pathDest);
void Graficar_journal(QString path, QString part, QString pathDest);
void Graficar_inodos(QString path, QString part, QString pathDest);
void Grafcar_bloques(QString path, QString part, QString pathDest);
void validarRep(Node *Raiz);

#endif // REPORTES_H
