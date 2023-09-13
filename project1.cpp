// Project Identifier: 39E995D00DDE0519FDF5506EED902869AEC1C39E

// EECS 281, Project 1

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
