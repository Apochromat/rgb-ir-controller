#include <IRremote.h>

#define IR_PIN 12
#define RED_PIN 6
#define GRE_PIN 9
#define BLU_PIN 10

byte maximumBrightness = 10;    // Maximum brightness level
byte Brightness = maximumBrightness;    // Brightness in the beginning
int currentColors[] = {0, 0, 0};    // Current Color outputed variable (black by default)
int effectSwitch = 0;   // Variable for effect type
bool onSwitch = 0;    // Variable for power state
bool buttonHoldMode = 1;    // Variable for hold mode, if True - holding is avaible

unsigned long lastEffectTime;   // Time of last effect event
int delayFlash = 1500;				
byte delayStrobe = 15;
byte delayFade = 20;
int delaySmooth = 1500;

int colorFade[3] = {255, 0, 0};
byte brightnessStrobe;

int result;

byte stageFade;
byte stageSmooth;
byte stageFlash;
byte speedStrobe = 1;				

IRrecv irrecv(IR_PIN);    // Initialization of IR control pin
decode_results results;

// Defining hex codes for the remote
#define BRIGHTNESS_UP_CODE 0xF700FF
#define BRIGHTNESS_DN_CODE 0xF7807F
#define OFF_CODE           0xF740BF
#define ON_CODE            0xF7C03F
#define RED_CODE           0xF720DF
#define GREEN_CODE         0xF7A05F
#define BLUE_CODE          0xF7609F
#define WHITE_CODE         0xF7E01F
#define CRIMSON_CODE       0xF710EF
#define OLIVE_CODE         0xF7906F
#define DEEP_SKY_BLUE_CODE 0xF750AF
#define ORANGE_CODE        0xF730CF
#define CYAN_CODE          0xF7B04F
#define NAVY_CODE          0xF7708F
#define PEACH_CODE         0xF708F7
#define TURQUOISE_CODE     0xF78877
#define ROYAL_BLUE_CODE    0xF748B7
#define YELLOW_CODE        0xF728D7
#define STEEL_BLUE_CODE    0xF7A857
#define PURPLE_CODE        0xF76897
#define FLASH_CODE         0xF7D02F
#define STROBE_CODE        0xF7F00F
#define FADE_CODE          0xF7C837
#define SMOOTH_CODE        0xF7E817

// Defining the avaialble colors
int BLACK_COLOR[3] = {0, 0, 0};
int RED_COLOR[3] = {255, 0, 0};
int GREEN_COLOR[3] = {0, 255, 0};
int BLUE_COLOR[3] =  {0, 0, 255};
int WHITE_COLOR[3] = {255, 255, 255};
int CRIMSON_COLOR[3] = {255, 10, 0};
int OLIVE_COLOR[3] = {128, 60, 0};
int DEEP_SKY_BLUE_COLOR[3] = {0, 191, 255};
int ORANGE_COLOR[3] = {255, 20, 0};
int CYAN_COLOR[3] = {0, 255, 255};
int NAVY_COLOR[3] = {0, 40, 128};
int PEACH_COLOR[3] = {255, 90, 10};
int TURQUOISE_COLOR[3] = {64, 224, 208};
int ROYAL_BLUE_COLOR[3] = {64, 105, 255};
int YELLOW_COLOR[3] = {255, 100, 0};
int STEEL_BLUE_COLOR[3] = {70, 130, 180};
int PURPLE_COLOR[3] = {196, 0, 128};


void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GRE_PIN, OUTPUT);
  pinMode(BLU_PIN, OUTPUT);

  
  irrecv.enableIRIn();
}

void loop() {
  findCode();
  if (effectSwitch!=0 && onSwitch ==1){
    switch(effectSwitch){
      case 1: Flash(); break; 	//Flash
      case 2: Strobe();  break; //Strobe
      case 3: Fade(); break; 	//Fade
      case 4: Smooth(); break; 	//Smooth
    }
  }
}

void setBrightness (bool add){
  if (effectSwitch == 0){
    if(add == 1 && Brightness < maximumBrightness){
      Brightness += 1;
    }else if (add == 1 && Brightness >= maximumBrightness){
      Brightness = maximumBrightness;
    }
    if(add == 0 && Brightness >0){
      Brightness -= 1;
    }else if (add == 0 && Brightness == 0){
      Brightness = 0;
    }
    sendColor();
  }
}

void findCode() {
  if (irrecv.decode(&results)) {
    if ((results.value != 4294967295) && (buttonHoldMode == 1)){
      result = results.value;
    }
    if (buttonHoldMode == 0){
      result = results.value;
    }
    interpretRemoteCode(result);
    irrecv.resume();
    }
  }

