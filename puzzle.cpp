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


struct Coordinate{
    public:
    size_t color = 0;
    size_t row = 0;
    size_t col = 0;
    
};



class PuzzleRunner{
    private: 
        //number of colors past ^
        size_t numColors = 0;
        //number of rows in puzzle
        size_t rows = 0;
        //number of columns in puzzle
        size_t cols = 0;
        //original puzzle map (to perhaps be changed if no solution is found)
        vector<vector<char> > map;
        //whether puzzle can be solved or not
        bool solutionFound = false;
        //track directions, NESW
        vector<vector<vector<char> > > backtrace;
        //position where the initial @ is
        Coordinate initialPosition;
        //the position at which the ? was found
        Coordinate solvedPosition;
        //deque of everything contained in searchContainer
        deque<Coordinate> searchContainer;
        //deque of everything contained in the currentState
        deque<Coordinate> currentState;
        //a vector of all (color, row, column) that have been discovered
        deque<Coordinate> placesDiscovered;
        //bool stack: false if queue mode, true if stack mode
        bool stack = false;

        //if listmode false: print as map, else print as list
        bool listmode = false;

        void initializeBacktrace(){
            //initialize default layer
            for(size_t r = 0; r < rows; r++){
                for(size_t c = 0; c < cols; c++){
                    backtrace[0][r][c] = ' ';
                }
            }
            //initialize every further layer
            for(char t = 'a'; t < 'a' + (int)numColors; t++){
                for(size_t r = 0; r < rows; r++){
                    for(size_t c = 0; c < cols; c++){
                        backtrace[charToRoom(t)][r][c] = ' ';
                    }
                }
            }
        }
    public:
        bool coordinatesEqual(const Coordinate& lhs, const Coordinate& rhs){
            if(lhs.color == rhs.color
            && lhs.row == rhs.row
            && lhs.col == rhs.col){
                return true;
            }
            return false;
        }
        void setListMode(bool t){
            if(t){
                listmode = true;
            }
            else{
                listmode = false;
            }
        }
        void resizeMap(){
            map.resize(rows, vector<char>(cols, -1));
        }
        void resizeBacktrace(){
            backtrace.resize(numColors + 1, 
                    vector<vector<char> >(rows, vector<char>(cols, -1)));
        }
        bool isComment(const string &l){
            if(*(l.begin()) == '/'){
                return true;
            }
            return false;
        }

        void throwReadingColsError(){
            cerr << "ERROR, NUMBER OF COLS IN MAP DOES NOT MATCH\n";
            cerr << "NUMBER OF COLS IN INITIALIZATION\n";
        }

