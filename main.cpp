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
        -backtrace is 3D
        - in previous example, first example was always 0
        - backtrace[color #][row][col]
        - always start in default character
        -while seach container not empty
        - at (^, 0, 0)
            - not on button so can do NESW
            - north doesnt exist, east does, hasn't been discovered, and is walkable
            -south does exist, never been discovered , something walkable, mark it as N (came from the NORTH)
            - west not possible
        - at (^, 0, 1)
            - not on button so can do NESW
            - north doesnt exist, east does, hasn't been discovered, and is not walkable,
            is a capital A, in default color As or Bs are solid, cant step
            -south 1does exist, never been discovered , something walkable, mark it as N (came from the NORTH)
            - west exists, already been discovered
         - at (^, 1, 0)
            - not on button so can do NESW
            - north valid, but has been discovered
            -east exists been been discovered
            -south exists, but is solid wall
            -west doesn't exist
        - at (^, 1, 1)
            -on button so can ONLY do button stuff
            -oh look im on an a, lets see if room a exists(it does)
            -is it someplace i've discovered (no)
            -so add that location (a,1,1)
            -mark a,1,1 as discovered AND where did I come from?
                -not NESW, but from a different color (default color)
                -mark it in B backtrace
        - at (a, 1, 1)
            -am i on a button? NO
            -in color a the a button is an open floor
            -the lowercase a and uppercase A are open floors 
            (need a function that given a current color and row color, what is that to you?)
            -a or A is a . to you
            -can do NESW stuff
            -north exists, have never been there, walkable, mark it
            -is east valid? yes, have i ever been, no, but it is a wall
            -south exists south has never been discovered but it is B, whichin color a is effectively a wall
            -west does exist and has never been discoverd as is walkable, mark it
        - at (a, 1, 1)
            -am i on a button? NO
            -can i go north? no doesn't exist
            -can i go east, YES, A is the same as a . in color a
            -south exists but has been discovered
            -west is the start point, however, that only exists in the default color, can be stepped in different color
            - add a,0,0 to search container, mark it
        - at (a, 1, 0)
            -can i go north? yes, but has been discovered
            -east has been discovered
            -south exists but is a solid wall
            -west does not exist
        -at (a,0,2)
            -not a button, standing on a dot
            -north exist? no
            -east? yes, mark it
            -south, no wall
            -west, yes but discovered
        -at (a,0,0)
            -not a button
            -north exist? no
            -east? yes, but discovered
            -south, yet but discovered
            -west, doesnt exist
        -CONTINUES
        -at (a,2,3)
            -button? yes, check out color b,2,3 (go to C backtrace), mark it!
                -mark by the color you came from, how i got here isnt a coordinate, its a COLOR
                -need only characters for baacktrace
                    -character for ive never been here
                    -character for how did i get here
                    -character for the start
                    -if we had 5 or 6 floors we could NOT use e to mean i got here from east and e to mean i got here from room e
                        -gotta go uppercase for one, lowercase for the other
            -don't even check NESW now
        -at (b,2,3)
        -... CONTINUES ...
        -at (b,3,1)
            -on dot, not button
            -check north, exists, walkable because its a capital B ( a dot), mark it
            -check east, exists, been there, can't repeat
            -south, doesn't exist
            -west, exists, never been, check if walkable, its the Target!!
            -add to serach container
            -? means done with searching, mark is_solution true!
        -proceed to output

        
        
           Map           A Backtrace      B Backtrace       C Backtrace
       0  1  2  3        0  1  2  3       0  1  2  3          0  1  2  3
     0 @  .  A  .      0 @  W           0 E  S  W  W        0          S
     1 .  a  .  .      1 N  N           1 E  ^     N        1          S
     2 #  B  .  b      2                2          N        2    S     a
     3 ?  .  B  .      3                3                   3 E  E  E  N


        Search Container                        Current State
        (^, 0, 0)X (^,0,1)X (^,1,0)X            (^, 0, 0)X (^,0,1)X (^,1,0)X
        (^,1,1)X (a,1,1)X (a,0,1)X              (^,1,1)X (a,1,1)X (a,0,1)X
        (a,1,0)X (a,0,2)X (a,0,0)X              (a,1,0)X (a,0,2)X (a,0,0)X
        (a,0,3)X (a,1,3)X (a,2,3)X              (a,0,3)X (a,1,3)X (a,2,3)X
        (b,2,3)X (b,1,3)X (b,3,3)X              (b,2,3)X (b,1,3)X (b,3,3)X
        (b,0,3)X (b,3,2)X (b,3,1)X              (b,0,3)X (b,3,2)X (b,3,1)
        (b,2,1)  (b,3,0)           

    - have to remember what color did I find the target in
    -found it in color b:
    -solution true, color b
    
    Path:
    (b,3,0), (b,3,1), (b,3,2), (b,3,3), (b,2,3), 
    (a,2,3), (a,1,3), (a,0,3), (a,0,2), (a,0,1), 
    (a,1,1), (^,1,1), (^,0,1), (^,0,0) -->this is start

    -when printing in list mode its more complicated
    -have to know current location now that its filled
        -current location is ^,0,0, then pop that off and go to the next and decide what to print there
    -map mode we would modify the things in the path in the backtrace info to say hey backtrace change info
    -AS SUCH:
        Map           A Backtrace      B Backtrace       C Backtrace
       0  1  2  3        0  1  2  3       0  1  2  3          0  1  2  3
     0 @  .  A  .      0 @  +           0 E  +  +  +        0          S
     1 .  a  .  .      1 N  %           1 E  @     +        1          S
     2 #  B  .  b      2                2          %        2    S     @
     3 ?  .  B  .      3                3                   3 ?  +  +  +

        MAP OUTPUT:
        //Color ^  
      @  .  A  . 
      .  a  .  .
      #  B  .  b 
      ?  .  B  . 
       //Color a
      .  +  +  +
      .  @  #  +
      #  B  #  %
      ?  .  B  .
       //Color b
      .  .  A  .
      .  a  #  .
      #  .  #  @
      ?  +  +  +

    Tips:
    - watch videos about the IDE!
    - if skipping command line tools, assume that we want queue mode and list mode output
    - then you have to read map in
    - map is going to look like
    - 2 colors beyond default 4 rows, 4 cols
    - 2 4 4 (then would have actual map)
        //VIDEO 3D EXAMPLE
    -once the reading started, don't use get line
    - cin >> numColors >> rows >> cols 
    - getline(cin, junk); (retrieves the \n that we didnt read in)
    - RESIZE VECTOR(S) (map and backtrace) NOW
    - row = 0;
    -while (getline(cin, line)){
        if(!comment){
            //transfer those characters over to the map 2d vector
            //must keep track of row im on
            row++;
            //check if thats valid, if theres a capital C, invaid map!
            //if bad characters, ( _ - etc., invalid map!
            //points on autograder for testing for invalid test cases
            //also if mistake made in own files, good for program to REJECT that input,
            rather than segfaulting etc.
            //have a function that says heres a color heres a row column, tell me what im looking at
                //if A in color a, should return .
                //if a and @ sign, youre looking at a .
                //etc.
            //Project 1 The STL and You - Puzzle: CHECK THIS OUT
            //one line of code in the beginning of int main()
                int main(){
                    ios_base::sync_with_stdio(false);
                }
                canvas/files/resources/optimization tips.pdf -->really useful stuff
            //have two functions:
                //first:
                    //int charToRoom(char)
                    //basically if given a character, would return room num 0
                    //if b, 2
                    //etc.
                //second:
                    //char roomToChar(int)
                    //if 0, tells ^
                    //if 2, b
                    //etc.
                    if backtrace[colorNum][row][col]
                    we know current state here: ^, 0, 2 but this is not a room num
                    after conversion --> 0,0,2
        }
    }


STL TIPS:
    - must #include <vector>
        - create an object of template class, vector<int> values;
        -starts empty with no room for values
        -to add to vector use .push_back()
        -each push back allocates double memory than what exists already
        -can use the operator[]
    STL containers are implemented as template classes, there are many types available
        -stack
        -queue
        -deque (can take place of both stack and queue)
    - must #include <stack>
        - create an object of template class, stack<int> values;
        - can push an element onto the top of the stack, look at the top element,
        and pop the top element from the stack
        -first in, last out
    - must #include <queue>
        - create an object of template class, queue<int> values;
        - can push element onto the back, look at the front element,
        and pop the front element from the queue
        -first in, first out
    - stack and queue containers use many of the same member fucntions
        - void push(elem) - add element to container
        - void pop() - remove the next element form the container
        - bool empty() - returns true/false 
    - only difference is which end the push() operation affects
    - the stack uses:
        - <T> top() - look at the "next" element (the top of the stack)    
    - the queue uses:
        - <T> front() - look at the "next" element (the front of the queue)    
    - USE THE DEQUE CONTAINER!
        - A double-ended queue
        - basically instead of being restricted to pushing or popping at a single end, you
        can perform either option at either end
        - #include <deque>
    - The deque provides the following:
        void push_front(elem)
        <T> front()
        void pop_front()

        void push_back(elem)
        <T> back()
        void pop_back()

        bool empty()
    - The deque serves as the search_container in the spec
        - always use .push_back()
        - when you're supposed to use a stack, use .back and .pop_back()
        - for a queue, use .front() and .pop_front()
    2D or 3D Data Structures
        -create a ** or *** (double/triple pointer)
        -create a nested vector<>
            -create the vector with the right size initially
            -use the .resize() member function on each dimension before reading the file
        - or any choice, explot locality of reference
            -use subscripts in this order: [color][row][col]
    Creating or Initializing a Vector
        -example of creating and initalizing 1D vector, with 10 entires, all -1
            uint32_t size = 10;
            vector<int> oneDimArray(size, -1)
        -since 10 vals aready exist, read data directly into them using [i], do not
        .push_back() more values
    Creating then resizing
        - if instead you want to declare vect the later change size
            vector<int> oneDimArray;
            uint32_t size;
            cin >> size;
            oneDimArray.resize(size, -1);
    Creating/Initializing a 2D Vector
        - example of creating and initializing a 2D vector, w/ 10 rows and 20 columns,
        all initialized to -1:
            uint32_t rows = 10;
            uint32_t cols = 20;
            vector<vector<int> > twoDimArray(rows, vector<int>(cols, -1));
        -each "row" is itself a vector<int>
        -can extend this upward to 3 dimensions
    Resizing multiple dimensions
        -if a 2D vector:
            uint32_t rows, cols;
            vector<vector<int> > twoDimArray;
            cin >> rows >> cols;
            twoDimArray.resize(rows, vector<int>(cols, -1))
        -if you have a 3d, extend this syntax upward to three dimensions
    Don't try to make one type of data to handle everything (i.e. the map, 
    backtracking, and deque)
        -make different data types for different purposes as needed
    Converting char <-> int
        -will probably find that conversions are needed
        -can add and subtract integers and characters and convert!
        -for example, character to number:
            return static_cast<uint32_t>(tile - 'a') + 1
        -or number to character:
            return static_cast<char>('a' + color - 1)
    Speeding up Input/Output
        -C++ cin and cout can be slow, but there are ways to speed it up:
            -DO turn off synchronization of C/C++ I/O
            -DO use '\n'
            -DON'T use string streams
                -this has NO real time benefit when using the latest version 
                of g++ and it wastes memory
            -DON'T produce a string object containing all your output
            (no speed gain, wastes memory)
    Synchronized I/O
        -since only using C++ I/O (and not a mix with C-style), turning off synchronization
        saves plenty of time
        -add the foloowing line of code in program as the first line of main:
            ios_base::sync_with_stdio(false);
    Warning:
        -if sychronized I/O, and then use valgrind, it will report potential memory leaks
        -appears as 122KB that is "still reachable"
        -to get accurate feedback on valgrind:
            1. comment out call to sync_with_stdio()
            2. recompile
            3. run valgrind
            4. un-comment the sync/false line
            5. procedd to edit/compile/submit/etc.
    '\n' versus endl
        -whenever the endl object is sent to a stream, after displaying a newline
        it also causes the stream to "flush"
            -same as calling stream.flush()
        -causes output to be written to the hard drive RIGHT NOW
            -doing this after every line takes up tiem
        -using '\n' does not flush
    Finding the Path
        -once you reach the goal, you have to display the path that found it
            -either as part of the map, or in list mode
        -the map, stack/queue/deque do not have this information
        -have to store it separately!
    Backtracking the Path
        -can't start at the beginning and work to the end
            -remember the start could have 4 possible places to go
        -think about it as such: when you're at the goal, how did you get here?
            -since each location is visited ONCE, there is exactly ONE location 
            "before" this one
    




