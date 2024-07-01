#include "pitches.h"

int melody[] = {
    NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};

int noteDurations[] = {
    4, 8, 8, 4, 4, 4, 4, 4};

const int lostGameContact = 4;
const int wonGameContact = 5;
const int ledRed = 11;
const int ledBlue = 12;
const int ledGreen = 10;
const int startButton = 6;
const int speakerPin = 8;
const int ledAmber = 13;
// variavel que grava o estado do botão (por conta do debounce):
int reading;
// variavel que armazena o estado anterior para que o debounce funcione:
int previous = LOW;
// variavel que armazena o estado inicial do botão:
int buttonState = HIGH;

bool startedGame = false;
bool wonGame;
bool lostGame;

// config do debounce:
unsigned long time = 0;
unsigned long debounce = 50;
unsigned long debounceContact = 1;

unsigned long lastLostGameContactTime = 0;
unsigned long lastWonGameContactTime = 0;
int lastLostGameContactState = HIGH;
int lastWonGameContactState = HIGH;

void setup()
{
    // pinmodes:
    pinMode(lostGameContact, INPUT_PULLUP);
    pinMode(wonGameContact, INPUT_PULLUP);
    pinMode(startButton, INPUT_PULLUP);
    pinMode(ledRed, OUTPUT);
    pinMode(ledBlue, OUTPUT);
    pinMode(ledGreen, OUTPUT);
    pinMode(ledAmber, OUTPUT);
    pinMode(speakerPin, OUTPUT);
}

void resetEverything()
{
    digitalWrite(ledRed, LOW);
    digitalWrite(ledAmber, LOW);
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledBlue, LOW);
    noTone(speakerPin);
    return;
}

void starter()
{
    startedGame = true;
    if (startedGame)
    {
        digitalWrite(ledBlue, HIGH);
        digitalWrite(ledAmber, LOW);
        digitalWrite(ledRed, LOW);
    }
    else if (wonGame)
    {
        digitalWrite(ledAmber, HIGH);
    }
    else if (lostGame)
    {
        digitalWrite(ledRed, HIGH);
    }
}

void checkStatus()
{

    int currentLostGameContactState = digitalRead(lostGameContact);
    int currentWonGameContactState = digitalRead(wonGameContact);

    unsigned long currentTime = millis();

    if (currentLostGameContactState != lastLostGameContactState)
    {
        lastLostGameContactTime = currentTime;
    }

    if ((currentTime - lastLostGameContactTime) > debounceContact)
    {
        if (currentLostGameContactState == LOW)
        {
            digitalWrite(ledBlue, LOW);
            digitalWrite(ledRed, HIGH);
            lost();
            startedGame = false;
        }
    }

    lastLostGameContactState = currentLostGameContactState;

    // Check for won game contact
    if (currentWonGameContactState != lastWonGameContactState)
    {
        lastWonGameContactTime = currentTime;
    }

    if ((currentTime - lastWonGameContactTime) > debounceContact)
    {
        if (currentWonGameContactState == LOW)
        {
            digitalWrite(ledBlue, LOW);
            digitalWrite(ledAmber, HIGH);
            won();
            startedGame = false;
        }
    }

    lastWonGameContactState = currentWonGameContactState;
}

// if (digitalRead(lostGameContact) == LOW)
//{

// digitalWrite(ledBlue, LOW);
//   digitalWrite(ledRed, HIGH);
// lost();
//  startedGame = false;
//}

// if (digitalRead(wonGameContact) == LOW)
//{
//     digitalWrite(ledBlue, LOW);
//     digitalWrite(ledAmber, HIGH);
//     won();
//   startedGame = false;
// }

void lost()
{
    lostGame = true;
    for (int thisNote = 3; thisNote < 4; thisNote++)
    {
        int noteDuration = 5000 / noteDurations[thisNote];
        tone(speakerPin, melody[thisNote], noteDuration);
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        noTone(speakerPin);
    }
}

void won()
{
    wonGame = true;

    for (int thisNote = 0; thisNote < 8; thisNote++)
    {
        int noteDuration = 1000 / noteDurations[thisNote];
        tone(speakerPin, melody[thisNote], noteDuration);
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        noTone(speakerPin);
    }
}

void loop()
{

    reading = digitalRead(startButton);

    if (reading != previous)
    {
        time = millis();
    }

    if ((millis() - time) > debounce)
    {
        if (reading != buttonState)
        {
            buttonState = reading;

            if (buttonState == LOW)
            {
                starter();
            }
        }
    }

    previous = reading;

    if (startedGame)
    {
        checkStatus();
    }
}
