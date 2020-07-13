#include <Servo.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
#define SERVO1PIN 3
#define SERVO2PIN 5

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

bool active = false;

// Set the laser wait time in milliseconds
int laserWaitTime = 2500;

// Setup a variable to record the last time the laser moved
long timeOfLastMovement;

// Setup servos
Servo servo1;
Servo servo2;

long randNumber;

void setup() 
{
  // Create a random seed using the analog noise
  randomSeed(analogRead(0));

  // Initiate a serial communication
  Serial.begin(9600);
  mfrc522.PCD_Init();

  // Attatch Servos
  servo1.attach(SERVO1PIN);
  servo2.attach(SERVO2PIN);

  // Reset Servo Pos
  servo1.write(50);
  servo2.write(65);
}

void loop() 
{
  // If the robot is active move the laser to random positions, then wait for the specified laser wait time.
  if (active)
  {
    if (laserWaitTime <= millis() - timeOfLastMovement) {
      // Set random positions
    float xRandPos = random(15, 86);
    float yRandPos = random(55, 96);

    // Move the laser
    servo1.write(xRandPos);
    servo2.write(yRandPos);

    // Record time
    timeOfLastMovement = millis();
    Serial.println(timeOfLastMovement);
    }
  }
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }

  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();

  if (content.substring(1) == "D5 53 0D AB" || content.substring(1) == "0D 3A 80 CB") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");

    if (!active)
    {
      active = true;
    }
    else
    {
      active = false;
    }
    delay(750);
  }
  else
  {
    Serial.println(" Access denied");
    
  }
} 
