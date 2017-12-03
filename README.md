# she_sells_seaSHELLs_by_the_seashore
DWSH<br>
by JonAlf Dyrland-Weaver and Neeraj Bhatia

## Features:<br>
* Forks and executes commands. 
* Parses multiple commands on one line. 
* Redirects using >, <.
	- Chaining redirections like ```ls > a > b > c``` works. 
* Simple piping, such as ```ls | wc``` works.
* Prints out shell prompt. 

## Attempted:
**The following did not end up working, but have been left in the code, commented out.**<br>
* Double redirection, like ```ls < a > b``` was not finished.

## Bugs:
* Putting two ;'s next to each other will break the parser<br>
* Redirecting to a file that does not exist occasionally does not work.<br>
	
## Files & Function Headers:
```fxn.c```<br>
**Contains all functions used within project** <br>

	/*======== int count_tokens() ==========
	Inputs:  char *line
        	  char delim 
	Returns: Number of tokens in line separated by delim

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

```dwsh.c```<br>
**Handles the sequence of running commands in the shell, by calling the functions in ```fxn.c```** <br>