        void throwReadingRowsError(){
            cerr << "ERROR, NUMBER OF ROWS IN MAP DOES NOT MATCH\n";
            cerr << "NUMBER OF ROWS IN INITIALIZATION\n";
        }
        vector<char> lineConvert(string l){
            vector<char> toReturn;
            size_t numCols = 0;
            for(string::iterator it = l.begin(); it != l.end(); it++){
                toReturn.push_back(*it);
                numCols++;
            }
            if(numCols > cols){
                throwReadingColsError();
                exit(1);
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
        void throwNumberBoundError(){
            cerr << "INPUT FOR INITIALIZING NUMCOLORS IS INVALID!\n";
            cerr << "MUST BE BETWEEN 0 AND 26 INCLUSIVE\n";
        }
        void throwHeightError(){
            cerr << "INPUT FOR INITIALIZING HEIGHT (NUMBER OF ROWS) IS INVALID!\n";
            cerr << "MUST BE BETWEEN GREATER THAN OR EQUAL TO 1\n";
        }
        void throwWidthError(){
            cerr << "INPUT FOR INITIALIZING WIDTH (NUMBER OF COLS) IS INVALID!\n";
            cerr << "MUST BE BETWEEN GREATER THAN OR EQUAL TO 1\n";
        }
        void checkValidInitializing(){
            if(0 > (int)numColors || numColors > 26){
                throwNumberBoundError();
                exit(1);
            }
            if(1 > rows){
                throwHeightError();
                exit(1);
            }
            if(1 > cols){
                throwWidthError();
                exit(1);
            }
        }
        Coordinate makeCoordinate(const size_t &co, const size_t &r, const size_t &c){
            Coordinate newCord;
            newCord.color = co;
            newCord.row = r;
            newCord.col = c;
            return newCord;
        }
        /*
        converts a given tile (in char form) to the integer it matches
        */
        size_t charToRoom(char tile){
            if(tile == '^'){
                return 0;
            }
            return static_cast<size_t>(tile - 'a') + 1;
        }

        //converts a given roomColor (in int form) to the character it matches
        char roomToChar(size_t roomColor){
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

        void throwInvalidCharacterError(){
            cerr << "Invalid character found in map for one of two reasons:\n";
            cerr << "   -The character is an invalid door or button for the colors provided \n";
            cerr << "   -The character is not a valid map character\n";
        }

        void tooManyAts(){
            cerr << "More than 1 @ symbol was found\n";
        }
        void tooManyQuestions(){
            cerr << "More than 1 ? symbol was found\n";
        }
        //checks to ensure all characters in the map are valid
        void checkMapValidity(){
            size_t atCounter = 0;
            size_t questionCounter = 0;
            
            for(size_t r = 0; r < rows; r++){
                for(size_t c = 0; c < cols; c++){
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
                        throwInvalidCharacterError();
                        exit(1);
                    }
                }
            }
            if(atCounter > 1){
                tooManyAts();
                exit(1);
            }
            if(questionCounter > 1){
                tooManyQuestions();
                exit(1);
            }

        }
        

        //finds the @ in the map and sets the initialPosition struct to it
        void findInitialPosition(){
            size_t rowCounter = 0;
            for(auto &charVec:map){
                size_t colCounter = 0;
                for(vector<char>::iterator it = charVec.begin(); it != charVec.end(); it++){
                    if(isAtSymbol(*it)){
                        initialPosition = makeCoordinate(charToRoom('^'),rowCounter, colCounter);
                    }
                    colCounter++;
                }
                rowCounter++;
            }
        }


        bool existsInBacktrace(const Coordinate &curr){
            if(gotDiscovered(curr)){
                return true;
            }
            if(numColors > 0){
                for(char counter = 'a'; counter < 'a' + (int)numColors; counter ++){
                    if(gotDiscovered(makeCoordinate(charToRoom(counter), curr.row, curr.col))){
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
            size_t rowCounter = 0;
            for(auto &charVec:map){
                size_t columnCounter = 0;
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
                    cout << "\n";
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
        bool isValidCoordinate(const Coordinate &toWalkOn){
            size_t r = toWalkOn.row;
            size_t c = toWalkOn.col;
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
            if(comp == '^' && curr != '^'){
                return true;
            }
            for(char i = 'a'; i < 'a' + (int)numColors; i ++){
                if(comp == i){
                    found = true;
                }
            }
            return found;
        }
        void printCoord(const size_t &color, const size_t &row, const size_t &column){
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
        bool gotDiscovered(const Coordinate &curr){
            for(size_t i = 0; i < placesDiscovered.size(); i++){
                if(placesDiscovered[i].color == curr.color
                && placesDiscovered[i].row == curr.row
                && placesDiscovered[i].col == curr.col){
                    return true;
                }
            }
            return false;
        }

        //looks north, east, south, west, for a queue
        void lookDirectionQueue(const Coordinate &curr){
            size_t currentColor = curr.color;
            char currentChar = roomToChar(curr.color);
            size_t currentRow = curr.row;
            size_t currentCol = curr.col;
            Coordinate north = makeCoordinate(currentColor, currentRow - 1, currentCol);
            Coordinate east = makeCoordinate(currentColor, currentRow, currentCol + 1);
            Coordinate south = makeCoordinate(currentColor, currentRow + 1, currentCol);
            Coordinate west = makeCoordinate(currentColor, currentRow, currentCol - 1);
            //provided they are not off the edge of the map, not previously discovered,
            // and not a wall or closed door:
                // 7. north/up { <color>, <row - 1>, <col> }
                if(!gotDiscovered(north) && isValidCoordinate(north) && isWalkable(currentChar, map[north.row][north.col])){
                    //cout << "looked north from ";
                   // printCoord(currentColor,currentRow,currentCol);
                    searchContainer.push_back(north);
                    backtrace[north.color][north.row][north.col] = 'S';
                    placesDiscovered.push_back(north);
                }
                // 8. east/right { <color>, <row>, <col + 1> }
                if(!gotDiscovered(east) && isValidCoordinate(east) && isWalkable(currentChar, map[east.row][east.col])){
                    //cout << "looked east from ";
                   // printCoord(currentColor,currentRow,currentCol);
                    searchContainer.push_back(east);

                    backtrace[east.color][east.row][east.col] = 'W';
                    placesDiscovered.push_back(east);
                }
                // 9. south/down { <color>, <row + 1>, <col> }
                if(!gotDiscovered(south) && isValidCoordinate(south)&& isWalkable(currentChar, map[south.row][south.col])){
                    //cout << "looked south from ";
                  //  printCoord(currentColor,currentRow,currentCol);
                    searchContainer.push_back(south);
                    backtrace[south.color][south.row][south.col] = 'N';
                    placesDiscovered.push_back(south);
                }
                //10. west/left { <color>, <row>, <col - 1> }
                if(!gotDiscovered(west) && isValidCoordinate(west) && isWalkable(currentChar, map[west.row][west.col])){
                    //cout << "looked west from ";
                   // printCoord(currentColor,currentRow,currentCol);
                    searchContainer.push_back(west);
                    backtrace[west.color][west.row][west.col] = 'E';
                    placesDiscovered.push_back(west);
                }
        }


        //when using queue: .front() and .pop_front()
        void solveQueue(){
            //Your solution should faithfully implement the following algorithm:
            
            //1. Initially mark all states as not discovered.

            //2. Add the initial state to the search_container and mark it as discovered.
            searchContainer.push_back(initialPosition);
            backtrace[initialPosition.color][initialPosition.row][initialPosition.col] = '@';
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
                Coordinate curr = currentState.front();
                // Coordinate trigger;
                // trigger.color = charToRoom('b');
                // trigger.row = 14;
                // trigger.col = 16;
                // if(curr.color == trigger.color
                // && curr.row == trigger.row
                // && curr.col == trigger.col){
                //     cout << "helloooooo";
                // }
                if(isQuestionMark(map[curr.row][curr.col])){
                    solutionFound = true;
                    solvedPosition = curr;
                }
                else if(isButton(roomToChar(curr.color),map[curr.row][curr.col])){
                    //cout << "\n(" << roomToChar(curr.color) << ", " << curr.row << ", " << curr.col << ") is a button\n";
                    Coordinate colorChangedCurr = 
                        makeCoordinate(charToRoom(map[curr.row][curr.col])
                            , curr.row, curr.col);
                    if(!gotDiscovered(colorChangedCurr)){
                        //If { <button>, <row>, <col> } is not yet discovered, 
                        //add the color change to the search_container and mark it as discovered. 
                        searchContainer.push_back(colorChangedCurr);
                        backtrace[colorChangedCurr.color][colorChangedCurr.row][colorChangedCurr.col] 
                                = currentColor;
                        currentColor = roomToChar(colorChangedCurr.color);
                        placesDiscovered.push_back(colorChangedCurr);
                    }
                    //Investigation from an active button will result in zero or one states being discovered.
                }//case where undiscovered button
                else{
                    lookDirectionQueue(curr);
                }
               //cout << "\n current state: (" << roomToChar(curr.color) << ", " << curr.row << ", " << curr.col << ")\n";
                currentState.pop_front();
            }
        }


        //looks north, east, south, west, for a stack
        void lookDirectionStack(const Coordinate &curr){
            size_t currentColor = curr.color;
            char currentChar = roomToChar(curr.color);
            size_t currentRow = curr.row;
            size_t currentCol = curr.col;
            Coordinate north = makeCoordinate(currentColor, currentRow - 1, currentCol);
            Coordinate east = makeCoordinate(currentColor, currentRow, currentCol + 1);
            Coordinate south = makeCoordinate(currentColor, currentRow + 1, currentCol);
            Coordinate west = makeCoordinate(currentColor, currentRow, currentCol - 1);
            //provided they are not off the edge of the map, not previously discovered,
            // and not a wall or closed door:
                // 7. north/up { <color>, <row - 1>, <col> }
                if(!gotDiscovered(north) && isValidCoordinate(north) && isWalkable(currentChar, map[north.row][north.col])){
                    //cout << "looked north from ";
                   // printCoord(currentColor,currentRow,currentCol);
                    searchContainer.push_front(north);
                    backtrace[north.color][north.row][north.col] = 'S';
                    placesDiscovered.push_back(north);
                }
                // 8. east/right { <color>, <row>, <col + 1> }
                if(!gotDiscovered(east) && isValidCoordinate(east) && isWalkable(currentChar, map[east.row][east.col])){
                    //cout << "looked east from ";
                   // printCoord(currentColor,currentRow,currentCol);
                    searchContainer.push_front(east);
                    backtrace[east.color][east.row][east.col] = 'W';
                    placesDiscovered.push_back(east);
                }
                // 9. south/down { <color>, <row + 1>, <col> }
                if(!gotDiscovered(south) && isValidCoordinate(south)&& isWalkable(currentChar, map[south.row][south.col])){
                    //cout << "looked south from ";
                  //  printCoord(currentColor,currentRow,currentCol);
                    searchContainer.push_front(south);
                    backtrace[south.color][south.row][south.col] = 'N';
                    placesDiscovered.push_back(south);
                }
                //10. west/left { <color>, <row>, <col - 1> }
                if(!gotDiscovered(west) && isValidCoordinate(west) && isWalkable(currentChar, map[west.row][west.col])){
                    //cout << "looked west from ";
                   // printCoord(currentColor,currentRow,currentCol);
                    searchContainer.push_front(west);
                    backtrace[west.color][west.row][west.col] = 'E';
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
            backtrace[initialPosition.color][initialPosition.row][initialPosition.col] = '@';
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
                Coordinate curr = currentState.front();
                //cout << "currently at ";
                //printCoord(curr.first,curr.row,curr.col);
                if(isQuestionMark(map[curr.row][curr.col])){
                    solutionFound = true;
                    solvedPosition = curr;
                }
                else if(isButton(roomToChar(curr.color),map[curr.row][curr.col])){
                    //cout << "this is a button\n";
                    Coordinate colorChangedCurr = 
                        makeCoordinate(charToRoom(map[curr.row][curr.col])
                            , curr.row, curr.col);
                    if(!gotDiscovered(colorChangedCurr)){
                        //If { <button>, <row>, <col> } is not yet discovered, 
                        //add the color change to the search_container and mark it as discovered. 
                        searchContainer.push_front(colorChangedCurr);
                        backtrace[colorChangedCurr.color][colorChangedCurr.row][colorChangedCurr.col] 
                                = currentColor;
                        currentColor = roomToChar(colorChangedCurr.color);
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
        bool isListChar(const char &comp){
            if(comp == '+' || comp == '@' || comp == '%' || comp == '?'){
                return true;
            }
            return false;
        }
        void printListOutput(){
            //for default case
            for(Coordinate r = placesDiscovered.front(); !coordinatesEqual(r,placesDiscovered.back()); placesDiscovered.pop_front()){
                r = placesDiscovered.front();
                if(isListChar(backtrace[r.color][r.row][r.col])){
                    printCoord(r.color,r.row,r.col);
                }
            }
            // //for all following cases
            // for(char t = 'a'; t < 'a' + (int)numColors; t++){
            //     for(size_t r = 0; r < rows; r++){
            //         for(size_t c = 0; c < cols; c++){
            //             if(isListChar(backtrace[charToRoom(t)][r][c])){
            //                 printCoord(charToRoom(t), r, c);
            //             }
            //         }
            //     }
            // }
        }


        void pathFinder(){
            Coordinate currentPosition = solvedPosition;
            bool cameFromButton = false;
            while(currentPosition.color != initialPosition.color 
            || currentPosition.row != initialPosition.row
            || currentPosition.col != initialPosition.col){
                size_t currColor = currentPosition.color;
                size_t currRow = currentPosition.row;
                size_t currColumn = currentPosition.col;
                if(currentPosition.color == solvedPosition.color 
                && currentPosition.row == solvedPosition.row
                && currentPosition.col == solvedPosition.col){
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

        bool isValidPrintingChar(const char &t){
            if(t != '@' && t != '+' && t != '%' && t != '?'){
                return false;
            }
            return true;
        }

        bool existsAtForColor(const size_t &current){
            for(size_t r = 0; r < rows; r++){
                for(size_t c = 0; c < cols; c++){
                    if(backtrace[current][r][c] == '@'){
                        return true;
                    }
                }
            }
            return false;
        }

        void printMapOutput(){
            cout << "// color ^\n";
            for(size_t r = 0; r < rows; r++){
                for(size_t c = 0; c < cols; c++){
                    char currentChar = backtrace[0][r][c];
                    if(map[r][c] == '^' && !isValidPrintingChar(currentChar)){
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
                for(size_t r = 0; r < rows; r++){
                    for(size_t c = 0; c < cols; c++){
                        char currentChar = backtrace[charToRoom(t)][r][c];
                        if(currentChar == t){
                            cout << '.';
                        }
                        else if(!isValidPrintingChar(currentChar) && map[r][c] == '@' && existsAtForColor(charToRoom(t))){
                            cout << '.';
                        }
                        else if(currentChar == '\xff'){
                            cout << map[r][c];
                        }
                        else if(!isValidPrintingChar(currentChar) && isButtonChar(map[r][c])){
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

            size_t currR = 0;
            string line;

            while(getline(cin, line)){
                if(!isComment(line)){
                    map[currR] = (lineConvert(line));
                    currR++;
                }
            }
            if(currR > rows){
                throwReadingRowsError();
                exit(1);
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

        void throwNoStackQueueError(){
            cerr << "Neither --stack/-s nor --queue/-q has been provided in the argument\n";
            cerr << "please correct and try again\n";
        }

        void throwStackQueueError(){
            cerr << "Both --stack/-s and --queue/-q have been provided in the argument\n";
            cerr << "please correct and try again\n";
        }
        void getMode(int argc, char * argv[]) {
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
            bool stackTriggered = false;
            bool queueTriggered = false;
            //indicates whether a stack or queue has been selected
            bool readerMode = false;
            while ((choice = getopt_long(argc, argv, "hqso:", long_options, &index)) != -1) {
                switch (choice) {
                    case 'h':
                        printHelp(argv);
                        exit(0);
                    
                    case 's':
                        readerMode = true;
                        stackTriggered = true;
                        stack = true;
                        break;
                       
                    case 'q':
                        readerMode = true;
                        queueTriggered = true;
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
                        cerr << "Error: invalid command line provided\n";
                        cerr << "See -h or --help for details\n";
                        exit(1);
                }  // switch ..choice
            }  // while
            if((stackTriggered==true) && (queueTriggered==true)){
                throwStackQueueError();
                exit(1);
            }
            if(!readerMode){
                throwNoStackQueueError();
                exit(1);
            }
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