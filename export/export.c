#include "export.h"

void		ft_export(t_env **env, char **args)
{
	if (args[1] == NULL)
		ft_export_without_args(env);
	else
		ft_export_with_args(env, args);
}