*/





#include <getopt.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <vector> 
#include <deque>

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
// struct Place{
//     private:
//         int color = 0;
//         int row = 0;
//         int column = 0;
//     public:
//         void setVals(int col, int r, int c){
//             color = col;
//             row = r;
//             column = c;
//         }
//         int getColor(){
//             return color;
//         }
//         int getRow(){
//             return row;
//         }
//         int getColumn(){
//             return column;
//         }
// };


class PuzzleRunner{
    private: 
        //number of colors past ^
        uint32_t numColors = 0;
        //number of rows in puzzle
        uint32_t rows = 0;
        //number of columns in puzzle
        uint32_t cols = 0;
        //original puzzle map (to perhaps be changed if no solution is found)
        vector<vector<char> > map;
        //whether puzzle can be solved or not
        bool solutionFound = false;
        //track directions, NESW
        vector<vector<vector<char> > > backtrace;
        //position where the initial @ is
        pair<int, pair<int, int>> initialPosition;
        //the position at which the ? was found
        pair<int, pair<int, int>> solvedPosition;
        //deque of everything contained in searchContainer
        deque<pair<int, pair<int, int>>> searchContainer;
        //deque of everything contained in the currentState
        deque<pair<int, pair<int, int>>> currentState;
        //a vector of all (color, row, column) that have been discovered
        vector<pair<int, pair<int, int>>> placesDiscovered;
        //bool stack: false if queue mode, true if stack mode
        bool stack = false;



