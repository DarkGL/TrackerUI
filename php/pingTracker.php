<?php
if( !isset( $_GET[ 'token' ] ) || strlen( $_GET[ 'token' ] ) != 32 ){
	die();
}

include 'config.php';

$token = $_GET[ 'token' ];

try{
	$pdo = new PDO('mysql:host='.$mysql_host.';dbname='.$database.';port='.$port, $username, $password );
		
	$pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	
	$stmt = $pdo -> prepare('SELECT count(*) FROM `tracker_exe_clients` WHERE `token` = :token' );
	
	$stmt -> bindValue( ':token', $token , PDO::PARAM_STR );
	
	$stmt -> execute();
	
	$rows = $stmt -> fetch( PDO::FETCH_NUM );
	
	echo $rows[ 0 ];
	
	if( $rows[ 0 ] != 0 ){
	
		$stmtUpdate = $pdo -> prepare('UPDATE `tracker_clients` SET `time` = :time WHERE `token` = :token)' );
	 
		$stmtUpdate -> bindValue( ':token', $token , PDO::PARAM_STR );
		$stmtUpdate -> bindValue( ':time', time() , PDO::PARAM_STR );
	
		$stmtUpdate -> execute();
	}
	 
}catch(PDOException $e){

}

?>