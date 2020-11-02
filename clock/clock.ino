#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>

const char *ssid     = "Martin Router King";
const char *password = "alleskleinohnezahlenundleerzeichen";

// On a Trinket or Gemma we suggest changing this to 1
#define PIN            13

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      60


const long utcOffsetInSeconds = 3600;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);


Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);



const uint32_t color5minutes = pixels.Color(0, 2, 1);
const uint32_t color15minutes = pixels.Color(0, 100, 0);

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  pixels.begin(); // This initializes the NeoPixel library.

  timeClient.begin();
}

void loop() {
  timeClient.update();

  //  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  //  Serial.print(", ");
  //  Serial.print(timeClient.getHours());
  //  Serial.print(":");
  //  Serial.print(timeClient.getMinutes());
  //  Serial.print(":");
  //  Serial.println(timeClient.getSeconds());
  int hour =  timeClient.getHours();
  int minute = timeClient.getMinutes();
  int second = timeClient.getSeconds();
  //  Serial.print(timeClient.getMinutes());
  //Serial.println(timeClient.getFormattedTime());

  if (hour >= 22) {
    if (minute % 5 == 0 && second <= 10) {
      blinkTime(hour, minute, -1, true);
    } else {
      pixels.clear();
      pixels.show();
    }
  } else if (hour >= 8 && hour < 16) {
    showProgress(hour, minute, second);
  }




  //  blinkTime(hour, minute, second);

}

void showProgress(int hour, int minute, int second) {

  int hourPiece;
  int minutePiece;
  if (hour % 2 == 0) {
    hourPiece = 0;
    minutePiece = minute - 15;
  } else {
    hourPiece = 45;
    minutePiece = minute;
  }

  int minutesProgress = hourPiece + minutePiece;

  Serial.print(minutesProgress); Serial.print("     ||     ");  Serial.println(map(minutesProgress, 0, 90, 0, 60));
  if ((second > 25 && second < 30) || minutesProgress > 90 || minutesProgress < 0) {
    pixels.clear();
    pixels.show();
    blinkTime(hour, minute, second, false);
  } else {
    pixels.fill(pixels.Color(0, 20, 0), 0, map(minutesProgress, 0, 90, 1, 60));
    pixels.show();
  }



}


void blinkTime(int hour, int minute, int seconds, boolean sleepMode) {
  if (hour > 12 ) {
    hour -= 12;
  }

  uint32_t colorHour = sleepMode ? pixels.Color(3, 0, 0) : pixels.Color(200, 0, 0) ;

  uint32_t colorMinute = sleepMode ? pixels.Color(0, 2, 1) : pixels.Color(100, 0, 0) ;


  showClockFace(sleepMode);
  pixels.setPixelColor((hour * 5), colorHour);
  pixels.show();
  delay(250);
  pixels.setPixelColor((minute), colorMinute);
  pixels.show();
  delay(250);
  pixels.setPixelColor((minute), colorMinute);
  pixels.show();
  delay(250);
  pixels.setPixelColor((minute), colorMinute);
  pixels.show();
  delay(250);

  pixels.clear();

  showClockFace(sleepMode);
  if (seconds > 0 && !sleepMode) {
    pixels.setPixelColor(seconds + 1, pixels.Color(200, 50, 20));
  }
  pixels.show();
  pixels.clear();
  pixels.show();



}

void showClockFace(boolean sleepMode) {

  uint32_t  color5 = sleepMode ? pixels.Color(0, 0, 1) : color5minutes;
  uint32_t  color15 = sleepMode ? pixels.Color(0, 1, 0) : color15minutes;

  pixels.setPixelColor(0, color15);
  pixels.setPixelColor(5, color5);
  pixels.setPixelColor(10, color5);
  pixels.setPixelColor(15, color15);
  pixels.setPixelColor(20, color5);
  pixels.setPixelColor(25, color5);
  pixels.setPixelColor(30, color15);
  pixels.setPixelColor(35, color5);
  pixels.setPixelColor(40, color5);
  pixels.setPixelColor(45, color15);
  pixels.setPixelColor(50, color5);
  pixels.setPixelColor(55, color5);



}


