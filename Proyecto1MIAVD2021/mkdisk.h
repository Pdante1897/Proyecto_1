#ifndef MKDISK_H
#define MKDISK_H
#include "estructuras.h"
#include <QString>
#include "Node.h"
bool makeDisk(QString, int, MBR);
bool validarMkDisk(Node *Raiz);
void verificarRuta(QString);

#endif // MKDISK_H
