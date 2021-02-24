#include "set_env.h"

void        print_env(t_env *env)
{
    while (env != NULL)
    {
        printf("%s=%s\n", env->key, env->value);
        env = env->next;
    }
}

int         main()
{
    t_env   *env;
    
    env = set_envlst();
    print_env(env);
    //system("leaks a.out");
}