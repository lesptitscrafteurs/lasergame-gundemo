/**
 * @file LaserGame_Gun2.ino
 * @author Les P'tits Crafteurs
 * @brief Code pour créer un emetteur recepteur IR basic afin de tester notre projet
 * @version 0.1
 * @date 2022-08-10
 * 
 * @copyright Copyright (c) 2022
 */

#include <IRremote.hpp>

#define IR_SEND_PIN           4
#define IR_RECEIVE_PIN        2
#define LED_PIN               3
#define TRIGGER_PIN           5

void setup() {
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Initialisation de la communication avec le moniteur série
  Serial.begin(115200);

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Initialisation du pin avec la LED rouge
  pinMode(LED_PIN, OUTPUT);
  // On allume la LED pour dire que l'arme est active
  digitalWrite(LED_PIN, HIGH); // Allumage de la LED : état haut = HIGH

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Initialisation du pin pour le bouton
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Initialisation du récepteur Infra Rouge intégré à l'arme
  IrReceiver.begin (IR_RECEIVE_PIN, false, 0);

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Initialisation de la LED Infrarouge
  IrSender.begin(IR_SEND_PIN, true);
}

void loop() {
  ////////////////////////////////////////////////////////
  // On regarde si on s'est fait tirer dessus
  if (IrReceiver.decode()) {                          // Nous avons reçu quelque chose
    if (IrReceiver.decodedIRData.protocol == NEC) {   // Nous ne traitons que les message envoyés avec le protocol NEC
      if (IrReceiver.decodedIRData.address != 1 ) {   // On ne traite le coup que s'il ne vient pas de l'arme elle-même
        digitalWrite(LED_PIN, LOW);                     // On éteind la LED
        delay(5000);                                    // On attend 5 secondes
        digitalWrite(LED_PIN, HIGH);                    // On allume la LED
      }
    }
    IrReceiver.resume();                              // On rend possible le fait de recevoir à nouveau quelque chose
  }

  ////////////////////////////////////////////////////////
  // On regarde si il faut tirer
  uint8_t btnState = digitalRead( TRIGGER_PIN );
  if ( btnState == LOW ) {
    // On déclenche l'envoi d'un message infrarouge (On utilise le protocole NEC pour cela)
    // Paramètre 1 : id unique de l'arme
    // Paramètre 2 : id du mode de tir utilisé (coup à coup)
    // Paramètre 3 : nombre de répétition
    IrSender.sendNEC(1, 0, 0);
  }

  delay (50);
}