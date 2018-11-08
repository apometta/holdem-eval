# holdem-eval

**holdem-eval** is an open-source, Unix terminal utility that performs poker hand analysis.  Effectively a wrapper around the [OMPEval library](https://github.com/zekyll/OMPEval), this program takes in poker hand ranges and a variety of relevant variables, and returns the equity of each range.  It was made for use in the HoldemEquityBot project: a Reddit bot that runs poker equity simulations.

* Supports complete enumeration or Monte Carlo simulation
* Maximum of 6 ranges
* Allows customization of board and dead cards, margin of error (for Monte Carlo), and time limits

While there are plenty of libraries for poker equity analysis, the only CLI program that I could find was the ps-eval portion of the much known [Pokerstove](https://github.com/andrewprock/pokerstove).  While the Windows GUI program is well loved, the Unix terminal version of the project, ps-eval, suffers from a large number of [issues](https://github.com/andrewprock/pokerstove/issues).  These include [non-acceptance of traditional Pokerstove hand range strings](https://github.com/andrewprock/pokerstove/issues/39), [inconsistent runtime](https://github.com/andrewprock/pokerstove/issues/38), and no Monte Carlo support.  It appears to still be in development.  Because my other project needs a CLI for poker equity calculations, I made my own with holdem-eval.

##Getting Started

###Requirements

holdem-eval is intended to be run in Unix terminals.  The OMPEval library is required and included in this package.  In addition, this package is intended to be compiled with g++, which contains support for the C++11 standard and the pthread library to utilize multithreading.  Although g++ comes installed by default on most Unix systems, it can be installed with the command `sudo apt-get install g++`.

###Installation

On Unix systems, the holdem-eval executable can be built using the pre-included Makefile with the command `make`.  The program can be run using the created executable.  See [Usage](#Usage) for details on how to run the program.

##Usage

```bash
holdem-eval [-h]
holdem-eval [-a] [--mc] [-b board] [-d dead] [-e error] [-t time] range1 range2 [range3...]
```

holdem-eval takes in, at minimum 2 hand ranges.  It can take more after, up to 6.  The ranges can be input in a syntax understandable by other poker programs such as Pokerstove.
