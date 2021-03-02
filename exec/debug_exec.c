#include "exec.h"

int     ft_strcmp(const char *s1, const char *s2)
{
    size_t  i;

    i = 0;
    while (s1[i] && (s1[i] == s2[i]))
        i++;
    return (s1[i] - s2[i]);
}

void    free_two_dim_ary(char **dst)
{
    int     i;

    i = 0;
    while (dst[i])
    {
        free(dst[i]);
        dst[i] = NULL;
        i++;
    }
    free(dst);
    dst = NULL;
}

t_env       *create_new_envlst(char *str)
{
    t_env	*new_env;
    char    *equal_ptr;

    if (!(new_env = (t_env *)malloc(sizeof(t_env))))
        exit(1);
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

t_env       *get_last_env(t_env *env)
{
    t_env   *curr_env;

    if (env == NULL)
        return (NULL);
    curr_env = env;
    while (curr_env->next != NULL)
        curr_env = curr_env->next;
    return (curr_env);
}

void        add_envlst_back(t_env **env, t_env *new_env)
{
    t_env   *curr_env;

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

t_env        *set_envlst(void)
{
    t_env           *env;
    t_env           *new_env;
    extern char     **environ;
    int             i;

    env = NULL;
    i = 0;
    while (environ[i])
    {
        new_env = create_new_envlst(environ[i]);
        add_envlst_back(&env, new_env);
        i++;
    }
    return (env);
}

int     main(int argc, char *argv[])
{
    t_env   *env;
    char	*args[] = {"/bin/ls", NULL};
    //char	*args[] = {"/bin/ls", "-a", NULL};
    //char	*args[] = {"./hello", NULL};
    //char	*args[] = {"hello", NULL};
    //char	*args[] = {"ls", NULL};
    //char	*args[] = {"ls", "-a", NULL};
    //char	*args[] = {"cat", "exec.c", NULL};

    env = set_envlst();
    exec_cmd(args, env);

    //system("leaks a.out");
}