#ifndef MKFS_H
#define MKFS_H
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
#include <math.h>

class mkfs
{
public:
    mkfs();
};

ParticionMount* getPart(QString id);
void MKFSExt2(int inicio, int tamanio, QString dir);
void MKFSExt3(int inicio, int tamanio, QString dir);
#endif // MKFS_H