        void initializeBacktrace(){
            //initialize default layer
            for(int r = 0; r < rows; r++){
                for(int c = 0; c < cols; c++){
                    backtrace[0][r][c] = ' ';
                }
            }
            //initialize every further layer
            for(char t = 'a'; t < 'a' + numColors; t++){
                for(int r = 0; r < rows; r++){
                    for(int c = 0; c < cols; c++){
                        backtrace[charToRoom(t)][r][c] = ' ';
                    }
                }
            }
        }
    public:
        /*
        Simply throws error if one of the following occurs:
(done)      - 0 <= num_colors <= 26 (0 colors is valid, it just means that there are no doors)
(done)      - 1 <= width
(done)      - 1 <= height
(to do)     - Exactly one of --stack/-s and --queue/-q are provided
(to do)     - The argument to --output/-o (if provided) is either "list" or "map"
(to do)     - No invalid command line options are provided (such as -x or --eecs281)
(done)      - No invalid door or button appears in the map (if <num_colors> == 3 , then 'M' and 'z' are invalid)
(done)      - No invalid characters appear in the map ('+' can’t appear in the map, but it could appear in a comment)
(done)      - '@' appears exactly once in the input map
(done)      - '?' appears exactly once in the input map   
        */
        void throwError(){
            exit(1);
        }
    
