// Project Identifier: A8A3A33EF075ACEF9B08F5B9845569ECCB423725

#include "puzzle.cpp"
#include <iostream>
#include <sstream>

using namespace std;




int main(int argc, char *argv[]) {
  // This should be in all of your projects, speeds up I/O
  ios_base::sync_with_stdio(false);


  // Get the mode from the command line and read in the data
  PuzzleRunner game;
  cout << "line 19\n";
  game.getMode(argc, argv);
  cout << "line 21\n";
  game.readPuzzle();
  cout << "line 23\n";

}

/*
LEGAL COMMAND LINE ARGS
$ ./puzzle --queue -o map < spec-simple.txt > output.txt
$ ./puzzle -o map -q < spec-simple.txt > output.txt
$ ./puzzle -qo map < spec-simple.txt > output.txt
$ ./puzzle -q --output map < spec-simple.txt > output.txt
$ ./puzzle -q < spec-simple.txt > output.txt
Run the program using a search container that behaves like a queue to perform a breadth-first search and output the results in map format; Input is redirected to cin from spec-simple.txt, and output is redirected from cout to output.txt.

$ ./puzzle -so list < spec-simple.txt
$ ./puzzle --stack --output list < spec-simple.txt
$ ./puzzle --output list --stack < spec-simple.txt
Run the program using a search container that behaves like a stack to perform a depth-first search and output the results in list format; Input is redirected to cin from spec-simple.txt, and output is printed to the screen (cout).

$ ./puzzle --help
$ ./puzzle -h
Prints a helpful message and exits.

ILLEGAL COMMAND LINE ARGS
$ ./puzzle --stack --queue < spec-simple.txt
Both stack and queue modes specified.

$ ./puzzle -o map -o list < spec-simple.txt
Both map and list output modes specified.

$ ./puzzle -o < spec-simple.txt
No argument provided to the output mode option.


*/
