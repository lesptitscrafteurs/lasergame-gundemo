/**
 * @brief Objet LaserGameTimer qui permet de définir une durée souhaiter et de contrôler si cette durée est dépassée ou non.
 * Cette classe permet de définir autant de Timer qu'on le souhaite sans avoir à utiliser la fonction delay
 */
class LaserGameTimer {
    private:
        unsigned long startTime;
        unsigned long delay;

    public:
        /**
         * @brief Constructeur d'un nouvel objet de type LaserGameTimer
         * 
         * @param aDelay durée souhaitée pour le timer
         */
        LaserGameTimer ( unsigned long aDelay);
        /**
         * @brief Fonction qui permet de démarrer le timer
         * 
         */
        void restart ();
        /**
         * @brief Fonction qui permet de savoir si le timer est dépassé ou non
         * 
         * @param showLog boolean qui indique si on doti afficher des logs ou non (pour debug uniquement)
         * 
         * @return boolean Vrai si le timer est terminé, Faux sinon
         */
        bool isExpired (bool showLog = false);
        /**
         * @brief Fonction qui permet d'obtenir le nombre de millisecondes restantes avant la fin du timer
         * 
         * @return long nombre de millisecondes avant la fin du Timer
         */
        long remainingTime ();
        /**
         * @brief Fonction qui permet de modifier le delais souhaité
         * 
         */
        void setDelay (unsigned long aDelay);
};

LaserGameTimer::LaserGameTimer ( unsigned long aDelay ) {
    delay = aDelay;
    startTime = millis();
}

void LaserGameTimer::restart () {
    startTime = millis();
}

bool LaserGameTimer::isExpired (bool showLog) {
    long m = millis ();
    bool result = ((m - startTime) > delay);
    if ( showLog ) Serial.println (String (m) + " - " + String(startTime) + " > " + String (delay) + " --> " + String (result));
    return result;
}

long LaserGameTimer::remainingTime () {
    long rem = startTime + delay - millis();
    return rem;
}

void LaserGameTimer::setDelay (unsigned long aDelay) {
    delay = aDelay;
    startTime = millis() - delay;
}