        void resizeMap(){
            map.resize(rows, vector<char>(cols, -1));
        }
        void resizeBacktrace(){
            backtrace.resize(numColors, 
                    vector<vector<char> >(rows, vector<char>(cols, -1)));
        }
        bool isComment(string l){
            if(*(l.begin()) == '/'){
                return true;
            }
            return false;
        }
        vector<char> lineConvert(string l){
            vector<char> toReturn;
            int numCols = 0;
            for(string::iterator it = l.begin(); it != l.end(); it++){
                toReturn.push_back(*it);
                numCols++;
            }
            if(numCols > cols){
                cout << "ERROR MESSAGE: NUMCOLS > COLS \n";
                throwError();
            }
            return toReturn;
        }
        /*
          - 2 4 4 (then would have actual map)
                //VIDEO 3D EXAMPLE
         -once the reading started, don't use get line
         - cin >> numColors >> rows >> cols 
            - getline(cin, junk); (retrieves the \n that we didnt read in)
            - RESIZE VECTOR(S) (map and backtrace) NOW
         */
        void checkValidInitializing(){
            if(0 > numColors || numColors > 26
            || 1 > rows || 1 > cols){
                cout << "INPUT FOR INITIALIZING TABLE INCORRECT\n";
                throwError();
            }
        }
        pair<int,pair<int,int>> makeCoordinate(int col, int r, int c){
            return make_pair(col, make_pair(r,c));
        }
        /*
        converts a given tile (in char form) to the integer it matches
        */
        int charToRoom(char tile){
            if(tile == '^'){
                return 0;
            }
            return static_cast<uint32_t>(tile - 'a') + 1;
        }

