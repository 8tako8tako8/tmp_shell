#include "env.h"

void    ft_env(t_env *env, char **args)
{
    if (args[1])
    {
        printf("Arguments are not supported\n");
        exit(1);//エラーメッセージ and free必要
    }
    while (env != NULL)
    {
        if (env->value != NULL)
            printf("%s=%s\n", env->key, env->value);
        env = env->next;
    }
}