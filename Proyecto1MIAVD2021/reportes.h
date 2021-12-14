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

void repDisco(QString , QString );
void repmbr(QString path, QString id, QString pathDest);
void repSB(string rutaParticion, string nombreParticion, string rutaDestino);
void rep_bm_inode(string rutaParticion, string nombreParticion, string rutaDestino);
void rep_bm_block(string rutaParticion, string nombreParticion, string rutaDestino);
void rep_journal(string rutaParticion, string nombreParticion, string rutaDestino);
void rep_inodos(string rutaParticion, string nombreParticion, string rutaDestino);
void rep_bloques(string rutaParticion, string nombreParticion, string rutaDestino);
void validarRep(Node *Raiz);

#endif // REPORTES_H
