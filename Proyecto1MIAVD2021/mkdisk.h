#ifndef MKDISK_H
#define MKDISK_H
#include "estructuras.h"
#include <QString>
bool makeDisk(QString, int, MBR);
void validarMkDisk();
void verificarRuta(QString);
#endif // MKDISK_H
