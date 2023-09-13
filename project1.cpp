// Project Identifier: A8A3A33EF075ACEF9B08F5B9845569ECCB423725

// EECS 281, Project 1
//use DEQUES
//use 3d vector for backtrace
/*
    2D Example, Queue
    Notes:
        - we only discover things once
        - things get discovered in the search container
        - how did i get to the start location? i just started there
        - backtrace looks like a 2d vector (in the real one a 3d vector)
        - linear searches in list are poor, the backtrace keeps track of everything
         that has been discovered, takes one access to row, one to column, see if its a space
         then im done
        - Loop while that search container is not empty, search container is known as
        the list of places that i still ahve to investigate
        - if search container is empty i have run out of places to look
        - when current state has been discovered, it moves to current state, then
        we need to investigate, i.e., where to go next.
        - if youre standing on a button ( lower case) or trap (which is ^)
            -this changes color
            -then i have to do button stuff
        - if not standing on trap or button
            -then north east south west
        - in scenario where only (1,1), not on button not on trap, 
            -lets do N, E, S, W (in that order)
            -when look north, first say, is north a valid place to look? 
            (i.e., am i in row 0)
            -check what is north of me?
                - is it valid
                - is it something i could walk on
                - is it someplace i've never discovered et
                - if all true then we can discover that location
            -then add location to seradch container and modify backtrace
            - I came from the south so (0,1) is marked by an S
            -now check east
            -now check south, 
                - can I walk? no, # means a solid wall
            - now look west
                -can i walk? no, # wall, move on
            -now target(?) has not been found, so done investigating this square, onto the next
            - (1,1) investigation is over, POP it
            -we're using a queue so we take from the FRONT of the queue in Search container
            - now the current state is (0,1) and POP from search container
            - can i look north? NO
            - east? YES add (0,2) to search container, mark with W (came from the west)
            - south?, no has been discovered already
            - west? yes, add (0,0), mark with E (came from the east)
            - discovery over, pop  (0,1) from current state
            - pull from FRONT of search container, now current state is (1,2)
            - CONTINUE THE SAME PROCESS...
            - at (2,3)
            - can i look N?
                -yes, can I walk? no, wall
            - can i look E? no, not a valid location
            - south? 
                -have i been there, could I wawlk on it, YES! target found
            - add 3,3 to serach container, mark it, I came from the north
            - now that its the target, stop, dont even look west
            - is there a solution? yes, mark such as true
            - or if in function return true
            - every location that has been dsicovered, bc they can only have been discovered once
            only has one place it came from
            - now lets not start at the beginning, lets start at the end
            - we need some sort of contianer here to look from the targetback to the beginning
                -really want to print how I got from start to end
                - could use stack, vector, or DEQUE



          Map                         Backtrace
       0  1  2  3                  0  1  2  3
     0 .  .  .  #                0 E  S  W 
     1 #  @  .  #                1    @  W  
     2 .  #  .  .                2       N  W
     3 .  .  #  ?                3          N
     4 .  # .  .                 4 

    Search Container(Row, Col)       Current State
    (1,1)X(0,1)X (1,2)X (0,2)X       (1,1)X (0,1)X (1,2)X
    (0,0)X (2,2)X (2,3)X              (0,2)X (0,0)X (2,2)X
                                     (2,3)
    
    
    -start with 3,3
        - hey 3,3 where did you come from?
        NORTH
        - ok 2,3
        -where did 2,3 come from
        WEST
        - ok 2,2
        -where did 2,2 come from
        NORTH
        - ok 1,2
        -where did 1,2 come from
        West
        - ok 1,1
        -where did 1,1 come from
        IM THE START
        -done filling up the path container

        -print list output mode, all i have to do is go backwards through my path
            - if stack, look from back and pop it
        - map output a little harder
            - to print map, we can go through our backtrace and change it bascially to what we want printed
            - what we want printed is, hey when i was at the start where did I go (east)
            -everything gets changed with + or  for arrivals and changes
            - go through path using plus signs in the map:

    original Backtrace              backtrace to be printed
      0  1  2  3                  0  1  2  3
    0 E  S  W                   0 E  S  W 
    1    @  W                   1    @  + 
    2       N  W                2       +  +
    3          N                3          ?
    4                           4

        -now when time to print the map we look at the backtrace
        -if it is @ or + or ? or % (comes in when multiple forms)
        - I would print backtrace, otherwise print whats in original map
    FINAL MAP:                 
       0  1  2  3         
     0 .  .  .  #        
     1 #  @  +  #             
     2 .  #  +  +             
     3 .  .  #  ?       
     4 .  # .  .  

    PATH:
    (3,3), (2,3), (2,2), (1,2), (1,1)



    3D Example, Queue:

           Map                         Backtrace
       0  1  2  3                  0  1  2  3
     0 .  .  .  #                0 E  S  W 
     1 #  @  .  #                1    @  W  
     2 .  #  .  .                2       N  W
     3 .  .  #  ?                3          N
     4 .  # .  .                 4 











*/





