#include "Node.h"

Node::Node(QString nombre, QString valor)
{
    this->nombre=nombre;
    this->valor=valor;
}

void Node::asignarTipo(){
    if (this->nombre=="mkdisk"){this->tipo=1; return;}
    if (this->nombre=="size"){this->tipo=2;return;}
    if (this->nombre=="fit"){this->tipo=3;return;}
    if (this->nombre=="unit"){this->tipo=4;return;}
    if (this->nombre=="path"){this->tipo=5;return;}


}
void Node::agregar(Node nodo){
    hijos.append(nodo);
}
