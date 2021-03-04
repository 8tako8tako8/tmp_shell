#include "exec.h"

char	*extract_env_path(t_env *env)
{
	while (env != NULL)
	{
		if (ft_strcmp(env->key, "PATH") == 0)
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (NULL);
}

void	check_executable(struct stat *st)
{
	if ((st->st_mode & S_IXUSR) != S_IXUSR)//実行権限があるか？
	{
		printf("Permission denied\n");
		exit(1);
	}
}

int		is_executable_cmd(char *path)
{
	struct stat		st;

	if (lstat(path, &st) != 0)
		return (0);
	if (S_ISLNK(st.st_mode))//シンボリックリンクであるか？
	{
		if (stat(path, &st) == -1)
			exit(1);//エラーメッセージとfree
		if (S_ISDIR(st.st_mode))//ディレクトリでないか？
			return (0);
		check_executable(&st);
		return (1);
	}
	else if (S_ISREG(st.st_mode))//通常のファイルであるか？
	{
		check_executable(&st);
		return (1);
	}
	return (0);
}

char	*search_cmd(t_env *env, char *cmd)
{
	char	*ret_path;
	char	*env_path;
	char	**paths;
	char	*tmp;
	int		i;

	if (!(env_path = extract_env_path(env)))
		exit(1);//pathがNULLの時の動作未確認
	if (!(paths = ft_split(env_path, ':')))
		exit(1);//エラーメッセージ and free必要
	i = 0;
	while (paths[i])
	{
		if (!(tmp = ft_strjoin(paths[i], "/")))
			exit(1);//エラーメッセージ and free必要
		if (!(ret_path = ft_strjoin(tmp, cmd)))
			exit(1);//エラーメッセージ and free必要
		free(tmp);
		if (is_executable_cmd(ret_path))
			return (ret_path);
		i++;
	}
	return (NULL);
}
