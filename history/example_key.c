#include <termios.h>
#include <unistd.h>
#include <stdio.h>

int		main(void)
{
	struct termios	term;// まず、端末の設定を受けてきて変更するためのtermios構造体を宣言する。

	// 端末オプションの制御
	tcgetattr(STDIN_FILENO, &term);// STDINの設定を受けてきて、lflagとc_ccを修正してくれる。変更された値は、tcsetattr（）を使用して反映させることができる。
	term.c_lflag &= ~ICANON;    // non-canonical input設定
	term.c_lflag &= ~ECHO;      // 入力時端子に見えないように
	term.c_cc[VMIN] = 1;        // 最小入力バッファサイズ
	term.c_cc[VTIME] = 0;       // バッファ空にする時間 (timeout)
	tcsetattr(STDIN_FILENO, TCSANOW, &term);

	/* non-canonical inputはVMINとVTIMEを持って入力処理の基準を設定するのに、
	VMINは最小入力バッファのサイズである。もし3の値に設定する場合は、3文字を入力頂いてから一番最初に入力されたことを処理する。
	VTIMEは、バッファを空にするtimeout時間である。設定された時間だけ入力がなければ、
	入力バッファにあった値を処理してくれる。今では一字ずつ、すぐに処理する必要がので、
	VMINとVTIMEをそれぞれ1と0に設定してくれる。 */

	int		c = 0;

	while (read(0, &c, sizeof(c)) > 0)// OSごとに異なりますが、64bit macOS / Linux環境で方向キーの場合、4バイトだけ読んでなければならない。
	{
		printf("keycode: %d\n", c);
        if (c == 4283163)
            printf("key: up\n");
        if (c == 4348699)
            printf("key: down\n");
        if (c == 4479771)
            printf("key: left\n");
        if (c == 4414235)
            printf("key: right\n");
		c = 0; // 読み使用した後にcを0に初期化して、バッファを空にする。
	}
}