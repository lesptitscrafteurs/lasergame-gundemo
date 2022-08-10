/**
 * @file LaserGame_Demo.ino
 * @author Les P'tits Crafteurs
 * @version 0.1
 * @date 2022-05-08
 * 
 * @brief Ce programme est un programme de démonstration pour tester les composants présents dans le gun
 * de notre projet de LaserGame. L'objectif de se programme est uniquement de tester le bon fonctionnement
 * des composants et de l'assemblage. Il n'est pas possible de faire un partie avec ce code.
 * 
 * Ce programme est fait pour fonctionner avec le montage suivante : https://
 */


#include <IRremote.hpp>             // Pour la gestion des leds infrarouges. L'utilisation de cette librairie va entrainer des warnings lors de la compilation du fait de son utilisation avec un ESP32. Cela n'affecte en rien le bon fonctionnement du programme.
#include <DFRobotDFPlayerMini.h>    // Pour la gestion du son
#include "LaserGamePushButton.h"    // Pour la gestion des boutons poussoires
#include "LaserGameLED.h"           // Pour la gestion des LEDs NeoPixel
#include "LaserGameTimer.h"         // Pour la gestion
#include "LaserGameShootingMode.h"  // Pour la gestion des modes de tir
#include "LaserGameScreen.h"        // Pour la gestion de l'écran OLED


/* définition des pin utilisés sur l'ESP32. A adapter si besoin */
#define TRIGGER_PIN           0
#define HAPTIC_PIN            4
#define STATUS_LED_PIN        12
#define MAGAZINE_LED_PIN      13
#define GRENADE_IR_PIN        14
#define SOUND_TX_PIN          16  /* /!\ Nous avons fait une erreur de montage et nous avons inversé le RX et le TX */
#define SOUND_RX_PIN          17  /* /!\ Nous avons fait une erreur de montage et nous avons inversé le RX et le TX */
#define SHOOTER_LED_PIN       18
#define MODE_PIN              19
#define RELOAD_PIN            23
#define NORMAL_IR_PIN         27
#define IR_RECEIVE_PIN        33

/* définition de quelques constantes. A adapter si besoin */
#define SHOOTER_LED_COUNT     12
#define STATUS_LED_COUNT      1
#define MAGAZINE_LED_COUNT    36
#define SOUND_ARMOR_SWAP      1
#define SOUND_SINGLE_SHOOT    2
#define SOUND_BURST_SHOOT     3
#define SOUND_RELOAD_END      4
#define SOUND_GRENADE_RELOAD  5
#define SOUND_ARMOR_ON        6
#define SOUND_GRENADE_SHOOT   7
#define SOUND_BURST_RELOAD    8
#define SOUND_ARMOR_EMPTY     9
#define SOUND_RELOAD          10
#define SOUND_SHOOTED         11
#define SOUND_RELOAD_START    12
#define SCREEN_WIDTH          128
#define SCREEN_HEIGHT         64
#define SCREEN_ADDRESS        0x3C
#define SHOOT_LOCATION_GUN          0
#define SHOOT_LOCATION_SHOULDER     1
#define SHOOT_LOCATION_FRONT        2
#define SHOOT_LOCATION_BACK         3

uint32_t fromHtmlColor (String htmlColor);

/* Création des structures qui permettent de stocker les différents paramètres de la partie */
struct Player {
  uint32_t gunId;
  uint32_t teamId;
  String pseudo;
} ;

struct Team {
  uint32_t id;
  String color;
} ;

struct Params {
  uint32_t startDelay;
  uint32_t gameDurantion;
  uint32_t respawnDelay;
} ;

struct Game {
  Player players [10];
  Team teams [4];
  Params params;
} ;


/* définition des constantes utilisées dans le programme */
const uint8_t gunUniqId = 0; // Identifiant unique de l'arme. Important qu'il soit unique pour chaque arme car cela permettra de savoir qui nous a tiré dessus
const Game gameInformation = {
  {
    { 0, 0, "Octuple" },  // Informations sur le joueur 1. équipe : 0; pseudo : Octuple 
    { 1, 1, "Raven" },    // Informations sur le joueur 1. équipe : 1; pseudo : Raven
    { 2, 1, "Ecumeuse" }  // Informations sur le joueur 1. équipe : 1; pseudo : Ecumeuse
  },
  {
    { 0, "#256DF4" },     // Information sur l'équipe 0
    { 1, "#F42525" },     // Information sur l'équipe 1
  },
  {
    10 * 1000,            // Durée de l'attente avant qu'on partie ne démarre (10 secondes)
    5 * 60 * 1000,        // Durée d'une partie (5 minutes)
    10 * 1000             // Durée d'attente avant de respawn après s'être fait shooté (10 secondes)
  }
};

