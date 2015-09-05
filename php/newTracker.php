<?php
include 'config.php';

try{
	$pdo = new PDO('mysql:host='.$mysql_host.';dbname='.$database.';port='.$port, $username, $password );
		
	$pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	
	$token = substr( md5( rand() ) , 0 , 32 );
	
	$stmt = $pdo -> prepare('INSERT INTO `tracker_exe_clients` ( `token` , `time` , `ip` ) VALUES ( :token , :time , :ip )' );
	 
	$stmt -> bindValue( ':token', $token , PDO::PARAM_STR );
    $stmt -> bindValue( ':time', time() , PDO::PARAM_STR );
	$stmt -> bindValue( ':ip', $_SERVER['REMOTE_ADDR'] , PDO::PARAM_STR );
	
	$stmt -> execute();
	
	echo $token;
	 
}catch(PDOException $e){

}

?>