#ifndef DISK_H
#define DISK_H
#include <QString>

class Disk
{
public:
    Disk();
    int size;
    char unit;
    char fit;
    QString path;
};

#endif // DISK_H
