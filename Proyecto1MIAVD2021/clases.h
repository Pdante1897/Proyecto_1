#ifndef CLASES_H
#define CLASES_H
#include <qstring.h>
#include <iostream>


class ParticionMount
{
public:
    ParticionMount(QString dir, QString name, char letra, int num, QString id);
    QString dir;
    QString name;
    char letra;
    int num;
    QString id;
};


#endif // CLASES_H
