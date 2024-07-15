# Electric Maze

The electric maze is a game with the purpose of teaching basic electricity concepts. It was built with a wooden base, a twisted metal tube and a metal ring. Its electronic base was assembled with Arduino Nano, one push button, one amber LED, one red LED, one blue LED and a speaker.

## How does it work?

The game starts silent and with the blue LED lit. The user starts the game by pressing the push button, activating the possibility of winning or losing by touching the ring to the path or the base. 

There is a current flowing through the tube, and if the ring touches the path before the finish line, a loud sound plays while the red LED lights up. The user loses. 

If the user reaches the end of the circuit and touches the ring to the base, a happy sound plays and the amber LED lights up, signaling that the user wins. 

The user is then able to take the ring back to the start without signaling any losses if it touches the tube in the way, and press the start button again, to restart the game from the base.

## Programming

To achieve this functionality, I used vscode with PlatformIO installed and the `pitches.h` lib. 
To avoid any kind of interference, like electrical noise, to interrupt the game, I set up a `resetEverything` and a `starter` function so the game always starts from the starting point, and a `checkStatus` function for debouncing.

The `lost()` and `won()` functions are called depending on whether the signal is HIGH or LOW.

```cpp
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

```

## Libs used

The lib used by this project for the sounds the game plays is [pitches](https://gist.github.com/mikeputnam/2820675) by [mikeputnam](https://github.com/mikeputnam).

## License

[MIT](https://choosealicense.com/licenses/mit/)
