#include "pwd.h"

int    is_symlink(char *path)
{
    struct stat     st;

    if (lstat(path, &st) != 0)
        return (0);
    if ((st.st_mode & S_IFMT) == S_IFLNK)
        return (1);
    return (0);
}

void    ft_pwd(t_env *env)
{
    char    path[100];
    int     i;
    char    *pwd_value;

    i = 0;
    while (i < 100)
    {
        path[i] = 0;
        i++;
    }
    pwd_value = get_value_in_env(env, "PWD");
    if (is_symlink(pwd_value))
    {
        printf("%s\n", pwd_value);
        return ;
    }
    if (!(getcwd(path, 100)))
        exit(1);
    printf("%s\n", path);
}