#include "cd.h"

void	ft_cd(t_env *env, char **args)
{
	char	*path;
	char	*old_pwd;
	char	*new_pwd;

	if (!args[1] && !(path = get_value_in_env(env, "HOME")))//ホームディレクトリ
			return (print_error_and_set_status("cd: HOME not set", 1));
	else if (args[2])//bashだとエラーにならないがzshだと出るので対応しておいた
		return (print_error_and_set_status("cd: too many arguments", 1));
	else//ホームディレクトリ以外
		path = ft_strdup(args[1]);
	if (chdir(path) != 0)
		print_error_and_set_status("cd: no such file or directory", 1);
	else
	{
		if (!(old_pwd = get_value_in_env(env, "PWD")))//新OLDPWD用にvalueを保管
			old_pwd = ft_strdup("");
		if (!(new_pwd = getcwd(0, 0)))//新PWD用にgetcwdで得る
			print_error_and_exit();
		update_env_value(&env, "OLDPWD", old_pwd);//OLDPWDの書き換え
		update_env_value(&env, "PWD", new_pwd);//PWDの書き換え
		free(old_pwd);
		free(new_pwd);
		free(path);
		//status = 0
	}
}