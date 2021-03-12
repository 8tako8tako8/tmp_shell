#include "export.h"

void		print_error_and_set_status(char *error_msg, int status)
{
	ft_putendl_fd(error_msg, 2);//print_error(, str, error_msg);
	//status = status;
}

void		print_error(char *cmd, char *args, char *error_msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	if (args)
	{
		ft_putstr_fd("`", 2);
		ft_putstr_fd(args, 2);
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

int			ft_strcmp(const char *s1, const char *s2)
{
	size_t  i;

	i = 0;
	while (s1[i] && (s1[i] == s2[i]))
		i++;
	return (s1[i] - s2[i]);
}

char		*get_cwd(void)
{
	char	*ret_pwd;

	ret_pwd = ft_calloc(1024, sizeof(char));
	if (!(getcwd(ret_pwd, 1024)))
		exit(1);//エラーメッセージ and free必要
	return (ret_pwd);
}

void		add_pwd_to_envlst(t_env **env)
{
	t_env	*new_env;
	char	*cwd;
	char	*tmp;

	cwd = get_cwd();
	new_env = create_new_envlst(tmp = ft_strjoin("PWD=", cwd));
	add_envlst_back(env, new_env);
	free(cwd);
	free(tmp);
}

void		add_oldpwd_to_envlst(t_env **env)
{
	t_env	*new_env;

	new_env = create_new_envlst("OLDPWD");
	add_envlst_back(env, new_env);
}

void		add_shlvl_to_envlst(t_env **env)
{
	t_env	*new_env;

	new_env = create_new_envlst("SHLVL=0");
	add_envlst_back(env, new_env);
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

void		add_envlst_back(t_env **env, t_env *new_env)
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

t_env	*create_new_envlst(char *str)
{
	t_env	*new_env;
	char	*equal_ptr;

	if (!(new_env = (t_env *)malloc(sizeof(t_env))))
		exit(1);
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

void		check_if_pwds_and_shlvl_exist(char *environ, int *pwd_flag, int *oldpwd_flag, int *shlvl_flag)
{
	if (!ft_strncmp("PWD=", environ, 4))
		*pwd_flag = 1;
	if (!ft_strncmp("OLDPWD=", environ, 7))
		*oldpwd_flag = 1;
	if (!ft_strncmp("SHLVL=", environ, 6))
		*shlvl_flag = 1;
}

static void		init_flags(int *pwd_flag, int *oldpwd_flag, int *shlvl_flag)
{
	*pwd_flag = 0;
	*oldpwd_flag = 0;
	*shlvl_flag = 0;
}

t_env			*create_envlst(void)
{
	t_env			*env;
	t_env			*new_env;
	extern char		**environ;
	int				i;
	int				pwd_flag;
	int				oldpwd_flag;
	int				shlvl_flag;

	init_flags(&pwd_flag, &oldpwd_flag, &shlvl_flag);
	env = NULL;
	i = -1;
	while (environ[++i])
	{
		check_if_pwds_and_shlvl_exist(environ[i], &pwd_flag, &oldpwd_flag, &shlvl_flag);
		new_env = create_new_envlst(environ[i]);
		add_envlst_back(&env, new_env);
	}
	if (!pwd_flag)
		add_pwd_to_envlst(&env);
	if (!oldpwd_flag)
		add_oldpwd_to_envlst(&env);
	if (!shlvl_flag)
		add_shlvl_to_envlst(&env);
	return (env);
}

void		print_env(t_env *env)
{
	while (env != NULL)
	{
		printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}

static void		free_envlst(t_env **env)
{
	t_env	*curr_lst;
	t_env	*tmp;

	curr_lst = *env;
	while (curr_lst != NULL)
	{
		free(curr_lst->key);
		curr_lst->key = NULL;
		if (curr_lst->value)
		{
			free(curr_lst->value);
			curr_lst->value = NULL;
		}
		tmp = curr_lst->next;
		free(curr_lst);
		curr_lst = NULL;
		curr_lst = tmp;
	}
}

int			main()
{
	t_env	*env;
	char	*args[] = {"export", "ccc=111", "ccc=", "ddd=111", "ccc", NULL};
	//char	*args[] = {"export", "ccc=1", "ccc=2", "ccc+=3", "ddd+=aaa", "eee=aaa", "eee+=", "fff", "fff+=aaa", NULL};
	//char	*args[] = {"export", "_ccc=1", "_cAB", "aB__=as", "a12+=aaa", NULL};
	//char	*args[] = {"export", "_", NULL};
	//char	*args[] = {"export", "1a=aaa", NULL};
	//char	*args[] = {"export", "+a=aaa", NULL};
	char	*args2[] = {"export", NULL};

	env = create_envlst();
	//print_env(env);
	ft_export(&env, args);
	ft_export(&env, args2);
	//print_env(env);
	free_envlst(&env);
	//system("leaks a.out");
}