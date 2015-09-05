<?php
include 'config.php';

$pdo = new PDO('mysql:host='.$mysql_host.';dbname='.$database.';port='.$port, $username, $password );
		
$pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

$stmtDelete = $pdo -> prepare( 'DELETE FROM `tracker_clients` WHERE `time` < :time - 600' );
	
$stmtDelete -> bindValue( ':time' , time() , PDO::PARAM_STR );
	
$stmtDelete -> execute();

$stmtDelete = $pdo -> prepare( 'DELETE FROM `tracker_exe_clients` WHERE `time` < :time - 600' );
	
$stmtDelete -> bindValue( ':time' , time() , PDO::PARAM_STR );
	
$stmtDelete -> execute();


?>