create database  if not EXISTS Practica1;

use Practica1;

SET FOREIGN_KEY_CHECKS=0;

DROP TABLE IF EXISTS countrycodes;
DROP TABLE IF EXISTS locations;
DROP TABLE IF EXISTS currenci;
DROP TABLE IF EXISTS geoname;
DROP TABLE IF EXISTS level;
DROP TABLE IF EXISTS transaction;
DROP TABLE IF EXISTS status;
DROP TABLE IF EXISTS project;


create table geoname(
    id int(11) NOT NULL AUTO_INCREMENT PRiMARY KEY,
    idGeonama int(11) NOT NULL,
    place_name varchar(100)NOT NULL,
    latitude decimal(16,5)NOT NULL,
    longitude decimal(16,5)NOT NULL,
    location_type_code int(11)NOT NULL,
    gazetteer_adm_code varchar(250)NOT NULL,
    gazetteer_adm_name varchar(250)NOT NULL,
    location_class int(2)NOT NULL,
    geographic_exactness int(2)NOT NULL
);

create table level(
    id int(11) NOT NULL AUTO_INCREMENT PRiMARY KEY,
    project_id int(11) NOT NULL,
    project_location_id varchar(15)NOT NULL,
    geoname_id int(11) NOT NULL,
    transactions_start_year int(11)NOT NULL,
    transactions_end_year int(11)NOT NULL,
    even_split_commitments decimal(16,5) NOT NULL,
    even_split_disbursements decimal(16,5) NOT NULL
);

create table locations(
    idLocation int(11) NOT NULL AUTO_INCREMENT PRiMARY KEY,
    location_type_code varchar(10)NOT NULL,
    location_type_name varchar(250)NOT NULL
);

create table status(
    idStatus int(11) NOT NULL AUTO_INCREMENT PRiMARY KEY,
    name varchar(250)NOT NULL
);

create table currenci(
    idCurrency int(11) NOT NULL AUTO_INCREMENT PRiMARY KEY,
    name_currency varchar(250)NOT NULL
);

create table project(
    idProyecto int(11) NOT NULL AUTO_INCREMENT PRiMARY KEY,
    project_id varchar(15) NOT NULL,
    is_geocoded int(2) NOT NULL,
    project_title varchar(250) NOT NULL,
    start_actual_isodate date NOT NULL,
    end_actual_isodate date NOT NULL,
    donors varchar(15) NOT NULL,
    donors_iso3 varchar(5) NOT NULL,
    recipients int(11) NULL,
    recipients_iso3 varchar(5) NOT NULL,
    ad_sector_codes varchar(200) NOT NULL,
    ad_sector_names varchar(200) NOT NULL,
    status int(11) NOT NULL,
    transactions_start_year int(11)NOT NULL,
    transactions_end_year int(11)NOT NULL,
    total_commitments decimal(16,5)NOT NULL,
    total_disbursements decimal(16,5)NOT NULL
);




create table countrycodes(
    id int(11) NOT NULL AUTO_INCREMENT PRiMARY KEY,
    name varchar(50) NOT NULL,
    iso2 varchar(15) NOT NULL,
    name_name varchar(50) NOT NULL,
    name_aiddata_code int(11) NOT NULL,
    name_aiddata_name varchar(50) NOT NULL,
    name_cow_alpha varchar(15) NOT NULL,
    name_cow_numeric int(5) NOT NULL,
    name_fao_code int(5) NOT NULL,
    name_fips varchar(6) NOT NULL,
    name_geonames_id int(11) NOT NULL,
    name_imf_code int(11) NOT NULL,
    name_iso2 varchar(6) NOT NULL,
    name_iso3 varchar(6)NOT NULL,
    name_iso_numeric int(11) NOT NULL,
    name_oecd_code int(11)NOT NULL,
    name_oecd_name varchar(50)NOT NULL,
    name_un_code int(11) NOT NULL,
    name_wb_code varchar(11) NOT NULL
);

create table transaction(
    id int(11) NOT NULL AUTO_INCREMENT PRiMARY KEY,
    id_Transaction varchar(25) NOT NULL,
    idProyecto int(11) NOT NULL,
    transaction_isodate date NOT NULL,
    transactions_year int(11)NOT NULL,
    transaction_value_code varchar(5)NOT NULL,
    transaction_currency int(11)NOT NULL,
    transaction_value decimal(16,5)NOT NULL
);

