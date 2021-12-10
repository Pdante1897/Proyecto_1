#ifndef CLASES_H
#define CLASES_H
#include <qstring.h>
#include <iostream>


class ParticionMount
{
public:
    ParticionMount(QString, QString, char, int);
    QString dir;
    QString name;
    char letra;
    int num;
};
#endif // CLASES_H
