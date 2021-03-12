#include "create_env.h"

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

void	print_env(t_env *env)
{
	while (env != NULL)
	{
		printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}

int		main()
{
	t_env	*env;
	
	env = create_envlst();
	print_env(env);
	printf("\n");
	increment_shlvl(&env);
	print_env(env);
	
	//system("leaks a.out");
}