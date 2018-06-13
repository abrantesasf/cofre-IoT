/* ====================================================================== */
/* Projeto Cofre-IoT                                                      */
/* Criação da tabela de teste                                             */
/* ====================================================================== */
/* Abrantes Araújo Silva Filho                                            */
/* abrantesasf@gmail.com                                                  */
/* ====================================================================== */

--DROP TABLE valores CASCADE CONSTRAINTS;
CREATE TABLE valores (
    id        NUMBER(15)                 CONSTRAINT nn_valores_id        NOT NULL,
    saldo     NUMBER(20,5)               CONSTRAINT nn_saldo             NOT NULL,
    CONSTRAINT pk_valores PRIMARY KEY (id) USING INDEX TABLESPACE cofreiot_indices
)
TABLESPACE cofreiot_dados;

ALTER TABLE valores ADD CONSTRAINT cc_saldo_positivo
    CHECK (saldo >= 0);

COMMENT ON TABLE  valores       IS 'Tabela que armazena o saldo atual de cada cofre';
COMMENT ON COLUMN valores.id    IS 'Chave primária, identifica unicamente cada cofre';
COMMENT ON COLUMN valores.saldo IS 'Saldo atual';

--DROP SEQUENCE seq_valores;
CREATE SEQUENCE seq_valores
    START WITH 1
    INCREMENT BY 1
    MINVALUE 1
    MAXVALUE 999999999999999
    NOCYCLE
    NOCACHE
    NOORDER;

-- DROP TRIGGER trg_bi_valores;
CREATE OR REPLACE TRIGGER trg_bi_valores
   BEFORE INSERT ON VALORES
   FOR EACH ROW
BEGIN
   IF :new.id IS NULL THEN
     SELECT seq_valores.nextval into :new.id from dual;
  END IF;

END trg_bi_valores;
/