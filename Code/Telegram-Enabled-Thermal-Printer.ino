         /////////////////////////////////////////////  
        //     Telegram-Enabled Thermal Printer    //
       //             w/ Nano 33 IoT              //
      //             ---------------             //
     //          (Arduino Nano 33 IoT)          //           
    //             by Kutluhan Aktar           // 
   //                                         //
  /////////////////////////////////////////////

//
// Via Telegram, control thermal printers to print notes, shopping lists, URLs, payments, product barcodes, and episode release dates.
//
// For more information:
// https://www.theamplituhedron.com/projects/Telegram-enabled-Thermal-Printer-with-Arduino-Nano-33-IoT
//
//
// Connections
// Arduino Nano 33 IoT :           
//                                Tiny (Embedded) Thermal Printer
// TX  --------------------------- RX
// RX  --------------------------- TX
// GND --------------------------- GND


// Include the required libraries.
#include <SPI.h>
#include <WiFiNINA.h>
#include "Adafruit_Thermal.h"


char ssid[] = "[_SSID_]";        // your network SSID (name)
char pass[] = "[_PASSWORD_]";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

// Change the server before running the code.
char server[] = "www.theamplituhedron.com";

// Define the pathway of the results page of the Telegram webhook.
String application = "/Telegram_Thermal_Printer_Bot/results.php";

// Initialize the Ethernet client library.
WiFiSSLClient client;

// Hardware Serial
Adafruit_Thermal printer(&Serial1);     // Pass addr to printer constructor

// Include icons and pictures to be printed.
#include "note.h"
#include "list.h"
#include "url.h"
#include "payment.h"
#include "product.h"
#include "series.h"
#include "home.h"

void setup() {
  // NOTE: SOME PRINTERS NEED 19200 BAUD instead of 9600, check test page.
  Serial1.begin(9600);  // Initialize hardware serial
  printer.begin();      // Init printer (same regardless of serial type)

  // Check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) { Serial.println("Communication with WiFi module failed!"); while (true); }
  // Attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // Wait 10 seconds for connection:
    delay(10000);
  }

  // Verify connection on the serial monitor.
  Serial.println("Connected to WiFi!");
  delay(1000);

  // Print the home page showing available commands and functions:
  // print_home("theamplituhedron.com");
}

void loop() {
  // Connect to the Telegram webhook's results page on TheAmplituhedron.
  if (client.connect(server, 443)){
    Serial.println("Connected to the results page!"); // If get a connection, report back via serial:
    // Make an HTTP request:
    client.println("GET " + application + " HTTP/1.1");
    client.println("Host: www.theamplituhedron.com");
    client.println("Connection: close");
    client.println();
  }else{
    Serial.println("Connection Error!");
  }

  delay(2000); // Wait 2 seconds after connection...
  
  // If there are incoming bytes available, get the response from the Telegram webhook's results page.
  String response = "";
  while (client.available()) { char c = client.read(); response += c; }
  if(response != "" && response.indexOf("%") > 0){
    // Split the response string by a pre-defined delimiter in a simple way. '%'(percentage) is defined as the delimiter in this project.
    int delimiter, delimiter_1, delimiter_2;
    delimiter = response.indexOf("%");
    delimiter_1 = response.indexOf("%", delimiter + 1);
    delimiter_2 = response.indexOf("%", delimiter_1 +1);
    // Glean information as substrings.
    String command = response.substring(delimiter + 1, delimiter_1);
    String content = response.substring(delimiter_1 + 1, delimiter_2);
    // Print the requested command with the given content:
    if(command == "_note") print_notes(content);
    if(command == "shopping") print_shopping_list(content);
    if(command == "url") print_URL(content);
    if(command == "payment") print_payments(content);
    if(command == "barcode"){ char converted[sizeof(content)]; content.toCharArray(converted, sizeof(converted)); print_product_barcode(converted); }
    if(command == "series") print_series(content);
    if(command == "home") print_home(content);
  }
  delay(3000); // Wait 3 seconds before connection...
}

