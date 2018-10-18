/*This file, written by Andrew H. Pometta, is the main implementation file
for the term-heval program.  It is in the testing phase: at the moment, we are
simply getting proper option input running. */

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <getopt.h>
#include <stdexcept>
//#include "OMPEval-master/omp/EquityCalculator.h"
//using namespace omp;
using namespace std;

/*OPTIONS:
--debug: debug.  don't use.  prints debug information
-b, --board: board.  one string.  default empty
-d, --dead: dead.  one string  default empty.
--mc, --monte-carlo: mc simulation.  default false.
-e, --margin, --stderr: margin of error.  expects a double. default 0.01
-t, --time: maximum time to calculate for in seconds.  default 30,
            0 for infinite calculation (use at your own risk)
-h, --help: print help information
*/

const int NUM_THREADS = 0;  //max parallelism
const int CALLBACK_INTERVAL = 5; //call callback every 5 seconds
bool debug = false; //debug flag

void fail_prog(string err_report, int status = EXIT_FAILURE){
  cerr << "\nterm-heval: error: " << err_report << "." << endl;
  exit(status);
}

void debug_print(string debug_check, bool space_info = false){
  if (!debug) return;
  if (space_info) cout << endl;
  cout << debug_check << endl;
}

int main(int argc, char **argv){
  //EquityCalculator eq;
  string board = ""; string dead = "";
  bool monte_carlo = false;
  double err_margin = 0.01; double time_max = 30;
  //set the proper value of callback later, when we know time limit
  /*auto callback = [&eq](const EquityCalculator::Results& results){
    (void) results;
  };
*/
  //Option gathering
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
      //For checking if the string-input options (board, dead) are valid, we
      //do this when setting the string as a range.  There is no reason to do
      //it here.
      case '0':
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
      {         //"jumpt to case label" error
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
        try {
          time_max = stod(optarg);
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
  //settings values & final error checking

  //running equity calculator & printing results

  return EXIT_SUCCESS;
}
