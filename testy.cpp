#include "OMPEval/omp/EquityCalculator.h"
#include <iostream>
#include <typeinfo>
using namespace omp;
using namespace std;

string range_combo_str(vector<array<uint8_t, 2>> range){
  string combos = "";
  for (auto i = range.begin(); i != range.end(); ++i){
    combos += to_string((*i)[0]) + " " + to_string((*i)[1]) + ", ";
  }
  return combos;
}
vector<CardRange> make_range(){
  vector<CardRange> ranges;
  CardRange *queens = new CardRange("AsAc,AsAd");
  ranges.push_back(*queens);
  delete queens;
  return ranges;
}
int main()
{
  vector<CardRange> ranges = make_range();
  cout << "in ranges " << range_combo_str(ranges[0].combinations()) << endl;
}
