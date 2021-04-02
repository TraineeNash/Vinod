#include <Adafruit_Fingerprint.h>
//#include <Streaming.h>
#include <SoftwareSerial.h>
#include <Wire.h>  // Comes with Arduino IDE
#include <LiquidCrystal_I2C.h>

//for Wifi Connection
#include<ESP8266WiFi.h>
#include<ESP8266HTTPClient.h>
WiFiClient client;

//variable for database
String finger_id = "";
boolean stat[3] = {false,false,false};
int count = 0;

#define Buzz 11
int Gate=0;
int wait = 0;

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
//LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  //some address is different

#define __Debug         1                               // if debug mode

#if __Debug
#define DBG(X)          Serial.println(X)
#else
#define DBG(X)
#endif

String inputString = "";         // a string to hold incoming data
String MyString = "";
boolean stringComplete = false;  // whether the string is complete
int commaPosition;
int MODE=0;
int idx=0;
int Timer=0;


uint8_t getFingerprintEnroll(uint8_t id);

SoftwareSerial mySerial(2, 3);                    // rx, tx

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void Wifi_setup() {
  WiFi.begin("wifi_id","wifi_password");
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void setup()
{
    pinMode(Buzz,OUTPUT);
    lcd.begin(16, 2);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Initialize ");
    lcd.setCursor(0, 1);
    lcd.print("FPrint Sensor.. ");
  
    delay(2000);

    Serial.begin(9600);
    finger.begin(57600);

    if (finger.verifyPassword()) 
    {
        //Serial.println("Fingerprint sensor init ok");
    lcd.setCursor(0, 0);
    lcd.print("Finger Print OK   ");
    
    } 
    else 
    {
        lcd.setCursor(0, 0);
        lcd.print("Finger Print     ");
        lcd.setCursor(0, 0);
        lcd.print("not found..");
      //  Serial.println("Did not find fingerprint sensor :(");
      //  while (1);
       
    }

    Wifi_setup(); //Wifi initialization to connect wifi
    
    lcd.clear();
  lcd.setCursor(0,0);  
  lcd.print("WELCOME..");
  lcd.setCursor(0,1);
  lcd.print("FP-ATD SYSTEM");
  delay(2000);
   lcd.clear();
  lcd.setCursor(0,0);  
  lcd.print("WELCOME");
  lcd.setCursor(0,1);
  lcd.print("............");
  delay(500);
  digitalWrite(Buzz,HIGH);
  delay(30);
    digitalWrite(Buzz,LOW);
  delay(30);
    digitalWrite(Buzz,HIGH);
  delay(30);
    digitalWrite(Buzz,LOW);
  delay(30);
}

void loop()                     // run over and over again
{
    //Serial.println("Type in the ID # you want to save this finger as...\r\n(end with an 'a', such as 15a)");
    uint8_t id = 0;
   //-----------------------------WAIT COMMAND------------------------------------------------------
   while (MODE == 0){  
    Timer++;
    delay(50);
    if (Timer>100){
      MODE=2;
    }
/*
  
  if (digitalRead(IR)==0){
  lcd.clear();
  lcd.setCursor(0,0);  
  lcd.print("WELCOME..pls");
  lcd.setCursor(0,1);
  lcd.print("Touch finger");
  MODE = 2;
  
  
  
  delay(200);
  }
  if (digitalRead(IR)==1){
  lcd.clear();
  lcd.setCursor(0,0);  
  lcd.print("FP ATTENDANCE");
  lcd.setCursor(0,1);
  lcd.print("BASED SYSTEM");
  delay(100);
 
  MODE = 0;
  
  
  }
 */ 
  
  ///////////////////////////////////////////////////////////////////////////////////////////////////
   

  
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    if (inChar != '\r') {
    inputString += inChar;
    }
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\r') {
      stringComplete = true;
    } 
    if (inChar == 'X') {
      MyString = "X";
      MODE = 1;
     

    } 
    if (inChar == 'Y') {
      MyString = "Y";
      MODE = 2;
      
    } 
    
  }
  



   if (stringComplete) {
    
    if (MyString == "X"){
     MODE = 1;
     delay(1000);
    }
    
    if (MyString == "Y"){
     
   MODE = 2;
    }
    
    
    inputString = "";
    stringComplete = false;
}
   }
