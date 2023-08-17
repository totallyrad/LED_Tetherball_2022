#include <FastLED.h>

/*
  AnalogReadSerial

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogReadSerial
*/

/*
 * LED Connections: Data to Pin 3, ground to GND, Power to VIN (5v)
 * Sensor connetions: VCC 5v pin, Ground to gnd, X to Analog 3. 
 * 
 */

#define DATA_PIN    3
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    400
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

const int xpin = A1;                  // x-axis of the accelerometer



// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);


}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int lightpattern;
  int X = analogRead(xpin)-370; // The value subtracted is the raw sensor value removed to give us 0

  int XYZavg;
  int sensVal;
 

// XYZavg take the absolute values of X, Y and Z which forces them to be positive values and adds them together
 
//  XYZavg = (abs(X)) + (abs(Y)) + (abs(Z)); 
  XYZavg = (abs(X));   

// Take the XYZavg value and constrain it to the min and max values the sensor provides, in this case 0 and 300

  sensVal = constrain(XYZavg, 0, 50);

//  lightpattern is the sensVal mapped so that the min/max set above is converted to the 0-255 rnge we have for patterns
  
  lightpattern = map (sensVal, 0, 50, 0, 255); 

  
// This section takes the range of sensor input value and changes the LED pattern based on what range it is in.

// Light pattern 1 (rest/attract mode)

  if ((lightpattern >= 0) && (lightpattern <= 80)) {
  sinelon();
  }

// Light pattern 2 (detects light movement)

  else if ((lightpattern >= 81) && (lightpattern <= 150)) {
  juggle();
  } 

// Light pattern 3 (detects heavier movement)

  else if ((lightpattern >= 151) && (lightpattern <= 200)) {
  bpm();
  }

// Light pattern 4 (hardest hit)

   else if ((lightpattern >= 201) && (lightpattern <= 255)) {
  rainbowWithGlitter();
  }

  FastLED.show();


  // print the sensor values:
//  Serial.print(X);

//    Serial.print("\t");
//  Serial.print(lightpattern);
//  Serial.println();
//  delay(1);        // delay in between reads for stability
}

// All of the LED patters code can go below here

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(100);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 170;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  uint8_t dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
