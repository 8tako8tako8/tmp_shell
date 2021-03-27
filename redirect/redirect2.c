#include <fcntl.h>

int main()
{
	int		tmpin = dup(0);
	int		tmpout = dup(1);
	char	infile = "testin";
	char	outfile = "testout";

	int		fdin;
	if (infile)
	{
		fdin = open(infile, O_RDONLY);
	}
	else
	{
		fdin = dup(tmpin);
	}
	/* 入力リダイレクトがある場合は、ファイルをinfileで開き、fdinに保存します。
	それ以外の場合、入力リダイレクトがない場合は、デフォルトの入力を参照するファイル記述子が作成されます。
	この命令ブロックの最後に、fdinは、コマンドラインの入力を持ち、
	親シェルプログラムに影響を与えることなく閉じることができるファイル記述子になります。 */
	int		ret;
	int		fdout;
	for (int i = 0; i < 3; i++)//コマンド数
	{
		dup2(fdin, 0);
		close(fdin);
		/* 標準入力をfdinからの入力にリダイレクトします。
		この後、stdinからの読み取りは、fdinが指すファイルから行われます。 */

		if (i == 2)
		{
			if (outfile)//上書き
			{
				fdout = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				//O_CREAT:ファイルが存在しなかった場合は作成 (create) する。
				//O_TRUNC:ファイルが既に存在し、通常ファイルであり、アクセスモードで書き込みが許可されている場合、長さ 0 に切り詰められる。
			}
			else
			{
				fdout = dup(tmpout);
			}
		}
		/* 「command> outfile」という形式の出力ファイルリダイレクトがあるかどうかをテストし、outfileを開いてfdoutに割り当てます。
		それ以外の場合、デフォルトの入力を指す新しいファイル記述子が作成されます。 */
		else
		{
			int		fdpipe[2];
			pipe(fdpipe);
			fdout = fdpipe[1];
			fdin = fdpipe[0];
		}
		/* これらの単純なコマンドの場合、出力はファイルではなくパイプになります。新しいパイプを作成します。
		新しいパイプ。パイプは、バッファを介して通信されるファイル記述子のペアです。
		ファイル記述子fdpipe [1]に書き込まれるものはすべて、fdpipe [0]から読み取ることができます。
		fdpipe [1]がfdoutに割り当てられ、fdpipe [0]がfdinに割り当てられています。 */

		dup2(fdout, 1);
		close(fdout);
		/* tdoutをリダイレクトして、fdoutが指すファイルオブジェクトに移動します。
		この行の後、stdinとstdoutはファイルまたはパイプのいずれかにリダイレクトされています。 */
		ret = fork();
		if (ret == 0)
		{
			exit(0);
		}
	}
	dup2(tmpin, 0);
	dup2(tmpout, 1);
	close(tmpin);
	close(tmpout);

	if (!(background))
	{
		waitpid(ret, NULL);
	}
}