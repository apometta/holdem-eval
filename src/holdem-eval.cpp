/*This file, written by Andrew H. Pometta, is the main implementation file
for the holdem-eval program.  It utilizes the OMPEval library to run poker
equity analysis on inputted ranges.  It is effectively a wrapper around that
library, for usage with a Unix terminal shell.  See README.md for more
information.  This is the main file for the program, which contains the main
function. */

#include <iostream>
#include <iomanip> //print formatting
#include <cstdlib>
#include <unistd.h> //getopt
#include <getopt.h>  //getopt_long
#include <stdexcept>
#include <cassert>
//#include <cmath> //round
#include "OMPEval/omp/EquityCalculator.h"
#include "PercentageToRange.h"
using namespace omp;
using namespace std;

string progname; //global scope to be accessed outside of main

/*Prints usage information for the program, to be used with -h.  Prints to
std::cerr by default, but can be changed with optional argument. */
void print_usage(ostream& outs = cerr){
  outs << "usage: " << progname << " [-ha] [--format] [--mc] [-b BOARD] "
       << "[-d DEAD] [-e ERROR] [-t TIME] range1 range2 [range3...]" << endl;
  outs << "\th: prints this help information and exits" << endl;
  outs << "\ta: print advanced statistics" << endl;
  outs << "\tformat: heavily abridges results printing" << endl;
  outs << "\tmc: enable monte-carlo evaluation" << endl;
  outs << "\tboard: the board cards (e.g. Th9s2c)" << endl;
  outs << "\tdead: the dead cards (e.g. Ad2s)" << endl;
  outs << "\te: margin of error, as proportion or percentage" << endl;
  outs << "\tt: maximum time for evaluation (0 for infinite)" << endl;
  outs << "\trange1, range2, etc.: range to be included in analysis" << endl;
  outs << "\tMaximum of 6 total ranges" << endl;
  outs << "\tRanges can be input in EquiLab/Pokerstove syntax";
  outs << " (e.g. QQ+,AKs) or as percentages (e.g. 1.3%)" << endl;
  outs << "See README.md for details" << endl;
}

/*Fails the program after printing specified error message.  Sets exit status,
which by default is EXIT_FAILURE. */
void fail_prog(string err_report, int status = EXIT_FAILURE,
               bool usage = false){
  cerr << progname << ": error: " << err_report << endl;
  if (usage) print_usage();
  exit(status);
}

/*Takes vector of given strings and returns necessary vector of hand ranges.
Does all error checking and fails out of program if errors are found.
A bad range is considered to be the empty range.  If maxlen is not a null
pointer, its contents become the length of the largest modified string. */
vector<CardRange> get_ranges_from_argv(vector<string>& range_strings,
                                       size_t *maxlen = nullptr){
  if (range_strings.size() < 2){
    fail_prog("less than 2 hand ranges", 5, true);
  }
  if (range_strings.size() > 6){
    fail_prog("more than 6 hand ranges", 5, true);
  }
  vector<CardRange> ranges;
  PercentageToRange perctor;
  vector<string> raw_strings;

  //create one vector for the raw strings to be used with EquityCalculator,
  //and another of formatted strings used in printing.
  for (auto i = range_strings.begin(); i != range_strings.end(); ++i){
    if ((*i).find("%") != string::npos){ //if the range is a percentage
      string converted_range;
      try {
        converted_range = perctor.percentage_to_str(*i);
      } catch (string e) { //PercentageToRange throws errors as a std::string
        fail_prog(e, 7, false);
      }
      raw_strings.push_back(converted_range);
      *i += " (" + converted_range + ")"; //used for printing
    }
    else raw_strings.push_back(*i);

    if (maxlen != nullptr && (*i).length() > *maxlen){
      *maxlen = (*i).length();
    }
  }

  //now go through and ensure all ranges are valid before adding them
  for (auto i = raw_strings.begin(); i != raw_strings.end(); ++i){
    CardRange *cr = new CardRange(*i);
    if (cr->combinations().empty()){
      //empty range, or range resulting from bad string.  fail out
      delete cr;
      fail_prog("range " + *i + " invalid", 6, false);
    }
    ranges.push_back(*cr);
    delete cr; //avoid memory leaks.  this doesn't compromise ranges
  }
  return ranges;
}