        //converts a given roomColor (in int form) to the character it matches
        char roomToChar(int roomColor){
            if(roomColor == 0){
                return '^';
            }
            return static_cast<char>(roomColor + 'a') - 1;
        }
        //checks if char is ?
        bool isQuestionMark(char t){
            return  t == '?';
        }
        //checks if char is @
        bool isAtSymbol(char t){
            return  t == '@';
        }
        //checks if character is valid
        bool isValidCharacter(char t){
            bool found = false;  
            if(t == '^' || t == '?' || t == '#'){
                return true;
            }
            //running from a to a + numColors (i.e. how many colors past default)
            for(int i = 'a'; i <= 'a' + numColors; i++){
                if(i == tolower(t)){
                    found = true;
                }
            }
            return false;
        }
        //checks to ensure all characters in the map are valid
        void checkMapValidity(){
            int atCounter = 0;
            int questionCounter = 0;
            
            for(auto &charVec:map){
                for(vector<char>::iterator it = charVec.begin(); it != charVec.end(); it++){
                    if(isAtSymbol(*it)){
                        atCounter++;
                    }
                    else if(isQuestionMark(*it)){
                        questionCounter++;
                    }
                    else if(isValidCharacter(*it)){
                        //maybe code to add here?
                    }
                    else{
                        throwError();
                    }
                }
            }
            if(atCounter > 1 || questionCounter > 1){
                throwError();
            }

        }
        

        //finds the @ in the map and sets the initialPosition struct to it
        void findInitialPosition(){
            int rowCounter = 0;
            for(auto &charVec:map){
                int colCounter = 0;
                for(vector<char>::iterator it = charVec.begin(); it != charVec.end(); it++){
                    if(isAtSymbol(*it)){
                        initialPosition = makeCoordinate(charToRoom('^'),rowCounter, colCounter);
                    }
                    colCounter++;
                }
                rowCounter++;
            }
        }


        bool existsInBacktrace(const pair<int, pair<int,int>> &curr){
            bool existsInAColor = false;
            if(gotDiscovered(curr)){
                return true;
            }
            if(numColors > 0){
                for(char counter = 'a'; counter < 'a' + numColors; counter ++){
                    if(gotDiscovered(makeCoordinate(counter, curr.second.first,curr.second.second))){
                        return true;
                    }
                }
            }
            return false;
        }
        /*
        All undiscovered locations replaced with '#'. A location is undiscovered i
        f it was never discovered in any color. This includes empty floors, doors, 
        buttons, and the target. If there is no solution to the puzzle, 
        the target will always be replaced with '#'.*/
        void rewriteMapNoSolution(){
            int rowCounter = 0;
            for(auto &charVec:map){
                int columnCounter = 0;
                for(vector<char>::iterator it = charVec.begin(); it != charVec.end(); it++){
                    if(!existsInBacktrace(makeCoordinate(*it, rowCounter, columnCounter))){
                        *it = '#';
                    }
                    columnCounter++;
                }
                rowCounter++;
            }
        }

        //prints the map
        void printMap(){
            if(!solutionFound){
                for(auto &charVec:map){
                    for(vector<char>::iterator it = charVec.begin(); it != charVec.end(); it++){
                        cout << *it;
                    }
                }
            }
            else{
               for(auto &i:backtrace){
                cout << "// color " << i[0][0]<< "\n";
                for(auto &row:i){
                    for(auto &col:row){
                        cout << col;
                    }
                    cout << "\n";
                }
               } 
            }
        }

        /*
         - The deque serves as the search_container in the spec
        - always use .push_back()
        - when you're supposed to use a stack, use .back and .pop_back()
        - for a queue, use .front() and .pop_front()
        */

        bool isButton(char &curr){
            if(curr == '^'){
                return true;
            }
            for(char i = 'a'; i < 'a' + numColors; i++){
                if(i == curr){
                    return true;
                }
            }
            return false;
        }

        //returns whether the provided coordinate is out of bounds or not
        bool isValidCoordinate(const pair<int,pair<int,int>> &toWalkOn){
            int r = toWalkOn.second.first;
            int c = toWalkOn.second.second;
            if(r < 0 || r >= rows || c < 0 || c >= cols){
                return false;
            }
            return true;
        }

        //returns whether given color, the second color is valid
        //i.e. if ^, can walk on a, b, c ...
        //if a, can walk on b, c, not ^ because already discovered
        //or if a, can walk on A, but not B
        bool isValidWalkingColor(const char &curr, const char &comp){
            bool found = false;
            if(curr == tolower(comp)){
                return true;
            }
            for(char i = curr; i < 'a' + numColors; i ++){
                if(comp == i){
                    found = true;
                }
            }
            return found;
        }
        void printCoord(int color, int row, int column){
            cout << "(" << roomToChar(color) << ", " << "(" << row << "," << column << "))\n";
        }
        //returns whether the provided coordinate is a wall given a color
        bool isWall(const int &currColor, const int &walkingColor){
            char currChar = roomToChar(currColor);
            char toWalk = roomToChar(walkingColor);
            if(toWalk == '#'){
                return true;
            }
            else if(toWalk != '.' && toWalk != '@' && toWalk != '?'
                && !isValidWalkingColor(currChar, toWalk)){
                return false;
            }
        }
        
