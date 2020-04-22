#include "NTPClient.h"
// change next line to use with another board/shield
#include <ESP8266WiFi.h>
//#include <WiFi.h> // for WiFi shield
//#include <WiFi101.h> // for WiFi 101 shield or MKR1000
#include <WiFiUdp.h>
#include "PrayerTimes.h"

//ini library untuk display di dot matrix
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include "Font3x78pt7b.h"

// Inisiasi Jadwal Sholat
double times[sizeof(TimeName) / sizeof(char*)];

//pin definition in wemos
#define buzzer D3  //7


const char *ssid     = "Nusantara62";
const char *password = "rumah62.";
const double utcOffsetInSeconds = 25200;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;

// Matrix Display
long period;
int offset=1,refresh=0;
int pinCS = D4; // Connecting the CS pin
int numberOfHorizontalDisplays = 4; // Number of LED matrices by Horizontal
int numberOfVerticalDisplays = 1; // Number of LED matrices per Vertical
Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);
int wait = 100; // Running speed
int spacer = 2;
int width = 5 + spacer; // Adjust the distance between the characters

int updCnt = 0;
long clkTime = 0;

// Ubah nama hari dan nama bulan
char weekDay[][3] = {"AH","SN","SL","RB","KM","JM","SB","AH"};
char monthYear[][4] = { " ", "JAN", "FEB", "MAR", "APR", "MEI", "JUN", "JUL", "AGU", "SEP", "OKT", "NOV", "DES" };

void setup() {
  //Matrix Display
  matrix.setIntensity(1); // Matrix brightness from 0 to 15
    
  // The initial coordinates of the matrices 8 * 8
  matrix.setRotation(0, 1);        // 1 matrix
  matrix.setRotation(1, 1);        // 2 matrix
  matrix.setRotation(2, 1);        // 3 matrix
  matrix.setRotation(3, 1);        // 4 matrix
  
  pinMode(buzzer, OUTPUT);
  //pinMode(button, INPUT);

  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
}

void loop() {
  timeClient.update();
  
  //  Serial.println(formattedDate);
  TampilJam();
  AlarmSholat10min();
  TampilJam();
  AlarmSholat();

  if(millis()-clkTime > 15000) {  //Every 15 seconds, tampilkan tanggal bergerak
  TampilTanggal();
  AlarmSholat();
  }

  if(millis()-clkTime > 18000) {  //Every 15 seconds, tampilkan tanggal bergerak
  TampilJadwalSholat();
  AlarmSholat();
  
  clkTime = millis();
}

  
//  TampilJadwalSholat();
}

//fungsi ini bertujuan untuk mengambil jadwal sholat yang ditentukan dari
//tanggal, bulan dan tahun.
void JadwalSholat() {

  formattedDate = timeClient.getFormattedTime();

  int tahun = timeClient.getYear();
  int bulan = timeClient.getMonth();
  int tanggal = timeClient.getDate();

  int dst = 7; // TimeZone

  int hours, minutes;

  set_calc_method(ISNA);
  set_asr_method(Shafii);
  set_high_lats_adjust_method(AngleBased);
  set_fajr_angle(20);
  set_isha_angle(18);

  //SETTING LOKASI DAN WAKTU
  float latitude = -5.397140;
  float longitude = 105.266792;

  get_prayer_times(tahun, bulan, tanggal, latitude, longitude, dst, times);
}

void AlarmSholat10min() {

  int Hor = timeClient.getHours();
  int Min = timeClient.getMinutes()+10;
  int Sec = timeClient.getSeconds();

  JadwalSholat();

  int hours, minutes;

  // Subuh
  get_float_time_parts(times[0], hours, minutes);

  if (minutes >= 60) {
    minutes = minutes - 60;
    hours ++;
  }

  if (Hor == hours && Min == minutes) {
    TeksBerjalan("10 menit lagi adzan subuh");
    Serial.println("SUBUH");
    BuzzerPendek();
    delay(6000);
  }

  // Dzuhur
  get_float_time_parts(times[2], hours, minutes);

  if (minutes >= 60) {
    minutes = minutes - 60;
    hours ++;
  }

  if (Hor == hours && Min == minutes) {
    TeksBerjalan("10 menit lagi adzan dzuhur");
    Serial.println("DZUHUR");
    BuzzerPendek();
    delay(6000);
  }

  // Ashar
  get_float_time_parts(times[3], hours, minutes);

  if (minutes >= 60) {
    minutes = minutes - 60;
    hours ++;
  }

  if (Hor == hours && Min == minutes) {
    TeksBerjalan("10 menit lagi adzan ashar");
    Serial.println("ASHAR");
    BuzzerPendek();
    delay(6000);
  }

  // Maghrib
  get_float_time_parts(times[5], hours, minutes);

  if (minutes >= 60) {
    minutes = minutes - 60;
    hours ++;
  }

  if (Hor == hours && Min == minutes) {
    TeksBerjalan("10 menit lagi adzan maghrib");
    Serial.println("MAGHRIB");
    BuzzerPendek();
    delay(6000);
  }   

  // Isya'
  get_float_time_parts(times[6], hours, minutes);

  if (minutes >= 60) {
    minutes = minutes - 60;
    hours ++;
  }

  if (Hor == hours && Min == minutes) {
    TeksBerjalan("10 menit lagi adzan isya");
    Serial.println("ISYA");
    BuzzerPendek();
    delay(6000);
  }

}

