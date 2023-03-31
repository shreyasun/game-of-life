## "The Game of Life" 
"The Game of Life," implements the Game of Life in C. I have completed this project as an assignment for CSE 13S in UCSC in Winter 2023. I have received instruction and help from several tutors and TA's. While their names are not listed within the code for privacy purposes, I have indicated what I received instruction and/or code on. 

## If you are a CSE 13S student, please do not look at my source code!

The Game of Life is played on a 2D grid of cells that represents a universe. Each cell within the grid is either dead or alive. The game progresses through generations, and after each generation, a live cell with 2 or 3 live neighbors survives; Any dead cell with exactly 3 live neighbors becomes a live cell; All other cells die (because of loneliness or overcrowding).

## Build
Type 'make', 'make all', or 'make life' on the command line within the Assignment 4 'asgn4' directory. This will create an executable binary file called 'life' which can then be run. This command will also compile and create object files for the different sorting files within this directory.
 
## Cleaning
Type 'make clean' to remove the executable binary file 'life' and all of the .o files from the Assignment 4 directory.

## Run Options
The life executable accepts a variety of command line arguments. Typing './life' will yield instructions and options information, and prompt the user to enter file data due to standard input. The same thing will occur when typing in './life -h'. (If the user does not want to enter data with standard input, they should be able to press Ctrl+C to exit and be able to enter other commands to the command line). Typing './life' followed by the '-t' command will specify a toroidal universe (with the default being a nontoroidal universe). Typing './life' followed by an '-s' command will silence ncurses and not play ncurses animation for the program on the screen (the default would be that the ncurses animation will play). Typing './life' followed by an '-i {filename}' will accept a file to read from, while otherwise it would default to reading from standard input. Typing './life' followed by an '-o {filename}' will accept a file to write to, while otherwise it would write to standard output. Typing './life' followed by '-n {generations}' would specify the number of generations the user would like the Game of Life to be played, and yield output of the universe at that final generation (the default is 100 generations).

