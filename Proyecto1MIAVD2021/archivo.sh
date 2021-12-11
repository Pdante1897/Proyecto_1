#Contenido de calificacion.sh
#Crea 2 discos

#CREANDO DISCOS ----------------------------------------------------------------------------------------
mkdisk -size~:~75 -unit~:~M -path~:~/home/archivos/fase2/Disco1.disk -fit~:~FF
mkdisk -unit~:~K -size~:~51200 -path~:~/home/archivos/fase2/Disco2.disk -fit~:~FF

rmdisk -path~:~/home/archivos/fase2/Disco1.disk
#CREANDO PARTICIONES ----------------------------------------------------------------------------------
#Crear particiones Disco1.disk
fdisk -size~:~20 -type~:~P -unit~:~M -name~:~Part1  -path~:~/home/archivos/fase2/Disco1.disk -fit~:~BF
fdisk -size~:~20 -type~:~E -unit~:~M -name~:~Part2  -path~:~/home/archivos/fase2/Disco1.disk -fit~:~BF
fdisk -size~:~5 -type~:~L -unit~:~M -name~:~Part4L  -path~:~/home/archivos/fase2/Disco1.disk -fit~:~BF
fdisk -type~:~P -unit~:~M -name~:~Part2 -size~:~15 -path~:~/home/archivos/fase1/Disco1.disk -fit~:~BF
fdisk -type~:~P -unit~:~M -name~:~Part3 -size~:~20 -path~:~/home/archivos/fase2/Disco1.disk -fit~:~BF
fdisk -delete~:~fast -name~:~Part2 -path~:~/home/archivos/fase2/Disco1.disk 
fdisk -delete~:~full -name~:~Part2 -path~:~/home/archivos/fase2/Disco1.disk 
#Crear particiones Disco2.disk
fdisk -type~:~P -unit~:~M -name~:~Part1_D2 -size~:~20 -path~:~/home/archivos/fase1/Disco2.disk -fit~:~BF
fdisk -type~:~P -unit~:~M -name~:~Part2_D2 -size~:~10 -path~:~/home/archivos/fase1/Disco2.disk -fit~:~BF
fdisk -type~:~P -unit~:~M -name~:~Part3_D2 -size~:~10 -path~:~/home/archivos/fase1/Disco2.disk -fit~:~BF

fdisk -add~:~25 -unit~:~m -name~:~Part2  -path~:~/home/archivos/fase2/Disco1.disk 
#Monta las particiones 



mount -path~:~/home/archivos/fase2/Disco1.disk -name~:~Part1 #451a
mount -path~:~/home/archivos/fase2/Disco1.disk -name~:~Part2 #451b
mount -path~:~/home/archivos/fase2/Disco1.disk -name~:~Part3 #451c

mount -path~:~/home/archivos/fase2/Disco2.disk -name~:~Part1_D2 #452a
mount -path~:~/home/archivos/fase2/Disco2.disk -name~:~Part2_D2 #452b
mount -path~:~/home/archivos/fase2/Disco2.disk -name~:~Part3_D2 #452c

#Crea el sistema de archivos en todas las particiones primarias
#fast reescribe el superbloque 
#full poner /0 y luego reescribir el superbloque

mkfs -type~:~fast -id~:~451a -fs~:~3fs
mkfs -type~:~full -id~:~451b
mkfs -type~:~fast -id~:~451c -fs~:~3fs
