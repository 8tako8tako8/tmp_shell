#include "create_env.h"

static char		*get_cwd(void)
{
	char	*ret_pwd;

	if (!(ret_pwd = getcwd(0, 0)))
		print_error_and_exit();
	return (ret_pwd);
}

void		    add_pwd_to_envlst(t_env **env)
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

void		    add_oldpwd_to_envlst(t_env **env)
{
	t_env	*new_env;

	new_env = create_new_envlst("OLDPWD");
	add_envlst_back(env, new_env);
}

void		    add_shlvl_to_envlst(t_env **env)
{
	t_env	*new_env;

	new_env = create_new_envlst("SHLVL=0");
	add_envlst_back(env, new_env);
}