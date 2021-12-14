#Crea 2 discos

#CREANDO DISCOS ----------------------------------------------------------------------------------------
mkdisk -size~:~75 -unit~:~M -path~:~/home/archivos/fase2/Disco1.disk -fit~:~FF
pause
mkdisk -unit~:~K -size~:~51200 -path~:~/home/archivos/fase2/Disco2.disk -fit~:~FF
pause
#CREANDO PARTICIONES ----------------------------------------------------------------------------------
#Crear particiones Disco1.disk
pause
fdisk -size~:~20 -type~:~P -unit~:~M -name~:~Part1  -path~:~/home/archivos/fase2/Disco1.disk -fit~:~BF
pause
fdisk -size~:~20 -type~:~P -unit~:~M -name~:~Part1  -path~:~/home/archivos/vacas/Disco1.disk -fit~:~BF
pause
fdisk -size~:~20 -type~:~E -unit~:~M -name~:~Part2  -path~:~/home/archivos/fase2/Disco1.disk -fit~:~BF
pause
fdisk -size~:~5 -type~:~L -unit~:~M -name~:~Part4L  -path~:~/home/archivos/fase2/Disco1.disk -fit~:~BF
pause
fdisk -size~:~5 -type~:~L -unit~:~M -name~:~Part5L  -path~:~/home/archivos/fase2/Disco1.disk -fit~:~BF
pause
fdisk -type~:~P -unit~:~M -name~:~Part2 -size~:~15 -path~:~/home/archivos/fase1/Disco1.disk -fit~:~BF
pause
fdisk -type~:~P -unit~:~M -name~:~Part3 -size~:~20 -path~:~/home/archivos/fase2/Disco1.disk -fit~:~BF
pause
fdisk -delete~:~fast -name~:~Part2 -path~:~/home/archivos/fase2/Disco1.disk 
pause
fdisk -delete~:~full -name~:~Part2 -path~:~/home/archivos/fase2/Disco1.disk 
pause
#Crear particiones Disco2.disk
fdisk -type~:~P -unit~:~M -name~:~Part1_D2 -size~:~20 -path~:~/home/archivos/fase1/Disco2.disk -fit~:~BF
pause
fdisk -type~:~P -unit~:~M -name~:~Part2_D2 -size~:~10 -path~:~/home/archivos/fase1/Disco2.disk -fit~:~BF
pause
fdisk -type~:~P -unit~:~M -name~:~Part3_D2 -size~:~10 -path~:~/home/archivos/fase1/Disco2.disk -fit~:~BF
pause

fdisk -add~:~25 -unit~:~m -name~:~Part2  -path~:~/home/archivos/fase2/Disco1.disk 
#Monta las particiones 
pause


mount -path~:~/home/archivos/fase2/Disco1.disk -name~:~Part1 
mount -path~:~/home/archivos/vacas/Disco1.disk -name~:~Part1 
pause

mount -path~:~/home/archivos/fase2/Disco1.disk -name~:~Part2 
mount -path~:~/home/archivos/fase2/Disco1.disk -name~:~Part3 
pause
mount -path~:~/home/archivos/fase2/Disco1.disk -name~:~Part4L 
mount -path~:~/home/archivos/fase2/Disco2.disk -name~:~Part1_D2 
pause
mount -path~:~/home/archivos/fase2/Disco2.disk -name~:~Part2_D2 
mount -path~:~/home/archivos/fase2/Disco2.disk -name~:~Part3_D2 
pause
#Crea el sistema de archivos en todas las particiones primarias
#fast reescribe el superbloque 
#full poner /0 y luego reescribir el superbloque

mkfs -type~:~full -id~:~vda1
pause
mkfs -type~:~fast -id~:~vdb1 -fs~:~3fs
pause
mkfs -type~:~full -id~:~vdc1
pause
mkfs -type~:~fast -id~:~vda2 -fs~:~3fs
pause