/* Définition des différents modes de tir */
LaserGameShootingMode SHOOTING_MODES [] = {
    //                    ir sender pin | mode rafale   | taille du chargeur | nombre total de munitions | munitions infinie | niveau du chargeur | label           | durée du tir | id du son
    LaserGameShootingMode(NORMAL_IR_PIN,    false,        72,                  ( 900 - 72 ),               true,               72,                   "Coup a coup",   60,            SOUND_SINGLE_SHOOT),
    LaserGameShootingMode(NORMAL_IR_PIN,    true,         36,                  ( 900 - 36 ),               true,               36,                   "Mitrailette",   160,           SOUND_BURST_SHOOT),
    LaserGameShootingMode(GRENADE_IR_PIN,   false,        1,                   ( 3 - 1 ),                  false,              1,                    "Grenade",       2290,          SOUND_GRENADE_SHOOT)
};

/* définition des différents rubans leds en utilisant la librairie LaserGameLED */
LaserGameLED shooter_leds  (SHOOTER_LED_COUNT,   SHOOTER_LED_PIN,  NEO_GRB + NEO_KHZ400); // cercle de LED à l'aant du gun pour indiquer quand on tire
LaserGameLED status_leds   (STATUS_LED_COUNT,    STATUS_LED_PIN,   NEO_GRB + NEO_KHZ400); // LED à l'avant du gun pour indiquer l'état de l'arme (allumée quand le joueur est actif ou éteinte quand le joueur s'est fait tirer dessus)
LaserGameLED magazine_leds (MAGAZINE_LED_COUNT,  MAGAZINE_LED_PIN, NEO_GRB + NEO_KHZ400); // rubans LEDs sur les cotés du gun pour indiquer le niveau du chargeur

/* définition des différents objets pour gérer les boutons */
LaserGamePushButton trigger_button ( TRIGGER_PIN );
LaserGamePushButton mode_button ( MODE_PIN );
LaserGamePushButton reload_button ( RELOAD_PIN );

/* définition des objets utiles pour la gestion du son */
HardwareSerial mySoftwareSerial(1);
DFRobotDFPlayerMini myDFPlayer;

/* définition de l'objet utilie pour l'écran */
LaserGameScreen display (SCREEN_WIDTH, SCREEN_HEIGHT);

/* définition des timers */
LaserGameTimer game_duration_timer (gameInformation.params.gameDurantion);  // Timer pour gérer la durée de la partie
LaserGameTimer screen_refresh_timer (200);                                  // Timer pour le rafraichissement de l'écran OLED
LaserGameTimer trigger_timer (0);                                           // Timer pour gérer le délais entre deux tirs (le délais va varier en fonction du mode de tir)
LaserGameTimer reload_btn_timer (150);                                      // Timer pour gérer le bouton pour recharge l'arme
LaserGameTimer mode_btn_timer (150);                                        // Timer pour gérer le bouton de changement de mode de tir
LaserGameTimer reloading_timer (580);                                       // Timer pour gérer la durée de rechargement de l'arme


/* définition de différentes variables utiles pour le programme */
uint8_t currentShootingMode = 0;      // Id du mode actuel de tir
bool isReloading            = false;  // Boolean qui indique si on est en train de recharger ou pas
                                      // Pour pas que l'on puisse tirer ou recharger en même temps...
                                      // Par contre pendant ce temps, on pourra se faire tirer dessus :D
bool isShooting             = false;  // Boolean qui indique si on est en train de tirer ou non
                                      // pour pas qu'on puisse recharger ou changer d'arme en même temps...
                                      // Par contre pendant ce temps, on pourra se faire tirer dessus :D
