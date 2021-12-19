#include "clases.h"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
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
    if (this->nombre=="rmdisk"){this->tipo=6;return;}
    if (this->nombre=="fdisk"){this->tipo=7;return;}
    if (this->nombre=="add"){this->tipo=8;return;}
    if (this->nombre=="type"){this->tipo=9;return;}
    if (this->nombre=="delete"){this->tipo=10;return;}
    if (this->nombre=="name"){this->tipo=11;return;}
    if (this->nombre=="mount"){this->tipo=12;return;}
    if (this->nombre=="umount"){this->tipo=13;return;}
    if (this->nombre=="mkfs"){this->tipo=14;return;}
    if (this->nombre=="exec"){this->tipo=15;return;}
    if (this->nombre=="pause"){this->tipo=16;return;}
    if (this->nombre=="rep"){this->tipo=17;return;}
    if (this->nombre=="ident"){this->tipo=18;return;}
    if (this->nombre=="fs"){this->tipo=19;return;}




}
void Node::agregar(Node nodo){
    hijitos.append(nodo);
}
