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

	struct termios raw = orig_termios;	

	// Why does turning off IXON need a seperate line?
	// ICRNL translates carriage return '13' into new line '10'
	// IXON stops data from being sent to terminal when Ctrl-S is
	// ...pressed until Ctrl-Q is pressed
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

	// OPOST translates \n into \r\n
	raw.c_oflag &= ~(OPOST);

	raw.c_cflag |= (CS8);

	// ECHO causes each key to be printed to the terminal.
	// ICANON enables canonical mode. Turning it off will all to read 
	// ...input byte by byte rather than line by line to the program
	// IEXTEN allows for sending a character literally after pressing 
	// ...Ctrl-V
	// ISIG enables Ctrl-C and Ctrl-Z to send their signals to the 
	// ...process
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);


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
		// Added \r to printf() because OPOST was turned off
		if(iscntrl(c)){
			printf("%d\r\n",c);
		}
		else{
			printf("%d ('%c')\r\n",c,c);
		}
	}

	return 0;
}
