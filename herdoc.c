/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 17:30:57 by ayoakouh          #+#    #+#             */
/*   Updated: 2025/06/20 21:36:55 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void write_to_file(char *str, int fd)
{
 
	int i = 0;
	if (str == NULL)
		ft_putchar_fd(0, fd);
	else {
	while (str && str[i])
	{
		ft_putchar_fd(str[i], fd);
		i++;
	}
	}
	close(fd);
	free(str);
}

int max_heredoc_checker(t_cmd *cmd)
{
	t_cmd *tmp = cmd;
	t_redir *tp = NULL;
	int count = 0;
	while (tmp)
	{
		tp = tmp->redirs;
		while (tp)
		{
			if (tp->type == 3)
				count++;
			tp = tp->next;
		}
		tmp = tmp->next;
	}
	if (count > 16)
		{
			write(2, "minishell:  maximum here-document count exceeded\n", 50);
			get_or_set(SET, 2);
			return 1;
		}
	return 0;
}

void heredoc_sigint_handler(int sig)
{
	(void)sig;
	// write(STDOUT_FILENO, "\n", 1);
	exit(130); // Standard SIGINT exit code
}

void close_heredoc_fds(t_redir *redir)
{
	if (!redir || !redir->fd)
		return ;
	if (redir->fd[0] > 2)
	{
		close(redir->fd[0]);
		redir->fd[0] = -1;
	}
	if (redir->fd[1] > 2)
	{
		close(redir->fd[1]);
		redir->fd[1] = -1;
	}
}
void handlll_sig(int sig)
{
	ft_putstr_fd("\n", 1);
	exit(1);
}


void check_here_doc(t_cmd *cmd, t_env *env)
{
	t_cmd *tmp = cmd;

	t_redir *tmp_redir;
	int status;
	pid_t pid;

	if (max_heredoc_checker(cmd))
		exit(2);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	while (tmp)
	{
		tmp_redir = tmp->redirs;
		while (tmp_redir)
		{
			if (tmp_redir->type == 3)
			{
				tmp_redir->fd = heredoc_opener();
				pid = fork();
				if (pid == 0)
				{
					signal(SIGINT, SIG_DFL);
					// signal(SIGQUIT, SIG_IGN);
					heredoc(tmp_redir->file, env, 0, tmp_redir->orig_token, tmp_redir->fd[0]);
					close_heredoc_fds(tmp_redir);
					exit(0);
				}
				else if (pid > 0)
				{
					waitpid(pid, &status, 0);
					if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
					{
						cmd->data.exit_status = get_or_set(SET, 1);
						close_heredoc_fds(tmp_redir);
						close_redirection(cmd->redirs);
						cmd->flag = 1;
						return;
					}
					cmd->flag = 0;
				}
				else
				{
					perror("fork");
					close_heredoc_fds(tmp_redir);
					cmd->flag = 1;
					return;
				}
			}
			tmp_redir = tmp_redir->next;
		}
		tmp = tmp->next;
	}
}

void heredoc(char *delmeter, t_env *env, int exit_status, char *orig_delimiter, int fd)
{
	char *line;
	char *heredoc;
	char *tmp1;
	char *tmp2;
	char *processed_delimiter = heredoc_delemter(ft_strdup(orig_delimiter));
	// int count = 0;
	heredoc = NULL;

	while(1)
	{
		line = readline("> ");
	   if (!line)
	   {    
			//write(1, "\n", 1);
			free(processed_delimiter);
			return ;
	   }
	   if (strcmp(line, processed_delimiter) == 0)
	   {
		free(line);
		break;
	   }
	   char *heredoc_expand = process_heredoc_epxand(line, env, exit_status, orig_delimiter);
	   tmp1 = ft_strjoin(heredoc_expand, "\n");
	   free(heredoc_expand);
	   free(line);
	   if (!tmp1)
	   {
		free(processed_delimiter);
		return ;
	   }
		if (heredoc == NULL)
			{
				heredoc = tmp1;
			}
		else 
		{
			tmp2 = ft_strjoin(heredoc, tmp1);
			free(heredoc);
			free(tmp1);

			if (!tmp2)
			{
				free(processed_delimiter);
				return ;
			}
			heredoc = tmp2;
		}
	}
	free(processed_delimiter);
   write_to_file(heredoc, fd);
}





// int *heredoc(char *delmeter, t_env *env, int exit_status, char *orig_delimiter)
// {
//     char *line;
//     char *heredoc;
//     char *tmp1;
//     char *tmp2;
//     char *processed_delimiter = delmeter;

//     heredoc = NULL;
//     while(1)
//     { 
//        line = readline("> ");
//        if (!line)
//        {
//             write(1, "\n", 1);
//             free(processed_delimiter);
//             return NULL;
//        }
	   
//        if (strcmp(line, processed_delimiter) == 0)
//        {
//         free(line);
//         break;
//        }
//        tmp1 = ft_strjoin(process_heredoc_epxand(line, env, exit_status, orig_delimiter), "\n");
//        free(line);
//        if (!tmp1)
//        {
//         free(processed_delimiter);
//         return NULL;
//        }
//         if (heredoc == NULL)
//         {
//             heredoc = tmp1;
//         }
//         else 
//         {
//             tmp2 = ft_strjoin(heredoc, tmp1);
//             free(heredoc);
//             free(tmp1);

//             if (!tmp2)
//             {
//                 free(processed_delimiter);
//                 return NULL;
//             }
//             heredoc = tmp2;
//         }
//     }
   
//     free(processed_delimiter);
//     return write_to_file(heredoc);
// }

// void check_here_doc(t_cmd *cmd, t_env *env)
// {
// 	t_cmd *tmp = cmd;
// 	t_redir *tmp_redir;
// 	int *fd;
// 	int status;
// 	pid_t pid;

// 	cmd->flag = 0; // Everything is OK by default
// 	while (tmp)
// 	{
// 		tmp_redir = tmp->redirs;
// 		while (tmp_redir)
// 		{
// 			if (tmp_redir->type == 3) // it's a heredoc
// 			{
// 					signal(SIGINT, SIG_DFL); // allow Ctrl+C to work normally
// 					fd = heredoc(tmp_redir->file, env, 0, tmp_redir->orig_token);
// 				pid = fork();
//                 tmp->redirs->fd = fd[1];
// 				if (pid == 0)
// 				{
// 					if (fd != NULL)
// 					{
// 						tmp_redir->fd = fd[1];
// 						close(fd[0]);
// 						exit(0);
// 					}
// 					else
// 					{
// 						tmp_redir->fd = -1;
// 						exit(1);
// 					}
// 				}
// 				else if (pid > 0) // parent process
// 				{
// 					waitpid(pid, &status, 0);
// 					if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
// 					{
// 						cmd->flag = 1; // heredoc interrupted
// 						return;
// 					}
// 				}
// 				else
// 				{
// 					perror("fork");
// 					cmd->flag = 1;
// 					return;
// 				}
// 			}
// 			tmp_redir = tmp_redir->next;
// 		}
// 		tmp = tmp->next;
// 	}
// }