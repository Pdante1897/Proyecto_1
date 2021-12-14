#ifndef NODE_H
#define NODE_H

#include <QString>
#include <QList>

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


#endif // NODE_H
