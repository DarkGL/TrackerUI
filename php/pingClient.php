<?php
if( !isset( $_GET[ 'token' ] ) || strlen( $_GET[ 'token' ] ) != 32 ){
	die();
}

include 'config.php';

$token = '';
$version = '';

if( isset( $_GET[ 'token' ] ) ){
	$token = $_GET[ 'token' ];
}
	
if( isset( $_GET[ 'version' ] ) ){
	$version = $_GET[ 'version' ];
}

try{
	$pdo = new PDO('mysql:host='.$mysql_host.';dbname='.$database.';port='.$port, $username, $password );
		
	$pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	
	$stmt = $pdo -> prepare('SELECT count(*) FROM `tracker_clients` WHERE `token` = :token' );
	
	$stmt -> bindValue( ':token', $token , PDO::PARAM_STR );
	
	$stmt -> execute();
	
	$rows = $stmt -> fetch( PDO::FETCH_NUM );
	
	if( $rows[ 0 ] != 0 ){
	
		$stmtUpdate = $pdo -> prepare('UPDATE `tracker_clients` SET `time` = :time WHERE `token` = :token)' );
	 
		$stmtUpdate -> bindValue( ':token', $token , PDO::PARAM_STR );
		$stmtUpdate -> bindValue( ':time', time() , PDO::PARAM_STR );
	
		$stmtUpdate -> execute();
	}
	else{
		$stmt = $pdo -> prepare('INSERT INTO `tracker_clients` ( `token` , `time` , `ip` , `version` ) VALUES ( :token , :time , :ip , :version )' );
	 
		$stmt -> bindValue( ':token', $token , PDO::PARAM_STR );
		$stmt -> bindValue( ':time', time() , PDO::PARAM_STR );
		$stmt -> bindValue( ':ip', $_SERVER['REMOTE_ADDR'] , PDO::PARAM_STR );
		$stmt -> bindValue( ':version' , $version , PDO::PARAM_STR );
	}
	 
}catch(PDOException $e){

}

?>