ALTER TABLE transaction
ADD FOREIGN KEY (transaction_currency) REFERENCES currenci(idCurrency); 

ALTER TABLE transaction
ADD FOREIGN KEY (idProyecto) REFERENCES project(idProyecto);

ALTER TABLE project
ADD FOREIGN KEY (status) REFERENCES status(idStatus);

ALTER TABLE project
ADD FOREIGN KEY (recipients) REFERENCES countrycodes(id);

ALTER TABLE level
ADD FOREIGN KEY (project_id) REFERENCES project(idProyecto);

ALTER TABLE level
ADD FOREIGN KEY (geoname_id) REFERENCES geoname(id);

ALTER TABLE geoname
ADD FOREIGN KEY (location_type_code) REFERENCES locations(idLocation);
create TEMPORARY table geoname_temp(
    id int(11) NOT NULL AUTO_INCREMENT PRiMARY KEY,
    geoname_id int(11) NOT NULL,
    place_name varchar(100)NOT NULL,
    latitude decimal(16,5)NOT NULL,
    longitude decimal(16,5)NOT NULL,
    location_type_code varchar(200)NOT NULL,
    location_type_name varchar(250)NOT NULL,
    gazetteer_adm_code varchar(250)NOT NULL,
    gazetteer_adm_name varchar(250)NOT NULL,
    location_class int(2)NOT NULL,
    geographic_exactness int(2)NOT NULL
);

create TEMPORARY table level_temp(
    id int(11) NOT NULL AUTO_INCREMENT PRiMARY KEY,
    project_id varchar(15) NOT NULL,
    project_location_id varchar(30)NOT NULL,
    geoname_id int(11) NOT NULL,
    transactions_start_year int(11)NOT NULL,
    transactions_end_year int(11)NOT NULL,
    even_split_commitments decimal(16,5) NOT NULL,
    even_split_disbursements decimal(16,5) NOT NULL
);

create TEMPORARY table locations_temp(
    idLocation int(11) NOT NULL AUTO_INCREMENT PRiMARY KEY,
    location_type_code varchar(20)NOT NULL,
    location_type_name varchar(250)NOT NULL
);


create TEMPORARY table project_temp(
    idProyecto int(11) NOT NULL AUTO_INCREMENT PRiMARY KEY,
    project_id varchar(15) NOT NULL,
    is_geocoded int(2) NOT NULL,
    project_title varchar(250) NOT NULL,
    start_actual_isodate date NOT NULL,
    end_actual_isodate date NOT NULL,
    donors varchar(15) NOT NULL,
    donors_iso3 varchar(5) NOT NULL,
    recipients varchar(20) NOT NULL,
    recipients_iso3 varchar(25) NOT NULL,
    ad_sector_codes varchar(200) NOT NULL,
    ad_sector_names varchar(200) NOT NULL,
    status varchar(250) NOT NULL,
    transactions_start_year int(11)NOT NULL,
    transactions_end_year int(11)NOT NULL,
    total_commitments decimal(16,5)NOT NULL,
    total_disbursements decimal(16,5)NOT NULL
);


create TEMPORARY table countrycodes_temp(
    id int(11) NOT NULL AUTO_INCREMENT PRiMARY KEY,
    name varchar(50) NOT NULL,
    iso2 varchar(15) NOT NULL,
    name_name varchar(50) NOT NULL,
    name_aiddata_code int(11) NOT NULL,
    name_aiddata_name varchar(50) NOT NULL,
    name_cow_alpha varchar(15) NOT NULL,
    name_cow_numeric int(5) NOT NULL,
    name_fao_code int(5) NOT NULL,
    name_fips varchar(6) NOT NULL,
    name_geonames_id int(11) NOT NULL,
    name_imf_code int(11) NOT NULL,
    name_iso2 varchar(6) NOT NULL,
    name_iso3 varchar(6)NOT NULL,
    name_iso_numeric int(11) NOT NULL,
    name_oecd_code int(11)NOT NULL,
    name_oecd_name varchar(50)NOT NULL,
    name_un_code int(11) NOT NULL,
    name_wb_code varchar(11) NOT NULL
);

create TEMPORARY table transaction_temp(
    id int(11) NOT NULL AUTO_INCREMENT PRiMARY KEY,
    transaction_id varchar(25) NOT NULL,
    project_id varchar(15) NOT NULL,
    transaction_isodate date NOT NULL,
    transaction_year int(11)NOT NULL,
    transaction_value_code varchar(5)NOT NULL,
    transaction_currency varchar(250)NOT NULL,
    transaction_value decimal(16,5)NOT NULL
);



