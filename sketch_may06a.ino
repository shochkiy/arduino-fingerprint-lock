#include <SPI.h>
#include <Wire.h> 
#include <Ethernet.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Fingerprint.h>

SoftwareSerial mySerial(2, 3);
LiquidCrystal_I2C lcd(0x27, 20, 4);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,0,105); 
char serv[] = "192.168.0.103";
EthernetClient cliente;

int sensor = 1;
String fID = "";
String conf = "";
int k=0;


void setup()
{
  Serial.begin(9600);
  delay(100);
  finger.begin(57600);
  lcd.begin();
  lcd.backlight();
  Ethernet.begin(mac, ip);
  for (int i=6;i<=9;i++) {
    pinMode(i, INPUT);
  }
  pinMode(5, OUTPUT);
}

void loop()
{   
getFingerprintIDez();
if (digitalRead(6)==HIGH){
  delay(500);
  while(!addfID());  
  } else if (digitalRead(7)==HIGH){
  delay(500);
  while(!delfID());  
  } else if (digitalRead(8)==HIGH){
    delay(500);
    while(!delBASE());
  }
}


void getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!

  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  fID = finger.fingerID;
  conf = finger.confidence;
  if ((fID != "") || ( conf != ""))
  {
  if (cliente.connect(serv, 80)) {
    Serial.println("connected");
    cliente.print("GET /fingerprint/data.php?");
    cliente.print("fingerID=");
    cliente.print(fID);
    cliente.print("&confidence=");
    cliente.println(conf);  
    fID = "";
    conf = "";
    cliente.stop();
    }
    else {
      Serial.println("connection failed");
     }
   delay(1000);
  }
  lcd.setCursor(0,0);
  lcd.print(fID);
  lcd.print(" ID find!");
  lcd.setCursor(0,2);
  lcd.print("OPENED");
  digitalWrite(5, HIGH);
  delay(3000);
  digitalWrite(5, LOW);
  lcd.clear();
}

uint8_t addfID(){
  int p=-1;
  int numh = 0;
  int numd = 0;
  int numo = 1;
  int num;
  lcd.setCursor(0,0);
  lcd.print("Add new ID.");
  lcd.setCursor(0,1);
  lcd.print("Enter new ID");
  lcd.setCursor(0,2);
  lcd.print("from 1 to 127");
  lcd.setCursor(0,3);
  lcd.print(numh);
  lcd.print(numd);
  lcd.print(numo);
  while (digitalRead(9) != HIGH) {
    if (digitalRead(6) == HIGH) {
      numh++;
      if (numh == 2) {numh=0;}
      lcd.setCursor(0,3);
      lcd.print(numh);
      delay(500); 
    }
    if (digitalRead(7) == HIGH) {
      numd++;
      if (numd == 10) {numd=0;}
      lcd.setCursor(1,3);
      lcd.print(numd);
      delay(500);
    }
    if (digitalRead(8) == HIGH) {
      numo++;
      if (numo == 10) {numo=1;}
      lcd.setCursor(2,3);
      lcd.print(numo);
      delay(500);
    }
  }
  num = numh*100+numd*10+numo;
  delay(500);
  lcd.clear();
    lcd.print("Place Finger. ID - ");
    lcd.print(num);
    while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    }
    lcd.clear();
    lcd.print("Image taken");
    p =  finger.image2Tz(1);
    delay(2000);
    lcd.clear();
    lcd.print("Image converted");
    lcd.setCursor(0,1);
    lcd.print("Remove finger");
    p=0;
    while (p != FINGERPRINT_NOFINGER) {
      p = finger.getImage();
      }
    lcd.clear();
    lcd.print("Place same finger");
    while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    }
    lcd.clear();
    lcd.print("Image taken");
    p =  finger.image2Tz(2);
    delay(2000);
    lcd.clear();
    lcd.print("Image converted");
    lcd.setCursor(0,1);
    lcd.print("Creating ID - ");
    lcd.print(num);
    lcd.setCursor(0,2);
    p = finger.createModel();
    if (p == FINGERPRINT_OK) {
      lcd.print("Prints matched");
    } else if (p == FINGERPRINT_ENROLLMISMATCH) {      
    lcd.print("Prints did not match");
    delay(3000);
    lcd.clear();
    return p;
    }
    p = finger.storeModel(num);
    if (p==FINGERPRINT_OK) {
      lcd.setCursor(0,3);
      lcd.print("ID - ");
      lcd.print(num);
      lcd.print(" stored!");
      if (cliente.connect(serv, 80)) {
        cliente.print("GET /fingerprint/baseiddel.php?");
        cliente.print("fingerID=");
        cliente.println(num);
        cliente.stop();
        Serial.println("connected del");
      }
      else {
      Serial.println("connection failed del");
      }
      delay(500);
      if (cliente.connect(serv,80)) {
        cliente.print("GET /fingerprint/baseid.php?");
        cliente.print("fingerID=");
        cliente.println(num);
        cliente.stop();
        Serial.println("connected add");
      }
      else {
         Serial.println("connection failed add");
      }
      delay(3000);
      lcd.clear();
     }   
}

bool delfID() {
  int p=-1;
  int numh = 0;
  int numd = 0;
  int numo = 1;
  int num;
  lcd.setCursor(0,0);
  lcd.print("Delete ID.");
  lcd.setCursor(0,1);
  lcd.print("Enter ID");
  lcd.setCursor(0,2);
  lcd.print("from 1 to 127");
  lcd.setCursor(0,3);
  lcd.print(numh);
  lcd.print(numd);
  lcd.print(numo);
  while (digitalRead(9) != HIGH) {
    if (digitalRead(6) == HIGH) {
      numh++;
      if (numh == 2) {numh=0;}
      lcd.setCursor(0,3);
      lcd.print(numh);
      delay(500);
    }
    if (digitalRead(7) == HIGH) {
      numd++;
      if (numd == 10) {numd=0;}
      lcd.setCursor(1,3);
      lcd.print(numd);
      delay(500);
    }
    if (digitalRead(8) == HIGH) {
      numo++;
      if (numo == 10) {numo=1;}
      lcd.setCursor(2,3);
      lcd.print(numo);
      delay(500);
    }
  }
  num = numh*100+numd*10+numo;
  delay(500);
  lcd.clear();
  lcd.print("Deleting ID - ");
  lcd.print(num);
  lcd.setCursor(0,1);
  lcd.print("Press 4 to confirm");
  while (digitalRead(9) != HIGH) {}
  p = finger.deleteModel(num);
  if (p == FINGERPRINT_OK) {
    lcd.setCursor(0,2);
    lcd.print("DELETED!");
    delay(3000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    return p;
  } else {
    return p;
  }  
  lcd.clear();
  }

bool delBASE(){
  int p=-1;
  lcd.print("Delete ALL prints?");
  lcd.setCursor(0,1);
  lcd.print("3-NO;   4-YES");
  while (digitalRead(9) != HIGH) {
    if (digitalRead(8) == HIGH);
    lcd.clear();
    return p;
  }
  finger.emptyDatabase();
  lcd.clear();
  lcd.print("DATABASE IS EMPTY");
  delay(3000);
  lcd.clear();
}
