<?php

// Abre a conexão ao Oracle:
require('db.inc.php');
$db = new \Oracle\Db("UPDATE", "Cofre-IoT");

// Obtém valores da URL
$v1 = filter_input(INPUT_GET, 'id', FILTER_VALIDATE_INT, FILTER_SANITIZE_NUMBER_INT);
$v2 = filter_input(INPUT_GET, 'numero', FILTER_SANITIZE_NUMBER_FLOAT, FILTER_FLAG_ALLOW_FRACTION);

// Query SQL:
$sql = "UPDATE valores SET saldo = saldo + (:v2_bv) WHERE id = (:v1_bv)";

// Executa o insert:
$db->execute($sql, "Update Query", array(array(":v1_bv", $v1, -1),
                                         array(":v2_bv", $v2, -1)));
?>