/* ====================================================================== */
/* Projeto Cofre-IoT                                                      */
/* Criação das tablespaces                                                */
/* ====================================================================== */
/* Abrantes Araújo Silva Filho                                            */
/* abrantesasf@gmail.com                                                  */
/* ====================================================================== */

CREATE TABLESPACE COFREIOT_DADOS
    DATAFILE '/u01/app/oracle/oradata/XE/COFRE_IOT_DADOS.DBF'
    SIZE 52428800 -- 50M
    REUSE
    AUTOEXTEND ON NEXT 10485760 -- 10M
    MAXSIZE 4294967296 -- 4G
    ONLINE 
SEGMENT SPACE MANAGEMENT AUTO;

CREATE TABLESPACE COFREIOT_INDICES
    DATAFILE '/u01/app/oracle/oradata/XE/COFRE_IOT_INDICES.DBF'
    SIZE 52428800 -- 50M
    REUSE
    AUTOEXTEND ON NEXT 10485760 -- 10M
    MAXSIZE 4294967296 -- 4G
    ONLINE 
SEGMENT SPACE MANAGEMENT AUTO;

CREATE TABLESPACE COFREIOT_LOBS
    DATAFILE '/u01/app/oracle/oradata/XE/COFRE_IOT_LOBS.DBF'
    SIZE 52428800 -- 50M
    REUSE
    AUTOEXTEND ON NEXT 10485760 -- 10M
    MAXSIZE 4294967296 -- 4G
    ONLINE 
SEGMENT SPACE MANAGEMENT AUTO;