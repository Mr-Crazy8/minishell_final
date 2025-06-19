/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 15:08:46 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/19 11:33:59 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	*create_failed_fd(void)
{
	int	*fd;

	fd = malloc(2 * sizeof(int));
	if (fd)
	{
		fd[0] = -1;
		fd[1] = -1;
	}
	return (fd);
}

void	process_redirection(t_cmd *cmd, t_redir *tp, int *failed)
{
	int	*fd;

	if (*failed == 0)
	{
		fd = open_file(cmd, tp->type, tp->file, tp->ambiguous);
		if (!fd)
		{
			tp->fd = create_failed_fd();
			*failed = 1;
		}
		else if (fd[0] == -1)
		{
			tp->fd = fd;
			*failed = 1;
		}
		else
			tp->fd = fd;
	}
	else
	{
		tp->fd = create_failed_fd();
		if (!tp->fd)
			tp->fd = NULL;
	}
}

void	file_opener(t_cmd *cmd, t_env *env)
{
	t_cmd	*tmp;
	t_redir	*tp;
	int		failed;

	tmp = cmd;
	while (tmp)
	{
		tp = tmp->redirs;
		failed = 0;
		while (tp)
		{
			process_redirection(cmd, tp, &failed);
			tp = tp->next;
		}
		tmp = tmp->next;
	}
}
