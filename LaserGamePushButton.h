/**
 * @brief Classe qui permet d'initialiser et de stocker les informations concernant un bouton poussoir. La classe comporte aussi quelques fonctions pour faciliter l'usage.
 */
class LaserGamePushButton {
  private:
    uint8_t pin;
    uint8_t mode;
    uint8_t lastState; // HIGH ou LOW
    
  public:
    /**
     * @brief Constructeur d'un nouvel objet de type LaserGamePushButton
     * 
     * @param aPin pin sur lequel est rattaché le bouton poussoir
     * @param aMode mode à affecter au bouton lors de son initialisation
     */
    LaserGamePushButton ( uint8_t aPin, uint8_t aMode = INPUT_PULLUP );
    /**
     * @brief Fonction qui indique si l'état du boutton est identique à celui enregistré
     * 
     * @return boolean retourne vrai si l'état du bouton est le même qu'enregistré avant ou faux sinon
     */

    /**
     * @brief Fonction qui indique si l'état du boutton est identique à celui enregistré
     * 
     * @return true Si l'état du bouton a changé depuis le dernier relevé
     * @return false Sinon
     */
    bool hasChanged ();
    /**
     * @brief Fonction qui indique si le dernier état enregistré est LOW ou non
     * 
     * @return boolean vrai si le dernier état enregistré est LOW. Faux sinon
     */
    bool isLow ();
    /**
     * @brief Fonction qui indique si le dernier état enregistré est HIGH ou non
     * 
     * @return boolean vrai si le dernier état enregistré est HIGH. Faux sinon
     */
    bool isHigh (); 
};

LaserGamePushButton::LaserGamePushButton ( uint8_t aPin, uint8_t aMode ) {
  pin = aPin;
  mode = aMode;
  pinMode (pin, mode);
  lastState = HIGH;
}

bool LaserGamePushButton::hasChanged () {
  uint8_t aState = digitalRead(pin);
  if ( aState != lastState ) {
    lastState = aState;
    return true;
  } else return false;
}

bool LaserGamePushButton:: isLow () {
  return lastState == LOW;
}

bool LaserGamePushButton:: isHigh () {
  return lastState == HIGH;
}