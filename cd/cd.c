#include "cd.h"

void    update_env(t_env **env, char *dstkey, char *dstvalue)
{
    t_env   *curr_env;
    char    *tmp;

    if (*env == NULL)
        return ;
    curr_env = *env;
    while (curr_env != NULL)
    {
        if (ft_strcmp(curr_env->key, dstkey) == 0)
        {
            tmp = curr_env->value;
            curr_env->value = ft_strdup(dstvalue);
            if (tmp != NULL)
                free(tmp);
        }
        curr_env = curr_env->next;
    }
}

void    ft_cd(t_env *env, char **args)
{
    char    *path;
    char    *old_pwd;
    char    *new_pwd;

    if (!args[1])//ホームディレクトリ
    {
        //homedirはsubject外？
        if (!(path = get_value_in_env(env, "HOME")))
        {
            printf("cd: HOME not set\n");
            return ;
        }
    }
    else if (ft_strcmp(args[1], "~") == 0)//HOMEをunsetした時の挙動は対応不可？
    {
        printf("want to move home dir\n");
        exit(0);
    }
    else if (args[2])
    {
        printf("cd: too many arguments\n");
        exit(1);
    }
    else//ホームディレクトリ以外
    {
        path = ft_strdup(args[1]);
    }
    if (chdir(path) != 0)
    {
        printf("cd: no such file or directory:\n");
    }
    else
    {
        //新OLDPWD用にvalueを保管(unsetされているとNULLになるので、別にPWD,OLDPWDを保管しておいた方がいいかも)
        old_pwd = get_value_in_env(env, "PWD");
        //絶対パスならそのまま、相対パスなら旧PWDと組み合わせてパス作り
        if (path[0] == '/')
            new_pwd = ft_strdup(path);
        else
            new_pwd = get_path_after_moving(old_pwd, path);//getcwdだとシンボリックリンクを取得できず
        //PWDとOLDPWDの書き換え
        update_env(&env, "OLDPWD", old_pwd);
        update_env(&env, "PWD", new_pwd);

        //getcwdで取得するバージョン
/*         char    *cwd;
        cwd = ft_calloc(100, 1);
        if (!(getcwd(cwd, 100)))
            exit(1);
        printf("%s\n", cwd);
        update_env(&env, "PWD", cwd); */
        //print_env(env);
        free(old_pwd);
        free(new_pwd);
        free(path);
    }
}