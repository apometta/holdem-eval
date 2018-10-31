/*This file, written by Andrew H. Pometta, exports the PercentageToRange class,
which takes in the percentage passed into it by the term-heval main
implementation file and gives back the string corresponding to the range
represented by that percentage.

To do: fill header comment with information. */

#ifndef PERCENTAGE_TO_RANGE_H
#define PERCENTAGE_TO_RANGE_H

#include <utility> //pairs
#include <string>
#include <vector>

class PercentageToRange {
public:
  /*Constructor for initializing data.  Bear in mind it loads a 169 element
  hardcoded array of pairs.  If this is expensive, avoid calling unless
  needed. */
  PercentageToRange();

  /*This method receives a request in the form of a double, which should be
  the percentage (NOT PROPORTION - i.e. the proportion multiplied by 100)
  of hands needed, and returns a string corresponding to the range.  If a
  percentage greater than 100 is given, the range corresponding to 100% of
  hands is returned.  If a percentage less than the lowest indexed percentage
  of hands is given (i.e. 0.1%, when AA is ~0.45%), the smallest range (AA)
  is returned.  However, if the percentage returned is 0, than
  the empty string is returned.  If the percentage inputted is negative, then
  an exception is thrown. */
  double percentage_to_str(double percentage);

  /*This method is the save as above, but takes in a string instead, and is
  responsible for performing the conversion from the string to a double.
  More exceptions are thrown if the conversion somehow fails. */
  std::string percentage_to_str(std::string percentage);

  /*Prints the ranges vector.  Use for debugging only, to be removed from
  final product. */
  void print_ranges();

private:
    //the data itself
    static const std::vector<std::pair<double,std::string>> ranges;
    //add more later if needed

};

#endif
