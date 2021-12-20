
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


