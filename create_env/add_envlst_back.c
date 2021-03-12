#include "create_env.h"

static t_env	*get_last_env(t_env *env)
{
	t_env	*curr_env;

	if (env == NULL)
		return (NULL);
	curr_env = env;
	while (curr_env->next != NULL)
		curr_env = curr_env->next;
	return (curr_env);
}

void			add_envlst_back(t_env **env, t_env *new_env)
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