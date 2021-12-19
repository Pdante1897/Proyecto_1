#ifndef CLASES_H
#define CLASES_H
#include <qstring.h>
#include <iostream>
#include <QList>


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
class Node
{
public:
    Node(QString nombre, QString valor);
    QString nombre;
    int tipo;
    QString valor;
    void asignarTipo();
    QList<Node> hijitos;
    void agregar(Node nodo);
};

#endif // CLASES_H