#----------------tabla Country codes ambas

LOAD DATA INFILE '/var/lib/mysql-files/country_codes.csv' 
IGNORE
INTO TABLE countrycodes_temp 
fields terminated by '\t' 
lines terminated by '\n' 
ignore 1 lines(name,iso2,name_name,name_aiddata_code,name_aiddata_name,name_cow_alpha,name_cow_numeric,name_fao_code,name_fips,name_geonames_id,name_imf_code,name_iso2,name_iso3,name_iso_numeric,name_oecd_code,name_oecd_name,name_un_code,name_wb_code);

delete from countrycodes_temp where name='Unspecified';


#----------------tabla locations ambas

LOAD DATA INFILE '/var/lib/mysql-files/locations.csv' 
INTO TABLE locations_temp 
FIELDS TERMINATED BY '\t' 
lines terminated by '\n' 
ignore 1 lines(location_type_code,location_type_name);

INSERT INTO locations 
SELECT * FROM locations_temp;

SELECT count(*) as tablaLocations FROM locations;



#----------------tabla Projects Temp

LOAD DATA   INFILE '/var/lib/mysql-files/projects.csv' 
ignore
INTO TABLE project_temp 
CHARACTER SET latin1
FIELDS TERMINATED BY ',' 
OPTIONALLY ENCLOSED BY '"'
LINES terminated by '\n' 
ignore 1 lines(project_id,is_geocoded,project_title,@var1,@var2,donors,donors_iso3,recipients,recipients_iso3,ad_sector_codes,ad_sector_names,status,transactions_start_year,transactions_end_year,total_commitments,total_disbursements)
set start_actual_isodate=STR_TO_DATE(@var1,'%d/%m/%Y %H:%i:%S'),
end_actual_isodate=STR_TO_DATE(@var2,'%d/%m/%Y %H:%i:%S');




#----------------tabla Level Temp


LOAD DATA INFILE '/var/lib/mysql-files/level_1a.csv' 
IGNORE
INTO TABLE level_temp 
FIELDS TERMINATED BY ',' 
lines terminated by '\n' 
ignore 1 lines(project_id,project_location_id,geoname_id,transactions_start_year,transactions_end_year,even_split_commitments,even_split_disbursements);

#----------------tabla Transactions Temp


LOAD DATA INFILE '/var/lib/mysql-files/transactions.csv' 
INTO TABLE transaction_temp 
FIELDS TERMINATED BY ',' 
lines terminated by '\n' 
ignore 1 lines(transaction_id,project_id,@var1,transaction_year,transaction_value_code,transaction_currency,transaction_value)
set transaction_isodate = STR_TO_DATE(@var1,'%d/%m/%Y %H:%i:%S');


#----------------tabla Geonames Temp


LOAD DATA INFILE '/var/lib/mysql-files/geonames.csv' 
IGNORE
INTO TABLE geoname_temp 
FIELDS TERMINATED BY ',' 
OPTIONALLY ENCLOSED BY '"'
lines terminated by '\n' 
ignore 1 lines(geoname_id,place_name,latitude,longitude,location_type_code,location_type_name,gazetteer_adm_code,gazetteer_adm_name,location_class,geographic_exactness);




#----------------tabla status

INSERT INTO status(name)
SELECT DISTINCT status FROM project_temp;

SELECT count(*) as tablaStatus from status;

#----------------tabla currency
INSERT INTO currenci(name_currency) 
SELECT DISTINCT transaction_currency FROM transaction_temp;

SELECT count(*) as tablaCurrenci from currenci;

#----------------tabla countrycodes NORMAL


INSERT INTO countrycodes 
SELECT * FROM countrycodes_temp;
SELECT count(*) as countrycodes from countrycodes;


#----------------tabla project NORMAL



INSERT INTO project(project_id, is_geocoded, project_title, start_actual_isodate , end_actual_isodate,donors, donors_iso3, project.recipients, recipients_iso3, ad_sector_codes, ad_sector_names, project.status, transactions_start_year, transactions_end_year, total_commitments, total_disbursements) 
 SELECT project_id, is_geocoded, project_title, start_actual_isodate,end_actual_isodate, donors, donors_iso3, (SELECT id from countrycodes where countrycodes.name=project_temp.recipients), recipients_iso3, ad_sector_codes, ad_sector_names, (SELECT idStatus from status where status.name=project_temp.status) , transactions_start_year, transactions_end_year, total_commitments, total_disbursements 
 FROM project_temp;

