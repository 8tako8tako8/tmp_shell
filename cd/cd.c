#include "cd.h"

void	ft_cd(t_env *env, char **args)
{
	char	*path;
	char	*old_pwd;
	char	*new_pwd;

	if (!args[1])//ホームディレクトリ
	{
		//homedirはsubject外？
		if (!(path = get_value_in_env(env, "HOME")))
		{
			printf("cd: HOME not set\n");
			exit(1);//エラーメッセージ and free必要
		}
	}
	else if (args[2])
	{
		printf("cd: too many arguments\n");//bashだとエラーにならないがzshだと出るので対応しておいた
		exit(1);//エラーメッセージ and free必要
	}
	else//ホームディレクトリ以外
	{
		path = ft_strdup(args[1]);
	}
	if (chdir(path) != 0)
	{
		printf("cd: no such file or directory:\n");
	}
	else
	{
		//新OLDPWD用にvalueを保管
		if (!(old_pwd = get_value_in_env(env, "PWD")))
			old_pwd = ft_strdup("");
		//新PWD用にgetcwdで得る
		new_pwd = ft_calloc(1024, sizeof(char));
		if (!(getcwd(new_pwd, 1024)))
			exit(1);//エラーメッセージ and free必要
		//PWDとOLDPWDの書き換え
		update_env_value(&env, "OLDPWD", old_pwd);
		update_env_value(&env, "PWD", new_pwd);
		free(old_pwd);
		free(new_pwd);
		free(path);
	}
}