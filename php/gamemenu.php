<?php

include("config.php");
$connect = @mysql_connect ( $host, $username, $password) or die ('Nie uda³o siê po³¹czyæ z baz¹ danych');
@mysql_select_db ( $database, $connect) or die('Nie uda³o siê wybraæ bazy danych.');
$sQuery = "
	SELECT *
	FROM cs_service
	WHERE ban='0'
	AND stawka >= 15.00
	ORDER BY ABS(stawka) DESC LIMIT 0 , 4
	";
$rResult = mysql_query( $sQuery, $connect ) or die(mysql_error());
while ( $aRowa = mysql_fetch_array( $rResult ) ) 
{
	$id = $aRowa['id_servera']; 
	$aRow = mysql_fetch_array(mysql_query("SELECT *	FROM cs_servers	WHERE wlaczony='1' AND id='$id'", $connect));
	
	if($aRow['wlaczony'])
	{
		echo $aRow['nazwa'];
		echo "\r\n";
		echo $aRow['ipport'];
		echo "\r\n";
	}
}

?>