SELECT count(*) as tablaProject from project;





#----------------tabla transaction NORMAL

INSERT  INTO transaction(id_Transaction, transaction.idProyecto, transaction_isodate, transactions_year, transaction_value_code, transaction.transaction_currency, transaction_value)
 SELECT transaction_id, (SELECT idProyecto from project where project.project_id=transaction_temp.project_id), transaction_isodate, transaction_year, transaction_value_code, (SELECT idCurrency from currenci where currenci.name_currency=transaction_temp.transaction_currency), transaction_value 
 FROM transaction_temp;

SELECT count(*) as tablaTransaction from transaction;



 #----------------tabla geoname NORMAL

INSERT  INTO geoname(idGeonama, place_name, latitude, longitude, geoname.location_type_code, gazetteer_adm_code, gazetteer_adm_name, location_class, geographic_exactness)
 SELECT geoname_id, place_name, latitude, longitude, (SELECT idLocation from locations where locations.location_type_code=geoname_temp.location_type_code), gazetteer_adm_code, gazetteer_adm_name, location_class, geographic_exactness
 FROM geoname_temp;

SELECT count(*) as tablaGeoname from geoname;

#----------------tabla level NORMAL


DELETE from geoname_temp;

insert into geoname_temp (id, geoname_id, place_name, latitude, longitude, location_type_code,location_type_name,gazetteer_adm_code,gazetteer_adm_name,location_class,geographic_exactness) 
SELECT DISTINCT geoname.id, geoname.idGeonama, geoname.place_name, geoname.latitude, geoname.longitude, geoname.location_type_code,' ', geoname.gazetteer_adm_code,geoname.gazetteer_adm_name,geoname.location_class,geoname.geographic_exactness
FROM geoname GROUP BY  geoname.idGeonama
HAVING COUNT(geoname.idGeonama) >= 1;


insert into level (project_id, project_location_id, geoname_id, transactions_start_year, transactions_end_year, even_split_commitments, even_split_disbursements)
select DISTINCT  project.idProyecto, level_temp.project_location_id, geoname_temp.id, level_temp.transactions_start_year, level_temp.transactions_end_year, level_temp.even_split_commitments, level_temp.even_split_disbursements from level_temp inner join project inner join geoname_temp   where project.project_id=level_temp.project_id and geoname_temp.geoname_id=level_temp.geoname_id   ;

SELECT count(*) as tablaLevel from level;
#---------------Consulta 1
select count(*) as Proyectos2011 from project where transactions_start_year=2011;

#---------------Consulta 2

SELECT idProyecto as ID, project_title as Proyecto, TIMESTAMPDIFF(DAY, start_actual_isodate, end_actual_isodate) AS Dias_transcurridos from project order by Dias_transcurridos desc limit 1 ;


#---------------Consulta 3

SELECT idProyecto as ID, project_title as Proyecto, TIMESTAMPDIFF(DAY, start_actual_isodate, end_actual_isodate) AS Dias_transcurridos from project where end_actual_isodate != '0000-00-00' order by Dias_transcurridos asc limit 1 ;


#---------------consulta 4

select name, count(recipients) as cantidad from project join countrycodes where countrycodes.id=project.recipients group by name having count(recipients)>0 order by cantidad desc limit 5;

#---------------consulta 5

select name, count(recipients) as cantidad from project join countrycodes where countrycodes.id=project.recipients group by name having count(recipients)>0 order by cantidad asc limit 5;

#---------------consulta 6

select project_title as Proyeto, total_commitments as costo from project order by total_commitments desc limit 1;

#---------------consulta 7

select project_title as Proyeto, total_commitments as costo from project  order by total_commitments asc limit 1;

#---------------consulta 8


select project_title as Proyeto, total_commitments as costo from project order by total_commitments desc limit 5;

#---------------consulta 9

select sum(total_commitments) as costoTotal from project;

#---------------consulta 10

select transaction.id, project_title, transaction_value as valor from transaction inner join project where project.idProyecto=transaction.idProyecto and project_title LIKE '%RI-East Africa Public Health Laboratory Networking Project%' order by transaction_value desc limit 3;

