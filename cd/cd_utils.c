#include "cd.h"

void    ft_strncpy(char *dst, char *src, size_t n)
{
    size_t  i;

    if (!src)
        return ;
    i = 0;
    while (src[i] != '\0' && i < n)
    {
        dst[i] = src[i];
        i++;
    }
}

char    *get_abs_path(char *base, char *path)
{
    char    *a_path;
    char    *bp;
    char    *pp;
    char    *rp;

    if (path[0] == '/')
        return (a_path = ft_strdup(path));
    if (!ft_strcmp(path, ".") || !ft_strcmp(path, "./"))
        return (a_path = ft_strdup(base));
    bp = base + ft_strlen(base);
    if (*(bp - 1) == '/')
        bp--;
    pp = path;
    while (*pp != '\0' && *pp == '.')
    {
        if (!ft_strncmp(pp, "../", 3))
        {
            pp += 3;
            while (bp > base && *(--bp) != '/');
        }
        else if (!ft_strncmp(pp, "./", 2))
            pp += 2;
        else if (!ft_strncmp(pp, "..\0", 3))
        {
            pp += 2;
            while (bp > base && *(--bp) != '/');
        }
        else
            break ;
    }
    if (!(a_path = malloc((bp - base) + 1 + ft_strlen(pp) + 1)))
        return (NULL);
    ft_strncpy(a_path, base, bp - base);
    rp = a_path + (bp - base);
    *rp++ = '/';
    ft_strlcpy(rp, pp, ft_strlen(pp) + 10);
    return (a_path);
}

void    del_dot_dot_slash(char **path, int i)
{
    char    *tmp;
    int     j;

    j = i - 2;
    tmp = *path;
    while (tmp[j] != '/')
        j--;
    if (!(*path = malloc(ft_strlen(tmp) - (i - j + 2))))
        exit(1);
    ft_strncpy(*path, tmp, j + 1);
    if (*(tmp + i + 3) != '\0')
        ft_strlcpy(*path + j + 1, tmp + i + 3, ft_strlen(tmp + i + 3) + 10);
    free(tmp);
}

void    del_dot_slash(char **path, int i)
{
    char    *tmp;

    tmp = *path;
    if (!(*path = malloc(ft_strlen(tmp) - 2)))
        exit(1);
    ft_strncpy(*path, tmp, i);
    if (*(tmp + i + 2) != '\0')
        ft_strlcpy(*path + i, tmp + i + 2, ft_strlen(tmp + i + 2) + 10);
    free(tmp);
}

char    *correct_abs_path(char *path)
{
    char    *tmp;
    int     i;

    //終端に/をつける関数
    if (path[ft_strlen(path) - 1] != '/')
    {
        tmp = path;
        path = ft_strjoin(path, "/");
        free(tmp);
    }
    i = 0;
    while (path[i] != '\0')
    {
        if (!ft_strncmp(path + i, "../", 3))
        {
            //一つ前の/まで削る関数
            del_dot_dot_slash(&path, i);
            i = 0;
        }
        else if (!ft_strncmp(path + i, "./", 2))
        {
            //./を削る
            del_dot_slash(&path, i);
            i = 0;
        }
        i++;
    }
    //終端の/を削る関数
    if (path[ft_strlen(path) - 1] == '/')
        path[ft_strlen(path) - 1] = '\0';//一文字余るのだめ？
    return (path);
}

char    *get_path_after_moving(char *oldpwd, char *dstpath)
{
    char    *ret_path;
    char    *tmp;

    if (!oldpwd)
        return (NULL);
    tmp = get_abs_path(oldpwd, dstpath);
    ret_path = correct_abs_path(tmp);
    return (ret_path);
}
