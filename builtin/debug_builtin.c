#include "builtin.h"

int     ft_strcmp(const char *s1, const char *s2)
{
    size_t  i;

    i = 0;
    while (s1[i] && (s1[i] == s2[i]))
        i++;
    return (s1[i] - s2[i]);
}

t_env       *create_new_envlst(char *str)
{
	t_env	*new_env;
    char    *equal_ptr;

	if (!(new_env = (t_env *)malloc(sizeof(t_env))))
		exit(1);
    equal_ptr = ft_strchr(str, '=');
    new_env->key = ft_substr(str, 0, equal_ptr - str);
    new_env->value = ft_strdup(equal_ptr + 1);
	new_env->next = NULL;
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

    i = 0;
    while (environ[i])
    {
        new_env = create_new_envlst(environ[i]);
        add_envlst_back(&env, new_env);
        i++;
    }
    return (env);
}

void    ft_echo(void)
{
    printf("call ft_echo\n");
}

void    ft_cd(void)
{
    printf("call ft_cd\n");
}

void    ft_pwd(void)
{
    printf("call ft_pwd\n");
}

void    ft_export(void)
{
    printf("call ft_export\n");
}

void    ft_unset(void)
{
    printf("call ft_unset\n");
}

void    ft_env(void)
{
    printf("call ft_env\n");
}

void    ft_exit(void)
{
    printf("call ft_exit\n");
}

int     main(int argc, char *argv[])
{
    t_env   *env;
    char	*args[] = {
        argv[1], NULL
    };

    if (argc == 1)
        return 0;

    env = set_envlst();
    exec_cmd(args, env);

    //system("leaks a.out");
}