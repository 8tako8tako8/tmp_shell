#include "unset.h"

void    del_env(t_env **env, char *dstkey)
{
    t_env   *curr_env;
    t_env   *prev_env;

    curr_env = *env;
    prev_env = NULL;
    while (curr_env != NULL)
    {
        if (ft_strcmp(curr_env->key, dstkey) == 0)
        {
            if (prev_env == NULL)
                *env = curr_env->next;
            else
                prev_env->next = curr_env->next;
            free(curr_env->key);
            free(curr_env->value);
            free(curr_env);
        }
        prev_env = curr_env;
        curr_env = curr_env->next;
    }
}

void    ft_unset(t_env *env, char **args)
{
    int     i;

    i = 1;
    while (args[i])
    {
        del_env(&env, args[i]);
        i++;
    }
}