uint32_t myTeamColor        = fromHtmlColor (gameInformation.teams[gameInformation.players[gunUniqId].teamId].color); // Couleur de notre équipe


void setup() {
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Initialisation de la communication avec le port série pour le son
  mySoftwareSerial.begin(9600, SERIAL_8N1, SOUND_RX_PIN, SOUND_TX_PIN);

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Initialisation de la communication avec le moniteur série
  Serial.begin(115200);

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // On attend un peu 
  delay(10);

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Initialisation de l'anneau LED (shooter)
  shooter_leds.init();

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Initialisation de la LED indiquant le statut de l'arme
  status_leds.init();

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Initialisation de la LED indiquant le niveau du chargeur de l'arme
  magazine_leds.init();

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Initialisation du moteur haptic
  pinMode ( HAPTIC_PIN, OUTPUT );
  digitalWrite ( HAPTIC_PIN, LOW );

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Initialisation du récepteur Infra Rouge intégré à l'arme
  IrReceiver.begin (IR_RECEIVE_PIN, false, 0);
  
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Initialisation de l'écran OLED
  Serial.println("-----------------------------------------------------------------------------");
  Serial.print  ("Initialisation de l'écran OLED : ");
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("Echec !");
    error();
  }
  Serial.println("Succès");
  display.drawLogo ();


  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Initialisation de la carte de lecture du son
  Serial.println("-----------------------------------------------------------------------------");
  Serial.print  ("Initialisation du mini lecteur MP3 DFPlayer : ");
  if (!myDFPlayer.begin(mySoftwareSerial)) {  // Utilisation du HardwareSerial pour communiquer avec le MP3
    Serial.println("Echec !");
    Serial.println("Vérifiez la connexion et contrôlez que la carte est bien insérée");
    error();
  }
  Serial.println("Succès");
  myDFPlayer.volume(30);                    // Choix du volume du son. De 0 à 30
  myDFPlayer.EQ(DFPLAYER_EQ_BASS);          // Choix du mode de l'equalizer


  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // On lance le compte à rebours avant le début de la partie
  Serial.println("-----------------------------------------------------------------------------");
  Serial.println("Lancement du compte à rebours avant le début de la partie");
  LaserGameTimer game_start_timer (gameInformation.params.startDelay);
  game_start_timer.restart ();
  while ( !game_start_timer.isExpired() ) {
    int remaining = int(game_start_timer.remainingTime() / 1000);
    display.clearDisplay();
    display.drawTextCentered (String(remaining), 8);
    display.display();
    delay (80);
  }
    

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // La partie peut démarrer !!!
  Serial.println("-----------------------------------------------------------------------------");
  Serial.println("Lancement de la partie !");
  digitalWrite ( HAPTIC_PIN, HIGH );
  myDFPlayer.playMp3Folder(SOUND_ARMOR_ON); // On joue un son
  shooter_leds.colorWipe (myTeamColor, 30); // On allume progressivement les LEDs de l'anneau sur la face avant du gun
  shooter_leds.colorWipe (0, 30);           // On eteins progressivement les LEDs de l'anneau sur la face avant du gun
  status_leds.colorWipe (myTeamColor);      // On change la couleur de la LED qui indique que l'arme est allumée et prête
  swapShootingMode ();                      // On initialise l'arme avec le mode de tir séléctionné
  game_duration_timer.restart();            // Lancement du timer pour la durée de la partie
  digitalWrite ( HAPTIC_PIN, LOW );
}

