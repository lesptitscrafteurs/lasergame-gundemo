class LaserGameShootingMode {
    private:
        /**
         * @brief Indique si le mode de tir est rafale ou pas
         */
        bool burst;
        /**
         * @brief taille du chargeur
         */
        uint16_t magazineSize;
        /**
         * @brief nombre de munition totale dans le packatage (uniquement utile si le endlessAmmo est à false)
         */
        uint16_t ammo;
        /**
         * @brief durée du rechargement en millisecondes
         */
        uint16_t reloadTime;
        /**
         * @brief munition infinie (dans ce cas la variable ammo n'est pas importante)
         */
        bool endlessAmmo;
        /**
         * @brief nombre de munitions présente actuellement dans le chargeur
         */
        uint16_t magazine;
        /**
         * @brief label du mode de tir
         */
        String label;
        /**
         * @brief durée d'un tir en millisecondes (temps d'attente entre deux tirs)
         */
        uint16_t shootTime;
        /**
         * @brief identifiant du son à jouer quand on tir dans ce mode
         */
        uint8_t shootSound;
        /**
         * @brief Numéro du pin à utiliser pour envoyer le signal Infrarouge
         * 
         */
        uint8_t irPin;

    public:
        /**
         * @brief Constructeur d'un nouvel objet de type LaserGameShooting
         * 
         * @param Numéro du pin à utiliser pour envoyer le signal Infrarouge
         * @param isBurst indique si c'est un mode de tir en rafale ou pas
         * @param magazineSize taille du chargeur
         * @param ammo nombre de munition totale dans le packatage (uniquement utile si le endlessAmmo est à false)
         * @param endlessAmmo munition infinie
         * @param magazine nombre de munitions présente actuellement dans le chargeur
         * @param label label du mode de tir
         * @param shootTime durée d'un tir (temps d'attente entre deux tirs)
         * @param shootSound identifiant du son à jouer quand on tir dans ce mode
         */
        LaserGameShootingMode (uint8_t irPin, bool burst, uint16_t magazineSize, uint16_t ammo, bool endlessAmmo, uint16_t magazine, String label, uint16_t shootTime, uint8_t shootSound);
        /**
         * @brief Rtourne la taille du chargeur
         * 
         * @return uint16_t taille du chargeur
         */
        uint16_t getMagazineSize ();
        /**
         * @brief Retourne le nombre total de munitions
         * 
         * @return uint16_t quantité totale de munitions
         */
        uint16_t getAmmo ();
        /**
         * @brief Indique si les munitions sont infinie pour ce mode de tir
         * 
         * @return true si les munitions sont infinies
         * @return false si les munitions ne sont pas infinies
         */
        bool isEndlessAmmo ();
        /**
         * @brief Retourne la quantité de munitions actuellement présentent dans le chargeur
         * 
         * @return uint16_t nombre de munitions actuelement présentent dans le chargeur
         */
        uint16_t getMagazine ();
        /**
         * @brief Retourne le label de ce mode de tir
         * 
         * @return String label du mode de tir
         */
        String getLabel ();
        /**
         * @brief Retourne la durée d'un tir
         * 
         * @return uint16_t durée d'un tir en millisecondes
         */
        uint16_t getShootTime ();
        /**
         * @brief Fonction qui utilise un munition du chargeur
         */
        void useOneAmmo ();
        /**
         * @brief Ajoute des munitions dans le chargeur
         * 
         * @param size quantité de munitions à ajouter
         */
        void reloadMagazine (int size);
        /**
         * @brief retourne l'id du son à jouer quand on tir dans ce mode
         * 
         * @return uint8_t id du son à jouer quand on tir dans ce mode
         */
        uint8_t getShootSound ();
        /**
         * @brief indique si le mode de tir est en rafale ou non
         * 
         * @return true si c'est un mode rafale
         * @return false sinon
         */
        bool isBurst ();

        uint8_t getIrPin ();
};

LaserGameShootingMode::LaserGameShootingMode (uint8_t aPin,  bool _burst, uint16_t _magazineSize, uint16_t _ammo, bool _endlessAmmo, uint16_t _magazine, String _label, uint16_t _shootTime, uint8_t _shootSound) {
    irPin = aPin;
    burst = _burst;
    magazineSize = _magazineSize;
    ammo = _ammo;
    endlessAmmo = _endlessAmmo;
    magazine = _magazine;
    label = _label;
    shootTime = _shootTime;
    shootSound = _shootSound;
}

uint16_t LaserGameShootingMode::getMagazineSize () {
    return magazineSize;
}

uint16_t LaserGameShootingMode::getAmmo () {
    return ammo;
}

bool LaserGameShootingMode::isEndlessAmmo () {
    return endlessAmmo;
}

uint16_t LaserGameShootingMode::getMagazine () {
    return magazine;
}

String LaserGameShootingMode::getLabel () {
    return label;
}

uint16_t LaserGameShootingMode::getShootTime () {
    return shootTime;
}

void LaserGameShootingMode::useOneAmmo () {
    magazine--;
}

void LaserGameShootingMode::reloadMagazine (int size) {
    if ( ( ammo > 0 || endlessAmmo ) && magazine < magazineSize ) {
        uint8_t needed = min(min (size, magazineSize - magazine), int(ammo));
        magazine += needed;
        ammo -= needed;
    }
}

uint8_t LaserGameShootingMode::getShootSound () {
    return shootSound;
}

bool LaserGameShootingMode::isBurst () {
    return burst;
}

uint8_t LaserGameShootingMode::getIrPin () {
    return irPin;
}