//Seed of Life

#include <Adafruit_NeoPixel.h>
#include <Keypad.h>

#define LED_PIN13 13
#define LED_COUNT 188

#define DURATION_BLINK 3000
#define DURATION_RADIATE 10
#define DURATION_PULSE 50
#define DURATION_VORTEX 50

int SLEEP_DURATION = DURATION_BLINK;

String PatternMode = "RADIATE";   /* Modes: BLINK, RADIATE, PULSE, VORTEX */
String ColorMode = "ROTATE";     /* Modes: ROTATE, EXCLUSIVE, RAINBOW   */
int *EXCLUSIVE_COLOR = NULL;

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', '4'}, /* colors 1 - 4                           */
  {'5', '6', '7', '8'}, /* Full Spectrum, colors OFF, ALL Specified Colors   */
  {'9', '0', 'A', 'B'}, /* Modes: Radiate, Pulse, Vortex, Blink   */
  {'<', '>', '-', '+'}  /* decrease brightness, increase brightness, decrease speed, increase speed */
};

int MAX_BRIGHTNESS = 255;

Adafruit_NeoPixel seedStrip(LED_COUNT, LED_PIN13, NEO_GRB + NEO_KHZ800);

/* Colors to rotate or program individually into the buttons. */
int colors[][3] = {
  /* Button Row 1 */
  { 255, 0, 0 },     /* red     */
  { 0, 255, 0 },     /* green   */
  { 250, 238, 6 },   /* yellow    */
  { 250, 172, 28 },    /* orange - FAAC1C  */
  { 255, 255, 255 }  /* OFF */
};

/* Colors to display in rainbow mode */
int rainbowColors[][3] = {
  { 255, 0, 0 },    /* red    */
  { 240, 129, 0 },  /* orange */
  { 240, 233, 16 }, /* yellow */
  { 0, 255, 0 },    /* green  */
  { 0, 0, 255 },    /* blue   */
  { 101, 13, 224 }, /* indigo */
  { 180, 32, 224 }  /* violet */
};


/* Contentric Circle Orientation Index */
/*
     --        --
   (08)      (09)
    --        --
       --  --
      (02)(03)
       --  --
  --  --  --  --  --
  (13)(07)(01)(04)(10)
  --  --  --  --  --
       --  --
      (06)(05)
       --  --
    --        --
   (12)      (11)
    --        --
*/

/* Pin #s that correspond to each concentric circle */
int contrentricCircles[][30] = {
  { 36, 37, 38, 39, 40, 41, 42, 43, 65, 66, 67, 68, 69, 70, 71, 72, 73, 156, 157, 158, 159, 160, 100, 101, 102 },      /* Circle 1 -  Center */
  { 74, 75, 76, 77, 78, 90, 91, 92, 93, 94, 95, 96 },                                                                  /* Circle 2 -  Inner  */
  { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 },                                                                           /* Circle 3 -  Inner  */
  { 130, 131, 132, 133, 134, 135, 148, 149, 150, 151, 152, 153, 154, 155, 156 },                                       /* Circle 4 -  Inner  */
  { 103, 104, 105, 106, 107, 108, 121, 122, 123, 124, 125, 126, 127, 128 },                                            /* Circle 5 -  Inner  */
  { 43, 44, 45, 46, 47, 48, 60, 61, 62, 63, 64, 65, 66 },                                                              /* Circle 6 -  Inner  */
  { 162, 163, 164, 165, 166, 167, 168, 181, 182, 183, 184, 185, 186 },                                                /* Circle 7 -  Inner  */

  { 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90 },                                                      /* Circle 8 -  Outer  */
  { 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 },                                                          /* Circle 9 -  Outer  */
  { 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149 },                                    /* Circle 10 - Outer  */
  { 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121 },                                         /* Circle 11 - Outer  */
  { 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60 },                                                          /* Circle 12 - Outer  */
  { 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181 }                                          /* Circle 13 - Outer  */
};

