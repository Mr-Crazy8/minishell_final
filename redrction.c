/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redrction.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:59:21 by ayoakouh          #+#    #+#             */
/*   Updated: 2025/06/20 21:47:29 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_redircte(t_redir *rederction, t_env *env, t_cmd *cmd)
{
	t_redir	*tmp;

	tmp = rederction;
	while (tmp)
	{
		if (tmp->type == 3)
		{
			if (tmp->fd[1] > 0)
			{
				dup2(tmp->fd[1], 0);
				close(tmp->fd[1]);
			}
		}
		else if (tmp->type == 0)
		{
			if(tmp->fd[0] > 0)
			{
				dup2(tmp->fd[0], 0);
				close(tmp->fd[0]);
			}
		}
		else if (tmp->type == 1 || tmp->type == 2)
		{
			if(tmp->fd[0] > 0)
			{
				dup2(tmp->fd[0], 1);
				close(tmp->fd[0]);
			}
		}
		tmp = tmp->next;
	}
}
// void ft_redircte(t_redir *redirection, t_env *env, t_cmd *cmd)
// {
//     t_redir *tmp = redirection;
//     while(tmp)
//     {
//         if(tmp->type == 0 || tmp->type == 3) 
//         {
//             if (tmp->fd >= 0)
//             {
//                 puts("nnnn");
//                 dup2(tmp->fd, 0); // redirect stdin
//                 close(tmp->fd);
//             }
//         }
//         else if (tmp->type == 1 || tmp->type == 2)
//         {
//             if (tmp->fd >= 0)
//             {
//                 dup2(tmp->fd, 1);
//                 close(tmp->fd);
//             }
//         }
//         tmp = tmp->next;
//     }
// }
// void	free_cmd(t_cmd *cmd)
// {
// 	t_redir	*redir;
// 	t_redir	*next_redir;
// 	int		i = 0;

// 	if (cmd->cmd)
// 		free(cmd->cmd);
// 	if (cmd->args)
// 	{
// 		while(cmd->args[i])
// 		{
// 			free(cmd->args[i])
// 			i++;
// 		}
// 		for (int i = 0; cmd->args[i]; i++)
// 			free(cmd->args[i]);
// 		free(cmd->args);
// 	}
// 	redir = cmd->redirs;
// 	while (redir)
// 	{
// 		next_redir = redir->next;
// 		if (redir->file)
// 			free(redir->file);
// 		free(redir);
// 		redir = next_redir;
// 	}
// 	free(cmd);
// }
// void ft_redircte(t_redir *rederction, t_env *env, t_cmd *cmd)
// {
//     t_redir *tmp;

//     tmp = rederction;

//     while(tmp)
//     {
//         puts("nnn");
//         if(tmp->type == 3)
//         {
//             dprintf(2, "this \n");
//                 dprintf(2, "this is fd : %d\n", tmp->fd);
//             if (tmp->fd >= 0)
//             {
//                 dup2(tmp->fd, 0);
//                 close(tmp->fd);
//                 tmp->fd = -1;
//             }
//         }
//         if(tmp->type == 0) 
//         {
//             dup2(tmp->fd, 0);
//             close(tmp->fd); 
//         }
//         else if (tmp->type == 1) 
//         {
//             // printf("-> %d \n", tmp->fd);
//             dup2(tmp->fd, 1);
//             close(tmp->fd);
//         }
//         else if(tmp->type == 2 )
//         {
//             dup2(tmp->fd, 1);
//             close (tmp->fd);
//         }
//         tmp = tmp->next;
//     }
// }