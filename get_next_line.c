/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marudlof <marudlof@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 20:20:45 by marudlof          #+#    #+#             */
/*   Updated: 2025/10/28 21:33:52 by marudlof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

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
	if (!line)
		return (NULL);
	i = 0;
	while (start[i] && start[i] != '\n')
	{
		line[i] = start[i];
		i++;
	}
	if (start[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
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
	if (!new_buff)
		return (NULL);
	j = 0;
	while (start[i + j])
	{
		new_buff[j] = start[i + j];
		j++;
	}
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
	char		*tmp;
	static char	*start_str;

	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		free(start_str);
		start_str = NULL;
		return (NULL);
	}
	start_str = ft_gnl_rl(start_str, fd);
	tmp = ft_readed_line(start_str);
	start_str = ft_move_start(start_str);
	return (tmp);
}
