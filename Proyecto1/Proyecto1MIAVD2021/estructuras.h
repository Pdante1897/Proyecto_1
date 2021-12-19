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

typedef struct{
    char part_status;
    char part_fit;
    int part_start;
    int part_size;
    int part_next;
    char part_name[16];
}EBR;

typedef struct{
    int s_filesystem_type;
    int s_inodes_count;
    int s_blocks_count;
    int s_free_blocks_count;
    int s_free_inodes_count;
    time_t s_mtime;
    time_t s_umtime;
    int s_mnt_count;
    int s_magic;
    int s_inode_size;
    int s_block_size;
    int s_first_ino;
    int s_first_blo;
    int s_bm_inode_start;
    int s_bm_block_start;
    int s_inode_start;
    int s_block_start;
}SuperBloque;

typedef struct{
    int i_uid;
    int i_gid;
    int i_size;
    time_t i_atime;
    time_t i_ctime;
    time_t i_mtime;
    int i_block[15];
    char i_type;
    int i_perm;
}TablaInodos;

typedef struct{
    char b_name[12];
    int b_inodo;
}Content;

typedef struct{
    Content b_content[4];
}BloqueCarpeta;

typedef struct{
    char b_content[64];
}BloqueDeArchivos;

typedef struct{
    int b_pointers[16];
}BloqueDeApuntadores;


typedef struct{
    char Journal__Tipo_Operacion[10];
    int Journal__tipo;//Archivo/Carpeta
    char Journal__nombre[100];
    char Journal__contenido[100];
    time_t Journal__fecha;
    int journal_propietario;
    int Journal__permisos;
}Journal;
#endif // ESTRUCTURAS_H
