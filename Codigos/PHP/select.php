<?php

// Abre a conexão ao Oracle:
require('db.inc.php');
$db = new \Oracle\Db("SELECT", "Cofre-IoT");

// Obtém valores da URL
$v1 = filter_input(INPUT_GET, 'id', FILTER_VALIDATE_INT, FILTER_SANITIZE_NUMBER_INT);
$v2 = filter_input(INPUT_GET, 'numero', FILTER_SANITIZE_NUMBER_FLOAT, FILTER_FLAG_ALLOW_FRACTION);

// Query SQL:
$sql = "SELECT saldo FROM valores WHERE id = (:v1_bv) and id = (:v2_bv)";

// Executa a query:
$res = $db->execFetchAll($sql, "Query Example", array(array(":v1_bv", $v1, -1),
                                                      array(":v2_bv", $v2, -1)
                                                     )
                        );

foreach ($res as $row) {
    $saldo = htmlspecialchars($row['SALDO'], ENT_NOQUOTES, 'UTF-8');
    echo "$saldo\r";
}


?>