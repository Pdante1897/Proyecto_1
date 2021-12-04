#ifndef NODE_H
#define NODE_H

#include <QString>
#include <QList>

class Node
{
public:
    Node(QString tipo, QString valor);
    QString tipo;
    QString valor;
};

#endif // NODE_H
