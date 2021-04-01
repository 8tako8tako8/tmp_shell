#include <termcap.h>
#include <stdio.h>
#include <unistd.h>

char	*tgetstr();
char	*tgoto();
char	*getenv();

extern char		*tgetstr(), *tgoto(), *getenv();
extern int		tgetent(), tgetflag(), tgetnum(), tputs();

int		putchar_tc(int tc)
{
	write(1, &tc, 1);
	return (0);
}

int		main()
{
	char	entrybuf[1024];
	char	stringbuf[1024];
	char	*bufptr;
	char	*term;
	char	*move;
	char	*clear;
	char	*s;

	if ((term = getenv("TERM")) == NULL)// getenvを使用して、TERMの設定を持って来る。
		term = "xterm";

	tgetent(entrybuf, term);                // xtermの設定を使用
	bufptr = stringbuf;
	move = tgetstr("cm", &bufptr);    		// cursor motion
	clear = tgetstr("cl", &bufptr);    	// clear line from cursor

	s = tgoto(move, 5, 5);
	tputs(s, 1, putchar_tc);
	//tputs(clear, 1, putchar_tc);
}
