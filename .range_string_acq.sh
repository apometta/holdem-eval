#!/usr/bin/env bash

: <<'INTRODUCTION'
This file, written by Andrew H. Pometta, is a bash script for inputting a
percentage to pokerstove, getting a string corresponding to the range it
represents, and outputting that string to a separate file.  It is not intended
to be run by the end user, or ever again: it is simply present to demonstrate
how these strings were acquired.  To repeat: DON'T RUN IT.  I have removed
execution permissions from this file before pushing with chmod.

The script assumes the following:
1. There is one window open that is pokerstove.
2. There is an empty text editor editting the target file
("range_strings.txt").  The file is currently empty.

It does some setting up, then does the following:
1. Opens the range window
2. Types in a percentage in the box, then clicks OK.
3. Selects all the text in the player one box, the string corresponding to the
range represented by the percentage we input.
4. Cuts the text, moves to the text editor, pastes it, and goes to the next
line.
5. Repeat for every line in the input percentages file
("percentages_rounded.txt").

Some questions to answer:

Q: Why use Pokerstove's percentages for the ranges?
A: There are other programs/utilities that have poker ranges corresponding to
a percentage.  pokerhandrange.com is a great one: it has 3 options based on
playing 1, 2 or 3 players, and prioritizes realized equity over raw equity the
more players there are (and accordingly adjusts the ranges).  It also has, in
the source code (press ctrl+u in your browser to see it) three arrays that
list each hand added to the range each time the marker goes up, which would
make it MUCH easier to find the string we need as opposed to doing it this way.
It's likely other utilities, such as various solvers, other equity calculators
or even other resources for poker learning have different percentage-to-range
conversions as well.

However, it is this difference that causes a problem: we can only use one.  In
making this terminal program, it is necessary to pick one.  They don't all
agree: other than stack sizes and number of opponents making a difference in
what range, for example, "40%" might mean, different resources might simply
have different opinions.  At 40%, for example, pokerstove would include K2s in
their open raising range, but NOT A2o.  This is not a decision everyone would
make.

Overall, I used pokerstove because of its ubiquity: everyone knows and uses it.
For that reason, I figured it would make a good authority, even if not everyone
agrees with it.

Q: Isn't there an easier way of doing this?
A: If we're using some other resource for converting percentages to range
strings, probably.  For using Pokerstove, probably not.  You might think there
is, but you'd be surprised how picky and annoying pokerstove is as a program.
A lot of the techniqes used in this script seem odd and unnecessarily
complicated, but if you try what seems simpler you will find something will
go wrong.

Q: Yes, but you could still do X instead of...
A: This only needs to be done once.  It isn't worth it to explore the most
efficient possible way once it is already done.  The script COULD be abstracted
to be run on other systems or make less assumptions than it does, but overall
it isn't necessary.  We have the text file of range strings: that's what we
came for.

Q: Why include this file?
A: In case anyone is curious how I got it.
INTRODUCTION

#set -x #enable (uncomment) to debug

#variables (can think of as "global") throughout the script
#window pids gotten from window name
pstove="$( xdotool search --sync --name "PokerStove*" )"
gedit="$( xdotool search --sync --name "range_strings" )"
#main window coordinates: all coordinates are relative, meaning based off of
#the window and not the screen as a whole
string="135 99" #where the  box is that the range is outputted
#clear_all="452 297" #the clear all button, in case we need it
player_one="41 101" #player one button
#preflop window coordinates (relative)
preflop="90 20" #preflop tab.  we use only once
#clear="456 195" #the clear button, in case we need it
percent="394 421" #input percentages
ok="375 501" #ok box
cancel="444 501" #cancel box

#Shorthand for sleeping a bit.  If we don't do this at certain times during
#the script, it will get ahead of itself and errors will occur.  Doing this
#ensures that the state of the script/computer is where we would expect.
delay () {
  sleep 0.12
}

#Clicks button on first window at specified next 2 coordinates.
click_b () {
  xdotool windowfocus $1
  xdotool mousemove --sync --window $1 $2 $3
  xdotool click 1
  delay
}

#Moves cursor to area, selects all text (highlights).  First argument is
#window pid, next 2 are coordinates.
select_all () {
  click_b $1 $2 $3
  #This might seem unnecessarily complicated, but both ctrl+a and double
  #clicking run into the same issues as with "cut_text" below.
  xdotool click 3
  xdotool key Down Down Down Down Down Down Return
  delay
}

#Function cuts the text on the given window at the next 2 given coordinates.
cut_text () {
  select_all $1 $2 $3
  #This may seem an unnecessarily roundabout way of doing it, but just trying
  #xdotool key ctrl+x was having issues, with and without the --clearmodifiers
  #option.  Pokerstove seems to not like it for some reason.  This works.
  xdotool click 3
  xdotool key Down Down Return
  delay
}

#Opens the range window and makes it the focus.  Also returns its pid for
#passing into other other functions
open_range_win () {
  click_b $pstove $player_one
  delay
  local range_win_pid=$( xdotool search --sync --name "Hold'em" )
  xdotool windowfocus $range_win_pid
  echo $range_win_pid
}

#: <<'END_COMMENT'
#setup environment
#Opens the range window and clicks on the "preflop" tab.  Every other time
#we open the window, the preflop tab will open by default.
range_win=$( open_range_win )
click_b $range_win $preflop
click_b $range_win $cancel
delay

#iterate through file
while read p; do
  #open preflop window, type percent, close it
  range_win=$( open_range_win )
  #click_b $range_win $clear
  select_all $range_win $percent
  xdotool type "$p"
  delay #clicking ok too fast will cause problems with pokerstove
  click_b $range_win $ok #could also press return - this is safer I think

  #slect text, copy, paste, repeat
  cut_text $pstove $string
  delay
  #It seems the text editor is not as "slow" as pokerstove, and can take
  #action without needing delays for every little thing.
  xdotool windowfocus $gedit
  xdotool key ctrl+v Return
  xdotool windowfocus $pstove
  delay
done <percentages_rounded.txt
