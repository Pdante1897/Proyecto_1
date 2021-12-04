#ifndef NODE_H
#define NODE_H

#include <QString>
#include <QList>

class Node
{
public:
    Node(QString nombre, int tipo, QString valor);
    QString nombre;
    int tipo;
    QString valor;
};

#endif // NODE_H
