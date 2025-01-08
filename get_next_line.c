/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <mwallis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 19:56:56 by mwallis           #+#    #+#             */
/*   Updated: 2024/12/23 00:09:23 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	*ft_calloc(size_t nb_elements, size_t size_type)
{
	char	*str;
	size_t	i;

	if (size_type != 0 && (nb_elements > SIZE_MAX / size_type))
	{
		return (NULL);
	}
	str = malloc(size_type * nb_elements);
	if (str == NULL)
	{
		return (NULL);
	}
	i = 0;
	while (i < (nb_elements * size_type))
	{
		str[i] = '\0';
		i++;
	}
	return (str);
}

static char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
		{
			return ((char *)&s[i]);
		}
		i++;
	}
	if (s[i] == (char)c)
	{
		return ((char *)&s[i]);
	}
	return (NULL);
}

static char	*read_and_store(int fd, char *fullstr)
{
	char	*str;
	char	*tmp;
	int		number_read;

	str = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (str == NULL)
		return (NULL);
	number_read = 1;
	while (number_read > 0)
	{
		number_read = read(fd, str, BUFFER_SIZE);
		if (number_read == -1)
			return (free(str), free(fullstr), NULL);
		str[number_read] = '\0';
		tmp = ft_strjoin(fullstr, str);
		free(fullstr);
		if (!tmp)
			return (free(str), NULL);
		fullstr = tmp;
		if (ft_strchr(str, '\n'))
			break ;
	}
	free(str);
	return (fullstr);
}

static char	*extract_line(char *fullstr, char **line, char *stash)
{
	char	*newline_position;
	size_t	len;

	newline_position = ft_strchr(fullstr, '\n');
	if (newline_position == NULL)
	{
		*line = ft_strdup(fullstr);
		if (!*line)
			return (NULL);
		free(stash);
		stash = NULL;
	}
	else
	{
		len = newline_position - fullstr + 1;
		*line = ft_substr(fullstr, 0, len);
		if (!*line)
			return (NULL);
		stash = ft_strdup(newline_position + 1);
		if (!stash)
			return (free(*line), *line = NULL, NULL);
	}
	return (stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;
	char		*fullstr;

	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		return (NULL);
	}
	line = NULL;
	fullstr = ft_strjoin(stash, "");
	free(stash);
	stash = NULL;
	if (fullstr == NULL)
		return (NULL);
	fullstr = read_and_store(fd, fullstr);
	if (fullstr == NULL)
		return (NULL);
	else if (fullstr[0] == '\0')
	{
		free(fullstr);
		return (NULL);
	}
	stash = extract_line(fullstr, &line, stash);
	free(fullstr);
	return (line);
}
