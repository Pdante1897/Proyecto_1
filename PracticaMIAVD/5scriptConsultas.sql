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

