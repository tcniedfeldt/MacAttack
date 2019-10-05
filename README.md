# MAC ATTACK

Attack that poorly implemented Mac!

## Build and Run

To build this project, run `make`

To run this project, run `./attack.exe`

If you want to change the messages being MAC'ed or appended, you'll have to change them in `main.c`

# Misc

The project prints out `DATA` and `DIGEST`, which are all the blocks of the data being hashed and the resultant hash, respectively. 0s are used as placeholders for the key when computing the padding and lengths for the new attack message hash.
