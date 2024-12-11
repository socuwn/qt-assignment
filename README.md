# Task
An interactive morse converter that can convert English letters to morse code;
the user can type either, and gets the converted text displayed in real time;

this can be a tool with a widget- or QML-based user interface,
or a console application.


# Specification

## General
Upper and lower case characters should be interchangeable.
Characters in the ASCII table with decimal values equal to and in the range of
33 and 176 should be supported.


## inputs
### text english
Every input should be stored into the parsed buffer.

Ignore invalid or unsupported characters.


### text morse
Every symbol is put into a buffer.
When a sequence is recognized store into the parsed buffer.

Ignore invalid or unsupported characters.


### (OPTIONAL) live / legacy morse
Any key is stored as a signal with time on into a buffer.
When a sequence is recognized store into the parsed buffer.

Ignore invalid or unsupported characters.


## outputs
### text english
Display as english text


### text morse
Display as dots and dashes (- .)


### (OPTIONAL) sound morse
Use synthesizer


### (OPTIONAL) sound english
Use TTS


# References
## Morse code
https://en.wikipedia.org/wiki/Morse_code

Five elements of morse code:
1. short mark, dot or dit: one time unit long
2. long mark, dash or dah: three time units long
3. inter-element gap between the dits and dahs within a character:
   one dot duration or one unit long
4. short gap between letters: three time units long
5. medium gap between words: seven time units long (formerly five)


# Design details
## Types
------------------------------------------------
name | length | description
------------------------------------------------
dit  |      1 | short mark
dah  |      3 | long mark
gp1  |      1 | gap between marks (within a character)
gp3  |      3 | gap between letters
gp5  |      5 | gap between words
gp7  |      7 | gap between words


## Timing
words per minute can be determined by the word PARIS or CODEX
PARIS consists of 50 dit units, @ 20 words per minute => 1 dit = 60ms
CODEX consists of 60 dit units, @ 20 words per minute => 1 dit = 50ms

T: 1 dit
W: words per minute

PARIS:
  T = 1200 / W
  W = 1200 / T

CODEX:
  T = 1000 / W
  W = 1000 / T

Optional:
  jitter


## Other

FunFact anecdote:
"He is in the garb of a kwaker"
"He is in the garb of a quaker"

Max length:
0 - dah dah dah dah dah dah
   0111011101110111011101110


### Progress bar
use multiple progress bars as a tempo indicator (dit, dah, eoc, eow)