void print_notes(String text){
  printer.printBitmap(80, 80, note_logo);
  printer.boldOn();
  printer.justify('R');
  printer.setSize('L');
  printer.println(F("Note\n"));
  printer.boldOff();
  printer.justify('L');
  printer.setSize('M');
  printer.println(text);
  printer.feed(5);
  printer.setDefault(); // Restore printer to defaults
}

void print_shopping_list(String text){
  printer.printBitmap(80, 80, list_logo);
  printer.boldOn();
  printer.justify('R');
  printer.setSize('L');
  printer.println(F("Shopping"));
  printer.println(F("List\n"));
  printer.boldOff();
  printer.justify('L');
  printer.setSize('S');
  printer.setLineHeight(50);
  printer.println(text);
  printer.setLineHeight();
  printer.feed(5);
  printer.setDefault(); // Restore printer to defaults
}

void print_URL(String text){
  printer.printBitmap(80, 80, url_logo);
  printer.boldOn();
  printer.justify('R');
  printer.setSize('L');
  printer.println(F("URL\n"));
  printer.boldOff();
  printer.justify('L');
  printer.setSize('S');
  printer.println(text);
  printer.feed(5);
  printer.setDefault(); // Restore printer to defaults
}

void print_payments(String text){
  printer.printBitmap(80, 80, payment_logo);
  printer.boldOn();
  printer.justify('R');
  printer.setSize('L');
  printer.println(F("Payment\n"));
  printer.boldOff();
  printer.justify('L');
  printer.setSize('M');
  printer.println(text);
  printer.feed(5);
  printer.setDefault(); // Restore printer to defaults
}

void print_product_barcode(const char *text){
  printer.printBitmap(80, 80, product_logo);
  printer.boldOn();
  printer.justify('R');
  printer.setSize('L');
  printer.println(F("Product\n"));
  printer.boldOff();
  printer.justify('L');
  // CODE 93: compressed version of CODE 39:
  printer.setBarcodeHeight(30);
  printer.printBarcode(text, CODE93);
  printer.feed(5);
  printer.setDefault(); // Restore printer to defaults
}

void print_series(String text){
  printer.printBitmap(80, 80, series_logo);
  printer.boldOn();
  printer.justify('R');
  printer.setSize('L');
  printer.println(F("New"));
  printer.println(F("Episode\n"));
  printer.boldOff();
  printer.justify('L');
  printer.setSize('M');
  printer.println(text);
  printer.feed(5);
  printer.setDefault(); // Restore printer to defaults
}

void print_home(String _server){
  printer.printBitmap(80, 80, home_logo);
  printer.printBitmap(80, 80, home_logo);
  printer.printBitmap(80, 80, home_logo);
  printer.boldOn();
  printer.justify('C');
  printer.setSize('L');
  printer.println(F("Telegram"));
  printer.println(F("Enabled"));
  printer.println(F("Thermal"));
  printer.println(F("Printer\n"));
  printer.justify('L');
  printer.setSize('M');
  printer.println(F("Available"));
  printer.println(F("Printer"));
  printer.println(F("Commands:\n"));
  printer.boldOff();
  printer.setSize('S');
  printer.setLineHeight(50);
  printer.println(F("1) Notes\n"));
  printer.printBitmap(80, 80, note_logo);
  printer.println(F("2) Shopping List\n"));
  printer.printBitmap(80, 80, list_logo);
  printer.println(F("3) URL\n"));
  printer.printBitmap(80, 80, url_logo);
  printer.println(F("4) Payments\n"));
  printer.printBitmap(80, 80, payment_logo);
  printer.println(F("5) Product Barcode\n"));
  printer.printBitmap(80, 80, product_logo);
  printer.println(F("6) Series\n"));
  printer.printBitmap(80, 80, series_logo);
  printer.println(F("7) Home\n"));
  printer.printBitmap(80, 80, home_logo);
  printer.justify('C');
  printer.println(F("Running On:"));
  printer.println(_server);
  printer.setSize('L');
  printer.inverseOn();
  printer.println(F("&&&&&"));
  printer.inverseOff();
  printer.feed(5);
  printer.setDefault(); // Restore printer to defaults
}
