#include "sounds.h"
#include <Arduino.h>

/***_Notes_***************************************/
#define B0  31
#define C1  33
#define CS1 35
#define D1  37
#define DS1 39
#define E1  41
#define F1  44
#define FS1 46
#define G1  49
#define GS1 52
#define A1  55
#define AS1 58
#define B1  62
#define C2  65
#define CS2 69
#define D2  73
#define DS2 78
#define E2  82
#define F2  87
#define FS2 93
#define G2  98
#define GS2 104
#define A2  110
#define AS2 117
#define B2  123
#define C3  131
#define CS3 139
#define D3  147
#define DS3 156
#define E3  165
#define F3  175
#define FS3 185
#define G3  196
#define GS3 208
#define A3  220
#define AS3 233
#define B3  247
#define C4  262
#define CS4 277
#define D4  294
#define DS4 311
#define E4  330
#define F4  349
#define FS4 370
#define G4  392
#define GS4 415
#define A4  440
#define AS4 466
#define B4  494
#define C5  523
#define CS5 554
#define D5  587
#define DS5 622
#define E5  659
#define F5  698
#define FS5 740
#define G5  784
#define GS5 831
#define A5  880
#define AS5 932
#define B5  988
#define C6  1047
#define CS6 1109
#define D6  1175
#define DS6 1245
#define E6  1319
#define F6  1397
#define FS6 1480
#define G6  1568
#define GS6 1661
#define A6  1760
#define AS6 1865
#define B6  1976
#define C7  2093
#define CS7 2217
#define D7  2349
#define DS7 2489
#define E7  2637
#define F7  2794
#define FS7 2960
#define G7  3136
#define GS7 3322
#define A7  3520
#define AS7 3729
#define B7  3951
#define C8  4186
#define CS8 4435
#define D8  4699
#define DS8 4978


unsigned int helloNotes[8] = {C4, G3, G3, A3, G3, 0, B3, C4};
byte helloNoteDurations[8] = {4, 8, 8, 4, 4, 4, 4, 4};

unsigned int lowBatNotes[5] = {DS7, 0, E4, E3, E2};
byte lowBatNoteDurations[5] = {4, 8, 4, 4, 2};

unsigned int lMidBatNotes[5] = {DS7, 0, G4, G3, G3};
byte lMidBatNoteDurations[5] = {4, 8, 4, 4, 2};

unsigned int hMidBatNotes[5] = {DS7, 0, G4, F4, A4};
byte hMidBatNoteDurations[5] = {4, 8, 4, 4, 2};

unsigned int highBatNotes[5] = {DS7, 0, F5, E5, B5};
byte highBatNoteDurations[5] = {4, 8, 4, 4, 2};

unsigned int disconnectNotes[3] = {D6, D5, D3};
byte disconnectNoteDurations[3] = {16, 8, 8};

unsigned int connectNotes[3] = {D3, D5, D6};
byte connectNoteDurations[3] = {16, 8, 8};

Sounds::Sounds(byte buzzerPin)  {
  _buzzerPin = buzzerPin;
}
void Sounds::play(byte melody) {
  switch (melody) {
    // Welcome melody
    case 0:
      for (int thisNote = 0; thisNote < 8; thisNote++) {
        // to calculate the note duration, take one second divided by the note type.
        // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1000 / helloNoteDurations[thisNote];
        tone(_buzzerPin, helloNotes[thisNote], noteDuration);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        noTone(_buzzerPin);
      }
      break;

    // Low battery level
    case 1:
      for (int thisNote = 0; thisNote < 5; thisNote++) {
        int noteDuration = 1000 / lowBatNoteDurations[thisNote];
        tone(_buzzerPin, lowBatNotes[thisNote], noteDuration);
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        noTone(_buzzerPin);
      }
      break;

    // Medium-low battery level
    case 2:
      for (int thisNote = 0; thisNote < 5; thisNote++) {
        int noteDuration = 1000 / lMidBatNoteDurations[thisNote];
        tone(_buzzerPin, lMidBatNotes[thisNote], noteDuration);
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        noTone(_buzzerPin);
      }
      break;

    // Medium-high battery level
    case 3:
      for (int thisNote = 0; thisNote < 5; thisNote++) {
        int noteDuration = 1000 / hMidBatNoteDurations[thisNote];
        tone(_buzzerPin, hMidBatNotes[thisNote], noteDuration);
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        noTone(_buzzerPin);
      }
      break;

    // High battery level
    case 4:
      for (int thisNote = 0; thisNote < 5; thisNote++) {
        int noteDuration = 1000 / highBatNoteDurations[thisNote];
        tone(_buzzerPin, highBatNotes[thisNote], noteDuration);
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        noTone(_buzzerPin);
      }
      break;

      // Disconnected
      case 5:
      for (int thisNote = 0; thisNote < 3; thisNote++) {
        int noteDuration = 1000 / disconnectNoteDurations[thisNote];
        tone(_buzzerPin, disconnectNotes[thisNote], noteDuration);
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        noTone(_buzzerPin);
      }
      break;

      // Connected
      case 6:
      for (int thisNote = 0; thisNote < 3; thisNote++) {
        int noteDuration = 1000 / connectNoteDurations[thisNote];
        tone(_buzzerPin, connectNotes[thisNote], noteDuration);
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        noTone(_buzzerPin);
      }
      break;
  }
}
