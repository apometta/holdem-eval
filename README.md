# holdem-eval

**holdem-eval** is an open-source, Unix terminal utility that performs poker hand analysis.  Effectively a wrapper around the [OMPEval library](https://github.com/zekyll/OMPEval), this program takes in poker hand ranges and a variety of relevant variables, and returns the equity of each range.  It was made for use in the HoldemEquityBot project: a Reddit bot that runs poker equity simulations.

* Supports complete enumeration or Monte Carlo simulation
* Maximum of 6 ranges
* Allows customization of board and dead cards, margin of error (for Monte Carlo), and time limits

While there are plenty of libraries for poker equity analysis, the only CLI program that I could find was the ps-eval portion of the much known [Pokerstove](https://github.com/andrewprock/pokerstove).  While the Windows GUI program is well loved, the Unix terminal version of the project, ps-eval, suffers from a large number of [issues](https://github.com/andrewprock/pokerstove/issues).  These include [non-acceptance of traditional Pokerstove hand range strings](https://github.com/andrewprock/pokerstove/issues/39), [inconsistent runtime](https://github.com/andrewprock/pokerstove/issues/38), and no Monte Carlo support.  It appears to still be in development.  Because my other project needs a CLI for poker equity calculations, I made my own with holdem-eval.
