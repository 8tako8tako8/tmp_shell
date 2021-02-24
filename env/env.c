#include "env.h"

void    ft_env(t_env *env)
{
    while (env != NULL)
    {
        if (env->value != NULL)
            printf("%s=%s\n", env->key, env->value);
        env = env->next;
    }
}