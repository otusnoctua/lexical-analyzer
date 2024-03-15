# Lecixal Analyzer
The name says it all. Small program written as an individual university task back in 2021.

## Language grammar

B &rarr; begin {S} end.\
S &rarr; I := V; do {S} loop until (V);\
V &rarr; W { [=|<|>|!=] W }\
W &rarr; X { [+|-|or] X } \
X &rarr; Y { [*|/|and] Y } \
Y &rarr; I | N | not V | (V) \
I &rarr; C | IC | IR \
N &rarr; R | NR \
R &rarr; 0 | 1 | ... | 9\
C &rarr; a | b | ... | z | A | ... | Z \

Comments format: (* ... *\)

## State diagram

![State diagram](https://github.com/otusnoctua/lexical-analyzer/blob/main/state-diagram.png?raw=true)