void interpretRemoteCode(int code) {
  int randomColor[3] = {random(256), random(256), random(256)};
  switch (code) {
    case BRIGHTNESS_UP_CODE: setBrightness(1); break;
    case BRIGHTNESS_DN_CODE: setBrightness(0); break;
    case OFF_CODE: effectSwitch=0;setColor(BLACK_COLOR); onSwitch = 0; digitalWrite(13, LOW); break;
    case ON_CODE: effectSwitch=0; onSwitch = 1; setColor(randomColor); digitalWrite(13, HIGH); break;
    case RED_CODE: effectSwitch=0; setColor(RED_COLOR); break;
    case GREEN_CODE: effectSwitch=0; setColor(GREEN_COLOR); break;
    case BLUE_CODE: effectSwitch=0; setColor(BLUE_COLOR); break;
    case WHITE_CODE: effectSwitch=0; setColor(WHITE_COLOR); break;
    case CRIMSON_CODE: effectSwitch=0; setColor(CRIMSON_COLOR); break;
    case OLIVE_CODE: effectSwitch=0; setColor(OLIVE_COLOR); break;
    case DEEP_SKY_BLUE_CODE: effectSwitch=0; setColor(DEEP_SKY_BLUE_COLOR); break;
    case ORANGE_CODE: effectSwitch=0; setColor(ORANGE_COLOR); break;
    case CYAN_CODE: effectSwitch=0; setColor(CYAN_COLOR); break;
    case NAVY_CODE: effectSwitch=0; setColor(NAVY_COLOR); break;
    case PEACH_CODE: effectSwitch=0; setColor(PEACH_COLOR); break;
    case TURQUOISE_CODE: effectSwitch=0; setColor(TURQUOISE_COLOR); break;
    case ROYAL_BLUE_CODE: effectSwitch=0; setColor(ROYAL_BLUE_COLOR); break;
    case YELLOW_CODE: effectSwitch=0; setColor(YELLOW_COLOR); break;
    case STEEL_BLUE_CODE: effectSwitch=0; setColor(STEEL_BLUE_COLOR); break;
    case PURPLE_CODE: effectSwitch=0; setColor(PURPLE_COLOR); break;
    case FLASH_CODE: setColor(BLACK_COLOR); lastEffectTime = millis(); effectSwitch=1; break;
    case STROBE_CODE: setColor(BLACK_COLOR); lastEffectTime = millis(); brightnessStrobe=0; effectSwitch=2; break;
    case FADE_CODE: setColor(BLACK_COLOR); lastEffectTime = millis(); effectSwitch=3; break;
    case SMOOTH_CODE: setColor(BLACK_COLOR); lastEffectTime = millis(); effectSwitch=4; break;
  }
}

void Flash(){
  int colorFlash[3] = {0, 0, 0};
  if (millis() - lastEffectTime >= delayFlash){
    lastEffectTime=millis();
    switch (stageFlash){
      case 0: colorFlash[0] = 255; colorFlash[1] = 0; colorFlash[2] = 0; break;
      case 1: colorFlash[0] = 0; colorFlash[1] = 255; colorFlash[2] = 0; break;
      case 2: colorFlash[0] = 0; colorFlash[1] = 0; colorFlash[2] = 255; break;
    }
    setColor(colorFlash);
    if(stageFlash <2){stageFlash++;} else if(stageFlash ==2){stageFlash = 0;};
  }
}

void Strobe(){
  if (millis() - lastEffectTime >= delayStrobe){
    lastEffectTime=millis();
    brightnessStrobe += speedStrobe;
    int colorStrobe[3]={brightnessStrobe, brightnessStrobe, brightnessStrobe};
    setColor(colorStrobe);
    if (brightnessStrobe <=3){speedStrobe=1;}
    if (brightnessStrobe >=254){speedStrobe=-1;}
  }
}

void Fade(){
  if (millis() - lastEffectTime >= delayFade){
    lastEffectTime=millis();
    switch (stageFade){
      case 0: colorFade[1] += 1; break;
      case 1: colorFade[2] += 1; break;
      case 2: colorFade[0] += -1; break;
      case 3: colorFade[1] += -1; break;
      case 4: colorFade[0] += 1; break;
      case 5: colorFade[2] += -1; break;
    }
    if (colorFade[0] == 255 && colorFade[1] == 255 && colorFade[2] == 0){stageFade = 1;
    }else if (colorFade[0] == 255 && colorFade[1] == 255 && colorFade[2] == 255){stageFade = 2;
    }else if (colorFade[0] == 0 && colorFade[1] == 255 && colorFade[2] == 255){stageFade = 3;
    }else if (colorFade[0] == 0 && colorFade[1] == 0 && colorFade[2] == 255){stageFade = 4;
    }else if (colorFade[0] == 255 && colorFade[1] == 0 && colorFade[2] == 255){stageFade = 5;
    }else if (colorFade[0] == 255 && colorFade[1] == 0 && colorFade[2] == 0){stageFade = 0;};

    setColor(colorFade);
  }
}

void Smooth(){
  int colorSmooth[3] = {0, 0, 0};
  if (millis() - lastEffectTime >= delaySmooth){
    lastEffectTime=millis();
    switch (stageSmooth){
      case 0: colorSmooth[0] = 255; colorSmooth[1] = 0; colorSmooth[2] = 0; break;
      case 1: colorSmooth[0] = 255; colorSmooth[1] = 255; colorSmooth[2] = 0; break;
      case 2: colorSmooth[0] = 0; colorSmooth[1] = 255; colorSmooth[2] = 0; break;
      case 3: colorSmooth[0] = 0; colorSmooth[1] = 255; colorSmooth[2] = 255; break;
      case 4: colorSmooth[0] = 0; colorSmooth[1] = 0; colorSmooth[2] = 255; break;
      case 5: colorSmooth[0] = 255; colorSmooth[1] = 0; colorSmooth[2] = 255; break;
    }
    setColor(colorSmooth);
    if(stageSmooth<5){stageSmooth++;} else if(stageSmooth ==5){stageSmooth = 0;};
  }
}

void setColor(int colors[]) {   // Set colors in variables to send
  if (onSwitch == 1){
  currentColors[0] = colors[0];
  currentColors[1] = colors[1];
  currentColors[2] = colors[2];
  sendColor();
  }
}


void sendColor()    // Applying the brightness parameter to colors
{
    int red = currentColors[0] /maximumBrightness*Brightness;
    int green = currentColors[1] /maximumBrightness*Brightness;
    int blue = currentColors[2] /maximumBrightness*Brightness;
  if (effectSwitch != 0){
    int red = currentColors[0];
    int green = currentColors[1];
    int blue = currentColors[2];
  }

    analogWrite(RED_PIN, red);    // Sends PWM signals to the pins
    analogWrite(GRE_PIN, green);
    analogWrite(BLU_PIN, blue);
}
