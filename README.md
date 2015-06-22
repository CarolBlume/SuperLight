# SuperLight
SuperLight

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>


File myFile;
boolean i = 1;
int si = 0, sf = 0;
float sum;
float pot;
tmElements_t tm;

void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  digitalWrite(2, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(analogRead(0) > 450 && i == 1) {
    myFile = SD.open("arquivo.txt", FILE_WRITE);
    digitalWrite(2, HIGH);
    myFile.println("Lampada ligada as:");
    si = tm.Second + 60*tm.Minute + 3600*tm.Hour;
    print2digits(tm.Hour);
    myFile.print(':');
    print2digits(tm.Minute);
    myFile.print(':');
    print2digits(tm.Second);
    print2digits2(tm.Hour);
      Serial.print(':');
      print2digits2(tm.Minute);
      Serial.print(':');
      print2digits2(tm.Second);
      Serial.println();
    myFile.print(", Date (D/M/Y) = ");
    myFile.print(tm.Day);
    myFile.print('/');
    myFile.print(tm.Month);
    myFile.print('/');
    myFile.print(tmYearToCalendar(tm.Year));
    myFile.println();
    i = 0;
  }
  else {
    if(analogRead(0) < 370 && i == 0) {
      digitalWrite(2, LOW);
      sf = tm.Second + 60*tm.Minute + 3600*tm.Hour;
      if (sf < si) 
        sf = sf + 24*60*60 - si;
      sf = sf - si;
      sum = sf*pot/(3600000.0);
      myFile.println("Lampada desligada as:");
      print2digits(tm.Hour);
      myFile.print(':');
      print2digits(tm.Minute);
      myFile.print(':');
      print2digits(tm.Second);
      print2digits2(tm.Hour);
      Serial.print(':');
      print2digits2(tm.Minute);
      Serial.print(':');
      print2digits2(tm.Second);
      Serial.println();
      myFile.print(", Date (D/M/Y) = ");
      myFile.print(tm.Day);
      myFile.print('/');
      myFile.print(tm.Month);
      myFile.print('/');
      myFile.print(tmYearToCalendar(tm.Year));
      myFile.print(" com consumo de:");
      myFile.print(sum);
      myFile.print("kWh");
      myFile.println();
      i = 1;
      myFile.close();
    }
  }
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

void print2digits2(int number) {
  if (number >= 0 && number < 10) {
    Serial.print('0');
  }
  Serial.print(number);
}
