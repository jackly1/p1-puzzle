// Project Identifier: A8A3A33EF075ACEF9B08F5B9845569ECCB423725

// EECS 281, Project 1

#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <vector> 
#include <deque>

using namespace std;

// This struct is used to pass options read from the command line, to
// the functions that actually do the work. Project 0 is simple enough to
// not need this, but it is a technique that could be helpful in doing more
// complicated command-line processing in future projects.


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
        pair<uint32_t, pair<uint32_t, uint32_t>> initialPosition;
        //the position at which the ? was found
        pair<uint32_t, pair<uint32_t, uint32_t>> solvedPosition;
        //deque of everything contained in searchContainer
        deque<pair<uint32_t, pair<uint32_t, uint32_t>>> searchContainer;
        //deque of everything contained in the currentState
        deque<pair<uint32_t, pair<uint32_t, uint32_t>>> currentState;
        //a vector of all (color, row, column) that have been discovered
        vector<pair<uint32_t, pair<uint32_t, uint32_t>>> placesDiscovered;
        //bool stack: false if queue mode, true if stack mode
        bool stack = false;

        //if listmode false: print as map, else print as list
        bool listmode = false;

        //commandline read in:
        /*
        prohibits arguments and causes the program to print a helpful message
        of own design, return 0 to exit
        --help/-h
        prohibits arguments and directs the program to use a search container 
        that behaves like a queue, performing a breadth first search
        --queue/-q
         prohibits arguments and directs the program to use a search container 
        that behaves like a stack, performing a depth first search
        --stack/-s
        requires an argument {TYPE} that must either be map or list
        and directs the program to print solution output in the specified format
        if --ouput not provided, assume --ouput map
        --output {TYPE} / -o {TYPE}
        */


        void initializeBacktrace(){
            //initialize default layer
            for(uint32_t r = 0; r < rows; r++){
                for(uint32_t c = 0; c < cols; c++){
                    backtrace[0][r][c] = ' ';
                }
            }
            //initialize every further layer
            for(char t = 'a'; t < 'a' + (int)numColors; t++){
                for(uint32_t r = 0; r < rows; r++){
                    for(uint32_t c = 0; c < cols; c++){
                        backtrace[charToRoom(t)][r][c] = ' ';
                    }
                }
            }
        }
    public:
        void setListMode(bool t){
            if(t){
                listmode = true;
            }
            else{
                listmode = false;
            }
        }
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
            backtrace.resize(numColors + 1, 
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
            uint32_t numCols = 0;
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
            if(0 > (int)numColors || numColors > 26
            || 1 > rows || 1 > cols){
                cout << "INPUT FOR INITIALIZING TABLE INCORRECT\n";
                throwError();
            }
        }
        pair<uint32_t,pair<uint32_t,uint32_t>> makeCoordinate(uint32_t col, uint32_t r, uint32_t c){
            return make_pair(col, make_pair(r,c));
        }
        /*
        converts a given tile (in char form) to the integer it matches
        */
        uint32_t charToRoom(char tile){
            if(tile == '^'){
                return 0;
            }
            return static_cast<uint32_t>(tile - 'a') + 1;
        }

        //converts a given roomColor (in int form) to the character it matches
        char roomToChar(uint32_t roomColor){
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
            if(t == '@' || t == '^' || t == '?' || t == '#' || t == '.'){
                return true;
            }
            //running from a to a + numColors (i.e. how many colors past default)
            for(char i = 'a'; i < 'a' + (int)numColors; i++){
                if(i == tolower(t)){
                    found = true;
                }
            }
            return found;
        }
        //checks to ensure all characters in the map are valid
        void checkMapValidity(){
            uint32_t atCounter = 0;
            uint32_t questionCounter = 0;
            
            for(uint32_t r = 0; r < rows; r++){
                for(uint32_t c = 0; c < cols; c++){
                    if(isAtSymbol(map[r][c])){
                        atCounter++;
                    }
                    else if(isQuestionMark(map[r][c])){
                        questionCounter++;
                    }
                    else if(isValidCharacter(map[r][c])){
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
            uint32_t rowCounter = 0;
            for(auto &charVec:map){
                uint32_t colCounter = 0;
                for(vector<char>::iterator it = charVec.begin(); it != charVec.end(); it++){
                    if(isAtSymbol(*it)){
                        initialPosition = makeCoordinate(charToRoom('^'),rowCounter, colCounter);
                    }
                    colCounter++;
                }
                rowCounter++;
            }
        }


        bool existsInBacktrace(const pair<uint32_t, pair<uint32_t,uint32_t>> &curr){
            if(gotDiscovered(curr)){
                return true;
            }
            if(numColors > 0){
                for(char counter = 'a'; counter < 'a' + (int)numColors; counter ++){
                    if(gotDiscovered(makeCoordinate(charToRoom(counter), curr.second.first,curr.second.second))){
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
            uint32_t rowCounter = 0;
            for(auto &charVec:map){
                uint32_t columnCounter = 0;
                for(vector<char>::iterator it = charVec.begin(); it != charVec.end(); it++){
                    if(!existsInBacktrace(makeCoordinate(charToRoom(*it), rowCounter, columnCounter))){
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
        bool isButtonChar(const char &button){
            if(button == '^'){
                return true;
            }
            for(char i = 'a'; i < 'a' + (int)numColors; i++){
                if(i == button){
                    return true;
                }
            }
            return false;
        }
        bool isButton(const char &curr, const char &potentialButton){
            if(curr != potentialButton && isButtonChar(potentialButton)){
                return true;
            }
            return false;
        }

        bool shouldChange(const char &curr, const char &button){
            if(curr == button){
                return false;
            }
            bool buttonFound = false;
            for(char i = 'a'; i < 'a' + (int)numColors; i++){
                if(i == curr){
                    buttonFound = true;
                }
            }
            return buttonFound;
        }

        //returns whether the provided coordinate is out of bounds or not
        bool isValidCoordinate(const pair<uint32_t,pair<uint32_t,uint32_t>> &toWalkOn){
            uint32_t r = toWalkOn.second.first;
            uint32_t c = toWalkOn.second.second;
            if((int)r < 0 || r >= rows || (int)c < 0 || c >= cols){
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
            for(char i = curr; i < 'a' + (int)numColors; i ++){
                if(comp == i){
                    found = true;
                }
            }
            return found;
        }
        void printCoord(uint32_t color, uint32_t row, uint32_t column){
            cout << "(" << roomToChar(color) << ", " << "(" << row << ", " << column << "))\n";
        }
        //returns whether the provided coordinate is a wall given a color
        bool isWall(const char &currColor, const char &walkingColor){
            if(walkingColor == '#'){
                return true;
            }
            else if(walkingColor != '.' && walkingColor != '@' && walkingColor != '?'
                && !isValidWalkingColor(currColor, walkingColor)){
                return true;
            }
            return false;
        }
        
        //given two coordinates, returns whether the second is walkable or not
        bool isWalkable(const char &currColor, const char &mapChar){
            if(isWall(currColor, mapChar)){
                return false;
            }
            else{
                return true;
            }
        }
        
        //runs through the discovered vector seeing if the curr location has been discovered
        bool gotDiscovered(const pair<uint32_t,pair<uint32_t,uint32_t>> &curr){
            for(uint32_t i = 0; i < placesDiscovered.size(); i++){
                if(placesDiscovered[i].first == curr.first
                && placesDiscovered[i].second == curr.second){
                    return true;
                }
            }
            return false;
        }

        //looks north, east, south, west, for a queue
        void lookDirectionQueue(pair<uint32_t,pair<uint32_t,uint32_t>> &curr){
            uint32_t currentColor = curr.first;
            char currentChar = roomToChar(curr.first);
            uint32_t currentRow = curr.second.first;
            uint32_t currentCol = curr.second.second;
            pair<uint32_t, pair<uint32_t, uint32_t>> north = makeCoordinate(currentColor, currentRow - 1, currentCol);
            pair<uint32_t, pair<uint32_t, uint32_t>> east = makeCoordinate(currentColor, currentRow, currentCol + 1);
            pair<uint32_t, pair<uint32_t, uint32_t>> south = makeCoordinate(currentColor, currentRow + 1, currentCol);
            pair<uint32_t, pair<uint32_t, uint32_t>> west = makeCoordinate(currentColor, currentRow, currentCol - 1);
            //provided they are not off the edge of the map, not previously discovered,
            // and not a wall or closed door:
                // 7. north/up { <color>, <row - 1>, <col> }
                if(!gotDiscovered(north) && isValidCoordinate(north) && isWalkable(currentChar, map[north.second.first][north.second.second])){
                    //cout << "looked north from ";
                   // printCoord(currentColor,currentRow,currentCol);
                    searchContainer.push_back(north);
                    backtrace[north.first][north.second.first][north.second.second] = 'S';
                    placesDiscovered.push_back(north);
                }
                // 8. east/right { <color>, <row>, <col + 1> }
                if(!gotDiscovered(east) && isValidCoordinate(east) && isWalkable(currentChar, map[east.second.first][east.second.second])){
                    //cout << "looked east from ";
                   // printCoord(currentColor,currentRow,currentCol);
                    searchContainer.push_back(east);

                    backtrace[east.first][east.second.first][east.second.second] = 'W';
                    placesDiscovered.push_back(east);
                }
                // 9. south/down { <color>, <row + 1>, <col> }
                if(!gotDiscovered(south) && isValidCoordinate(south)&& isWalkable(currentChar, map[south.second.first][south.second.second])){
                    //cout << "looked south from ";
                  //  printCoord(currentColor,currentRow,currentCol);
                    searchContainer.push_back(south);
                    backtrace[south.first][south.second.first][south.second.second] = 'N';
                    placesDiscovered.push_back(south);
                }
                //10. west/left { <color>, <row>, <col - 1> }
                if(!gotDiscovered(west) && isValidCoordinate(west) && isWalkable(currentChar, map[west.second.first][west.second.second])){
                    //cout << "looked west from ";
                   // printCoord(currentColor,currentRow,currentCol);
                    searchContainer.push_back(west);
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
            searchContainer.push_back(initialPosition);
            backtrace[initialPosition.first][initialPosition.second.first][initialPosition.second.second] = '@';
            placesDiscovered.push_back(initialPosition);
            char currentColor = '^';
            // 3. Loop while the search_container is NOT empty.
            while(!searchContainer.empty() && !solutionFound){
                // 4. Remove the “next” item from the search_container. This is the current_state, which consists of a 
                //<color> plus <row> and <col> coordinates { <color>, <row>, <col> }.
                currentState.push_back(searchContainer.front());
                searchContainer.pop_front();
                // 5. If current_state { <color>, <row>, <col> } is standing on an active button,
                // <button>, there is a chance to add a color change. 
                pair<uint32_t,pair<uint32_t,uint32_t>> curr = currentState.front();
                //cout << "currently at ";
                //printCoord(curr.first,curr.second.first,curr.second.second);
                if(isQuestionMark(map[curr.second.first][curr.second.second])){
                    solutionFound = true;
                    solvedPosition = curr;
                }
                else if(isButton(roomToChar(curr.first),map[curr.second.first][curr.second.second])){
                    //cout << "this is a button\n";
                    pair<uint32_t,pair<uint32_t,uint32_t>> colorChangedCurr = 
                        makeCoordinate(charToRoom(map[curr.second.first][curr.second.second])
                            , curr.second.first, curr.second.second);
                    if(!gotDiscovered(colorChangedCurr)){
                        //If { <button>, <row>, <col> } is not yet discovered, 
                        //add the color change to the search_container and mark it as discovered. 
                        searchContainer.push_back(colorChangedCurr);
                        backtrace[colorChangedCurr.first][colorChangedCurr.second.first][colorChangedCurr.second.second] 
                                = currentColor;
                        currentColor = roomToChar(colorChangedCurr.first);
                        placesDiscovered.push_back(colorChangedCurr);
                    }
                    //Investigation from an active button will result in zero or one states being discovered.
                }//case where undiscovered button
                else{
                    lookDirectionQueue(curr);
                }
                currentState.pop_front();
            }
        }


        //looks north, east, south, west, for a stack
        void lookDirectionStack(pair<uint32_t,pair<uint32_t,uint32_t>> &curr){
            uint32_t currentColor = curr.first;
            char currentChar = roomToChar(curr.first);
            uint32_t currentRow = curr.second.first;
            uint32_t currentCol = curr.second.second;
            pair<uint32_t, pair<uint32_t, uint32_t>> north = makeCoordinate(currentColor, currentRow - 1, currentCol);
            pair<uint32_t, pair<uint32_t, uint32_t>> east = makeCoordinate(currentColor, currentRow, currentCol + 1);
            pair<uint32_t, pair<uint32_t, uint32_t>> south = makeCoordinate(currentColor, currentRow + 1, currentCol);
            pair<uint32_t, pair<uint32_t, uint32_t>> west = makeCoordinate(currentColor, currentRow, currentCol - 1);
            //provided they are not off the edge of the map, not previously discovered,
            // and not a wall or closed door:
                // 7. north/up { <color>, <row - 1>, <col> }
                if(!gotDiscovered(north) && isValidCoordinate(north) && isWalkable(currentChar, map[north.second.first][north.second.second])){
                    //cout << "looked north from ";
                   // printCoord(currentColor,currentRow,currentCol);
                    searchContainer.push_front(north);
                    backtrace[north.first][north.second.first][north.second.second] = 'S';
                    placesDiscovered.push_back(north);
                }
                // 8. east/right { <color>, <row>, <col + 1> }
                if(!gotDiscovered(east) && isValidCoordinate(east) && isWalkable(currentChar, map[east.second.first][east.second.second])){
                    //cout << "looked east from ";
                   // printCoord(currentColor,currentRow,currentCol);
                    searchContainer.push_front(east);
                    backtrace[east.first][east.second.first][east.second.second] = 'W';
                    placesDiscovered.push_back(east);
                }
                // 9. south/down { <color>, <row + 1>, <col> }
                if(!gotDiscovered(south) && isValidCoordinate(south)&& isWalkable(currentChar, map[south.second.first][south.second.second])){
                    //cout << "looked south from ";
                  //  printCoord(currentColor,currentRow,currentCol);
                    searchContainer.push_front(south);
                    backtrace[south.first][south.second.first][south.second.second] = 'N';
                    placesDiscovered.push_back(south);
                }
                //10. west/left { <color>, <row>, <col - 1> }
                if(!gotDiscovered(west) && isValidCoordinate(west) && isWalkable(currentChar, map[west.second.first][west.second.second])){
                    //cout << "looked west from ";
                   // printCoord(currentColor,currentRow,currentCol);
                    searchContainer.push_front(west);
                    backtrace[west.first][west.second.first][west.second.second] = 'E';
                    placesDiscovered.push_back(west);
                }
        }
       //when using stack: .back() and .pop_back()
        void solveStack(){
             //Your solution should faithfully implement the following algorithm:
            
            //1. Initially mark all states as not discovered.
            //done in the creation of Place

            //2. Add the initial state to the search_container and mark it as discovered.
            searchContainer.push_front(initialPosition);
            backtrace[initialPosition.first][initialPosition.second.first][initialPosition.second.second] = '@';
            placesDiscovered.push_back(initialPosition);
            char currentColor = '^';
            // 3. Loop while the search_container is NOT empty.
            while(!searchContainer.empty() && !solutionFound){
                // 4. Remove the “next” item from the search_container. This is the current_state, which consists of a 
                //<color> plus <row> and <col> coordinates { <color>, <row>, <col> }.
                currentState.push_front(searchContainer.front());
                searchContainer.pop_front();
                // 5. If current_state { <color>, <row>, <col> } is standing on an active button,
                // <button>, there is a chance to add a color change. 
                pair<uint32_t,pair<uint32_t,uint32_t>> curr = currentState.front();
                //cout << "currently at ";
                //printCoord(curr.first,curr.second.first,curr.second.second);
                if(isQuestionMark(map[curr.second.first][curr.second.second])){
                    solutionFound = true;
                    solvedPosition = curr;
                }
                else if(isButton(roomToChar(curr.first),map[curr.second.first][curr.second.second])){
                    //cout << "this is a button\n";
                    pair<uint32_t,pair<uint32_t,uint32_t>> colorChangedCurr = 
                        makeCoordinate(charToRoom(map[curr.second.first][curr.second.second])
                            , curr.second.first, curr.second.second);
                    if(!gotDiscovered(colorChangedCurr)){
                        //If { <button>, <row>, <col> } is not yet discovered, 
                        //add the color change to the search_container and mark it as discovered. 
                        searchContainer.push_front(colorChangedCurr);
                        backtrace[colorChangedCurr.first][colorChangedCurr.second.first][colorChangedCurr.second.second] 
                                = currentColor;
                        currentColor = roomToChar(colorChangedCurr.first);
                        placesDiscovered.push_back(colorChangedCurr);
                    }
                    //Investigation from an active button will result in zero or one states being discovered.
                }//case where undiscovered button
                else{
                    lookDirectionStack(curr);
                }
                currentState.pop_front();
            }
        }
        //solvePuzzle checks if there is a solution to either
        //the queue call or the stack call depending on the argument passed
        void solvePuzzle(){
            if(!stack){ 
                solveQueue();
            } //if queue
            else{
                solveStack();
            }//if stack

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
            for(uint32_t r = 0; r < rows; r++){
                for(uint32_t c = 0; c < cols; c++){
                    if(backtrace[charToRoom('^')][r][c] != ' '){
                        printCoord(0, r, c);
                    }
                }
            }
            //for all following cases
            for(char t = 'a'; t < 'a' + (int)numColors; t++){
                for(uint32_t r = 0; r < rows; r++){
                    for(uint32_t c = 0; c < cols; c++){
                        if(backtrace[charToRoom(t)][r][c] != ' '){
                            printCoord(charToRoom(t), r, c);
                        }
                    }
                }
            }
        }

        void pathFinder(){
            pair<uint32_t,pair<uint32_t,uint32_t>> currentPosition = solvedPosition;
            bool cameFromButton = false;
            while(currentPosition != initialPosition){
                uint32_t currColor = currentPosition.first;
                uint32_t currRow = currentPosition.second.first;
                uint32_t currColumn = currentPosition.second.second;
                if(currentPosition == solvedPosition){
                    if(backtrace[currColor][currRow][currColumn] == 'N'){
                        backtrace[currColor][currRow][currColumn] = '?';
                        currentPosition = makeCoordinate(currColor, currRow - 1, currColumn);
                    }
                    else if(backtrace[currColor][currRow][currColumn] == 'E'){
                        backtrace[currColor][currRow][currColumn] = '?';
                        currentPosition = makeCoordinate(currColor, currRow, currColumn + 1);
                    }
                    else if(backtrace[currColor][currRow][currColumn] == 'S'){
                        backtrace[currColor][currRow][currColumn] = '?';
                        currentPosition = makeCoordinate(currColor, currRow + 1, currColumn);
                    }
                    else if(backtrace[currColor][currRow][currColumn] == 'W'){
                        backtrace[currColor][currRow][currColumn] = '?';
                        currentPosition = makeCoordinate(currColor, currRow, currColumn - 1);
                    }
                }
                else if(isButtonChar(backtrace[currColor][currRow][currColumn])){
                    char roomCameFrom = backtrace[currColor][currRow][currColumn];
                    backtrace[currColor][currRow][currColumn] = '@';
                    currentPosition = makeCoordinate(charToRoom(roomCameFrom), currRow, currColumn);
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
                        else if(backtrace[currColor][currRow][currColumn] == 'W'){
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
                        else if(backtrace[currColor][currRow][currColumn] == 'W'){
                            backtrace[currColor][currRow][currColumn] = '+';
                            currentPosition = makeCoordinate(currColor, currRow, currColumn - 1);
                        }
                    }
                }
            }
        }
        bool isValidPrintingChar(char t){
            if(t != '@' && t != '+' && t != '%' && t != '?'){
                return false;
            }
            return true;
        }
        void printMapOutput(){
            cout << "// color ^\n";
            for(uint32_t r = 0; r < rows; r++){
                for(uint32_t c = 0; c < cols; c++){
                    char currentChar = backtrace[0][r][c];
                    if(map[r][c] == '^'){
                        cout << '.';
                    }
                    else if(currentChar == '\xff'){
                        cout << map[r][c];
                    }
                    else if((!isValidPrintingChar(currentChar) && isWalkable('^', map[r][c]))){
                        cout << '.';
                    } 
                    else if(!isValidPrintingChar(currentChar) && !isWalkable('^', map[r][c])){
                        cout << map[r][c];
                    }
                    else{
                        cout << currentChar;
                    }
                }
                cout << "\n";
            } //for default color
            for(char t = 'a'; t < 'a' + (int)numColors; t++){
                cout << "// color " << t << "\n";
                for(uint32_t r = 0; r < rows; r++){
                    for(uint32_t c = 0; c < cols; c++){
                        char currentChar = backtrace[charToRoom(t)][r][c];
                        if(map[r][c] == t){
                            cout << '.';
                        }
                        else if(currentChar == '\xff'){
                            cout << map[r][c];
                        }
                        else if(!isValidPrintingChar(currentChar) && isWalkable(t, map[r][c])){
                            cout << '.';
                        } 
                        else if(!isValidPrintingChar(currentChar) && !isWalkable(t, map[r][c])){
                            cout << map[r][c];
                        }
                        else{
                            cout << currentChar;
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

            uint32_t currR = 0;
            string line;

            while(getline(cin, line)){
                if(!isComment(line)){
                    map[currR] = (lineConvert(line));
                    currR++;
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
                pathFinder();
                if(listmode){
                     printListOutput();
                }
                else{
                     printMapOutput();
                }
            }
            //else print no solution + information to follow
            else{
                cout << "No solution.\nDiscovered:\n";
                rewriteMapNoSolution();
                printMap();
            }

        }

        void getMode(int argc, char * argv[]) {
            // cout << "argc: " << argc << "\n";
            // cout << "argv(s): ";
            // for (int i = 0; i < argc; i++){
            //     cout << "(" << i << ") " << argv[i] << "\n"; 
            // }
            // cout << "entered getMode\n";
            // These are used with getopt_long()
            opterr = false; // Let us handle all error output for command line options
            int choice = 0;
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
            bool outputMode = false;
            while ((choice = getopt_long(argc, argv, "hqso:", long_options, &index)) != -1) {
                switch (choice) {
                    case 'h':
                        printHelp(argv);
                        exit(0);
                    
                    case 's':
                        stack = true;
                        break;
                       
                    case 'q':
                        stack = false;
                        break;

                    case 'o': {  // Need a block here to declare a variable inside a case
                        string arg{optarg};
                        if (arg != "map" && arg != "list") {
                            // The first line of error output has to be a 'fixed' message
                            // for the autograder to show it to you.
                            cerr << "Error: invalid mode\n";
                            // The second line can provide more information, but you
                            // won't see it on the AG.
                            cerr << "  I don't recognize: " << arg <<
                            ". Please provide \"map\" or \"list\" instead\n";
                            exit(1);
                        }  // if ..arg valid
                        if (arg == "map") {
                            outputMode = true;
                            listmode = false;
                        } 
                        else {
                            outputMode = true;
                            listmode = true;
                        }
                        break;
                    }  // case 'o'
                    
                    default:
                        cout << "error case\n";
                        cerr << "Error: invalid option. See -h or --help for details\n";
                        exit(1);
                }  // switch ..choice
            }  // while
            if (!outputMode) {
                listmode = false;
            }  // if ..mode
        }  // getMode()

        // Print help for the user when requested.
        // argv[0] is the name of the currently executing program
        void printHelp(char *argv[]) {
            cout << "Usage: " << argv[0] << " [-o [map|list] | -q | -s | -h] < \"inputfile\" > \"outputfile\" \n";
            //create a more helpful print message
        }  // printHelp()
}; //end of PuzzleRunner Class