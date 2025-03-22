<?php

// Define the themal_printer class and functions:
class thermal_printer {
	public $token, $web_path, $conn, $table;
	
	public function __init__($token, $web_path, $conn, $table){
		$this->token = $token;
		$this->web_path = $web_path.$token;
		$this->conn = $conn;
		$this->table = $table;
	}
	
	public function send_message($id, $string){
		$new_message = $this->web_path."/sendMessage?chat_id=".$id."&text=".urlencode($string);
		file_get_contents($new_message);
	}
	
	public function send_photo($id, $photo, $caption){
	    $new_photo = $this->web_path."/sendPhoto?chat_id=".$id."&photo=".$photo."&caption=".$caption;
	    file_get_contents($new_photo);
	}
	
	public function database_update($command, $content){
		$sql = "UPDATE `$this->table` SET `command`='$command', `content`='$content', `status`='ready' LIMIT 1";
		mysqli_query($this->conn, $sql);
	}
}

// Define database and server settings:
$server = "localhost";
$username = "<username>";
$password = "<password>";
$database_name = "telegramprinter";
$table_name = "entries";

$conn = mysqli_connect($server, $username, $password, $database_name);

$printer = new thermal_printer();
$printer->__init__("123456:ABC-DEF1234ghIkl-zyx57W2v1u123ew11", "https://api.telegram.org/bot", $conn, $table_name);

// Get updates from the Telegram Bot API.
$updates = json_decode(file_get_contents('php://input'), TRUE); 

// Get commands.
if($updates['update_id']){
	$chat_id =   $updates['message']['chat']['id'];
	$message = $updates['message']['text'];
    
	if($updates["message"]["photo"]){
		$printer->send_message($chat_id, "Thank you for sending me a photo but I cannot process it yet 🎞");
	}else if($updates["message"]["video"]){
		$printer->send_message($chat_id, "Thank you for sending me a video but I cannot process it yet  📹");
	}else if($updates["message"]["document"]){
		$printer->send_message($chat_id, "Thank you for sending me a file but I cannot process it yet  📜");
	}else{
		if(strpos($message, "/print_notes") !== FALSE){
			$command = "_note";
			$content = explode(" ", $message, 2)[1];
			$printer->database_update($command, $content);
			$printer->send_message($chat_id, "Printed: ".$content);
		}
		if(strpos($message, "/print_shopping_list") !== FALSE){
			$command = "shopping";
			$content = explode(" ", $message, 2)[1];
			$printer->database_update($command, $content);
			$printer->send_message($chat_id, "Printed: ".$content);
		}
		if(strpos($message, "/print_url") !== FALSE){
			$command = "url";
			$content = explode(" ", $message, 2)[1];
			$printer->database_update($command, $content);
			$printer->send_message($chat_id, "Printed: ".$content);
		}
		if(strpos($message, "/print_payments") !== FALSE){
			$command = "payment";
			$content = explode(" ", $message, 2)[1];
			$printer->database_update($command, $content);
			$printer->send_message($chat_id, "Printed: ".$content);
		}
		if(strpos($message, "/print_product_barcode") !== FALSE){
			$command = "barcode";
			$content = explode(" ", $message, 2)[1];
			$printer->database_update($command, $content);
			$printer->send_message($chat_id, "Printed: ".$content);
		}
		if(strpos($message, "/print_series") !== FALSE){
			$command = "series";
			$content = explode(" ", $message, 2)[1];
			$printer->database_update($command, $content);
			$printer->send_message($chat_id, "Printed: ".$content);
		}
		
		switch($message){
		  case '/print_home':
		  $printer->database_update("home", "theamplituhedron.com");
		  $printer->send_message($chat_id, "Printed: OK");
		  break;	
		  case '/start':
		  $printer->send_message($chat_id, "Leave a blank space before entering the text you want to print with thermal printers for these commands:\n\n/print_notes text\n/print_shopping_list item1, item2...\n/print_url text\n/print_payments text\n/print_product_barcode text\n/print_series text\n\nEnter /help to view descriptions.");
		  break;			
		  case 'How r u?':
		  $printer->send_message($chat_id, "Thanks for asking.\nData processing is running just fine 😃");
		  break;
		  case '/cover':
		  $printer->send_photo($chat_id, "https://www.theamplituhedron.com/Telegram_Thermal_Printer_Bot/spiderman.jpg", 'Friendly Neighborhood Spider-Man');
	      break;
		  case '/arduino':
		  $printer->send_photo($chat_id, "https://www.theamplituhedron.com/Telegram_Thermal_Printer_Bot/arduino.jpg", 'Arduino Nano 33 IoT Schematics');
	      break;
		  case '/help':
		  $printer->send_message($chat_id, "Leave a blank space before entering the text you want to print with thermal printers for these commands:\n\n/print_notes text\n/print_shopping_list item1, item2...\n/print_url text\n/print_payments text\n/print_product_barcode text\n/print_series text\n\nDescriptions:\n\n/print_home - print all available functions\n/print_notes - send and print a note\n/print_shopping_list - create and print a shopping list\n/print_url - send and print URLs\n/print_payments - define and print payments\n/print_product_barcode - define and print product barcodes\n/print_series - enter and print a new episode date\n/cover - display the given cover picture\n/arduino - display Nano 33 IoT schematics\n/help - functions and instructions");
		  break;	  
	    }
	}
}

?>