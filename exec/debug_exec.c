#include "exec.h"

void		print_error_and_set_status(char *error_msg, int status)
{
	ft_putendl_fd(error_msg, 2);//print_error(, str, error_msg);
	//status = status;
}

void	print_error(char *cmd, char *arg, char *error_msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	if (arg)
	{
		ft_putstr_fd("`", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("'", 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd(error_msg, 2);
}

void	print_error_and_exit(void)
{
	ft_putendl_fd(strerror(errno), 2);
	exit(1);
}

int		ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && (s1[i] == s2[i]))
		i++;
	return (s1[i] - s2[i]);
}

void	free_two_dim_ary(char **dst)
{
	int		i;

	i = 0;
	while (dst[i])
	{
		free(dst[i]);
		dst[i] = NULL;
		i++;
	}
	free(dst);
	dst = NULL;
}

t_env	*create_new_envlst(char *str)
{
	t_env	*new_env;
	char	*equal_ptr;

	if (!(new_env = (t_env *)malloc(sizeof(t_env))))
		exit(1);//エラーメッセージ and free必要
	if (!(equal_ptr = ft_strchr(str, '=')))
	{
		new_env->key = ft_strdup(str);
		new_env->value = NULL;
		new_env->next = NULL;
	}
	else
	{
		new_env->key = ft_substr(str, 0, equal_ptr - str);
		new_env->value = ft_strdup(equal_ptr + 1);
		new_env->next = NULL;
	}
	return (new_env);
}

t_env	*get_last_env(t_env *env)
{
	t_env	*curr_env;

	if (env == NULL)
		return (NULL);
	curr_env = env;
	while (curr_env->next != NULL)
		curr_env = curr_env->next;
	return (curr_env);
}

void	add_envlst_back(t_env **env, t_env *new_env)
{
	t_env	*curr_env;

	if (!env || !new_env)
		return ;
	if (!*env)
	{
		*env = new_env;
		return ;
	}
	curr_env = get_last_env(*env);
	curr_env->next = new_env;
}

t_env	*create_envlst(void)
{
	t_env			*env;
	t_env			*new_env;
	extern char		**environ;
	int				i;

	env = NULL;
	i = 0;
	while (environ[i])
	{
		new_env = create_new_envlst(environ[i]);
		add_envlst_back(&env, new_env);
		i++;
	}
	return (env);
}

int		main(int argc, char *argv[])
{
	t_env	*env;
	char	*args[] = {"/bin/ls", NULL};
	//char	*args[] = {"/bin/ls", "-a", NULL};
	//char	*args[] = {"./hello", NULL};
	//char	*args[] = {"hello", NULL};
	//char	*args[] = {"ls", NULL};
	//char	*args[] = {"ls", "-a", NULL};
	//char	*args[] = {"cat", "exec.c", NULL};
	//char	*args[] = {"./s_hello", NULL};
	//char	*args[] = {"s_hello", NULL};

	env = create_envlst();
	exec_cmd(args, env);

	//system("leaks a.out");
}