#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H
#include <time.h>
//Mis estructuras :3
typedef struct {
    char part_status;
    char part_type;
    char part_fit;
    int part_start;
    int part_size;
    char part_name[16];
} Partition;

typedef struct{
    int mbr_tamanio;
    time_t mbr_fecha_creacion;
    int mbr_disk_signature;
    char mbr_disk_fit;
    Partition mbr_partitions[4];
}MBR;
#endif // ESTRUCTURAS_H
