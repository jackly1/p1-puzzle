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
            -south does exist, never been discovered , something walkable, mark it as N (came from the NORTH)
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


class PuzzleRunner{
    private: 
        //when using stack: .back() and .pop_back()
        //when using queue: .front() and .pop_front()
        uint32_t numColors = 0;
        uint32_t rows = 0;
        uint32_t cols = 0;
        vector<vector<char> > map;
        vector<vector<vector<char> > > backtrace;
        deque<int, pair<int, int>> searcherStorer;
    public:
        
        /*
        Simply throws error if one of the following occurs:
(done)      - 0 <= num_colors <= 26 (0 colors is valid, it just means that there are no doors)
(done)      - 1 <= width
(done)      - 1 <= height
(to do)      - Exactly one of --stack/-s and --queue/-q are provided
(to do)      - The argument to --output/-o (if provided) is either "list" or "map"
(to do)      - No invalid command line options are provided (such as -x or --eecs281)
(to do)      - No invalid door or button appears in the map (if <num_colors> == 3 , then 'M' and 'z' are invalid)
(to do)      - No invalid characters appear in the map ('+' can’t appear in the map, but it could appear in a comment)
(to do)      - '@' appears exactly once in the input map
(to do)      - '?' appears exactly once in the input map   
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

        /*
        converts a given tile (in char form) to the integer it matches
        */
        int charToRoom(char tile){
            return static_cast<uint32_t>(tile - 'a') + 1;
        }

        /*
        converts a given roomColor (in int form) to the character it matches
        */
        char roomToChar(int roomColor){
            return static_cast<char>(roomColor + 'a') - 1;
        }
        void readPuzzle(){
            string junk;
            cin >> numColors >> rows >> cols;
            checkValidInitializing();
            getline(cin, junk);
            //figure out why we need to count row here
            int row = 0;
            string line;
            while(getline(cin, line )){
                if(!isComment(line)){
                   map.push_back(lineConvert(line));    //might be incorrect
                   row++;
                }
            }
        }

        bool solveQueue(){
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
            return false;
        }

        bool solveStack(){
            return false;
        }
        //solvePuzzle checks if there is a solution to either
        //the queue call or the stack call depending on the argument passed
        bool solvePuzzle(){
            //if queue
            return solveQueue();
            //if stack
            return solveStack();
        }
        void printNoSolution(){
            //now print the map as it was provided upon input
            //however, with all undiscovered locations replaced with #
            //this includes empty floors, doors, buttons, and the target,
            //if no sultion, target will always be replaced with #
        }
        void printListOutput(){
        /*
            For list output, print the list of states in the solution path from 
            the starting location to the target, inclusive. 
            Use the format (<color>, (<row>, <col>)) with <color> being a 
            lowercase letter or '^' to indicate the active color, 
            <row> being the 0-indexed row of the location, and <col> being 
            the 0-indexed column of the location. The first state printed 
            should be '^' with the starting location, and the last state 
            printed should be the current color when the location of the target 
            was first discovered by the algorithm.

            List mode output using a queue search_container:
                (^, (0, 0))
                (^, (0, 1))
                (^, (1, 1))
                (a, (1, 1))
                (a, (0, 1))
                (a, (0, 2))
                (a, (0, 3))
                (a, (0, 4))
                (a, (0, 5))
                (a, (0, 6))
                (b, (0, 6))
                (b, (0, 5))
                (b, (0, 4))
                (b, (1, 4))
                (b, (2, 4))
                (b, (3, 4))
                (b, (3, 3))
                (b, (3, 2))
                (b, (3, 1))
                (b, (3, 0))
        */
        }
        void printMapOutput(){
        /*
            - Print the map similar to the way it was given in the input 
            (excluding the map parameters and any comments). 
            - Print each color in a separaate map one after the other,
            starting with ^ and working alphabetically from A to all others
            - Each color map should be preceded by a comment indicating the 
            color being output and include the following replacements:
                - On the solution path between the starting location and target, 
                empty floors and open doors should be replaced with '+'
                - On the solution path, replace inactive buttons with '%'
                on the map where they were discovered and with '@' on the map that
                matches the button
                - On the //color ^ map, all trapped buttons not on the solution path
                marked with '.'
                - On the //color a map, all matching buttons a and doors A not on the 
                solution path are replaced with '.'
                Color maps following 'a' replace buttons and doors with '.', similar to 'a'
            - This is map output using a queue search_container; it begins with 
            the starting location @ in the upper left of the //color ^ map,
            moves east to the + and then south onto the a button, which is pressed, 
            and replcaed by %. he a button press moves the solution path to the 
            // color a map at the same location, which is replaced by @. 
            Then the trail of + shows travel north and east until button b is 
            pressed and replaced by %. The b button press moves the solution path 
            to the // color b map at the same location, which is replaced by @, 
            then the trail of + shows travel west then south then west until 
            the target is discovered. The starting location is only displayed 
            on the ^ map, but the target is displayed on all color maps. 
            
            Map output with queue:
                // color ^
                @  +  .  A  .  .  b
                .  %  .  #  B  #  #
                #  #  #  #  .  .  .
                ?  .  .  B  .  .  .
                // color a
                .  +  +  +  +  +  %
                .  @  .  #  B  #  #
                #  #  #  #  .  .  .
                ?  .  .  B  .  ^  ^
                // color b
                .  .  .  A  +  +  @  
                .  a  .  #  +  #  #
                #  #  #  #  +  .  .
                ?  +  +  +  +  ^  ^
            Map output with stack:
                // color ^
                @  .  .  A  .  .  b
                .  %  .  #  B  #  #
                #  #  #  #  .  .  .
                ?  .  .  B  .  .  .
                // color a
                .  +  +  +  +  +  %
                .  @  .  #  B  #  #
                #  #  #  #  .  .  .
                ?  .  .  B  .  ^  ^
                // color b
                .  .  .  A  +  +  @  
                .  a  .  #  +  #  #
                #  #  #  #  +  .  .
                ?  +  +  +  +  ^  ^
            normally a bigger difference can be found in a more complicated map
        */
        }

        //calls on either list or map outputs to be printed
        void printSolved(){
            //if --ouput/-o list
            printListOutput();
            //if --output/-o map
            printMapOutput();
        }
        
        //runs the whole program, starting by a call to see if the puzzle is solvable
        void play(){
            if(!solvePuzzle){
                cout << "No solution.\nDiscovered:";
                printNoSolution();
            }
            else{
                printSolved();
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