#include <getopt.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// These modes represent the different ways to process the input data.
// There are three modes and the state where no mode has been selected.
enum class Mode {
  kNone = 0,
  kResize,
  kReserve,
  kNoSize,
};  // Mode{}


// This struct is used to pass options read from the command line, to
// the functions that actually do the work. Project 0 is simple enough to
// not need this, but it is a technique that could be helpful in doing more
// complicated command-line processing in future projects.
struct Options {
  Mode mode = Mode::kNone;
};  // Options{}


// Print help for the user when requested.
// argv[0] is the name of the currently executing program
void printHelp(char *argv[]) {
  cout << "Usage: " << argv[0] << " [-m resize|reserve|nosize] | -h\n";
  cout << "This program is to help you learn command-line processing,\n";
  cout << "reading data into a vector, the difference between resize and\n";
  cout << "reserve and how to properly read until end-of-file." << endl;
}  // printHelp()



void getMode(int argc, char * argv[], Options &options) {
  // These are used with getopt_long()
  opterr = false; // Let us handle all error output for command line options
  int choice;
  int index = 0;
  option long_options[] = {
    // ./project0 --mode (option) nosize (arg with the option named mode)
    //./project0 -m (option) nosize
    // ./project0  --help (option) (no required arg)
    //long option  req or noreq  null    short version
    {"help", no_argument, nullptr, 'h'},
    {"queue", no_argument, nullptr, 'q'},
    {"stack", no_argument, nullptr, 's'},
    {"output", required_argument, nullptr, 'o'},
    { nullptr, 0, nullptr, '\0' }, //leave alone this has to be last thing in long options
  };  // long_options[]

  
  while ((choice = getopt_long(argc, argv, "m:h", long_options, &index)) != -1) {
    switch (choice) {
      case 'h':
        printHelp(argv);
        exit(0);

      case 'm': {  // Need a block here to declare a variable inside a case
        string arg{optarg};
        if (arg != "resize" && arg != "reserve" && arg != "nosize") {
          // The first line of error output has to be a 'fixed' message
          // for the autograder to show it to you.
          cerr << "Error: invalid mode" << endl;
          // The second line can provide more information, but you
          // won't see it on the AG.
          cerr << "  I don't recognize: " << arg << endl;
          exit(1);
        }  // if ..arg valid

        if (arg == "reserve") {
          options.mode = Mode::kReserve;
        } else if (arg[0] == 'r') {
          options.mode = Mode::kResize;
        } else {
          options.mode = Mode::kNoSize;
        }  // if ..arg
        break;
      }  // case 'm'

      default:
        cerr << "Error: invalid option" << endl;
        exit(1);
    }  // switch ..choice
  }  // while

  if (options.mode == Mode::kNone) {
    cerr << "Error: no mode specified" << endl;
    exit(1);
  }  // if ..mode
}  // getMode()


void readWithResize(vector<double> &data) {
  //step 1
  size_t size;
  cin >> size;

  //step 2
  data.resize(size);
  for(size_t i = 0; i < size; i++){
      cin >> data[i];
  } //for i < vector size

}  // readWithResize()


void readWithReserve(vector<double> &data) {
  size_t size;
  cin >> size;

  data.reserve(size);

  for(size_t i = 0; i < size; i++){
    size_t curr;
    cin >> curr;
    data.push_back(curr);
  }//for i < size

}  // readWithReserve()


void readWithNoSize(vector<double> &data) {
  size_t currData;
  while(cin >> currData){
    data.push_back(currData);
  } //while still numbers left

}  // readWithNoSize()


double getAverage(const vector<double> &data) {
  double average;
  double sum = 0;

  for (size_t i = 0; i < data.size(); ++i)
    sum += data[i];

  average = sum / static_cast<double>(data.size());
  return average;
}  // getAverage()


double getMedian(vector<double> &data) {
  // Sort the data
  sort(data.begin(), data.end());

  // Figure out if vector size is even or odd
  if (data.size() % 2 == 1)
    return data[data.size() / 2];

  // Even size, average 2 middle values
  auto mid = data.size() / 2;
  double v1 = data[mid];
  double v2 = data[mid - 1];
  return (v1 + v2) / 2;
}  // getMedian()


int main(int argc, char *argv[]) {
  // This should be in all of your projects, speeds up I/O
  ios_base::sync_with_stdio(false);

  // Set two digits of precision on output
  cout << fixed << showpoint << setprecision(2);

  // Get the mode from the command line and read in the data
  Options options;
  getMode(argc, argv, options);
  vector<double> data;
  if (options.mode == Mode::kResize)
    readWithResize(data);
  else if (options.mode == Mode::kReserve)
    readWithReserve(data);
  else if (options.mode == Mode::kNoSize)
    readWithNoSize(data);

  // Print out the average and median of the data
  if (data.size() == 0) {
    cout << "No data => no statistics!" << '\n';
  } else {
    cout << "Average: " << getAverage(data) << '\n';
    cout << " Median: " << getMedian(data) << '\n';
  }  // if ..data

  return 0;
}  // main()