void loop() {
  if ( !game_duration_timer.isExpired() ) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Dans le cas où la partie est toujours en cours
    ////////////////////////////////////////////////////////
    // On regarde si on s'est fait tirer dessus au niveau de
    // l'arme. Les tirs qui viennent du plastron ne sont pas
    // gérés dans cette version...
    // Pas de timer sur cette étape (on regarde tout le 
    // temps pour plus de précision)
    if (IrReceiver.decode()) {                          // Nous avons reçu quelque chose
      if (IrReceiver.decodedIRData.protocol == NEC) {   // Nous ne traitons que les message envoyés avec le protocol NEC
        shooted ( IrReceiver.decodedIRData.command, IrReceiver.decodedIRData.address, SHOOT_LOCATION_GUN );
      }
      IrReceiver.resume();                              // On rend possible le fait de recevoir à nouveau quelque chose
    }

    ////////////////////////////////////////////////////////
    // Mise à jour de l'écran OLED
    if ( screen_refresh_timer.isExpired() ) {
      display.drawInfo (SHOOTING_MODES[currentShootingMode], game_duration_timer);
    }

    ////////////////////////////////////////////////////////
    // Gestion du rechargement de l'arme
    // (quand elle vient de se terminer)
    if ( isReloading && reloading_timer.isExpired() ) {
      // On calcul le nombre de munition dont nous allons avoir besoin 
      // par défaut on a besoin de remplir le chargeur donc on a besoin de la taille de notre chargeur moins ce qu'il contient encore
      // mais on ne prendra que ce qu'il nous reste de munition dans notre inventaire si l'inventaire est inférérieur à ce dont on besoin
      uint8_t needed = min (SHOOTING_MODES[currentShootingMode].getMagazineSize() - SHOOTING_MODES[currentShootingMode].getMagazine(), int(SHOOTING_MODES[currentShootingMode].getAmmo()));
      // On recharge le chargeur
      SHOOTING_MODES[currentShootingMode].reloadMagazine (needed);
      // On met à jour les LEDs du chargeur
      updateMagazineLeds ();
      // On relance le timer pour le bouton de rechargement
      reload_btn_timer.restart();
      // On indique qu'on est plus en train de recharger
      isReloading = false;
    }    

    ////////////////////////////////////////////////////////
    // Gestion de la gachette
    if ( !isReloading && trigger_timer.isExpired() ) {
      if ( trigger_button.hasChanged() ) {
        ////////////////////////////////////////////////////
        // L'état de la gachette a changé
        if (trigger_button.isLow()) {
          //////////////////////////////////////////////////
          // Le joueur vient d'appuyer dessus : 
          //  on commence à tirer
          startShooting ();
        } else if (trigger_button.isHigh ()) {
          //////////////////////////////////////////////////
          // Le joueur vient de la relacher
          //  on arrête de tirer
          stopShooting ();
        }
      } else {
        ////////////////////////////////////////////////////
        // L'état de la gachette a changé
        if (trigger_button.isLow()) {
          //////////////////////////////////////////////////
          // Le joueur est resté appuyé dessus
          //  on continu de tirer
          continueShooting ();
        }
      }
    }

    ////////////////////////////////////////////////////////
    // Gestion du bouton de rechargement de l'arme
    if ( !isShooting && !isReloading && reload_btn_timer.isExpired() ) {
      if ( reload_button.hasChanged() && reload_button.isLow() ) {
        ////////////////////////////////////////////////////
        // L'état du bouton de rechargement a changé et il
        // est appuyé alors on lance un rechargement.
        if ( ( SHOOTING_MODES[currentShootingMode].getAmmo() > 0 || SHOOTING_MODES[currentShootingMode].isEndlessAmmo() ) && SHOOTING_MODES[currentShootingMode].getMagazine() < SHOOTING_MODES[currentShootingMode].getMagazineSize() ) {
          // On indique qu'on est en train de recharger
          isReloading = true;
          // On joue le son pour indique qu'on recharge l'arme
          myDFPlayer.playMp3Folder( SOUND_RELOAD_START );
          // On lance le timer gérant le temps de rechargement
          reloading_timer.restart ();
        }
      }
    }

    ////////////////////////////////////////////////////////
    // Gestion du bouton de changement de mode de tir
    if ( !isShooting && !isReloading && mode_btn_timer.isExpired () ) {
      if ( mode_button.hasChanged() && mode_button.isLow() ) {
        ////////////////////////////////////////////////////
        // L'état du bouton de changement de mode de tir a 
        // changé et il est appuyé alors on lance un
        // rechargement.
        myDFPlayer.playMp3Folder(SOUND_ARMOR_SWAP);
        currentShootingMode = ( currentShootingMode + 1 ) %  3;
        swapShootingMode ();
      }
      mode_btn_timer.restart();
    }
  } else {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Dans le cas où la partie est terminée
    Serial.println ("La partie terminée...");
    stopShooting();
    status_leds.colorWipe(0);
    magazine_leds.colorWipe(0);
    bool inverted = false;
    while (1) {
      display.gameOver(inverted);
      inverted = !inverted;
      delay (1500);
    }
  }
}

