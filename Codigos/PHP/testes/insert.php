<?php

// Abre a conexão ao Oracle:
require('db.inc.php');
$db = new \Oracle\Db("INSERT", "Cofre-IoT");

// Obtém valores da URL
$v1 = filter_input(INPUT_GET, 'id', FILTER_VALIDATE_INT, FILTER_SANITIZE_NUMBER_INT);
$v2 = filter_input(INPUT_GET, 'nome', FILTER_SANITIZE_STRING);

// Query SQL:
$sql = "INSERT INTO teste_esp8266 VALUES (:v1_bv, :v2_bv)";

// Executa o insert:
$db->execute($sql, "Insert Example", array(array(":v1_bv", $v1, -1),
                                           array(":v2_bv", $v2, -1)));
?>