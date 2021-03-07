#include "cd.h"

void	ft_pwd(char **args)
{
	char	*new_pwd;

	if (args[1])
	{
		printf("Arguments are not supported\n");
		exit(1);//エラーメッセージ and free必要
	}
	new_pwd = ft_calloc(1024, sizeof(char));
	if (!(getcwd(new_pwd, 1024)))
		exit(1);//エラーメッセージ and free必要
	printf("%s\n", new_pwd);
	free(new_pwd);
}