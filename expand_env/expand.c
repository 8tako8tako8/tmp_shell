#include "expand.h"

char    *get_value_in_env(t_env *env, char *dstkey)
{
    while (env != NULL)
    {
        if (ft_strcmp(env->key, dstkey) == 0)
            return (env->value);
        env = env->next;
    }
    return (NULL);
}
