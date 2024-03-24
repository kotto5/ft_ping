/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kakiba <kakiba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 13:10:57 by kakiba            #+#    #+#             */
/*   Updated: 2024/03/24 22:12:52 by kakiba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

//static kouzoutai ni ireru
char	*get_next_line(int fd)
{
	char		*buf;
	char		*new_line;
	size_t		allocate_size;
	static char	remain[OPEN_MAX][BUFFER_SIZE + 1UL];

	if (fd == -1)
		return (NULL);
	allocate_size = BUFFER_SIZE + 1UL;
	buf = malloc(allocate_size);
	if (!buf)
		return (NULL);
	new_line = NULL;
	if (remain[fd][0] != '\0')
	{
		allocate_size += ft_strlcpy_(buf, remain[fd], BUFFER_SIZE);
		ft_bzero_(remain[fd], BUFFER_SIZE);
		new_line = ft_strchr_(buf, '\n', BUFFER_SIZE);
	}
	buf = read_file(fd, buf, &new_line, &allocate_size);
	format_to_return(&buf, remain[fd], allocate_size);
	return (buf);
}

// read error to seijoukei ga same syori
char	*read_file(int fd, char *buf, char **new_line,
		size_t *allocate_size)
{
	ssize_t	read_length;
	char	*s_last;

	read_length = BUFFER_SIZE;
	while (*new_line == NULL && read_length == BUFFER_SIZE)
	{
		buf = ft_realloc(buf, *allocate_size - BUFFER_SIZE, *allocate_size);
		if (!buf)
			return (NULL);
		s_last = buf + *allocate_size - BUFFER_SIZE - 1;
		ft_bzero_(s_last, BUFFER_SIZE + 1);
		read_length = read(fd, s_last, BUFFER_SIZE);
		*new_line = ft_strchr_(s_last, '\n', read_length);
		if (*allocate_size == BUFFER_SIZE + 1UL && read_length <= 0)
		{
			free (buf);
			return (NULL);
		}
		*allocate_size += read_length;
	}
	return (buf);
}

void	format_to_return(char **buf, char remain[BUFFER_SIZE], \
			size_t allocate_size)
{
	char	*new_line;

	new_line = ft_strchr_(*buf, '\n', allocate_size);
	if (new_line && new_line[1] != '\0')
	{
		ft_strlcpy_(remain, new_line + 1, BUFFER_SIZE);
		*buf = ft_realloc(*buf, allocate_size, (new_line - *buf + 2));
		return ;
	}
	ft_bzero_(remain, BUFFER_SIZE);
}
