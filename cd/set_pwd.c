#include "cd.h"

char    *set_pwd(void)
{
    extern char     **environ;
    int             i;
    char            *cwd;

    i = 0;
    while (environ[i])
    {
        if (ft_strncmp(environ[i], "PWD=", 4) == 0)
        {
            if (*(environ[i] + 4) == '/')
                return (ft_strdup(environ[i] + 4));
        }
        i++;
    }
    //environから見つからなければgetcwdで作る
    cwd = ft_calloc(100, 1);
    if (!(getcwd(cwd, 100)))
        exit(1);
    return (cwd);
}