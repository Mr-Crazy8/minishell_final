/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 11:32:18 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/21 16:06:59 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	print_and_set_error(char *file, t_cmd *cmd, int error_type)
{
	print_file_error(file, error_type);
	cmd->data.exit_status = get_or_set(SET, 1);
}

void	handle_file_type(int type, char *file, int *fd, t_cmd *cmd)
{
	if (type == 0)
	{
		fd[0] = open(file, O_RDONLY);
		if (access(file, R_OK) == -1)
			print_and_set_error(file, cmd, 3);
		else if (fd[0] == -1)
			print_file_error(file, 0);
	}
	else if (type == 1)
	{
		fd[0] = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
		if (access(file, W_OK) == -1)
			print_and_set_error(file, cmd, 3);
		else if (fd[0] == -1)
			print_file_error(file, 1);
	}
	else if (type == 2)
	{
		fd[0] = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (access(file, W_OK) == -1)
			print_and_set_error(file, cmd, 3);
		if (fd[0] == -1)
			print_file_error(file, 2);
	}
}

int	*handle_file_opening(int type, char *file, t_cmd *cmd)
{
	int	*fd;

	fd = malloc(2 * sizeof(int));
	if (!fd)
		return (NULL);
	fd[1] = -1;
	handle_file_type(type, file, fd, cmd);
	return (fd);
}

int	*open_file(t_cmd *cmd, int type, char *file, int Ambiguous)
{
	int	*fd;

	if (Ambiguous == 1 && type != 3)
	{
		fd = malloc(2 * sizeof(int));
		if (!fd)
			return (NULL);
		fd[0] = -1;
		fd[1] = -1;
		return (fd);
	}
	return (handle_file_opening(type, file, cmd));
}

void	print_file_error(char *file, int i)
{
	write(2, "minishell : ", 13);
	write(2, file, ft_strlen(file));
	if (i == 0)
		write(2, " : No such file or directory\n", 29);
	else if (i == 1)
		write(2, ": Cannot create or write to file\n", 33);
	else if (i == 2)
		write(2, ": Cannot append to file\n", 24);
	else if (i == 3)
	{
		write(2, ": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		write(2, "\n", 1);
	}
}