        //given two coordinates, returns whether the second is walkable or not
        bool isWalkable(const pair<int, pair<int,int>> &curr, const pair<int,pair<int,int>> &toWalkOn){
            if(!isValidCoordinate(toWalkOn)){
                return false;
            }
            else if(isWall(curr.first, toWalkOn.first)){
                return false;
            }
            else{
                return true;
            }

        }
        
        //runs through the discovered vector seeing if the curr location has been discovered
        bool gotDiscovered(const pair<int,pair<int,int>> &curr){
            for(int i = 0; i < placesDiscovered.size(); i++){
                if(placesDiscovered[i].first == curr.first
                && placesDiscovered[i].second == curr.second){
                    return true;
                }
            }
            return false;
        }

        //looks north, east, south, west, if possible
        void lookDirection(pair<int,pair<int,int>> &curr){
            int currentColor = curr.first;
            int currentRow = curr.second.first;
            int currentCol = curr.second.second;
            pair<int, pair<int, int>> north = makeCoordinate(currentColor, currentRow - 1, currentCol);
            pair<int, pair<int, int>> east = makeCoordinate(currentColor, currentRow, currentCol + 1);
            pair<int, pair<int, int>> south = makeCoordinate(currentColor, currentRow + 1, currentCol);
            pair<int, pair<int, int>> west = makeCoordinate(currentColor, currentRow, currentCol - 1);
            //provided they are not off the edge of the map, not previously discovered,
            // and not a wall or closed door:
                // 7. north/up { <color>, <row - 1>, <col> }
                if(!gotDiscovered(north) && isWalkable(curr, north)){
                    searchContainer.push_front(north);
                    backtrace[north.first][north.second.first][north.second.second] = 'S';
                    placesDiscovered.push_back(north);
                }
                // 8. east/right { <color>, <row>, <col + 1> }
                if(!gotDiscovered(east) && isWalkable(curr, east)){
                    searchContainer.push_front(east);
                    backtrace[east.first][east.second.first][east.second.second] = 'W';
                    placesDiscovered.push_back(east);
                }
                // 9. south/down { <color>, <row + 1>, <col> }
                if(!gotDiscovered(south) && isWalkable(curr, south)){
                    searchContainer.push_front(south);
                    backtrace[south.first][south.second.first][south.second.second] = 'N';
                    placesDiscovered.push_back(south);
                }
                //10. west/left { <color>, <row>, <col - 1> }
                if(!gotDiscovered(west) && isWalkable(curr, west)){
                    searchContainer.push_front(west);
                    backtrace[west.first][west.second.first][west.second.second] = 'E';
                    placesDiscovered.push_back(west);
                }
        }

