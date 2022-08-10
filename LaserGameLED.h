#include <Adafruit_NeoPixel.h>    // Il est nécessaire d'inclure cette librairie pour la gestion des leds

#define BREATH_MODE_TRIANGLE  100
#define BREATH_MODE_CIRCULAR  101
#define BREATH_MODE_GAUSSIAN  102

const uint8_t defaultLedBrightness = 200;     // Intensité lumineuse par défaut des LEDs
const float breath_gaussian_gamma = 0.14;     // affects the width of peak (more or less darkness)
const float breath_gaussian_beta = 0.5;       // shifts the gaussian to be symmetric


class LaserGameLED: public Adafruit_NeoPixel {
    private:
        uint32_t step;
        uint32_t ceil;

    public:
        /**
         * @brief Constructeur d'un nouvel objet de type LaserGameLED
         * 
         * @param n nombre de LED
         * @param pin pin auquel sont rattachées les LEDs
         * @param type type de LEDs
         */
        LaserGameLED (uint16_t n, int16_t pin = 6, neoPixelType type = NEO_GRB + NEO_KHZ800) : Adafruit_NeoPixel (n, pin, type) { }

        /**
         * @brief Fonction qui permet d'affecter une couleur aux LEDs d'un même ruban les unes après les autres en respectant un délais entre chaque
         * 
         * @param leds ruban contenant les LEDs à modifier
         * @param color couleur à affecter aux LEDs
         * @param wait delais à respecter entre chaque LED
         */
        void colorWipe (uint32_t color, uint8_t wait=0);

        /**
         * @brief Fonction qui permet de faire varier les leds d'un ruban de façon à simuler la respiration
         * 
         * @param leds ruban contenant les LEDs à modifier
         * @param color couleur à affecter aux LEDs
         * @param breathStep étape de la respiration
         * @param ceil seuil de respiration. Plus le chiffre est grand plus la respiration est lente (par défaut à 500)
         * @param breathMode mode de respiration. Accèpte les valeurs :
         *  - BREATH_MODE_TRIANGLE = 100
         *  - BREATH_MODE_CIRCULAR = 101
         *  - BREATH_MODE_GAUSSIAN = 102
         *  Par défaut sur BREATH_MODE_GAUSSIAN
         */
        void colorBreath (uint32_t color, uint8_t breathMode=BREATH_MODE_GAUSSIAN);

        /**
         * @brief Fonction qui permet de faire tourner la lumière sur un ruban led
         * 
         * @param leds ruban contenant les LEDs à modifier
         * @param color couleur à affecter aux LEDs
         * @param circleStep étape de circulation de la couleur
         */
        void colorCircle (uint32_t color);
        /**
         * @brief Réinitialise à 0 le compte de pas pour les fonctions colorBreath et colorCircle
         */
        void initStep (uint32_t ceil=500);

        void init ();
};

void LaserGameLED::init () {
  begin();
  setBrightness(defaultLedBrightness);
  colorWipe (0);
  show();
}

void LaserGameLED::initStep (uint32_t aCeil) {
    step = 0;
    ceil = aCeil;
}

void LaserGameLED::colorWipe (uint32_t color, uint8_t wait) {
  for (uint16_t numPixel=0; numPixel<numPixels(); numPixel++) {
    setPixelColor (numPixel, color);
    show ();
    delay (wait);
  }
}

void LaserGameLED::colorBreath (uint32_t color, uint8_t breathMode) {
  float pwm_val = 1.0 -  abs((2.0*(float(step%ceil)/ceil))-1.0); // Triangle Wave
  if (breathMode == BREATH_MODE_CIRCULAR) pwm_val = sqrt(1.0 -  pow(abs((2.0*(float(step%ceil)/ceil))-1.0),2.0)); // Circular Wave
  else if (breathMode == BREATH_MODE_GAUSSIAN) pwm_val = exp(-(pow(((float(step%ceil)/ceil)-breath_gaussian_beta)/breath_gaussian_gamma,2.0))/2.0); // Gaussian Wave
  step++;
  uint8_t r = color >> 16;
  uint8_t g = color >> 8;
  uint8_t b = color;
  colorWipe (Color (int(pwm_val*r),int(pwm_val*g),int(pwm_val*b)));
}

void LaserGameLED::colorCircle (uint32_t color) {
  uint16_t start = step % 3;
  step++;
  for (uint16_t i=0; i<numPixels(); i++) setPixelColor(i, 0);
  for (uint16_t i=start; i<numPixels(); i=i+4) setPixelColor(i, color);
  show();
}