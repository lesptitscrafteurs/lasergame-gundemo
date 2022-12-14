#define LOGO_HEIGHT           64
#define LOGO_WIDTH            64

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const unsigned char logo_bmp [] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x06, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x30, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x3f, 0x80, 0x40, 0x00, 
    0x00, 0x00, 0x00, 0x1c, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0x80, 0x00, 
    0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0x80, 0x00, 
    0x00, 0x00, 0xa0, 0x7f, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x63, 0xff, 0xff, 0xff, 0x80, 0x00, 
    0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xc0, 0x00, 
    0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xc0, 0x1c, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xf0, 0x3c, 
    0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xf8, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0x80, 
    0x0f, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x1f, 0xc0, 0x3f, 0xff, 0xff, 0xff, 0xfe, 0x00, 
    0x3f, 0xe0, 0x7f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 
    0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 
    0x1f, 0xc1, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x16, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 
    0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xe0, 
    0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x00, 
    0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xfe, 0x00, 
    0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 
    0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xfc, 0x00, 
    0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xe0, 
    0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xf8, 
    0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0x1f, 0xf8, 0x00, 0x00, 0x01, 0xfb, 0xff, 0xfe, 0x0f, 0xf8, 
    0x00, 0x00, 0x03, 0xc0, 0xff, 0xfc, 0x07, 0xf0, 0x00, 0x00, 0x03, 0xc0, 0x7e, 0x3c, 0x07, 0xf0, 
    0x00, 0x00, 0x07, 0x80, 0x38, 0x0c, 0x03, 0xc0, 0x00, 0x00, 0x03, 0x00, 0x20, 0x0c, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

class LaserGameScreen: public Adafruit_SSD1306 {
  private:
    
  public:
    /**
     * @brief Constructeur d'un nouvel objet de type LaserGameScreen
     * 
     * @param w largeur de l'??cran en pixel
     * @param h hauteur de l'??cran en pixel
     * @param twi 
     * @param rst_pin reset pin
     */
    LaserGameScreen (uint8_t w, uint8_t h, TwoWire *twi = &Wire, int8_t rst_pin = -1) : Adafruit_SSD1306 (w, h, twi, rst_pin) { }
    /**
     * @brief Affiche le logo sur l'??cran
     * 
     * @param screen 
     */
    void drawLogo ();
    /**
     * @brief Fonction qui affiche sur l'??cran un text ?? la taille souhait??e et dans la couleur souhait??e en le centrant sur l'??cran.
     * 
     * @param text Texte ?? afficher
     * @param size Taille souhait??e pour le texte
     * @param color Couleur souhait??e pour le texte. Peux ??tre SSD1306_WHITE ou SSD1306_BLACK
     */
    void drawTextCentered (String text, uint8_t size, uint16_t color = SSD1306_WHITE);
    /**
     * @brief Affiche sur l'??cran OLED les informations sur la partie
     * 
     * @param shootingMode mode de tir actuel
     * @param gameTimer Timer g??rant la dur??e de la partie. Utilis?? pour afficher le temps restant
     * @param color Couleur souhait??e l'affichage. Peux ??tre SSD1306_WHITE ou SSD1306_BLACK
     */
    void drawInfo (LaserGameShootingMode shootingMode, LaserGameTimer gameTimer, uint16_t color = SSD1306_WHITE);
    /**
     * @brief Affiche ?? l'??cran que la partie est termin??e 
     * 
     * @param invers Si vrai ca passe ?? noir sur blanc. Sinon c'est blanc sur noir
     */
    void gameOver (bool invers);
    /**
     * @brief Fonction qui permet d'afficher le nom de la personne qui nous a tir?? dessus
     * 
     * @param shooter pseudo de la personne qui nous a tir?? dessus
     */
    void showShooter (String shooter, uint16_t color = SSD1306_WHITE);
};

