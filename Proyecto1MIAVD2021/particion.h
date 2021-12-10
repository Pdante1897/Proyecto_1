#ifndef PARTICION_H
#define PARTICION_H
#include <QString>
#include <string>
#include <iostream>
#include "estructuras.h"


class Particion
{
public:
    Particion();
    bool existePart(QString, QString);
    void crearPartPri(QString, QString, int, char, char);

};

#endif // PARTICION_H
