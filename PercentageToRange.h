/*This file, written by Andrew H. Pometta, exports the PercentageToRange class,
which takes in the percentage passed into it by the omp-eval main
implementation file and gives back the string corresponding to the range
represented by that percentage.  It is implemented in PercentageToRange.cpp.

To use this class, simply create and run percentage_to_str on whatever
percentage to receive the string corresponding to the range.  The function is
overloaded, so you can input either a double or a string: just ensure that the
value input is a percentage and not the raw proportion (i.e. 45% instead of
0.45).  Exceptions are thrown as std::string.

Even though only one object of this class should ever be used, it is still a
good idea to use OOP to do it.  See the following:
https://en.wikipedia.org/wiki/Singleton_pattern
*/

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
  is returned.  However, if the percentage returned is exactly 0, than
  the empty string is returned.  If the percentage inputted is negative, then
  an exception is thrown as a std::string.

  When the percentage is not exactly equal to one corresponding to a stored
  one, we round it to the nearest one, with the exception of 100%.  If a
  percentage is to be rounded to 100%, the next lowest range is returned
  instead: "random" is never returned unless the inputted percentage is
  equal to or greater than 100%.  With an exact tie, return the lower range.*/
  std::string percentage_to_str(const double percentage);

  /*This method is the save as above, but takes in a string instead, and is
  responsible for performing the conversion from the string to a double.
  Exceptions are thrown as strings.  This also does it's own error checking
  for strings, so strings can be input straight from argv in the main
  program. */
  std::string percentage_to_str(const std::string percentage);

private:
    /*The vector of pairs, corresponding to a percentage and a string for the
    range. */
    static const std::vector<std::pair<double,std::string>> ranges;
};

#endif