/**
 * @brief Fonction qui déclenche les actions à faire quand on change le mode de l'arme
 */
void swapShootingMode () {
  IrSender.begin(SHOOTING_MODES[currentShootingMode].getIrPin(), false);        // On indique le pin à utiliser pour envoyer les messages infra rouge.
  trigger_timer.setDelay (SHOOTING_MODES[currentShootingMode].getShootTime());  // On indique le délais entre deux tir en fonction du mode de tir actuel
  updateMagazineLeds ();                                                        // On met à jour l'affichage des LEDs qui indiquent le niveau du chargeur
}

/**
 * @brief fonction qui permet de se mettre en erreur (modification des LEDs)
 */
void error () {
  status_leds.initStep(200);
  shooter_leds.initStep(200);
  while (1) {
    status_leds.colorBreath (status_leds.Color(255, 0, 0));
    shooter_leds.colorBreath (shooter_leds.Color(255, 0, 0));
    delay(5);
  }
}

/**
 * @brief Fonction à utiliser quand le joueur commence à tirer
 */
void startShooting () {
  // Dans tous les cas, on ne tire que si le chargeur contient encore des munitions
  if ( SHOOTING_MODES[currentShootingMode].getMagazine() > 0 ) {
    // On fait vibrer le retour haptic
    digitalWrite ( HAPTIC_PIN, HIGH );
    // On déclenche l'envoi d'un message infra rouge (On utilise le protocole NEC pour cela)
    IrSender.sendNEC(gunUniqId, currentShootingMode, 0);
    // On joue le son correspondant au mode de tir
    myDFPlayer.playMp3Folder( SHOOTING_MODES[currentShootingMode].getShootSound() );
    // En fonction du type de tir (SINGLE ou BURST)
    if ( SHOOTING_MODES[currentShootingMode].isBurst() ) {
      // On joue le son du mode de tir mais en mode "boucle"
      // myDFPlayer.loop( SHOOTING_MODES[currentShootingMode].getShootSound() );
      // On initialise un cycle pour les LEDs du shooter
      shooter_leds.initStep ();
      // On allume les LEDs du shooter
      shooter_leds.colorCircle( myTeamColor );
    } else {
      // On joue le son correspondant au mode de tir
      // myDFPlayer.playMp3Folder( SHOOTING_MODES[currentShootingMode].getShootSound() );
      // On allume les LEDs du shooter
      shooter_leds.colorWipe ( myTeamColor );
    }
    // On consomme une munition
    SHOOTING_MODES[currentShootingMode].useOneAmmo ();
    // On met à jour les LEDs du chargeur
    updateMagazineLeds ();
    // On relance le timer de la gachette
    trigger_timer.restart ();
    // On indique qu'on est en train de shooter
    isShooting = true;
  } else {
    // Le joueur n'a plus de munitions. On ne fait rien
    myDFPlayer.playMp3Folder (SOUND_ARMOR_EMPTY);
    // On arrête de shooter s'il n'y a plus de munitions
    stopShooting ();
  }
}

/**
 * @brief Fonction à utiliser quand l'utilisateur continue de tirer
 * 
 */
void continueShooting () {
  // Dans tous les cas, on ne tire que si le chargeur contient encore des munitions
  if ( SHOOTING_MODES[currentShootingMode].getMagazine() > 0 ) {
    if ( SHOOTING_MODES[currentShootingMode].isBurst() ) {
      // On joue le son correspondant au mode de tir
      myDFPlayer.playMp3Folder( SHOOTING_MODES[currentShootingMode].getShootSound() );
      // Si c'est un mode Burst, on peut continuer de shooter
      // On déclenche l'envoi d'un message infra rouge (On utilise le protocole NEC pour cela)
      IrSender.sendNEC(gunUniqId, currentShootingMode, 0);
      // On met à jour l'affichage des LEDs du shooter
      shooter_leds.colorCircle(myTeamColor);
      // On utilise une munition
      SHOOTING_MODES[currentShootingMode].useOneAmmo ();
      // On met à jour les LEDs du chargeur
      updateMagazineLeds ();
      // On relance le timer de la gachette
      trigger_timer.restart ();
    } else {
      // Si ce n'est pas un mode burst, on arrête de shooter
      stopShooting ();
    }
  } else {
    // Le joueur n'a plus de munitions. On ne tire plus
    myDFPlayer.playMp3Folder (SOUND_ARMOR_EMPTY);
    // On arrête de shooter s'il n'y a plus de munitions
    stopShooting ();
  }
}