/*This function takes a string corresponding to a list of cards and returns
a "cardmask": uint64_t (large int) corresponding to that specific combination.
This is used for setting board and dead, and also checks for errors.
An optional boolean argument, board, is used when checking for the board,
which has stricter standards (i.e. card maximum).*/
uint64_t get_cardmask(string cards, bool board = false){
  //While a valid board with <=5 cards might have a string longer than 10
  //characters, it would only do so by accident (e.g. "Ts5cQh9s2d,,,,")
  //If this is the case, we can still return invalid board
  if (cards.length() > 10){
    fail_prog("invalid board argument " + cards, 1, false);
  }
  //We need to ensure that the string is valid, i.e. check the bitmask.
  uint64_t bitmask = CardRange::getCardMask(cards);
  if (bitmask == 0){
    if (board) fail_prog("invalid board argument " + cards, 1, false);
    else fail_prog("invalid dead argument " + cards, 1, false);
  }
  return bitmask;
}

int main(int argc, char **argv){
  progname = argv[0];
  //default values
  uint64_t board = 0; uint64_t dead = 0;
  bool monte_carlo = false;
  bool print_advanced_info = false; bool format_results = false;
  double err_margin = 1e-4; double time_max = 30;

  static struct option long_options[] = {
    {"board", required_argument, 0, 'b'},
    {"dead", required_argument, 0, 'd'},
    {"mc", no_argument, 0, 'm'},
    {"monte-carlo", no_argument, 0, 'm'}, //same as --mc
    {"margin", required_argument, 0, 'e'},
    {"stdev", required_argument, 0, 'e'}, //same as --margin
    {"time", required_argument, 0, 't'},
    {"help", no_argument, 0, 'h'},
    {"advanced", no_argument, 0, 'a'},
    {"format", no_argument, 0, 'f'},
    {0, 0, 0, 0} //required by getopt_long
  };
  int opt_character;
  while ((opt_character = getopt_long(argc, argv, "b:d:me:t:ha", long_options,
    nullptr)) != -1){
    switch(opt_character){
      case 'b':
        board = get_cardmask(optarg, true);
        break;
      case 'd':
        dead = get_cardmask(optarg, false);
        break;
      case 'm':
        monte_carlo = true;
        break;
      case 'e': //wrapping the case in brackets prevents
      {         //"jump to case label" error
        string cpp_err = optarg;
        string::size_type trail_pos;
        try {
          err_margin = stod(cpp_err, &trail_pos);
          string trail = cpp_err.substr(trail_pos);
          if ((trail != "") && (trail.at(0) == '%')){
            err_margin /= 100; //error margin inputted as a percentage
          }
        } catch (out_of_range oor) {
          fail_prog("Error margin " + cpp_err + " out of range", 2, false);
        } catch (invalid_argument ia) {
          fail_prog("Invalid error margin argument " + cpp_err, 2, false);
        }
        break;
      }
      case 't':
      {
        string cpp_time = optarg;
        try {
          time_max = stod(cpp_time);
        } catch (out_of_range oor) {
          fail_prog("Maximum time " + cpp_time + " out of range " +
                    "(use -t 0 for no time limit)", 2, false);
        } catch (invalid_argument ia) {
          fail_prog("Invalid maximum time argument " + cpp_time, 2, false);
        }
        break;
      }
      case 'h': //since this is the expected result, the usage printing
                //prints to cout
        print_usage(cout);
        exit(EXIT_SUCCESS);
        break;
      case 'a':
        print_advanced_info = true;
        break;
      case 'f':
        format_results = true;
        break;
      default:
        //getopt prints the error message for us
        //./holdem-eval: invalid option -- '(option)'
        print_usage(cerr);
        exit(4);
        break;
    }
  }
  //make sure running is non-infinite
  if (monte_carlo && (err_margin == 0) && (time_max == 0)){
    fail_prog("infinite simulation queried (set time limit, error margin"
              " or disable monte-carlo)", 3, false);
  }

  //put all remaining ranges into vector of strings
  vector<string> range_strs; //the strings passed into EquityCalculator
  for (int i = optind; i < argc; ++i) range_strs.push_back(argv[i]);

  //Set the ranges & final error checking.  Note that the vector of strings
  //is modified by get_ranges_from_argv so that the strings are viable for
  //printing.
  size_t range_str_max = 0;
  vector<CardRange> ranges = get_ranges_from_argv(range_strs, &range_str_max);

  //Run equity calculation.  monte-carlo is the default evaluation method
  //by the library, so we falsify our boolean
  EquityCalculator eq;
  eq.setTimeLimit(time_max);
  //Before we call eq.wait(), we make sure that eq doesn't just bail out on us
  //If start returns false, something went wrong
  if (!eq.start(ranges, board, dead, !monte_carlo, err_margin)){
    //There are a number of errors that could cause this, but with the ones
    //we've filtered out so far with our program, this can only be one thing:
    //A range conflict.  A dead card is in someone's range, on the board,
    //or the same card is in two people's hand/in a hand and on the board
    fail_prog("range conflict with dead, board, or other range", 8, false);
  }
  eq.wait();

  auto r = eq.getResults();
  bool completed = (r.progress >= 1);
  cout << fixed; cout.precision(2); //always 2 digits after the decimal

  if (format_results){
    if (completed) cout << "0" << endl;
    else cout << "1" << endl;

    for (unsigned int i = 0; i < r.players; ++i){
      cout << range_strs.at(i) << ": " << r.equity[i] * 100 << "%" << endl;
    }
    cout << endl; //blank line between equities and other information
    cout << "time: " << r.time;
    if (print_advanced_info){
      cout << "hands: " << r.hands;
      cout << "hands/s: " << r.speed;
    }
  }

  //printing results
  assert (range_strs.size() == r.players);
  cout << "Equity between " + to_string(r.players) + " players:" << endl;
  cout << "***" << endl;
  //we are traversing 2 lists at the same time, so we just use an int
  for (unsigned int i = 0; i < r.players; ++i){
    cout << setw(range_str_max) << range_strs.at(i) << setw(0);
    cout << ": " << r.equity[i] * 100 << "%" << endl;
  }
  cout << "***" << endl;

  if (completed){
    cout << "Calculation completed in " << r.time << " seconds." << endl;
  } else {
    cout << "Calculation timed out after " << r.time << " seconds." << endl;
    if (r.enumerateAll){
      cout << "Calculation progress: " << r.progress * 100 << "%." << endl;
      cout << "Consider using monte-carlo with --mc" << endl;
    } else { //we need more significant digits to print stdev
      cout.precision(6); //default precision
      cout << "Standard deviation: " << r.stdev * 100 << "%." << endl;
      cout.precision(2);
    }
  }

  if (print_advanced_info){
    cout << r.hands << " hands evaluated at " << r.speed << " hands/s."
         << endl;

    if (r.enumerateAll){
      double skipped_pfc =
        (static_cast<double>(r.skippedPreflopCombos) / r.preflopCombos) * 100;
      double showdown =
        (static_cast<double>(r.evaluations) / r.hands) * 100;

      cout << r.skippedPreflopCombos << " of " << r.preflopCombos << " ("
           << skipped_pfc << "%) preflop combinations skipped." << endl;

      cout << r.evaluations << " (" << showdown << "%) of hands reached "
           << "showdown." << endl;
    } else if (!r.enumerateAll && completed){
      cout.precision(6);
      cout << "Standard deviation: " << r.stdev << endl;
    }
  }

  return EXIT_SUCCESS;
}
