# holdem-eval

**holdem-eval** is an open-source, Unix terminal utility that performs poker hand analysis.  Effectively a wrapper around the [OMPEval library](https://github.com/zekyll/OMPEval), this program takes in poker hand ranges and a variety of relevant variables, and returns the equity of each range.  It was made for use in the HoldemEquityBot project: a Reddit bot that runs poker equity simulations.

* Supports complete enumeration or Monte Carlo simulation
* Maximum of 6 ranges
* Allows customization of board and dead cards, margin of error (for Monte Carlo), and time limits

While there are plenty of libraries for poker equity analysis, the only CLI program that I could find was the ps-eval portion of the much known [Pokerstove](https://github.com/andrewprock/pokerstove).  While the Windows GUI program is well loved, the Unix terminal version of the project, ps-eval, suffers from a large number of [issues](https://github.com/andrewprock/pokerstove/issues).  These include [non-acceptance of traditional Pokerstove hand range strings](https://github.com/andrewprock/pokerstove/issues/39), [inconsistent runtime](https://github.com/andrewprock/pokerstove/issues/38), and no Monte Carlo support.  It appears to still be in development.  Because my other project needs a CLI for poker equity calculations, I made my own with holdem-eval.

## Getting Started

### Requirements

holdem-eval is intended to be run in Unix terminals.  The OMPEval library is required and included in this package.  In addition, this package is intended to be compiled with g++, which contains support for the C++11 standard and the pthread library to utilize multithreading.  Although g++ comes installed by default on most Unix systems, it can be installed with the command `sudo apt-get install g++`.

### Installation

On Unix systems, the holdem-eval executable can be built using the pre-included Makefile with the command `make`.  The program can be run using the created executable.  See [Usage](#Usage) for details on how to run the program.

## Usage

```bash
holdem-eval [-a] [--mc] [-b BOARD] [-d DEAD] [-e ERROR] [-t TIME] range1 range2 [range3...]
holdem-eval [-h]
```

holdem-eval takes in, at minimum 2 hand ranges.  It can take more after, up to 6.  The ranges can be input in a syntax understandable by other poker programs such as Pokerstove.  As an alternative to this, a percentage can also be input, which will be interpreted as the best percentage of preflop hand combinations someone can have according to Pokerstove.  For instance, range arguments `3.2% 9.5%` and `99+,AKs 88+,ATs+,KTs+,QJs,AJo+,KQo` are equivalent.  The argument `random` will be interpreted as any two cards, or 100%.  Note that an empty range is **not** valid, as the program will interpret it as an input error.  Options can be inserted before the ranges, and are defined as follows:

* **-h**: prints help information and exits the program.
* **-a**: prints advanced information when printing equity results.
* **--mc**, **--monte-carlo**: Enables Monte Carlo enumeration, as opposed to enumerating over every possibility.
* **-b**, **--board** BOARD: sets the board cards to be equal to BOARD.  The board cards are the cards already in play at the time of equity analysis.  There must be at least one and no more than 5.  Each individual card is specified *without commas* as one string by rank and suit.  Example: `-b TsJc2d`
* **-d**, **dead** DEAD: sets the dead cards equal to DEAD.  The dead cards are those known not to be in play, by not being on the board, in any player's hand or in the deck.  They are specified the same way as the board cards.  Example: `-d 3h9c`
* **-e**, **--margin**, **--stdev** ERROR: sets the target standard deviation to the specified ERROR, which must be a number.  Once the target is reached during Monte Carlo evaluation, the calculation is stopped.  The default is 0.01%, although this option does nothing unless **--mc** is enabled.  ERROR can be either a raw number or a percent: if it is a percent, it is converted to a number by dividing by 100.  For instance, `-e 0.002%`, `-e 2e-5` and `-e 0.00002` are all equivalent.  An argument of 0 means to continue evaluation until time runs out.
* **-t**, **--time** TIME: sets the maximum time allotted to the equity calculation in seconds.  If the calculation is not complete before the time limit, it is stopped, the current results are printed, and more useful information is printed below the results.  An argument of 0 means no time limit.

### Examples

#### Basic Example

```bash
$ #Assuming the holdem-eval executable file is in the current working directory (.)
$ ./holdem-eval 8h9h 10.3%
Equity between 2 players:
***
                               8h9h: 34.52%
10.3% (77+,A9s+,KTs+,QTs+,AJo+,KQo): 65.48%
***
Calculation completed in 0.08 seconds.
```

#### Advanced Example

```bash
$ ./holdem-eval -a --mc -b Ks5h2h -d 7d6d -e 0.003% -t 15 TT+,AJs+,KQs,AQo+ 55-22,A5s-A2s,QTs+,JTs,T9s,98s,QJo,JTo random
Equity between 3 players:
***
                     TT+,AJs+,KQs,AQo+: 12.67%
55-22,A5s-A2s,QTs+,JTs,T9s,98s,QJo,JTo: 81.27%
                                random: 6.06%
***
Calculation completed in 1.40 seconds.
155701248 hands evaluated at 111189178.89 hands/s.
Standard deviation: 0.000028
```

### Exit Status

The program's exit status will be set as follows:
* **0**: Success
* **1**: Invalid argument for BOARD or DEAD
* **2**: Invalid argument for ERROR or TIME
* **3**: Infinite simulation queried.  This occurs when `--mc`, `-e 0` and `-t 0` are all set, which would cause the program to never stop
* **4**: Invalid option
* **5**: Too many (>6) or too few (<2) hand ranges inputted
* **6**: Invalid range argument
* **7**: Invalid percentage range argument
* **8**: Range conflict.  This occurs when the requested situation is impossible due to a range being impossible.  For example, if someone's hand was set as `7c7d`, but the option `-d 9h7cJc` was used: it is impossible for the 7 of clubs to be both on the board and in someone's hand.
