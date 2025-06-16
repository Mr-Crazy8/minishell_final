/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayoakouh <ayoakouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 12:33:13 by ayoakouh          #+#    #+#             */
/*   Updated: 2025/06/10 16:03:38 by ayoakouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
}

int	handel_echo_n(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' && str[i + 1] == 'n')
	{
		i += 2;
		while (str[i] == 'n')
			i++;
		if (str[i] == '\0')
			return (1);
	}
	return (0);
}

int	echo(char **argv)
{
	int		i;
	int		flag;
	char	*helper;

	i = 1;
	flag = 0;
	while (argv[i] && handel_echo_n(argv[i]) == 1)
	{
		flag = 1;
		i++;
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (flag == 0)
		ft_putstr_fd("\n", 1);
	return (0);
}

// void echo(char **argv)
// {
//     int i = 2;  
//     int j;
//     int flag = 0;
//     while (argv[i] && handel_echo_n(argv[i]) == 1)
//     {
//         flag = 1;
//         i++;
//     }
//     while (argv[i])
//     {
//         j = 0;

//         while (argv[i][j])
//         {
//             // ft_putstr_fd(argv[i], fd)
//             write(1, &argv[i][j], 1);
//             j++;
//         } 
//         if (argv[i + 1])
//             write(1, " ", 1);    
//         i++;
//     }
//     // newline 
//     if (flag == 0)
//         write(1, "\n", 1);
// }