/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marudlof <marudlof@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 20:20:56 by marudlof          #+#    #+#             */
/*   Updated: 2025/10/28 20:30:57 by marudlof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 8
# endif
# include <stdlib.h>
# include <unistd.h>

size_t	ft_strlen(char *str);
char	*get_next_line(int fd);
char	*ft_strchr(char *s, int c);
char	*ft_strjoin(char *start, char *buff);
char	*ft_join(char *dest, char *s1, char *s2);

#endif
