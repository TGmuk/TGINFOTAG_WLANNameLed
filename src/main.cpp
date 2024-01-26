#include "mbed.h"
#include "ESP8266Webserver.h"
#include "LCD.h"

lcd lcd;
ESP8266Webserver server;

PortOut led(PortC, 0xFF);

string getRootPage() {

    string webpage;
    webpage = "<!DOCTYPE html>";
    //HTML
    webpage += "<html>";
    webpage += "<head>";
    webpage += "<title>STM32 HTTP</title>";
    webpage += "</head>";
    webpage += "<body>";
    webpage += "<div style=\"position: absolute; top:0; bottom: 0; left: 0; right: 0; margin: auto; text-align: center;\">";
    webpage += "<h1 style=\"font-family: Arial;\">WLAN mit dem Mikrocontroller</h1>\n";

    webpage += "<h3 style=\"font-family: Arial;\">Gib hier deinen Namen ein:</h3>\n";
    webpage += "<form action='lcd' method='POST'>";
    webpage += "<input name='lcdout' id='lcdout' style=\"font-size: 30px;\">";
    webpage += "<br>";
    webpage += "<input type='submit' value='Auf dem Display anzeigen' style=\"font-size: 20px;\">";
    webpage += "</form>";

    webpage += "<br>";
    webpage += "<h3 style=\"font-family: Arial;\">Hier kannst du LEDs an und aus machen</h3>\n";
    webpage += "<form action='buttons' method='POST'>";
    webpage += "<button name='buttonout' id='buttonout' value='0' style=\"font-size: 30px;\">LED0</button>";
    webpage += "<button name='buttonout' id='buttonout' value='1' style=\"font-size: 30px;\">LED1</button>";
    webpage += "</form>";

    webpage += "</div>";
    webpage += "</body>";
    webpage += "</html>";
    return webpage;
    /*
  char page[300];
  int count = sprintf(page, "\
<!DOCTYPE html>\n\
<html>\n\
  <head>\n\
    <title>Sixth Webpage</title>\n\
  </head>\n\
  <body>\n\
    <h1>Your Name:</h1>\n\
		<form action='/' method='POST'>\n\
      <input type='text' name='lcdout'>\n\
      <input type='submit' value='Show on LCD'>\n\
		</form>\n\
  </body>\n\
</html>\n");
  printf("%d\n", count);
  return page;
     */
}

void handleLCD() {
    printf("handleLCD\n");

    const char* lcdString = server.gibWert("lcdout");
    printf("%s\n", server.gibWert("Content-Length")); // I dont know why but this needs to be there. Even if it does nothing
    printf("%s\n", lcdString);



    if(lcdString){
        lcd.clear();
        lcd.cursorpos(0);
        lcd.printf("Hallo %-10s", lcdString);

    }
    server.send(200, "text/html", getRootPage());




}

void handleButton() {
    printf("handleButton\n");
    const char* ledData = server.gibWert("buttonout");
    printf("%s\n", server.gibWert("Content-Length")); // I dont know why but this needs to be there. Even if it does nothing
    printf("LED Data: %s\n", ledData);


    char* indexStr = (strstr(ledData, "="));
    char index;
    if(indexStr!=nullptr){
        index = *(indexStr+1);
    }else{
        index = *ledData;
    }
    printf("LED index: %c\n", index);

    if(isdigit(index)){
        int indexInt = (int)index - 48;
        printf("LED index as int: %d\n", indexInt);
        if(indexInt>= 0 && indexInt <=3){
            led = led ^ (1 << indexInt);
        }
    }







    server.send(200, "text/html", getRootPage());

}

void handleRoot() {
    printf("handleRoot\n");




    server.send(200, "text/html", getRootPage());
}

int main() {
    server.on("lcd", &handleLCD);
    server.on("button", &handleButton);

    server.on("/", &handleRoot);
    server.begin();


    lcd.clear();
    lcd.cursorpos(0);
    led = 0x00;

    while (true) {
        server.handleClient();
    }
}
