
create database Practica1;
use Practica1;

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
    recipients int(11) NOT NULL,
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