#include "cd.h"

int     ft_strcmp(const char *s1, const char *s2)
{
    size_t  i;

    i = 0;
    while (s1[i] && (s1[i] == s2[i]))
        i++;
    return (s1[i] - s2[i]);
}

char    *get_value_in_env(t_env *env, char *dstkey)
{
    while (env != NULL)
    {
        if (ft_strcmp(env->key, dstkey) == 0)
            return (ft_strdup(env->value));
        env = env->next;
    }
    return (NULL);
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

int    is_symlink(char *path)
{
    struct stat     st;

    if (lstat(path, &st) != 0)
        return (0);
    if ((st.st_mode & S_IFMT) == S_IFLNK)
        return (1);
    return (0);
}

void        print_env(t_env *env)
{
    while (env != NULL)
    {
        printf("%s=%s\n", env->key, env->value);
        env = env->next;
    }
}

int     main()
{
    t_env   *env;
    char    *args[] = {
        "cd", NULL
    };
    env = set_envlst();
    ft_cd(env, args);

    print_env(env);
    //system("leaks a.out");
}