void LaserGameScreen::drawLogo () {
    clearDisplay();
    drawBitmap(
        (width() - LOGO_WIDTH ) / 2,
        (height() - LOGO_HEIGHT) / 2,
        logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
    display();
}

void LaserGameScreen::drawTextCentered (String text, uint8_t size, uint16_t color) {
    int16_t x1, y1;
    uint16_t w, h;
    setTextSize (size);                                      // On change la taille du texte
    getTextBounds(text, 0, 0, &x1, &y1, &w, &h);             // On recherche la place que ca prend sur l'??cran pour calculer la position pour le centrer
    setCursor (int((width()-w)/2), int((height()-h)/2));     // On se positionne ?? la position calcul??e pr??c??dement
    setTextColor (color);                                    // On change la couleur du texte pour mettre celle souhait??e
    println (text);
}

void LaserGameScreen::gameOver (bool invers) {
    // On commence par retirer ce qui est affich?? ?? l'??cran
    clearDisplay ();
    setTextColor (SSD1306_WHITE);   // On change la couleur du texte pour mettre celle souhait??e
    setTextSize (3);                // On change la taille du texte

    int16_t x1, y1;
    uint16_t w1, h1, w2, h2;
    getTextBounds("Game", 0, 0, &x1, &y1, &w1, &h1);             // On recherche la place que ca prend sur l'??cran pour calculer la position pour le centrer
    getTextBounds("Over", 0, 0, &x1, &y1, &w2, &h2);             // On recherche la place que ca prend sur l'??cran pour calculer la position pour le centrer
    setCursor (int((width()-w1)/2), int((height()-h1-h2-4)/2));     // On se positionne ?? la position calcul??e pr??c??dement
    println ("Game");
    setCursor (int((width()-w2)/2), int((height()-h1-h2-4)/2)+h1+4);     // On se positionne ?? la position calcul??e pr??c??dement
    println ("Over");

    // On inverse les couleur en fonction de ce qui est demand?? en param??tre
    invertDisplay(invers);
    // Pour finir, on affiche sur l'??cran
    display();
}

void LaserGameScreen::showShooter (String shooter, uint16_t color) {
    // On commence par retirer ce qui est affich?? ?? l'??cran
    clearDisplay ();
    setTextColor (SSD1306_WHITE);   // On change la couleur du texte pour mettre celle souhait??e
    setTextSize (3);                // On change la taille du texte

    int16_t x1, y1;
    uint16_t w1, h1, w2, h2;

    setTextSize (2);                                          // On change la taille du texte
    getTextBounds(shooter, 0, 0, &x1, &y1, &w2, &h2);         // On recherche la place que ca prend sur l'??cran pour calculer la position pour le centrer
    setCursor (int((width()-w2)/2), int((height()-h2)/2));    // On se positionne ?? la position calcul??e pr??c??dement
    println (shooter);

    setTextSize (1);                                                // On change la taille du texte
    getTextBounds("Touch" "\x82" " par", 0, 0, &x1, &y1, &w1, &h1);  // On recherche la place que ca prend sur l'??cran pour calculer la position pour le centrer
    setCursor (int((width()-w1)/2), int((height()-h2)/2) - h1 - 4); // On se positionne ?? la position calcul??e pr??c??dement
    println ("Touch" "\x82" " par");
    
    // Pour finir, on affiche sur l'??cran
    display();
}

void LaserGameScreen::drawInfo (LaserGameShootingMode shootingMode, LaserGameTimer gameTimer, uint16_t color) {
    // On commence par retirer ce qui est affich?? ?? l'??cran
    clearDisplay ();
    // On dessine ensuite les lignes aux quatres coins de l'??cran.
    drawLine(0,             0,          0,            6,            color);
    drawLine(0,             0,          15,           0,            color);
    drawLine(width()-1,     0,          width()-1,    6,            color);
    drawLine(width()-1-15,  0,          width()-1,    0,            color);
    drawLine(0,             height()-1, 0,            height()-1-6, color);
    drawLine(0,             height()-1, 15,           height()-1,   color);
    drawLine(width()-1,     height()-1, width()-1,    height()-1-6, color);
    drawLine(width()-1,     height()-1, width()-1-15, height()-1,   color);
    // On dessine ensuite les 3 lignes horizontales s??paratrices
    drawLine(20, height()/4,   width()-1-20, height()/4,    color);
    drawLine(20, height()/4*2, width()-1-20, height()/4*2,  color);
    drawLine(20, height()/4*3, width()-1-20, height()/4*3,  color);
    // On change la couleur et la taille du texte pour la suite
    setTextSize(1);
    setTextColor(color);
    // Sur la premi??re ligne, on affiche le mode de tir actuel
    setCursor (6, 4);
    print ("Mode : ");
    println (shootingMode.getLabel());
    // Sur la seconde ligne, on affiche l'??tat des munitions pour ce mode de tir
    setCursor(6, height()/4+4);
    print ("Chargeur : ");
    print (shootingMode.getMagazine());
    if (shootingMode.isEndlessAmmo()) println ("");             // Si les munitions sont infinies on ne fait rien
    else println (" / " + String (shootingMode.getAmmo()));     // Si les munitions ne sont pas infinies on affiche le stocke de munitions
    // Sur la troisi??me ligne, on affiche le temps restant
    setCursor(6, height()/4*3+4);
    print ("Dur" "\x82" "e : ");
    long remaining = gameTimer.remainingTime ();                                             // On r??cup??re le temps restant dans la partie (en milliseconde)
    int rem_min   = int ( remaining / ( 60 * 1000 ) );                                      // On calcul combien cela fait de minutes restantes
    int rem_sec   = int ( ( remaining - ( rem_min * 60 * 1000 ) ) / 1000 );          // Puis on calcul le nombre de seconde restantes
    println ( String(rem_min) + ":" + ((rem_sec < 10)?"0":"") + String(rem_sec) );          // On affiche le r??sultat de notre calcul
    // Pour finir, on affiche sur l'??cran
    display();
}