        //when using queue: .front() and .pop_front()
        void solveQueue(){
            //Your solution should faithfully implement the following algorithm:
            
            //1. Initially mark all states as not discovered.
            //done in the creation of Place

            //2. Add the initial state to the search_container and mark it as discovered.
            searchContainer.push_front(initialPosition);
            backtrace[initialPosition.first][initialPosition.second.first][initialPosition.second.second] = '@';
            char currentColor = '^';
            // 3. Loop while the search_container is NOT empty.
            while(!searchContainer.empty() && !solutionFound){
                // 4. Remove the “next” item from the search_container. This is the current_state, which consists of a 
                //<color> plus <row> and <col> coordinates { <color>, <row>, <col> }.
                currentState.push_front(searchContainer.front());
                searchContainer.pop_front();
                // 5. If current_state { <color>, <row>, <col> } is standing on an active button,
                // <button>, there is a chance to add a color change. 
                pair<int,pair<int,int>> curr = currentState.front();
                if(isQuestionMark(map[curr.second.first][curr.second.second])){
                    solutionFound = true;
                    solvedPosition = curr;
                }
                else if(isButton(map[curr.second.first][curr.second.second])){
                    pair<int,pair<int,int>> colorChangedCurr = 
                        makeCoordinate(charToRoom(map[curr.second.first][curr.second.second])
                            , curr.second.first, curr.second.second);
                    if(!gotDiscovered(colorChangedCurr)){
                        //If { <button>, <row>, <col> } is not yet discovered, 
                        //add the color change to the search_container and mark it as discovered. 
                        searchContainer.push_front(colorChangedCurr);
                        backtrace[colorChangedCurr.first][colorChangedCurr.second.first][colorChangedCurr.second.second] 
                                = roomToChar(currentColor);
                        currentColor = colorChangedCurr.first;
                        placesDiscovered.push_back(colorChangedCurr);
                    }
                    //Investigation from an active button will result in zero or one states being discovered.
                }//case where undiscovered button
                else{
                    lookDirection(curr);
                }
                currentState.pop_front();
            }
        }
       //when using stack: .back() and .pop_back()
        void solveStack(){
            
        /*
            Your solution should faithfully implement the following algorithm:

            1. Initially mark all states as not discovered.

            2. Add the initial state to the search_container and mark it as discovered.
            3. Loop while the search_container is NOT empty.
            4. Remove the “next” item from the search_container. This is the current_state, which consists of a <color> plus <row> and <col> coordinates { <color>, <row>, <col> }.
            5. If current_state { <color>, <row>, <col> } is standing on an active button, <button>, there is a chance to add a color change. If { <button>, <row>, <col> } is not yet discovered, add the color change to the search_container and mark it as discovered. Investigation from an active button will result in zero or one states being discovered.
            6. f current_state { <color>, <row>, <col> } is NOT standing on an active button, there is a chance to add adjacent locations. Discover the following four states, provided they are not off the edge of the map, not previously discovered, and not a wall or closed door:
            7. north/up { <color>, <row - 1>, <col> }
            8. east/right { <color>, <row>, <col + 1> }
            9. south/down { <color>, <row + 1>, <col> }
            10. west/left { <color>, <row>, <col - 1> }
            11. Repeat from step 3.
        */
        }
        //solvePuzzle checks if there is a solution to either
        //the queue call or the stack call depending on the argument passed
        void solvePuzzle(){
            //if queue
            solveQueue();

            //if stack
            solveStack();
        }


        // For list output, print the list of states in the solution path from 
        // the starting location to the target, inclusive. 
        // Use the format (<color>, (<row>, <col>)) with <color> being a 
        // lowercase letter or '^' to indicate the active color, 
        // <row> being the 0-indexed row of the location, and <col> being 
        // the 0-indexed column of the location. The first state printed 
        // should be '^' with the starting location, and the last state 
        // printed should be the current color when the location of the target 
        // was first discovered by the algorithm.
        void printListOutput(){
            //for default case
            for(int r = 0; r < rows; r++){
                for(int c = 0; c < cols; c++){
                    if(backtrace[charToRoom('^')][r][c] != ' '){
                        printCoord(0, r, c);
                    }
                }
            }
            //for all following cases
            for(char t = 'a'; t < 'a' + numColors; t++){
                for(int r = 0; r < rows; r++){
                    for(int c = 0; c < cols; c++){
                        if(backtrace[charToRoom(t)][r][c] != ' '){
                            printCoord(charToRoom(t), r, c);
                        }
                    }
                }
            }
        }

