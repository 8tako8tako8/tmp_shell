#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>
# include "../libft/libft.h"
# define BUFFER_SIZE 50
# define MAX_FD 256

int			get_next_line(int fd, char **line);
int			ft_find_line(char **line, char **save, int fd, char *buf);
int			ft_stdin_or_read_file(char **line, char **save, int fd);
int			ft_read_error(char **line, char **save, int fd);
void		ft_strlcpy2(char *dst, const char *src, size_t dstsize);
char		*ft_strjoin2(char const *s1, char const *s2, size_t len2);
size_t		ft_strchr2(const char *s, char c);

#endif