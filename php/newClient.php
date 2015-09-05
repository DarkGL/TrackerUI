<?php
include 'config.php';

try{
	$version = '';
	
	if( isset( $_GET[ 'version' ] ) ){
		$version = $_GET[ 'version' ];
	}
	
	$pdo = new PDO('mysql:host='.$mysql_host.';dbname='.$database.';port='.$port, $username, $password );
		
	$pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	
	$token = substr( md5( rand() ) , 0 , 32 );
	
	$stmt = $pdo -> prepare('INSERT INTO `tracker_clients` ( `token` , `time` , `ip` , `version` ) VALUES ( :token , :time , :ip , :version )' );
	 
	$stmt -> bindValue( ':token', $token , PDO::PARAM_STR );
    $stmt -> bindValue( ':time', time() , PDO::PARAM_STR );
    $stmt -> bindValue( ':ip', $_SERVER['REMOTE_ADDR'] , PDO::PARAM_STR );
	$stmt -> bindValue( ':version' , $version , PDO::PARAM_STR );
	
	$stmt -> execute();
	
	$stmt = $pdo -> prepare('INSERT INTO `tracker_log` ( `time` , `token` , `ip` , `version` ) VALUES ( :time , :token , :ip , :version )' );
	
	$stmt -> bindValue( ':token', $token , PDO::PARAM_STR );
    $stmt -> bindValue( ':time', time() , PDO::PARAM_STR );
    $stmt -> bindValue( ':ip', $_SERVER['REMOTE_ADDR'] , PDO::PARAM_STR );
	$stmt -> bindValue( ':version' , $version , PDO::PARAM_STR );
	
	$stmt -> execute();
	
	echo $token;
	 
}catch(PDOException $e){

}

?>