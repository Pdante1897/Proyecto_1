#ifndef FDISK_H
#define FDISK_H
#include<QList>
#include"Node.h"
#include "clases.h"
void crearPartPri(QString direccion, QString name, int size, char fit);
void crearPartExt(QString direccion, QString name, int size, char fit);
void crearLogica(QString direccion, QString name, int size, char fit);
void addParticion(QString path,QString name, int valorAdd, char valorUnit);
void delParticion(QString direccion, QString name, QString valorType);
bool validarFDk(Node *Node);
void montarParticion(Node *Node);
int Letra(QString path);
void imprimirLista();
bool validarMontaje(Node *Node);
#endif // FDISK_H