//-----------------------------------------------------------------------------------------
   
   
   
   
   
   

   
   
   if (MODE == 1){
  lcd.begin(16, 2);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("[Enrolling] ");
   
   
    
   
    while (Serial.available())
    {
        char c = Serial.read();
    }
     
            
    while (true) 
    {
        while (! Serial.available());
        char c = Serial.read();
       
        //---------------------------MODE 1-------------------------
       
        if (! isdigit(c)) 
        {
            while(Serial.available())
            {
                c = Serial.read();
            }
            break;
        }
        id *= 10;
        id += c - '0';
        
        //----------------------------------------------------------
    }
   }
    //-------------------------------------------ENROLLING------------------------------------------
   // Serial.print("Enrolling ID #");
   // Serial.println(id);
   while (MODE == 1){
   // lcd.begin(16, 2);
   // lcd.clear();
   // lcd.setCursor(0, 0);
   // lcd.print("Enrolling ID ");
    idx = idx - idx + id;
    lcd.print(id);
    delay(1000);
    while (!getFingerprintEnroll(idx));
    MODE = 0;
    delay(1000);
    Serial.flush();
   }
    //----------------------------------------------------------------------------------------------
    
    //---------------------------------------------------VERIFY PROCESS------------------------------
    while (MODE == 2){
    
  /*
  if (digitalRead(IR)==1){
  delay(1000);
  MODE = 0;
  wait = 0;
  }
  */

  //From now, the code to send fingerprint id to myPHPadmin
    HTTPClient http;
  String payload = "";
  http.begin("http://192.168.0.109/nas/checkStatus.php?type="+btn[count]);

  int httpCode = http.GET();
  if (httpCode > 0) 
    {
      Serial.print("httpCode: ");
      Serial.println(httpCode);
      Serial.print("httpPayload ");
      Serial.println(http.getString());
      payload = http.getString();
      
      if (payload == "0") {
        stat[count] = false;
      } else if (payload == "1") {
        stat[count] = true;
      }
      
    }
    http.end(); //Close connection

    
  
  if(wait == 0){
    lcd.begin(16, 2);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WELCOME..pls");
    lcd.setCursor(0, 1);
    lcd.print("scan finger..");
  }
    if(getFingerprintIDez()>=0)
    {
        
        lcd.begin(16, 2);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("ID ");
        lcd.print(finger.fingerID);
        
        //Convert finger id in int into String
        finger_id = String(finger.fingerID);
        //Send finger id to database
        sendStatus(finger_id);
        
        lcd.print(" verified");
        delay(2000);
        lcd.begin(16, 2);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WELCOME..pls");
    lcd.setCursor(0, 1);
    lcd.print("scan finger..");
       wait = 0;
    }
    delay(50);
    }
    
    //------------------------------------------------------------------------------------------------    
    
    
}


//Function to send fingerprint id value towards database
void sendStatus(String finger_id) {
  HTTPClient http;
  
  //url for the script to send value to database
  http.begin("http://192.168.43.84/nas/onclick.php?type="+rfid+"&&stat="+stat);

  int httpCode = http.GET();
  if (httpCode > 0) 
    {
      Serial.print("httpCode: ");
      Serial.println(httpCode);
      String payload1 = http.getString();
      
      if (payload1 == "0") {
        stat[count] = false;
      } else if (payload1 == "1") {
        stat[count] = true;
      }
  http.end();
  }
}

