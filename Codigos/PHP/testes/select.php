<?php

// test_db.php

require('db.inc.php');

$db = new \Oracle\Db("select", "Cofre-IoT");
$sql = "SELECT id AS id, nome AS nome FROM teste_esp8266";
$res = $db->execFetchAll($sql, "Query Example");
// echo "<pre>"; var_dump($res); echo "</pre>\n";

echo "<table border='1'>\n";
echo "<tr><th>ID</th><th>Nome</th></tr>\n";
foreach ($res as $row) {
    $name = htmlspecialchars($row['ID'], ENT_NOQUOTES, 'UTF-8');
    $pn   = htmlspecialchars($row['NOME'], ENT_NOQUOTES, 'UTF-8');
    echo "<tr><td>$name</td><td>$pn</td></tr>\n";
}
echo "</table>";

?>