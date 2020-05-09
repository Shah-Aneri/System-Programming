/* xsh_hello.c - xsh_hello */
#include <xinu.h>
#include <string.h>
#include <stdio.h>


/*------------------------------------------------------------------------
 * xsh_hello - Displays the Hello Message on the screen
 *------------------------------------------------------------------------
 */


shellcmd xsh_hello(int nargs, char *args[]) {

/* If more arguments are passed, then throws error message*/

if (nargs > 2) {
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
			args[0]);
		return 1;
	}

/* If less arguments are passed, then throws error message*/
if (nargs < 2) {
		fprintf(stderr, "%s: too less arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
			args[0]);
		return 1;
	}

/* Prints the message passed to it on the screen */
if(nargs==2){
	fprintf(stderr,"%s: Hello  %s, Welcome to the world of Xinu!!!\n",args[0],args[1]);
	return 1;
}
}