//----------------------------------------------------END LOOP PROGRAM--------------------------
uint8_t getFingerprintEnroll(uint8_t id) 
{
    uint8_t p = -1;
    //Serial.println("Waiting for valid finger to enroll");
    lcd.begin(16, 2);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Place your finger..");
    delay(1000);
    while (p != FINGERPRINT_OK) {
        p = finger.getImage();
        switch (p) {
            case FINGERPRINT_OK:
            //Serial.println("Image taken");
            lcd.begin(16, 2);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Done!..");
            delay(1000);
            break;
            case FINGERPRINT_NOFINGER:
            //Serial.println(".");
            break;
            case FINGERPRINT_PACKETRECIEVEERR:
            //Serial.println("Communication error");
            lcd.begin(16, 2);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Communication");
            lcd.setCursor(0, 1);
            lcd.print("Error!");
            delay(1000);
            break;
            case FINGERPRINT_IMAGEFAIL:
            //Serial.println("Imaging error");
            lcd.begin(16, 2);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Imaging Error");
            break;
            default:
            //Serial.println("Unknown error");
            lcd.begin(16, 2);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Unknown Error");
            break;
        }
    }

    // OK success!

    p = finger.image2Tz(1);
    switch (p) {
        case FINGERPRINT_OK:
        //Serial.println("Image converted");
        lcd.begin(16, 2);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Image converted!");
        break;
        case FINGERPRINT_IMAGEMESS:
        //Serial.println("Image too messy");
        lcd.begin(16, 2);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Messy Image");
        return p;
        case FINGERPRINT_PACKETRECIEVEERR:
        //Serial.println("Communication error");
        lcd.begin(16, 2);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Error");
        return p;
        case FINGERPRINT_FEATUREFAIL:
        //Serial.println("Could not find fingerprint features");
            lcd.begin(16, 2);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Your finger");
            lcd.setCursor(0, 1);
            lcd.print("not found!");
        return p;
        case FINGERPRINT_INVALIDIMAGE:
        //Serial.println("Could not find fingerprint features");
        lcd.begin(16, 2);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Your finger");
            lcd.setCursor(0, 1);
            lcd.print("not found!");
        return p;
        default:
        //Serial.println("Unknown error");
            lcd.begin(16, 2);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Unknown Error!");
            
        return p;
    }

   // Serial.println("Remove finger");
            lcd.begin(16, 2);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Remove finger");
    delay(2000);
    p = 0;
    while (p != FINGERPRINT_NOFINGER) {
        p = finger.getImage();
    }

    p = -1;
   // Serial.println("Place same finger again");
   lcd.begin(16, 2);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Place your");
            lcd.setCursor(0, 1);
            lcd.print("finger again..");
    while (p != FINGERPRINT_OK) {
        p = finger.getImage();
        switch (p) {
            case FINGERPRINT_OK:
            //Serial.println("Image taken");
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Image Taken");
            
            break;
            case FINGERPRINT_NOFINGER:
            //Serial.print(".");
            break;
            case FINGERPRINT_PACKETRECIEVEERR:
            //Serial.println("Communication error");
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Error");
            break;
            case FINGERPRINT_IMAGEFAIL:
            //Serial.println("Imaging error");
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Image Error");
            break;
            default:
            //Serial.println("Unknown error");
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Unknown Error");
            break;
        }
    }

    // OK success!

    p = finger.image2Tz(2);
    switch (p) {
        case FINGERPRINT_OK:
        //Serial.println("Image converted");
        lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Image Converted");
        break;
        case FINGERPRINT_IMAGEMESS:
        //Serial.println("Image too messy");
        lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Messy Image");
        return p;
        case FINGERPRINT_PACKETRECIEVEERR:
        //Serial.println("Communication error");
        lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Error");
        return p;
        case FINGERPRINT_FEATUREFAIL:
        //Serial.println("Could not find fingerprint features");
        lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Not found");
        return p;
        case FINGERPRINT_INVALIDIMAGE:
       // Serial.println("Could not find fingerprint features");
       lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Not found");
        return p;
        default:
        //Serial.println("Unknown error");
        lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Unknown Error");
        return p;
    }


    // OK converted!
    p = finger.createModel();
    if (p == FINGERPRINT_OK) {
       // Serial.println("Prints matched!");
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Matched..Success");
          
             MODE = 0;
            delay(3000);
    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
       // Serial.println("Communication error");
         lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Error");
        return p;
    } else if (p == FINGERPRINT_ENROLLMISMATCH) {
       // Serial.println("Fingerprints did not match");
       lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Not Matched");
        return p;
    } else {
      //  Serial.println("Unknown error");
      lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Error");
        return p;
    }

    p = finger.storeModel(idx);
    if (p == FINGERPRINT_OK) {
        //Serial.println("Stored!");
        lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("ID ");
             lcd.print(idx);
            lcd.print(" Stored");
         //   Serial.print("X,");
         //   Serial.print(idx);
         //   Serial.println(",STORED,");
            
    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
       // Serial.println("Communication error");
       lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Error");
        return p;
    } else if (p == FINGERPRINT_BADLOCATION) {
       // Serial.println("Could not store in that location");
       lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Error");
        return p;
    } else if (p == FINGERPRINT_FLASHERR) {
       // Serial.println("Error writing to flash");
        lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Error");
        return p;
    } else {
       // Serial.println("Unknown error");
       lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Error");
        return p;
    }
    
}




// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez()
{

    if (!finger.verifyPassword())
    {
       // DBG("Invalid!(");
       lcd.setCursor(0, 0);
            lcd.print("INVALID!!!");
        return -1;
    }

    uint8_t p = finger.getImage();
    if (p != FINGERPRINT_OK)
    {
        return -1;
    }

    p = finger.image2Tz();
    if (p != FINGERPRINT_OK)
    {
        return -1;
    }

    p = finger.fingerFastSearch();
    if (p != FINGERPRINT_OK)
    {
        return -1;
    }

#if __Debug
    Serial.print("*");
    Serial.print(finger.fingerID);
    Serial.println("#");
   // Serial.print(",success,");
    //DBG(finger.confidence);
   
#endif

    return finger.fingerID;
    
}

//--------------------------------void serial event-------------------------------
