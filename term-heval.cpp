/*This file, written by Andrew H. Pometta, is the main implementation file
for the term-heval program.  It is in the testing phase: at the moment, we are
simply getting proper option input running. */

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <getopt.h>
#include <stdexcept>
#include "OMPEval/omp/EquityCalculator.h"
using namespace omp;
using namespace std;

/*OPTIONS:
--debug: debug.  don't use.  prints debug information
-b, --board: board.  one string.  default empty
-d, --dead: dead.  one string  default empty.
--mc, --monte-carlo: mc simulation.  default false.
-e, --margin, --stderr: margin of error.  expects a double. default 0.1%
                        does nothing unless --mc is specified
-t, --time: maximum time to calculate for in seconds.  default 30,
            0 for infinite calculation (use at your own risk)
-h, --help: print help information
*/

const int NUM_THREADS = 0;  //max parallelism
const int CALLBACK_INTERVAL = 0.25; //call callback every 5 seconds
bool debug = false; //debug flag

/*Fails the program after printing specified error message.  Sets exit status,
which by default is EXIT_FAILURE. */
void fail_prog(string err_report, int status = EXIT_FAILURE){
  cerr << "\nterm-heval: error: " << err_report << "." << endl;
  exit(status);
}

/*Prints specified string if debug flag is specified, otherwise does nothing.
Can optionally add additional space before to separate important messages.*/
void debug_print(string debug_check, bool space_info = false){
  if (!debug) return;
  if (space_info) cout << endl;
  cout << debug_check << endl;
}

/*Takes vector of given strings and returns necessary vector of hand ranges.
Does all error checking and fails out of program if errors are found.
A bad range is considered to be the empty range. */
vector<CardRange> get_ranges_from_argv(const vector<string>& range_strings){
  if (range_strings.size() < 2) fail_prog("less than 2 hand ranges");
  if (range_strings.size() > 10) fail_prog("more than 10 hand ranges");
  vector<CardRange> ranges;
  for (auto i = range_strings.begin(); i != range_strings.end(); ++i){
    CardRange *cr = new CardRange(*i);
    if (cr->combinations().empty()){
      //empty range, or range resulting from bad string.  fail out
      fail_prog("range " + *i + " invalid");
    }
    ranges.push_back(*cr);
    delete cr; //avoid memory leaks
  }
  return ranges;
}

int main(int argc, char **argv){
  //Option gathering
  string board = ""; string dead = "";
  bool monte_carlo = false;
  double err_margin = 1e-3; double time_max = 30;
  static struct option long_options[] = {
    {"debug", no_argument, 0, '0'},
    {"board", required_argument, 0, 'b'},
    {"dead", required_argument, 0, 'd'},
    {"mc", no_argument, 0, 'm'},
    {"monte-carlo", no_argument, 0, 'm'}, //same as --mc
    {"margin", required_argument, 0, 'e'},
    {"stderr", required_argument, 0, 'e'}, //same as --margin
    {"time", required_argument, 0, 't'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
  };
  int opt_character;
  while ((opt_character = getopt_long(argc, argv, "0b:d:me:t:h", long_options,
    nullptr)) != -1){
    switch(opt_character){
      case '0': //if --debug is set, it should be set first.
        debug = true;
        debug_print("debug = true");
        break;
      case 'b':
        board = optarg; //C++ string object has a constructor for C char*
        debug_print("board = " + board);
        break;
      case 'd':
        dead = optarg;
        debug_print("dead = " + dead);
        break;
      case 'm':
        monte_carlo = true;
        debug_print("monte_carlo = " + to_string(monte_carlo));
        break;
      case 'e': //wrapping the case in brackets prevents
      {         //"jump to case label" error
        string cpp_err = optarg;
        try {
          err_margin = stod(cpp_err);
        } catch (out_of_range oor) {
          fail_prog("Error margin out of range");
        } catch (invalid_argument ia) {
          fail_prog("Invalid error margin argument");
        }
        debug_print("err_margin = " + to_string(err_margin));
        break;
      }
      case 't':
      {
        string cpp_time = optarg;
        try {
          time_max = stod(cpp_time);
        } catch (out_of_range oor) {
          fail_prog("Maximum time out of range (use -t 0 for no time limit)");
        } catch (invalid_argument ia) {
          fail_prog("Invalid maximum time argument");
        }
        debug_print("time_max = " + to_string(time_max));
        break;
      }
      case 'h':
        debug_print("Printing help info");
        break;
    }
  }
  //put all remaining ranges into vector of strings
  vector<string> range_strings;
  for (int i = optind; i < argc; ++i){
    range_strings.push_back(argv[i]);
  }
  debug_print("options parsed, remaing range strings acquired from argv.");

  //setting values & final error checking
  vector<CardRange> ranges = get_ranges_from_argv(range_strings);
  debug_print("get_ranges_from_argv successful.", true;);

  //running equity calculator & printing results

  return EXIT_SUCCESS;
}