byte rowPins[ROWS] = { 6, 7, 8, 9 };
byte colPins[COLS] = { 5, 4, 3, 2 };

Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);

  seedStrip.begin();                          // INITIALIZE NeoPixel strip object (REQUIRED)
  seedStrip.show();                           // Turn OFF all pixels ASAP
  seedStrip.setBrightness(MAX_BRIGHTNESS);    // Set BRIGHTNESS to about 1/5 (max = 255)

  printConcatLine("si", "Pixel Count: ", seedStrip.numPixels());
}

void printConcatLine(const char* mask, ...) {
  va_list params;
  va_start(params, mask);

  char *ptr = (char *)mask;
  while (*ptr != '\0') {
    if (*ptr == 'c') {
      int c = va_arg(params, int);
      Serial.write(c);
    } else if (*ptr == 'i') {
      int i = va_arg(params, int);
      Serial.print(i);
    } else if (*ptr == 's') {
      const char *s = va_arg(params, const char *);
      Serial.print(s);
    } else if (*ptr == 'f' || *ptr == 'd') {
      // Be careful with this. It's not portable. On AVR float
      // and double are the same, but that's not the case on other
      // microcontrollers. It would be better to split them.
      double d = va_arg(params, double);
      Serial.print(d);
    } else {
      Serial.write(*ptr);
    }
    ptr++;
  }

  va_end(params);
  Serial.println();
}

void increaseSpeed() {
  SLEEP_DURATION -= 50;
  if (SLEEP_DURATION < 50) SLEEP_DURATION = 50;
  printConcatLine("ci", "Sleep Duration: ", SLEEP_DURATION);
}

void decreaseSpeed() {
  SLEEP_DURATION += 50;
  printConcatLine("ci", "Sleep Duration: ", SLEEP_DURATION);
}

void increaseBrightness() {
  MAX_BRIGHTNESS += 25;
  if (MAX_BRIGHTNESS > 255) MAX_BRIGHTNESS = 255;

  printConcatLine("ci", "Max Brightness: ", MAX_BRIGHTNESS);
}

void decreaseBrightness() {
  MAX_BRIGHTNESS -= 25;
  if (MAX_BRIGHTNESS < 75) MAX_BRIGHTNESS = 75;
  printConcatLine("ci", "Max Brightness: ", MAX_BRIGHTNESS);
}


/* int - Returns 1: on success. */
/*       Returns 2: if EXCLUSIVE_COLOR is set and successful. */

int eventButtonPress() {
  char btnPressed = keypad.getKey();

  printConcatLine("sc", "Button pressed: ", btnPressed);

  if (btnPressed) {
    switch (btnPressed) {
      case '1':
      case '2':
      case '3':
      case '4':
        EXCLUSIVE_COLOR = colors[(int) btnPressed - 49];
        ColorMode = "EXCLUSIVE";

        return 2;
        break;

      case '5':
        ColorMode = "RAINBOW";
        return 2;
        break;

      case '6':
        // Lazy Off Mode
        /*










          //Change to sizeof(colors) last el of array
        */

        EXCLUSIVE_COLOR = colors[4];
        ColorMode = "EXCLUSIVE";
        return 2;
        break;

      case '7':
        ColorMode = "ROTATE";
        return 2;
        break;

      case '8':
        break;

      case '9':
        PatternMode = "RADIATE";
        SLEEP_DURATION = DURATION_RADIATE;
        return 2;
        break;

      case '0':
        PatternMode = "PULSE";
        SLEEP_DURATION = DURATION_PULSE;
        return 2;
        break;

      case 'A':
        PatternMode = "VORTEX";
        SLEEP_DURATION = DURATION_VORTEX;
        return 2;
        break;

      case 'B':
        PatternMode = "BLINK";
        SLEEP_DURATION = DURATION_BLINK;
        return 2;
        break;

      case '<':
        decreaseBrightness();
        break;

      case '>':
        increaseBrightness();
        break;

      case '-':
        decreaseSpeed();
        break;

      case '+':
        increaseSpeed();
        break;
    }
    Serial.println(btnPressed);
  }
  return 1;
}



// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait, int brightness, Adafruit_NeoPixel& seedStrip, int* circleLedArr) {
  printConcatLine("s", "colorWipe");
  
  seedStrip.setBrightness(brightness);

  if(circleLedArr) {
    for(int iii = 0; iii < 30; iii++) {
      if(circleLedArr[iii]) {
        seedStrip.setPixelColor(circleLedArr[iii], color);
      }
    }
  
  } else {
    for(int iii = 1; iii < seedStrip.numPixels(); iii++) {
      seedStrip.setPixelColor(iii, color);
    }
  }

  if(!PatternMode.equals("RADIATE") && !PatternMode.equals("PULSE")) {
    seedStrip.show();
  }
}

bool dimmerRangeCycle(int* color, int brightness, int* concentricCircle) {
  int resp;
  int* passedConcentricCircle = concentricCircle ? concentricCircle : 0;

  colorWipe(seedStrip.Color(color[0], color[1], color[2]), 0, brightness, seedStrip, passedConcentricCircle);
  /*
    resp = eventButtonPress();
    if(resp == 2) {
    //exclusive color set
    return false;
    }
  */
  if(PatternMode.equals("BLINK")) {
    delay(SLEEP_DURATION);
  }

  return true;
}

bool processColor(int* color, int* concentricCircle) {
  printConcatLine("sisisis", "Color: {", color[0], ", ", color[1], ", ", color[2], "}");

  int* passedConcentricCircle = concentricCircle ? concentricCircle : 0;

  bool success = dimmerRangeCycle(color, MAX_BRIGHTNESS, passedConcentricCircle);

  if (!success) {
    printConcatLine("s", "Process Color Failed");
    return false;
  }
  return true;
}

bool processColorRadiate(int* color) {
  seedStrip.clear();

  //process center
  processColor(color, contrentricCircles[0]);
  seedStrip.show();
  delay(SLEEP_DURATION);

  //process inner circle
  processColor(color, contrentricCircles[1]);
  processColor(color, contrentricCircles[2]);
  processColor(color, contrentricCircles[3]);
  processColor(color, contrentricCircles[4]);
  processColor(color, contrentricCircles[5]);
  processColor(color, contrentricCircles[6]);
  seedStrip.show();
  delay(SLEEP_DURATION);

  //process outer circle
  processColor(color, contrentricCircles[7]);
  processColor(color, contrentricCircles[8]);
  processColor(color, contrentricCircles[9]);
  processColor(color, contrentricCircles[10]);
  processColor(color, contrentricCircles[11]);
  processColor(color, contrentricCircles[12]);
  seedStrip.show();
  delay(SLEEP_DURATION);
}

bool processRadiate() {
  printConcatLine("s", "ProcessRadiate");
  if (ColorMode == "ROTATE") {
    for (int x = 0; x < 5; x++ ) {
      /* Skip Black Color (Lazy Off Mode) */
      if (!(colors[x][0] == 255 && colors[x][1] == 255 && colors[x][2] == 255)) {
        processColorRadiate(colors[x]);
      }
    }
  } else if (ColorMode == "EXCLUSIVE") {
    processColorRadiate(EXCLUSIVE_COLOR);
  } else { //RAINBOW
    for (int x = 0; x < 7; x++ ) {
      processColorRadiate(rainbowColors[x]);
    }
  }
}

//process radiate
bool processPulse() {

}

//process radiate
bool processVortex() {

}

//process radiate
bool processBlink() {
  if (ColorMode == "ROTATE") {
    for (int x = 0; x < 5; x++ ) {
      /* Skip Black Color (Lazy Off Mode) */
      if (!(colors[x][0] == 255 && colors[x][1] == 255 && colors[x][2] == 255)) {
        processColor(colors[x], 0);
      }
    }
  } else if (ColorMode == "EXCLUSIVE") {
    processColor(EXCLUSIVE_COLOR, 0);
  } else { //RAINBOW
    for (int x = 0; x < 7; x++ ) {
      processColor(rainbowColors[x], 0);
    }
  }
}

void loop() {
  printConcatLine("ss", "Processing Mode: ", PatternMode);

  if (PatternMode == "RADIATE") {
    processRadiate();
  } else if (PatternMode == "PULSE") {
    processPulse();
  } else if (PatternMode == "VORTEX") {
    processVortex();
  } else {
    processBlink();
  }
}
