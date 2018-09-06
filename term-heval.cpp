/*This file, written by Andrew H. Pometta, is the main implementation file
for the term-heval program.  It is in the testing phase: at the moment, we are
simply getting proper option input running. */

#include "OMPEval-master/omp/EquityCalculator.h"
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <getopt.h>
using namespace omp;
using namespace std;

/*OPTIONS:
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
  while ((opt_character = getopt_long(argc, argv, "b:d:me:t:h", long_options,
    nullptr)) != -1){
    switch(opt_character){
      //For checking if the string-input options (board, dead) are valid, we
      //do this when setting the string as a range.  There is no reason to do
      //it here.
      case 'b':
        board = optarg; //C++ string object has a constructor for C char*
        cout << "board = " << optarg << endl;
        break;
      case 'd':
        dead = optarg;
        cout << "dead = " << optarg << endl;
        break;
      case 'm':
        monte_carlo = true;
        cout << "monte_carlo = true" << endl;
        break;
      case 'e':
        err_margin = strtod(optarg, nullptr);
        if (err_margin == 0){
          //TO DO: HANDLE INVALID TIME INPUT
        }
        cout << "-e option received: " << optarg << endl;
        break;
      case 't':
        cout << "-t option received: " << optarg << endl;
        break;
      case 'h':
        cout << "-h option received" << endl;
        break;
    }
  }
  //settings values & final error checking

  //running equity calculator & printing results

  return EXIT_SUCCESS;
}
