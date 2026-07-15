/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marudlof <marudlof@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 20:21:46 by marudlof          #+#    #+#             */
/*   Updated: 2025/10/28 22:44:33 by marudlof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*ft_readed_line(char *start)
{
	int		i;
	char	*line;

	if (!start || !start[0])
		return (NULL);
	i = 0;
	while (start[i] && start[i] != '\n')
		i++;
	if (start[i] == '\n')
		i++;
	line = malloc((1 + i) * sizeof(char));
	if (line)
	{
		i = 0;
		while (start[i] && start[i] != '\n')
		{
			line[i] = start[i];
			i++;
		}
		if (start[i] == '\n')
			line[i++] = '\n';
		line[i] = '\0';
	}
	return (line);
}

static char	*ft_move_start(char	*start)
{
	char	*new_buff;
	int		i;
	int		j;

	i = 0;
	while (start && start[i] && start[i] != '\n')
		i++;
	if (!start || start[i] == '\0')
	{
		free(start);
		return (NULL);
	}
	i += (start[i] == '\n');
	new_buff = malloc((1 + ft_strlen(start) - i) * sizeof(char));
	j = 0;
	while (new_buff && start[i + j])
	{
		new_buff[j] = start[i + j];
		j++;
	}
	if (new_buff)
		new_buff[j] = '\0';
	free(start);
	return (new_buff);
}

static char	*ft_gnl_rl(char *start_str, int fd)
{
	char	*tmp;
	int		fd_read;

	tmp = NULL;
	tmp = malloc((1 + BUFFER_SIZE) * sizeof(char));
	if (!tmp)
		return (NULL);
	fd_read = 1;
	while (!(ft_strchr(start_str, '\n')) && fd_read != 0)
	{
		fd_read = read(fd, tmp, BUFFER_SIZE);
		if (fd_read == -1)
		{
			free(start_str);
			start_str = NULL;
			break ;
		}
		tmp[fd_read] = '\0';
		start_str = ft_strjoin(start_str, tmp);
	}
	free(tmp);
	return (start_str);
}

char	*get_next_line(int fd)
{
	char		*newline;
	static char	*start_str[1024] = {NULL};

	if (fd < 0 || fd >= 1024 || BUFFER_SIZE <= 0)
	{
		free(start_str[fd]);
		start_str[fd] = NULL;
		return (NULL);
	}
	start_str[fd] = ft_gnl_rl(start_str[fd], fd);
	newline = ft_readed_line(start_str[fd]);
	start_str[fd] = ft_move_start(start_str[fd]);
	return (newline);
}
