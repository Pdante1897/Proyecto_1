cp country_codes.csv /var/lib/mysql-files/
cp geonames.csv /var/lib/mysql-files/
cp level_1a.csv /var/lib/mysql-files/
cp locations.csv /var/lib/mysql-files/
cp projects.csv /var/lib/mysql-files/
cp transactions.csv /var/lib/mysql-files/
sudo cat *.sql  > comandos.sql
mysql -h localhost -u root -p <comandos.sql 