void AlarmSholat() {

  int Hor = timeClient.getHours();
  int Min = timeClient.getMinutes();
  int Sec = timeClient.getSeconds();

  JadwalSholat();

  int hours, minutes;

  // Subuh
  get_float_time_parts(times[0], hours, minutes);

  if (minutes >= 60) {
    minutes = minutes - 60;
    hours ++;
  }

  if (Hor == hours && Min == minutes) {
    TeksStatis("SUBUH");
    Serial.println("SUBUH");
    BuzzerPanjang();
    delay(6000);
  }

  // Dzuhur
  get_float_time_parts(times[2], hours, minutes);

  if (minutes >= 60) {
    minutes = minutes - 60;
    hours ++;
  }

  if (Hor == hours && Min == minutes) {
    TeksStatis("DZUHUR");
    Serial.println("DZUHUR");
    BuzzerPanjang();
    delay(6000);
  }

  // Ashar
  get_float_time_parts(times[3], hours, minutes);

  if (minutes >= 60) {
    minutes = minutes - 60;
    hours ++;
  }

  if (Hor == hours && Min == minutes) {
    TeksStatis("ASHAR");
    Serial.println("ASHAR");
    BuzzerPanjang();
    delay(6000);
  }

  // Maghrib
  get_float_time_parts(times[5], hours, minutes);

  if (minutes >= 60) {
    minutes = minutes - 60;
    hours ++;
  }

  if (Hor == hours && Min == minutes) {
    TeksStatis("MAGHRIB");
    Serial.println("MAGHRIB");
    BuzzerPanjang();
    delay(6000);
  }   

  // Isya'
  get_float_time_parts(times[6], hours, minutes);

  if (minutes >= 60) {
    minutes = minutes - 60;
    hours ++;
  }

  if (Hor == hours && Min == minutes) {
    TeksStatis("ISYA");
    Serial.println("ISYA");
    BuzzerPanjang();
    delay(6000);
  }

}

void BuzzerPanjang() {
  digitalWrite(buzzer, HIGH);
  delay(1000);
  digitalWrite(buzzer, LOW);
  delay(1000);
  digitalWrite(buzzer, HIGH);
  delay(1000);
  digitalWrite(buzzer, LOW);
  delay(50);
}

void BuzzerPendek() {
  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);
  delay(50);
}

void TampilJadwalSholat() {

  JadwalSholat();

  char sholat[7];
  char jam[5];
  char TimeName[][9] = {" SUBUH","TERBIT"," DZUHUR"," ASHAR","TERBENAM","MAGHRIB","  ISYA", "WAKTU"};
//  char TimeName[][8] = {"SU","TE","DZ","AS","TE","MG","IS","WA"};
  int hours, minutes;

  for (int i=0;i<8;i++){

    get_float_time_parts(times[i], hours, minutes);
    
    if (minutes >= 60) {
      minutes = minutes - 60;
      hours ++;
    }
    else {
      ;
    }
    
    if (i==0 || i==2 || i==3 || i==5 || i==6) { //Tampilkan hanya Subuh, Dzuhur, Ashar, Maghrib, Isya
//      sprintf(sholat,"%s",TimeName[i]); 
//      sprintf(jam,"%02d:%02d", hours, minutes);
      sprintf(jam,"%s %02d:%02d",TimeName[i], hours, minutes);
//      Serial.println(sholat);
//      Serial.println(" : ");
//      TeksStatis(jam);
      TeksBerjalan(jam);
      Serial.println(jam);
      delay(2000);
    }
  }
}

void TampilJam(){
  

  formattedDate = timeClient.getFormattedTime();

  char jam[8];

  sprintf(jam, "%02d:%02d:%02d",    //%02d print jam dengan format 2 digit
                  timeClient.getHours(),       //get hour method
                  timeClient.getMinutes(),     //get minute method
                  timeClient.getSeconds()      //get second method
  );
  
  Serial.println(jam);    //print the string to the serial port

  TeksStatis(jam);

  delay(1000);            //second delay

}

void TampilTanggal(){

  formattedDate = timeClient.getFormattedTime();
  
  char tanggal[18];

  sprintf(tanggal, "%s,%02d%s",             //%02d allows to print an integer with leading zero 2 digit to the string, %s print sebagai string
                  weekDay[timeClient.getDay()],   //ambil method hari dalam format lengkap
                  timeClient.getDate(),                 //get day method
                  monthYear[timeClient.getMonth()]     //get month method
  );
  
  Serial.println(tanggal);    //print the string to the serial port

//  TeksStatis(tanggal);
  TeksBerjalan(tanggal);
  delay(3000);

}

void TeksBerjalan (String text){

  // Setting font untuk jam

  spacer = 2;
  width = 5 + spacer;
  matrix.setFont(); // Reset font to default
  
  for ( int i = 0 ; i < width * text.length() + matrix.width() - 1 - spacer; i++ ) {
    if (refresh==1) i=0;
    refresh=0;
    matrix.fillScreen(LOW);
    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2; // Center text on Vertical

    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < text.length() ) {
        matrix.drawChar(x, y, text[letter], HIGH, LOW, 1);
      }
      letter--;
      x -= width;
    }
  matrix.write(); // Display
  delay(wait);
  }
}

void TeksStatis(String text){

  matrix.fillScreen(LOW);

  // Setting font untuk jam
  spacer = 2;
  width = 3 + spacer;
  matrix.setFont(&Font3x78pt7b);

  for (int i=0; i<text.length(); i++){
  
  int letter =(matrix.width())- i * (width-1);
  int x = (matrix.width() +0) -letter;
  int y = 11; // Set posisi vertikal font kadang suka terlalu atas atau terlalu bawah
  matrix.drawChar(x, y, text[i], HIGH, LOW, 1);
    
  matrix.write(); // Tampilkan

  }

}