        void pathFinder(){
            pair<int,pair<int,int>> currentPosition = solvedPosition;
            bool cameFromButton = false;
            while(currentPosition != initialPosition){
                int currColor = currentPosition.first;
                int currRow = currentPosition.second.first;
                int currColumn = currentPosition.second.second;
                if(currentPosition == solvedPosition){
                    backtrace[currColor][currRow][currColumn] = '?';
                }
                if(isButton(backtrace[currColor][currRow][currColumn])){
                    char roomCameFrom = backtrace[currColor][currRow][currColumn];
                    backtrace[currColor][currRow][currColumn] = '@';
                    currentPosition = makeCoordinate(roomToChar(roomCameFrom), currRow, currColumn);
                    cameFromButton = true;
                }
                else{
                    if(cameFromButton){
                        if(backtrace[currColor][currRow][currColumn] == 'N'){
                            backtrace[currColor][currRow][currColumn] = '%';
                            currentPosition = makeCoordinate(currColor, currRow - 1, currColumn);
                        }
                        else if(backtrace[currColor][currRow][currColumn] == 'E'){
                            backtrace[currColor][currRow][currColumn] = '%';
                            currentPosition = makeCoordinate(currColor, currRow, currColumn + 1);
                        }
                        else if(backtrace[currColor][currRow][currColumn] == 'S'){
                            backtrace[currColor][currRow][currColumn] = '%';
                            currentPosition = makeCoordinate(currColor, currRow + 1, currColumn);
                        }
                        else if(backtrace[currColor][currRow][currColumn] == 'w'){
                            backtrace[currColor][currRow][currColumn] = '%';
                            currentPosition = makeCoordinate(currColor, currRow, currColumn - 1);
                        }
                        cameFromButton = false;
                    }
                    else{
                        if(backtrace[currColor][currRow][currColumn] == 'N'){
                            backtrace[currColor][currRow][currColumn] = '+';
                            currentPosition = makeCoordinate(currColor, currRow - 1, currColumn);
                        }
                        else if(backtrace[currColor][currRow][currColumn] == 'E'){
                            backtrace[currColor][currRow][currColumn] = '+';
                            currentPosition = makeCoordinate(currColor, currRow, currColumn + 1);
                        }
                        else if(backtrace[currColor][currRow][currColumn] == 'S'){
                            backtrace[currColor][currRow][currColumn] = '+';
                            currentPosition = makeCoordinate(currColor, currRow + 1, currColumn);
                        }
                        else if(backtrace[currColor][currRow][currColumn] == 'w'){
                            backtrace[currColor][currRow][currColumn] = '+';
                            currentPosition = makeCoordinate(currColor, currRow, currColumn - 1);
                        }
                    }
                }
            }
        }
        void printMapOutput(){
            
            // - Print the map similar to the way it was given in the input 
            // (excluding the map parameters and any comments). 
            // - Print each color in a separate map one after the other,
            // starting with ^ and working alphabetically from A to all others
            // - Each color map should be preceded by a comment indicating the 
            // color being output and include the following replacements:
            //     - On the solution path between the starting location and target, 
            //     empty floors and open doors should be replaced with '+' (done)
            //     - On the solution path, replace inactive buttons with '%'
            //     on the map where they were discovered and with '@' on the map that
            //     matches the button (done)
            //     - On the //color ^ map, all trapped buttons not on the solution path
            //     marked with '.'
            //     - On the //color a map, all matching buttons a and doors A not on the 
            //     solution path are replaced with '.' (NEED TO DO, ALSO TWEAK THE BUTTONS MAYE???)
            
            cout << "// color '^'\n";
            for(int r = 0; r < rows; r++){
                for(int c = 0; c < cols; c++){
                    if(backtrace[0][r][c] != ' '){
                            cout << backtrace[0][r][c];
                    } 
                    else{
                        cout << map[r][c];
                    }
                }
                cout << "\n";
            } //for default color
            for(char t = 'a'; t < 'a' + numColors; t++){
                cout << "// color " << t << "\n";
                for(int r = 0; r < rows; r++){
                    for(int c = 0; c < cols; c++){
                        if(backtrace[charToRoom(t)][r][c] != ' '){
                            cout << backtrace[charToRoom(t)][r][c];
                        } 
                        else{
                            cout << map[r][c];
                        }
                    }
                cout << "\n";
                }
            } // for all colors
        }

        void readPuzzle(){
            string junk;
            cin >> numColors >> rows >> cols;
            checkValidInitializing();
            getline(cin, junk);
            //these call on the newly defined numColors, rows, cols
            resizeBacktrace();
            resizeMap();
            //figure out why we need to count row here
            int row = 0;
            string line;
            while(getline(cin, line )){
                if(!isComment(line)){
                   map.push_back(lineConvert(line));    //might be incorrect
                   row++;
                }
            }
            checkMapValidity();
            findInitialPosition();
            //calls on solve puzzle now that the map has been filled in
            //i.e., start filling search container and current state, filling backtrace
            solvePuzzle();
            //if solution was found, print backtrace w solution
            if(solutionFound){
                //if --ouput/-o list
                printListOutput();
                //if --output/-o map
                printMapOutput();
                    //with stack
                    //with queue
                
            }
            //else print no solution + information to follow
            else{
                cout << "No solution.\nDiscovered:";
                rewriteMapNoSolution();
                printMap();
            }

        }
}; //end of PuzzleRunner Class

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

int main(int argc, char *argv[]) {
  // This should be in all of your projects, speeds up I/O
  ios_base::sync_with_stdio(false);


  // Get the mode from the command line and read in the data
  Options options;
  getMode(argc, argv, options);
  vector<double> data;
//   if (options.mode == Mode::kResize)
//     readWithResize(data);
//   else if (options.mode == Mode::kReserve)
//     readWithReserve(data);
//   else if (options.mode == Mode::kNoSize)
//     readWithNoSize(data);

}