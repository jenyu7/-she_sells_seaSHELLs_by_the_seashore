# she_sells_seaSHELLs_by_the_seashore
DWSH<br>
by JonAlf Dyrland-Weaver and Neeraj Bhatia

Features:<br>
	Forks and executes commands!<br>
	Parses multiple commands on one line!<br>
	Redirects using >, <!<br>
	Guaranteed to regrow hair!<br>
	Can summon a winged unicorn for fast & magical transit!<br>

Attempted:<br>
	The following did not end up working, but have been left in the code, commented out.<br>
	Could not get pipes to work<br>
	Tried to implement >>, but it kept overwriting the file<br>
	Looked at replacing ~/ with home directory, got seg faults<br>
	Was unable to have the "puppy" command produce a puppy and eject it from the optical drive.<br>

Bugs:<br>
	Putting two ;'s next to each other will break the parser<br>
	Redirecting to a file that does not exist occasionally does not work.<br>
	Hair regrowth function may result in a seg fault.<br>
	Occasionally, the unicorn command will fail, opening a great chasm in the earth and summoning the demon Beelzebub, who will proceeded to unleash his wrath and begin his reign of terror.<br>
	
Files & Function Headers:<br>
parse.c<br>
	Handles all line parsing fucntions<br>
	/*======== int count_tokens() ==========<br>
	Inputs:  char *line<br>
        	  char delim <br>
	Returns: Number of tokens in line separated by delim<br>

	Counts the number of times the character delim appears in the
	 string line
	The number of tokens is 1 more than the number of appearences
	of delim<br>
	If delim does not appear, 1 is returned<br>
	====================*/

	/*======== char ** parse_line() ==========
	Inputs:  char *line <br>
	Returns: Array of strings where each entry is a token<br>
	separated by delim<br>

	If line contains multiple tokens separated by delim, this <br>
	function will put each token into an array of strings<br>
	====================*/

	/*======== char * trim() ==========
	Inputs:  char *line <br>
	Returns: Pointer to the beginning of line<br>

	Removes leading and trailing whitespace on the string line.<br>
	Terminating '\0' is placed at a new location if necessary.<br>
	====================*/

dwsh.c<br>
	Handles the forking an executing of commands...<br>

magical.c<br>
UNOCORNS! <br>
