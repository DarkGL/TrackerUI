<?php
if( !isset( $_GET[ 'token' ] ) || strlen( $_GET[ 'token' ] ) != 32 ){
	die();
}

include 'config.php';

$token = $_GET[ 'token' ];

try{
	$pdo = new PDO('mysql:host='.$mysql_host.';dbname='.$database.';port='.$port, $username, $password );
		
	$pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	
	$stmt = $pdo -> prepare('DELETE FROM `tracker_clients` WHERE `token` = :token' );
	 
	$stmt -> bindValue( ':token', $token , PDO::PARAM_STR );
	
	$stmt -> execute();
	 
}catch(PDOException $e){

}

?>