/**
 * @brief Fonction à utiliser quand le joueur arrête de tirer
 */
void stopShooting () {
  digitalWrite ( HAPTIC_PIN, LOW ); // On stop les vibrations
  shooter_leds.colorWipe (0);       // On stop les LEDs sur le shooter
  // myDFPlayer.disableLoop();         // Si nous étions dans une boucle au niveau du son (mode burst), on l'arrête
  isShooting = false;               // On indique qu'on est plus en train de tirer
}

/**
 * @brief Fonction qui permet de mettre à jour les LEDs du chargeur
 */
void updateMagazineLeds () {
  // On calcul ensuite combien il faut en allumer
  uint16_t numPixel = (float)magazine_leds.numPixels() / (float)SHOOTING_MODES[currentShootingMode].getMagazineSize() * (float)SHOOTING_MODES[currentShootingMode].getMagazine();
  // On allume toutes les leds jusqu'au niveau souhaité
  for (uint16_t i=0; i<numPixel; i++) magazine_leds.setPixelColor(i, myTeamColor);
  // On éteind les autres
  for (uint16_t i=numPixel; i<magazine_leds.numPixels(); i++) magazine_leds.setPixelColor(i, 0);
  // On affiche ce qu'on a fait
  magazine_leds.show();
}

/**
 * @brief Fonction qui gère les actions à réaliser quand on s'est fait tirer dessus
 * 
 * @param weaponId Identifiant du mode de tir avec lequel on a été touché
 * @param shooter Identifiant unique de l'arme qui nous a touché 
 * @param location Identifiant de l'endoit où l'on s'est fait toucher
 */
void shooted (uint16_t weaponId, uint16_t shooter, uint16_t location ) {
  // On commence par arrêter de tirer
  stopShooting ();
  // On joue le son pour indiquer qu'on s'est fait toucher
  myDFPlayer.playMp3Folder (SOUND_SHOOTED);
  // On éteind les LEDs statut et chargeur (le shooter c'est déjà fait dans stopShooting)
  status_leds.colorWipe (0);
  magazine_leds.colorWipe (0);
  // On initialise un timer de 10 seconde pour attendre avant de respawn
  LaserGameTimer respawn_timer ( gameInformation.params.respawnDelay );
  respawn_timer.restart ();
  // On affiche les informations sur la personne qui nous a touchée
  display.showShooter (gameInformation.players[shooter].pseudo);
  delay(2000);
  // Puis on affiche un compte à rebours pour indiquer quand on va respawn
  while ( !respawn_timer.isExpired() ) {
    int remaining = int(respawn_timer.remainingTime() / 1000);
    display.clearDisplay();
    display.drawTextCentered (String(remaining), 8);
    display.display();
    delay (80);
  }
  // Une fois le temps écoulé, on peut ré-activer l'arme
  digitalWrite ( HAPTIC_PIN, HIGH );
  myDFPlayer.playMp3Folder(SOUND_ARMOR_ON); // On joue un son
  shooter_leds.colorWipe (myTeamColor, 30); // On allume progressivement les LEDs de l'anneau sur la face avant du gun
  shooter_leds.colorWipe (0, 30);           // On eteins progressivement les LEDs de l'anneau sur la face avant du gun
  status_leds.colorWipe (myTeamColor);      // On change la couleur de la LED qui indique que l'arme est allumée et prête
  swapShootingMode ();                      // On initialise l'arme avec le mode de tir séléctionné
  digitalWrite ( HAPTIC_PIN, LOW );
}

uint32_t fromHtmlColor (String htmlColor) {
  unsigned long n = strtol( htmlColor.c_str()+1, nullptr, 16 );
  byte r = n >> 16;
  byte g = n >> 8;
  byte b = n;
  return Adafruit_NeoPixel::Color(int(r), int(g), int(b));;
}
