#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

// Called when program is terminated to reset the terminal attributes.
void disableRawMode(){
	tcsetattr(STDIN_FILENO,TCSAFLUSH,&orig_termios);
}

void enableRawMode(){
	
	// Read the terminals attributes into a struct.
	tcgetattr(STDIN_FILENO,&orig_termios);

	// Register function disableRawMode() to be called at program end.
	atexit(disableRawMode);

	// Turn off ECHO manually within the struct.
	// ECHO causes each key to be printed to the terminal.
	// ICANON enables canonical mode. Turning it off will all to read 
	// input byte by byte rather than line by line.
	struct termios raw = orig_termios;	
	raw.c_lflag &= ~(ECHO | ICANON);

	// Apply the modified terminal attributes.
	tcsetattr(STDIN_FILENO,TCSAFLUSH,&raw);
}

int main(){
	enableRawMode();

	char c;

	// Read 1 byte from standard input into variable C.
	// Returns 0 when reached end of file or 'q' is reached
	// Characters left unread wil remain in the input queue
	while(read(STDIN_FILENO,&c,1) == 1 && c != 'q'){
	
		// Tests if a character is a control character
		if(iscntrl(c)){
			printf("%d\n",c);
		}
		else{
			printf("%d ('%c')\n",c,c);
		}
	}
	return 0;
}
