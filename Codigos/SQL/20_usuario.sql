/* ====================================================================== */
/* Projeto Cofre-IoT                                                      */
/* Criação do usuário                                                     */
/* ====================================================================== */
/* Abrantes Araújo Silva Filho                                            */
/* abrantesasf@gmail.com                                                  */
/* ====================================================================== */

CREATE USER cofreiot IDENTIFIED BY <insira aqui a senha>
DEFAULT TABLESPACE "COFREIOT_DADOS"
TEMPORARY TABLESPACE "TEMP";

ALTER USER cofreiot QUOTA UNLIMITED ON COFREIOT_LOBS;
ALTER USER cofreiot QUOTA UNLIMITED ON COFREIOT_DADOS;
ALTER USER cofreiot QUOTA UNLIMITED ON COFREIOT_INDICES;

GRANT "CONNECT" TO cofreiot;
GRANT "RESOURCE" TO cofreiot;

GRANT CREATE TRIGGER TO cofreiot ;
GRANT CREATE SEQUENCE TO cofreiot ;
GRANT CREATE TABLE TO cofreiot ;
GRANT CREATE JOB TO cofreiot ;
GRANT CREATE PROCEDURE TO cofreiot ;
GRANT CREATE SYNONYM TO cofreiot ;
GRANT CREATE VIEW TO cofreiot ;
GRANT CREATE TYPE TO cofreiot ;
GRANT CREATE PUBLIC SYNONYM TO cofreiot ;
GRANT CREATE PUBLIC DATABASE LINK TO cofreiot ;
GRANT CREATE INDEXTYPE TO cofreiot ;
GRANT CREATE MATERIALIZED VIEW TO cofreiot ;
GRANT CREATE DATABASE LINK TO cofreiot ;
GRANT ALTER SESSION TO cofreiot ;
GRANT CREATE USER TO cofreiot ;