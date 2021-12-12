#ifndef CLASES_H
#define CLASES_H
#include <qstring.h>
#include <iostream>


class ParticionMount
{
public:
    ParticionMount(QString, QString, char, int, QString);
    QString dir;
    QString name;
    char letra;
    int num;
    QString id;
};
#endif // CLASES_H
