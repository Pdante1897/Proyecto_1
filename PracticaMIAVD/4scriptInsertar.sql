
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
