#ifndef FDISK_H
#define FDISK_H
#include<QList>
#include"Node.h"

bool validarFDk(Node *Raiz);
void crearPartPri(QString, QString, int, char, char);
void addParticion(QString path,QString name, int valorAdd, char valorUnit);
void delParticion();
#endif // FDISK_H
