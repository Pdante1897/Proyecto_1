#include "clases.h"

ParticionMount::ParticionMount(QString dir, QString name, char letra, int num, QString id)
{
    this->dir = dir;
    this->name = name;
    this->letra = letra;
    this->num = num;
    this->id= id;
}



class Disco
{
public:
    Disco();
    int size;
    char unit;
    char fit;
    QString path;
};

class Report
{
public:
    Report(QString dir, QString name, char letra, int num, QString id);
    QString dir;
    QString name;
    char letra;
    int num;
    QString id;
};
