<?php

// Define database and server settings:
$server = "localhost";
$username = "<username>";
$password = "<password>";
$database_name = "telegramprinter";
$table_name = "entries";

$conn = mysqli_connect($server, $username, $password, $database_name);

$sql = "SELECT * FROM `$table_name` LIMIT 1";
$result = mysqli_query($conn, $sql);
$check = mysqli_num_rows($result);
if($check < 1){
	echo 'No entry found!';
}else{
	if($row = mysqli_fetch_assoc($result)){
		if($row['status'] == "ready"){
			echo "%".$row['command']."%".$row['content']."%";
			$sql_update = "UPDATE `$table_name` SET `status`='printed' LIMIT 1";
			mysqli_query($conn, $sql_update);
		}else if($row['status'] == "printed"){
			echo "Already Printed! Waiting new commands...";
		}
	}
}

?>