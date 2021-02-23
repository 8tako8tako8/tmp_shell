#include "get_next_line.h"

void		ft_strlcpy2(char *dst, const char *src, size_t dstsize)
{
	size_t		i;

	if (dstsize == 0)
		return ;
	i = 0;
	while (src[i] != '\0' && i < dstsize)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

char		*ft_strjoin2(char const *s1, char const *s2, size_t len2)
{
	size_t		len1;
	char		*str;

	len1 = ft_strlen((char *)s1);
	if (!(str = (char *)malloc(sizeof(char) * (len1 + len2 + 1))))
		return (NULL);
	ft_strlcpy2(str, s1, len1);
	ft_strlcpy2(str + len1, s2, len2);
	str[len1 + len2] = '\0';
	return (str);
}

size_t		ft_strchr2(const char *s, char c)
{
	size_t		i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			return (i);
		i++;
	}
	return (i);
}