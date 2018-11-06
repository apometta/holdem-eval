/*This file, written by Andrew H. Pometta, implements the PercentageToRange
class specified in percentage_to_range.h.  The Makefile for this project
has this file listed as a source for building: to build manually, ensure
this file is seen and linked to by the compiler.  For usage details, see
PercentageToRange.h.  */

#include "PercentageToRange.h" //has <utility>, <string> and <vector>
#include <algorithm> //lower_bound
#include <cassert>
#include <stdexcept>

/*Constructor: does nothing.  Since ranges is a static, it can't be defined
in the constructor.  It is hardcoded at the bottom of this file.  The
object itself, as a result, doesn't need to be instantiated with the keyword
new: PercentageToRange perctor; would be enough to do anything needed. */
PercentageToRange::PercentageToRange() {}

/*Given double input, return the necessary string.  See the method in
PercentageToRange.h for details and return value. */
std::string PercentageToRange::percentage_to_str(const double percentage){
  //First check for errors and edge cases
  if (percentage == 0) return "";
  else if (percentage < 0){
    throw std::string("negative percentage range inputted");
  } else if (percentage > 0 && percentage <= ranges.front().first){
    return ranges.front().second; //return AA
  }
  else if (percentage >= 100) return ranges.back().second; //return random

  //lower_bound gives the first element !< percentage, which is what we want
  //as an upper bound.  fourth argument is a lambda expression needed by
  //lower_bound to work between a double and a pair
  auto upper = std::lower_bound(ranges.begin(), ranges.end(), percentage,
  [](std::pair<double,std::string> p, double d){ return p.first < d; });
  //Since we know from above that percentage is greater than the first element
  //in the list, this is guaranteed to work, i.e. upper isn't ranges.begin()
  auto lower = upper - 1;
  if ((upper + 1) == ranges.end()){
    //percentage is <100% but greater than the next highest - return next
    //highest range, as per method documentation
    return (*lower).second; //random - 32o
  }
  //sanity check
  assert ((percentage > (*lower).first) && ((*upper).first >= percentage));

  //Note that the rounding that occurs here can differ from Pokerstove (where
  //the ranges were gotten from) at various percentages, most likely due to
  //Pokerstove taking into account the combination difference between each
  //interval.  However, these differences are unlikely to significantly matter.
  //If the user is so concerned about 0.09% making a one-hand difference in
  //the range affecting their equity calculation, they can input the range
  //manually
  if (((*upper).first - percentage) < (percentage - (*lower).first)){
    return (*upper).second;
  } else return (*lower).second;
}

/*Given string input, return the necessary string.  See the method in
PercentageToRange.h for details and return value. */
std::string PercentageToRange::percentage_to_str(const std::string percentage){
  //Given what's happening in holdem-eval, we know the string contains the %
  //symbol, and has no whitespace.  If the conversion succeeds but there is
  //anything other than % signs, we complain
  double d; std::size_t trail_pos;
  try {
    d = std::stod(percentage, &trail_pos);
    //We catch exceptions here because we don't "trust" the program calling
    //this method to necessarily do it themselves, but throw them as our
    //own errors to display the problem easily
  } catch (std::out_of_range oor){
    throw std::string("percentage range " + percentage + " out of range");
  } catch (std::invalid_argument ia){
    //This will occurr if there is anything before the number itself, e.g.
    //input "fdssa60%"
    throw std::string("percentage range " + percentage + " invalid");
  }

  std::string trail = percentage.substr(trail_pos); //what's leftover
  if (trail == "" || trail[0] != '%'){
    //Either there is no percentage sign in the string, or there is something
    //between the number converted and the percent sign, e.g. input
    //"60" or "60fdsa%"
    throw std::string("percentage range " + percentage + " invalid");
  }
  trail.erase(std::remove(trail.begin(), trail.end(), '%'), trail.end());
  if (trail != ""){
    //After removing the percentage signs, if there is anything leftover,
    //then we complain nonetheless: this is done to ensure accidental inputs
    //aren't counted.  e.g. input "60%fdsa"
    throw std::string("percentage range " + percentage + " invalid");
  }

  //Note that we do allow multiple percentage signs, e.g. input "60%%%" is
  //valid input.
  return percentage_to_str(d);
}

