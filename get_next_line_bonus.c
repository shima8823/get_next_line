/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shima <shima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 20:48:19 by shima             #+#    #+#             */
/*   Updated: 2022/08/15 10:45:06 by shima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static bool	save_str(char *buf, char **save);
static void	create_line(char *buf, char **line, char **save);
static void	when_save_exists(char *buf, char **line, char **save);
static bool	line_save_factory(int fd, char *buf, char **line, char **save);

char	*get_next_line(int fd)
{
	static char	*save[256];
	char		*buf;
	char		*line;

	if (BUFFER_SIZE <= 0 || fd < 0 || fd >= 256)
		return (NULL);
	errno = 0;
	buf = malloc((size_t)(BUFFER_SIZE + 1) * sizeof(char));
	if (errno != 0)
		return (NULL);
	line = NULL;
	while (!line_save_factory(fd, buf, &line, &save[fd]))
		;
	free(buf);
	if (errno != 0)
	{
		free(line);
		free(save[fd]);
		return (NULL);
	}
	return (line);
}

static bool	line_save_factory(int fd, char *buf, char **line, char **save)
{
	size_t	len;

	len = read(fd, buf, BUFFER_SIZE);
	if (errno != 0)
		return (true);
	buf[len] = '\0';
	if (ft_strchr(buf, '\n') || len == 0)
		create_line(buf, line, save);
	else
		if (save_str(buf, save))
			return (false);
	return (true);
}

static void	create_line(char *buf, char **line, char **save)
{
	size_t	i;

	if (!(*save))
	{
		if (buf[0] == '\0')
			return ;
		*line = strdup_until_c(buf, '\n');
		i = str_c_len(buf, '\n');
		if (!(buf[i]))
			return ;
		*save = strdup_until_c(&buf[i], '\0');
		return ;
	}
	when_save_exists(buf, line, save);
}

static void	when_save_exists(char *buf, char **line, char **save)
{
	size_t	i;
	char	*tmp;

	i = str_c_len(*save, '\n');
	if (ft_strchr(*save, '\n') || (*save)[i])
	{
		*line = strdup_until_c(*save, '\n');
		tmp = *save;
		*save = strjoin_until_c(&(*save)[i], buf, '\0');
		free(tmp);
		return ;
	}
	*line = strjoin_until_c(*save, buf, '\n');
	i = str_c_len(buf, '\n');
	if (buf[i])
	{
		tmp = *save;
		*save = strdup_until_c(&buf[i], '\0');
		free(tmp);
		return ;
	}
	free(*save);
	*save = NULL;
}

static bool	save_str(char *buf, char **save)
{
	char	*tmp;

	if (!(*save))
	{
		*save = strdup_until_c("", '\0');
		if (errno != 0)
			return (false);
	}
	tmp = *save;
	*save = strjoin_until_c(*save, buf, '\0');
	free(tmp);
	if (errno != 0)
		return (false);
	return (true);
}
