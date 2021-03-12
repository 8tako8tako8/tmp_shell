#include "create_env.h"

t_env	*create_new_envlst(char *str)
{
	t_env	*new_env;
	char	*equal_ptr;

	if (!(new_env = (t_env *)malloc(sizeof(t_env))))
		print_error_and_exit();
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

static void		check_if_pwds_and_shlvl_exist(char *environ, int *pwd_flag, int *oldpwd_flag, int *shlvl_flag)
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
		if ((environ[i][0] != '_') || (ft_strlen(environ[i]) != 1))
		{
			check_if_pwds_and_shlvl_exist(environ[i], &pwd_flag, &oldpwd_flag, &shlvl_flag);
			new_env = create_new_envlst(environ[i]);
			add_envlst_back(&env, new_env);
		}
	}
	if (!pwd_flag)
		add_pwd_to_envlst(&env);
	if (!oldpwd_flag)
		add_oldpwd_to_envlst(&env);
	if (!shlvl_flag)
		add_shlvl_to_envlst(&env);
	return (env);
}