/*Initialize ranges here.  For details on why these values were used, see
the Readme.  For an explanation of how these were acquired, see
.range_string_acq.sh. */
const std::vector<std::pair<double,std::string>> PercentageToRange::ranges =
{
  std::make_pair(0.452489, "AA"),
  std::make_pair(0.904977, "KK+"),
  std::make_pair(1.35747, "QQ+"),
  std::make_pair(1.80995, "JJ+"),
  std::make_pair(2.26244, "TT+"),
  std::make_pair(2.5641, "TT+,AKs"),
  std::make_pair(3.01659, "99+,AKs"),
  std::make_pair(3.31825, "99+,AQs+"),
  std::make_pair(4.22323, "99+,AQs+,AKo"),
  std::make_pair(4.52489, "99+,AJs+,AKo"),
  std::make_pair(4.82655, "99+,AJs+,KQs,AKo"),
  std::make_pair(5.27903, "88+,AJs+,KQs,AKo"),
  std::make_pair(5.58069, "88+,ATs+,KQs,AKo"),
  std::make_pair(6.48567, "88+,ATs+,KQs,AQo+"),
  std::make_pair(6.78733, "88+,ATs+,KJs+,AQo+"),
  std::make_pair(7.08899, "88+,ATs+,KTs+,AQo+"),
  std::make_pair(7.39065, "88+,ATs+,KTs+,QJs,AQo+"),
  std::make_pair(8.29563, "88+,ATs+,KTs+,QJs,AJo+"),
  std::make_pair(9.2006, "88+,ATs+,KTs+,QJs,AJo+,KQo"),
  std::make_pair(9.50226, "88+,ATs+,KTs+,QTs+,AJo+,KQo"),
  std::make_pair(9.80392, "88+,A9s+,KTs+,QTs+,AJo+,KQo"),
  std::make_pair(10.2564, "77+,A9s+,KTs+,QTs+,AJo+,KQo"),
  std::make_pair(11.1614, "77+,A9s+,KTs+,QTs+,ATo+,KQo"),
  std::make_pair(11.463, "77+,A9s+,KTs+,QTs+,JTs,ATo+,KQo"),
  std::make_pair(12.368, "77+,A9s+,KTs+,QTs+,JTs,ATo+,KJo+"),
  std::make_pair(12.6697, "77+,A8s+,KTs+,QTs+,JTs,ATo+,KJo+"),
  std::make_pair(12.9713, "77+,A8s+,K9s+,QTs+,JTs,ATo+,KJo+"),
  std::make_pair(13.8763, "77+,A8s+,K9s+,QTs+,JTs,ATo+,KJo+,QJo"),
  std::make_pair(14.178, "77+,A7s+,K9s+,QTs+,JTs,ATo+,KJo+,QJo"),
  std::make_pair(15.083, "77+,A7s+,K9s+,QTs+,JTs,ATo+,KTo+,QJo"),
  std::make_pair(15.3846, "77+,A7s+,K9s+,Q9s+,JTs,ATo+,KTo+,QJo"),
  std::make_pair(15.6863, "77+,A7s+,A5s,K9s+,Q9s+,JTs,ATo+,KTo+,QJo"),
  std::make_pair(16.1388, "66+,A7s+,A5s,K9s+,Q9s+,JTs,ATo+,KTo+,QJo"),
  std::make_pair(16.4404, "66+,A5s+,K9s+,Q9s+,JTs,ATo+,KTo+,QJo"),
  std::make_pair(17.3454, "66+,A5s+,K9s+,Q9s+,JTs,ATo+,KTo+,QTo+"),
  std::make_pair(17.6471, "66+,A5s+,K9s+,Q9s+,J9s+,ATo+,KTo+,QTo+"),
  std::make_pair(18.552, "66+,A5s+,K9s+,Q9s+,J9s+,A9o+,KTo+,QTo+"),
  std::make_pair(18.8537, "66+,A5s+,K9s+,Q9s+,J9s+,T9s,A9o+,KTo+,QTo+"),
  std::make_pair(19.1554, "66+,A4s+,K9s+,Q9s+,J9s+,T9s,A9o+,KTo+,QTo+"),
  std::make_pair(19.457, "66+,A4s+,K8s+,Q9s+,J9s+,T9s,A9o+,KTo+,QTo+"),
  std::make_pair(20.362, "66+,A4s+,K8s+,Q9s+,J9s+,T9s,A9o+,KTo+,QTo+,JTo"),
  std::make_pair(20.6637, "66+,A4s+,K7s+,Q9s+,J9s+,T9s,A9o+,KTo+,QTo+,JTo"),
  std::make_pair(21.5686, "66+,A4s+,K7s+,Q9s+,J9s+,T9s,A8o+,KTo+,QTo+,JTo"),
  std::make_pair(21.8703, "66+,A3s+,K7s+,Q9s+,J9s+,T9s,A8o+,KTo+,QTo+,JTo"),
  std::make_pair(22.1719, "66+,A3s+,K7s+,Q8s+,J9s+,T9s,A8o+,KTo+,QTo+,JTo"),
  std::make_pair(23.0769, "66+,A3s+,K7s+,Q8s+,J9s+,T9s,A8o+,K9o+,QTo+,JTo"),
  std::make_pair(23.3786, "66+,A2s+,K7s+,Q8s+,J9s+,T9s,A8o+,K9o+,QTo+,JTo"),
  std::make_pair(23.6802, "66+,A2s+,K6s+,Q8s+,J9s+,T9s,A8o+,K9o+,QTo+,JTo"),
  std::make_pair(23.9819, "66+,A2s+,K6s+,Q8s+,J8s+,T9s,A8o+,K9o+,QTo+,JTo"),
  std::make_pair(24.2836, "66+,A2s+,K6s+,Q8s+,J8s+,T8s+,A8o+,K9o+,QTo+,JTo"),
  std::make_pair(25.1885, "66+,A2s+,K6s+,Q8s+,J8s+,T8s+,A7o+,K9o+,QTo+,JTo"),
  std::make_pair(25.641, "55+,A2s+,K6s+,Q8s+,J8s+,T8s+,A7o+,K9o+,QTo+,JTo"),
  std::make_pair(26.546, "55+,A2s+,K6s+,Q8s+,J8s+,T8s+,A7o+,K9o+,Q9o+,JTo"),
  std::make_pair(26.8477, "55+,A2s+,K6s+,Q8s+,J8s+,T8s+,98s,A7o+,K9o+,Q9o+,JTo"),
  std::make_pair(27.1493, "55+,A2s+,K5s+,Q8s+,J8s+,T8s+,98s,A7o+,K9o+,Q9o+,JTo"),
  std::make_pair(27.451, "55+,A2s+,K5s+,Q7s+,J8s+,T8s+,98s,A7o+,K9o+,Q9o+,JTo"),
  std::make_pair(28.356, "55+,A2s+,K5s+,Q7s+,J8s+,T8s+,98s,A7o+,K9o+,Q9o+,J9o+"),
  std::make_pair(29.2609, "55+,A2s+,K5s+,Q7s+,J8s+,T8s+,98s,A7o+,A5o,K9o+,Q9o+,J9o+"),
  std::make_pair(30.1659, "55+,A2s+,K5s+,Q7s+,J8s+,T8s+,98s,A7o+,A5o,K9o+,Q9o+,J9o+,T9o"),
  std::make_pair(31.0709, "55+,A2s+,K5s+,Q7s+,J8s+,T8s+,98s,A5o+,K9o+,Q9o+,J9o+,T9o"),
  std::make_pair(31.3725, "55+,A2s+,K4s+,Q7s+,J8s+,T8s+,98s,A5o+,K9o+,Q9o+,J9o+,T9o"),
  std::make_pair(32.2775, "55+,A2s+,K4s+,Q7s+,J8s+,T8s+,98s,A5o+,K8o+,Q9o+,J9o+,T9o"),
  std::make_pair(32.5792, "55+,A2s+,K4s+,Q6s+,J8s+,T8s+,98s,A5o+,K8o+,Q9o+,J9o+,T9o"),
  std::make_pair(32.8808, "55+,A2s+,K4s+,Q6s+,J7s+,T8s+,98s,A5o+,K8o+,Q9o+,J9o+,T9o"),
  std::make_pair(33.1825, "55+,A2s+,K4s+,Q6s+,J7s+,T7s+,98s,A5o+,K8o+,Q9o+,J9o+,T9o"),
  std::make_pair(34.0875, "55+,A2s+,K4s+,Q6s+,J7s+,T7s+,98s,A4o+,K8o+,Q9o+,J9o+,T9o"),
  std::make_pair(34.3891, "55+,A2s+,K4s+,Q6s+,J7s+,T7s+,97s+,A4o+,K8o+,Q9o+,J9o+,T9o"),
  std::make_pair(34.6908, "55+,A2s+,K3s+,Q6s+,J7s+,T7s+,97s+,A4o+,K8o+,Q9o+,J9o+,T9o"),
  std::make_pair(34.9925, "55+,A2s+,K3s+,Q6s+,J7s+,T7s+,97s+,87s,A4o+,K8o+,Q9o+,J9o+,T9o"),
  std::make_pair(35.2941, "55+,A2s+,K3s+,Q5s+,J7s+,T7s+,97s+,87s,A4o+,K8o+,Q9o+,J9o+,T9o"),
  std::make_pair(36.1991, "55+,A2s+,K3s+,Q5s+,J7s+,T7s+,97s+,87s,A4o+,K7o+,Q9o+,J9o+,T9o"),
  std::make_pair(36.6516, "44+,A2s+,K3s+,Q5s+,J7s+,T7s+,97s+,87s,A4o+,K7o+,Q9o+,J9o+,T9o"),
  std::make_pair(37.5566, "44+,A2s+,K3s+,Q5s+,J7s+,T7s+,97s+,87s,A4o+,K7o+,Q8o+,J9o+,T9o"),
  std::make_pair(38.4615, "44+,A2s+,K3s+,Q5s+,J7s+,T7s+,97s+,87s,A3o+,K7o+,Q8o+,J9o+,T9o"),
  std::make_pair(38.7632, "44+,A2s+,K2s+,Q5s+,J7s+,T7s+,97s+,87s,A3o+,K7o+,Q8o+,J9o+,T9o"),
  std::make_pair(39.6682, "44+,A2s+,K2s+,Q5s+,J7s+,T7s+,97s+,87s,A3o+,K7o+,Q8o+,J8o+,T9o"),
  std::make_pair(39.9698, "44+,A2s+,K2s+,Q4s+,J7s+,T7s+,97s+,87s,A3o+,K7o+,Q8o+,J8o+,T9o"),
  std::make_pair(40.8748, "44+,A2s+,K2s+,Q4s+,J7s+,T7s+,97s+,87s,A3o+,K7o+,Q8o+,J8o+,T8o+"),
  std::make_pair(41.1765, "44+,A2s+,K2s+,Q4s+,J6s+,T7s+,97s+,87s,A3o+,K7o+,Q8o+,J8o+,T8o+"),
  std::make_pair(42.0814, "44+,A2s+,K2s+,Q4s+,J6s+,T7s+,97s+,87s,A3o+,K6o+,Q8o+,J8o+,T8o+"),
  std::make_pair(42.9864, "44+,A2s+,K2s+,Q4s+,J6s+,T7s+,97s+,87s,A2o+,K6o+,Q8o+,J8o+,T8o+"),
  std::make_pair(43.2881, "44+,A2s+,K2s+,Q4s+,J6s+,T6s+,97s+,87s,A2o+,K6o+,Q8o+,J8o+,T8o+"),
  std::make_pair(44.1931, "44+,A2s+,K2s+,Q4s+,J6s+,T6s+,97s+,87s,A2o+,K6o+,Q8o+,J8o+,T8o+,98o"),
  std::make_pair(44.4947, "44+,A2s+,K2s+,Q4s+,J6s+,T6s+,97s+,87s,76s,A2o+,K6o+,Q8o+,J8o+,T8o+,98o"),
  std::make_pair(44.7964, "44+,A2s+,K2s+,Q4s+,J6s+,T6s+,97s+,86s+,76s,A2o+,K6o+,Q8o+,J8o+,T8o+,98o"),
  std::make_pair(45.098, "44+,A2s+,K2s+,Q4s+,J6s+,T6s+,96s+,86s+,76s,A2o+,K6o+,Q8o+,J8o+,T8o+,98o"),
  std::make_pair(45.3997, "44+,A2s+,K2s+,Q3s+,J6s+,T6s+,96s+,86s+,76s,A2o+,K6o+,Q8o+,J8o+,T8o+,98o"),
  std::make_pair(45.7014, "44+,A2s+,K2s+,Q3s+,J5s+,T6s+,96s+,86s+,76s,A2o+,K6o+,Q8o+,J8o+,T8o+,98o"),
  std::make_pair(46.6063, "44+,A2s+,K2s+,Q3s+,J5s+,T6s+,96s+,86s+,76s,A2o+,K5o+,Q8o+,J8o+,T8o+,98o"),
  std::make_pair(47.5113, "44+,A2s+,K2s+,Q3s+,J5s+,T6s+,96s+,86s+,76s,A2o+,K5o+,Q7o+,J8o+,T8o+,98o"),
  std::make_pair(47.813, "44+,A2s+,K2s+,Q2s+,J5s+,T6s+,96s+,86s+,76s,A2o+,K5o+,Q7o+,J8o+,T8o+,98o"),
  std::make_pair(48.1146, "44+,A2s+,K2s+,Q2s+,J4s+,T6s+,96s+,86s+,76s,A2o+,K5o+,Q7o+,J8o+,T8o+,98o"),
  std::make_pair(48.5671, "33+,A2s+,K2s+,Q2s+,J4s+,T6s+,96s+,86s+,76s,A2o+,K5o+,Q7o+,J8o+,T8o+,98o"),
  std::make_pair(48.8688, "33+,A2s+,K2s+,Q2s+,J4s+,T6s+,96s+,86s+,76s,65s,A2o+,K5o+,Q7o+,J8o+,T8o+,98o"),
  std::make_pair(49.7738, "33+,A2s+,K2s+,Q2s+,J4s+,T6s+,96s+,86s+,76s,65s,A2o+,K5o+,Q7o+,J7o+,T8o+,98o"),
  std::make_pair(50.6787, "33+,A2s+,K2s+,Q2s+,J4s+,T6s+,96s+,86s+,76s,65s,A2o+,K5o+,Q7o+,J7o+,T7o+,98o"),
  std::make_pair(51.5837, "33+,A2s+,K2s+,Q2s+,J4s+,T6s+,96s+,86s+,76s,65s,A2o+,K4o+,Q7o+,J7o+,T7o+,98o"),
  std::make_pair(51.8854, "33+,A2s+,K2s+,Q2s+,J4s+,T6s+,96s+,86s+,75s+,65s,A2o+,K4o+,Q7o+,J7o+,T7o+,98o"),
  std::make_pair(52.187, "33+,A2s+,K2s+,Q2s+,J4s+,T5s+,96s+,86s+,75s+,65s,A2o+,K4o+,Q7o+,J7o+,T7o+,98o"),
  std::make_pair(53.092, "33+,A2s+,K2s+,Q2s+,J4s+,T5s+,96s+,86s+,75s+,65s,A2o+,K4o+,Q6o+,J7o+,T7o+,98o"),
  std::make_pair(53.3937, "33+,A2s+,K2s+,Q2s+,J3s+,T5s+,96s+,86s+,75s+,65s,A2o+,K4o+,Q6o+,J7o+,T7o+,98o"),
  std::make_pair(53.6953, "33+,A2s+,K2s+,Q2s+,J3s+,T5s+,95s+,86s+,75s+,65s,A2o+,K4o+,Q6o+,J7o+,T7o+,98o"),
  std::make_pair(54.6003, "33+,A2s+,K2s+,Q2s+,J3s+,T5s+,95s+,86s+,75s+,65s,A2o+,K4o+,Q6o+,J7o+,T7o+,98o,87o"),
  std::make_pair(54.902, "33+,A2s+,K2s+,Q2s+,J3s+,T5s+,95s+,85s+,75s+,65s,A2o+,K4o+,Q6o+,J7o+,T7o+,98o,87o"),
  std::make_pair(55.8069, "33+,A2s+,K2s+,Q2s+,J3s+,T5s+,95s+,85s+,75s+,65s,A2o+,K4o+,Q6o+,J7o+,T7o+,97o+,87o"),
  std::make_pair(56.1086, "33+,A2s+,K2s+,Q2s+,J3s+,T4s+,95s+,85s+,75s+,65s,A2o+,K4o+,Q6o+,J7o+,T7o+,97o+,87o"),
  std::make_pair(57.0136, "33+,A2s+,K2s+,Q2s+,J3s+,T4s+,95s+,85s+,75s+,65s,A2o+,K3o+,Q6o+,J7o+,T7o+,97o+,87o"),
  std::make_pair(57.3152, "33+,A2s+,K2s+,Q2s+,J2s+,T4s+,95s+,85s+,75s+,65s,A2o+,K3o+,Q6o+,J7o+,T7o+,97o+,87o"),
  std::make_pair(57.6169, "33+,A2s+,K2s+,Q2s+,J2s+,T4s+,95s+,85s+,75s+,65s,54s,A2o+,K3o+,Q6o+,J7o+,T7o+,97o+,87o"),
  std::make_pair(58.5219, "33+,A2s+,K2s+,Q2s+,J2s+,T4s+,95s+,85s+,75s+,65s,54s,A2o+,K3o+,Q5o+,J7o+,T7o+,97o+,87o"),
  std::make_pair(58.8235, "33+,A2s+,K2s+,Q2s+,J2s+,T4s+,95s+,85s+,75s+,64s+,54s,A2o+,K3o+,Q5o+,J7o+,T7o+,97o+,87o"),
  std::make_pair(59.1252, "33+,A2s+,K2s+,Q2s+,J2s+,T3s+,95s+,85s+,75s+,64s+,54s,A2o+,K3o+,Q5o+,J7o+,T7o+,97o+,87o"),
  std::make_pair(59.5777, "22+,A2s+,K2s+,Q2s+,J2s+,T3s+,95s+,85s+,75s+,64s+,54s,A2o+,K3o+,Q5o+,J7o+,T7o+,97o+,87o"),
  std::make_pair(60.4827, "22+,A2s+,K2s+,Q2s+,J2s+,T3s+,95s+,85s+,75s+,64s+,54s,A2o+,K2o+,Q5o+,J7o+,T7o+,97o+,87o"),
  std::make_pair(60.7843, "22+,A2s+,K2s+,Q2s+,J2s+,T3s+,95s+,85s+,74s+,64s+,54s,A2o+,K2o+,Q5o+,J7o+,T7o+,97o+,87o"),
  std::make_pair(61.6893, "22+,A2s+,K2s+,Q2s+,J2s+,T3s+,95s+,85s+,74s+,64s+,54s,A2o+,K2o+,Q5o+,J7o+,T7o+,97o+,87o,76o"),
  std::make_pair(61.991, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,95s+,85s+,74s+,64s+,54s,A2o+,K2o+,Q5o+,J7o+,T7o+,97o+,87o,76o"),
  std::make_pair(62.8959, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,95s+,85s+,74s+,64s+,54s,A2o+,K2o+,Q4o+,J7o+,T7o+,97o+,87o,76o"),
  std::make_pair(63.8009, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,95s+,85s+,74s+,64s+,54s,A2o+,K2o+,Q4o+,J6o+,T7o+,97o+,87o,76o"),
  std::make_pair(64.1026, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,95s+,84s+,74s+,64s+,54s,A2o+,K2o+,Q4o+,J6o+,T7o+,97o+,87o,76o"),
  std::make_pair(64.4042, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,94s+,84s+,74s+,64s+,54s,A2o+,K2o+,Q4o+,J6o+,T7o+,97o+,87o,76o"),
  std::make_pair(65.3092, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,94s+,84s+,74s+,64s+,54s,A2o+,K2o+,Q4o+,J6o+,T7o+,97o+,86o+,76o"),
  std::make_pair(66.2142, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,94s+,84s+,74s+,64s+,54s,A2o+,K2o+,Q4o+,J6o+,T6o+,97o+,86o+,76o"),
  std::make_pair(67.1192, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,94s+,84s+,74s+,64s+,54s,A2o+,K2o+,Q4o+,J6o+,T6o+,96o+,86o+,76o"),
  std::make_pair(67.4208, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,94s+,84s+,74s+,64s+,53s+,A2o+,K2o+,Q4o+,J6o+,T6o+,96o+,86o+,76o"),
  std::make_pair(67.7225, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,93s+,84s+,74s+,64s+,53s+,A2o+,K2o+,Q4o+,J6o+,T6o+,96o+,86o+,76o"),
  std::make_pair(68.6275, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,93s+,84s+,74s+,64s+,53s+,A2o+,K2o+,Q3o+,J6o+,T6o+,96o+,86o+,76o"),
  std::make_pair(69.5324, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,93s+,84s+,74s+,64s+,53s+,A2o+,K2o+,Q3o+,J5o+,T6o+,96o+,86o+,76o"),
  std::make_pair(69.8341, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,93s+,84s+,74s+,63s+,53s+,A2o+,K2o+,Q3o+,J5o+,T6o+,96o+,86o+,76o"),
  std::make_pair(70.1357, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,93s+,84s+,74s+,63s+,53s+,43s,A2o+,K2o+,Q3o+,J5o+,T6o+,96o+,86o+,76o"),
  std::make_pair(70.4374, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,84s+,74s+,63s+,53s+,43s,A2o+,K2o+,Q3o+,J5o+,T6o+,96o+,86o+,76o"),
  std::make_pair(70.7391, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,84s+,73s+,63s+,53s+,43s,A2o+,K2o+,Q3o+,J5o+,T6o+,96o+,86o+,76o"),
  std::make_pair(71.644, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,84s+,73s+,63s+,53s+,43s,A2o+,K2o+,Q3o+,J5o+,T6o+,96o+,86o+,76o,65o"),
  std::make_pair(72.549, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,84s+,73s+,63s+,53s+,43s,A2o+,K2o+,Q2o+,J5o+,T6o+,96o+,86o+,76o,65o"),
  std::make_pair(73.454, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,84s+,73s+,63s+,53s+,43s,A2o+,K2o+,Q2o+,J4o+,T6o+,96o+,86o+,76o,65o"),
  std::make_pair(73.7557, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,83s+,73s+,63s+,53s+,43s,A2o+,K2o+,Q2o+,J4o+,T6o+,96o+,86o+,76o,65o"),
  std::make_pair(74.6606, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,83s+,73s+,63s+,53s+,43s,A2o+,K2o+,Q2o+,J4o+,T6o+,96o+,86o+,75o+,65o"),
  std::make_pair(74.9623, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,83s+,73s+,63s+,52s+,43s,A2o+,K2o+,Q2o+,J4o+,T6o+,96o+,86o+,75o+,65o"),
  std::make_pair(75.8673, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,83s+,73s+,63s+,52s+,43s,A2o+,K2o+,Q2o+,J4o+,T6o+,96o+,85o+,75o+,65o"),
  std::make_pair(76.1689, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,82s+,73s+,63s+,52s+,43s,A2o+,K2o+,Q2o+,J4o+,T6o+,96o+,85o+,75o+,65o"),
  std::make_pair(77.0739, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,82s+,73s+,63s+,52s+,43s,A2o+,K2o+,Q2o+,J4o+,T5o+,96o+,85o+,75o+,65o"),
  std::make_pair(77.9789, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,82s+,73s+,63s+,52s+,43s,A2o+,K2o+,Q2o+,J4o+,T5o+,95o+,85o+,75o+,65o"),
  std::make_pair(78.8839, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,82s+,73s+,63s+,52s+,43s,A2o+,K2o+,Q2o+,J3o+,T5o+,95o+,85o+,75o+,65o"),
  std::make_pair(79.1855, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,82s+,73s+,62s+,52s+,43s,A2o+,K2o+,Q2o+,J3o+,T5o+,95o+,85o+,75o+,65o"),
  std::make_pair(80.0905, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,82s+,73s+,62s+,52s+,43s,A2o+,K2o+,Q2o+,J3o+,T5o+,95o+,85o+,75o+,65o,54o"),
  std::make_pair(80.3922, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,82s+,73s+,62s+,52s+,42s+,A2o+,K2o+,Q2o+,J3o+,T5o+,95o+,85o+,75o+,65o,54o"),
  std::make_pair(81.2971, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,82s+,73s+,62s+,52s+,42s+,A2o+,K2o+,Q2o+,J3o+,T4o+,95o+,85o+,75o+,65o,54o"),
  std::make_pair(82.2021, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,82s+,73s+,62s+,52s+,42s+,A2o+,K2o+,Q2o+,J2o+,T4o+,95o+,85o+,75o+,65o,54o"),
  std::make_pair(82.5038, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,82s+,72s+,62s+,52s+,42s+,A2o+,K2o+,Q2o+,J2o+,T4o+,95o+,85o+,75o+,65o,54o"),
  std::make_pair(83.4087, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,82s+,72s+,62s+,52s+,42s+,A2o+,K2o+,Q2o+,J2o+,T4o+,95o+,85o+,75o+,64o+,54o"),
  std::make_pair(84.3137, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,82s+,72s+,62s+,52s+,42s+,A2o+,K2o+,Q2o+,J2o+,T3o+,95o+,85o+,75o+,64o+,54o"),
  std::make_pair(84.6154, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,82s+,72s+,62s+,52s+,42s+,32s,A2o+,K2o+,Q2o+,J2o+,T3o+,95o+,85o+,75o+,64o+,54o"),
  std::make_pair(85.5204, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,82s+,72s+,62s+,52s+,42s+,32s,A2o+,K2o+,Q2o+,J2o+,T3o+,95o+,85o+,74o+,64o+,54o"),
  std::make_pair(86.4253, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,82s+,72s+,62s+,52s+,42s+,32s,A2o+,K2o+,Q2o+,J2o+,T3o+,95o+,84o+,74o+,64o+,54o"),
  std::make_pair(87.3303, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,82s+,72s+,62s+,52s+,42s+,32s,A2o+,K2o+,Q2o+,J2o+,T2o+,95o+,84o+,74o+,64o+,54o"),
  std::make_pair(88.2353, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,82s+,72s+,62s+,52s+,42s+,32s,A2o+,K2o+,Q2o+,J2o+,T2o+,94o+,84o+,74o+,64o+,54o"),
  std::make_pair(89.1403, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,82s+,72s+,62s+,52s+,42s+,32s,A2o+,K2o+,Q2o+,J2o+,T2o+,94o+,84o+,74o+,64o+,53o+"),
  std::make_pair(90.0452, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,82s+,72s+,62s+,52s+,42s+,32s,A2o+,K2o+,Q2o+,J2o+,T2o+,93o+,84o+,74o+,64o+,53o+"),
  std::make_pair(90.9502, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,82s+,72s+,62s+,52s+,42s+,32s,A2o+,K2o+,Q2o+,J2o+,T2o+,93o+,84o+,74o+,63o+,53o+"),
  std::make_pair(91.8552, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,82s+,72s+,62s+,52s+,42s+,32s,A2o+,K2o+,Q2o+,J2o+,T2o+,93o+,84o+,74o+,63o+,53o+,43o"),
  std::make_pair(92.7602, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,82s+,72s+,62s+,52s+,42s+,32s,A2o+,K2o+,Q2o+,J2o+,T2o+,92o+,84o+,74o+,63o+,53o+,43o"),
  std::make_pair(93.6652, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,82s+,72s+,62s+,52s+,42s+,32s,A2o+,K2o+,Q2o+,J2o+,T2o+,92o+,84o+,73o+,63o+,53o+,43o"),
  std::make_pair(94.5701, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,82s+,72s+,62s+,52s+,42s+,32s,A2o+,K2o+,Q2o+,J2o+,T2o+,92o+,83o+,73o+,63o+,53o+,43o"),
  std::make_pair(95.4751, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,82s+,72s+,62s+,52s+,42s+,32s,A2o+,K2o+,Q2o+,J2o+,T2o+,92o+,83o+,73o+,63o+,52o+,43o"),
  std::make_pair(96.3801, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,82s+,72s+,62s+,52s+,42s+,32s,A2o+,K2o+,Q2o+,J2o+,T2o+,92o+,82o+,73o+,63o+,52o+,43o"),
  std::make_pair(97.2851, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,82s+,72s+,62s+,52s+,42s+,32s,A2o+,K2o+,Q2o+,J2o+,T2o+,92o+,82o+,73o+,63o+,52o+,42o+"),
  std::make_pair(98.19, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,82s+,72s+,62s+,52s+,42s+,32s,A2o+,K2o+,Q2o+,J2o+,T2o+,92o+,82o+,73o+,62o+,52o+,42o+"),
  std::make_pair(99.095, "22+,A2s+,K2s+,Q2s+,J2s+,T2s+,92s+,82s+,72s+,62s+,52s+,42s+,32s,A2o+,K2o+,Q2o+,J2o+,T2o+,92o+,82o+,72o+,62o+,52o+,42o+"),
  std::make_pair(100, "random"),
};
