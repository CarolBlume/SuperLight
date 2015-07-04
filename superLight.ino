#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <Ethernet.h> 


File myFile;            //The Log file
boolean i = 0;
int si = 0, sf = 0;
float sum;
int pot = 60;
tmElements_t tm;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.print("Initializing SD card...");
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
}

void loop() { 
  if(analogRead(0) > 450 && i == 1) {
    digitalWrite(2, LOW);
    sf = tm.Second + 60*tm.Minute + 3600*tm.Hour;
      if (sf < si) 
        sf = sf + 24*60*60 - si;
      sf = sf - si;
      sum = sf*pot/(3600.0);
    myFile.println("Lampada desligada as:");
    print2digits(tm.Hour);
    myFile.print(':');
    print2digits(tm.Minute);
    myFile.print(':');
    print2digits(tm.Second);
    myFile.print(", Date (D/M/Y) = ");
    myFile.print(tm.Day);
    myFile.print('/');
    myFile.print(tm.Month);
    myFile.print('/');
    myFile.print(tmYearToCalendar(tm.Year));
    myFile.println();
    myFile.print(" com consumo de:");
    myFile.print(sum);
    myFile.print("kWh");
    myFile.println();
    i = 0;
    myFile.close();
  }
  else {
    if(analogRead(0) < 250 && i == 0) {
      myFile = SD.open("arquivo.txt", FILE_WRITE);
      digitalWrite(2, HIGH);
      myFile.println("Lampada ligada as:");
      si = tm.Second + 60*tm.Minute + 3600*tm.Hour;
      print2digits(tm.Hour);
      myFile.print(':');
      print2digits(tm.Minute);
      myFile.print(':');
      print2digits(tm.Second);
      myFile.print(", Date (D/M/Y) = ");
      myFile.print(tm.Day);
      myFile.print('/');
      myFile.print(tm.Month);
      myFile.print('/');
      myFile.print(tmYearToCalendar(tm.Year));
      myFile.println();
      i = 1;
    }
  }
  checkRTC();
}

void checkRTC () {
  if (!RTC.read(tm)) {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
  }
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    myFile.print('0');
  }
  myFile.print(number);
}
