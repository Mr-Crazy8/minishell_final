/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 16:16:16 by ayoakouh          #+#    #+#             */
/*   Updated: 2025/06/20 15:53:20 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    pipe_all(t_cmd *head)
{
	t_cmd *cmd = head;
	t_cmd *tmp = head;

	while (cmd)
	{
		
		if (cmd->pipe_out)
		{
			if(pipe(cmd->fd_pipe) == -1)
			{
				close_all_pipes(tmp);
				kill_and_wait_all(tmp);
				waitpid(-1, NULL, 0);
				perror("pipe");
				return ;
			}
		}
		cmd = cmd->next;
	}
}

char **convert_env_list(t_env **list)
{
	t_env *tmp = *list;
	char **helper;
	char *str;
	char *temp;
	int i = 0;
	int len = ft_list_size(*list);

	helper = malloc(sizeof(char *) * (len + 1));
	while (tmp)
	{
		str = ft_strjoin(tmp->key, "=");
		temp = ft_strjoin(str, tmp->value);
		free(str);
		helper[i++] = temp;
		tmp = tmp->next;
	}
	helper[i] = NULL;
	return (helper);
}
static void	close_child_pipes(t_cmd *head)
{
	t_cmd	*tmp;

	tmp = head;
    while (tmp)
    {
        if (tmp->pipe_out)
        {
			if(tmp->fd_pipe[0] != -1)
			{
            	close(tmp->fd_pipe[0]);
				tmp->fd_pipe[0] = -1;
			}
			if(tmp->fd_pipe[1] != -1)
			{
            	close(tmp->fd_pipe[1]);
				tmp->fd_pipe[1] = -1;
			}
        }
        tmp = tmp->next;
    }
}

void ft(char **args, t_env **list) // pipe moust be check and access;;;;*
{
    char **split_path;
    char **helper;
    char *path;
    int i= 0;
    char *tmp;

    helper = convert_env_list(list);
    path = check_path(list);
    if(!args || !args[0])
        return ;
    if(ft_strchr(args[0], '/'))
    {
        if(!(access(args[0], X_OK) == -1))
        {
            execve(args[0], args, helper);
            exit (1); //ila failat execve
        }
        write(1, "command not found\n", 18);
    }
    else
    {
        split_path = ft_split(path, ':');
        if(!split_path)
            return ;
        while(split_path[i])
        {
            tmp = ft_strjoin(split_path[i], "/");
            char *str = ft_strjoin(tmp, args[0]);
            free(tmp);
            if (!helper)
            {
                i++;
                continue;
            }
            if(!(access(str, X_OK) == -1))
            {
                execve(str, args, helper);
                exit (1);
            }
            free(str);
            i++;
        }
        i = 0;
        write(2, "command not found\n", 18);
        ft_free_split(split_path);
        exit(127);
    }        
}
void close_all_pipes(t_cmd *cmd)
{
	t_cmd *tmp;
	t_redir *red;

	tmp = cmd;
	while (tmp)
	{
		if (tmp->pipe_out)
		{
			if(tmp->fd_pipe[0] != -1)
			{
				close(tmp->fd_pipe[0]);
				tmp->fd_pipe[0] = -1;
			}
			if(tmp->fd_pipe[1] != -1)
			{
				close(tmp->fd_pipe[1]);
				tmp->fd_pipe[1] = -1;
			}
		}
		red = tmp->redirs;
		while(red)
		{
			close(red->fd[1]);
			red = red->next;
		}
		tmp = tmp->next;
	}
}

void ft_free_redrect(t_redir *redir)
{
	t_redir *tmp = redir;
	while (tmp)
	{
		if (tmp->fd[1] != -1)
			close(tmp->fd[1]);
		if(tmp->fd[0] != -1)
			close(tmp->fd[0]);
		tmp = tmp->next;
	}
}

static void sig_default_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

static void dup_pipe(t_cmd *cmd, t_cmd *prev)
{
	if (prev && prev->pipe_out)
	{
		dup2(prev->fd_pipe[0], STDIN_FILENO);
		close(prev->fd_pipe[0]);
		prev->fd_pipe[0] = -1;
	}
	if (cmd->pipe_out)
	{
		dup2(cmd->fd_pipe[1], STDOUT_FILENO);
		close(cmd->fd_pipe[1]);
		cmd->fd_pipe[1] = -1;
	}
}

static void close_prev_pipe(t_cmd *prev)
{
	if (prev && prev->pipe_out)
	{
		if(prev->fd_pipe[0] != -1)
		{
			close(prev->fd_pipe[0]);
			prev->fd_pipe[0] = -1;
		}
		if(prev->fd_pipe[1] != -1)
		{
			close(prev->fd_pipe[1]);
			prev->fd_pipe[1] = -1;
		}
	}
}

void check_close_red(t_cmd *cmd, t_cmd *prev, t_env *env)
{
	if (cmd->redirs && !isatty(cmd->redirs->fd[1]))
		ft_redircte(cmd->redirs, env, cmd);
}

void close_redirection(t_redir *redirs)
{
    t_redir *tmp = redirs;
    
    while (tmp)
    {
        if (tmp->fd[0] > 2)
        {
            close(tmp->fd[0]);
			tmp->fd[0] = -1;
        }
		if(tmp->fd[1] > 2)
		{
			close(tmp->fd[1]);
			tmp->fd[1] = -1;
		}
        tmp = tmp->next;
    }
}
void kill_and_wait_all(t_cmd *cmd)
{
    t_cmd *tmp = cmd;

    while (tmp != NULL)
    {
        if (tmp->pid > 0)
        {
			kill(tmp->pid, SIGKILL);
			waitpid(tmp->pid, NULL, 0);
            tmp->pid = -1;
        }
        tmp = tmp->next;
    }
    // // Wait for all children
}
// void kill_and_wait_all(t_cmd *cmd)
// {
//     t_cmd *tmp = cmd;

//     while (tmp != NULL)  // Check for NULL
//     {
//         if (tmp->pid > 0)  // Only kill if pid is valid
//         {
//             kill(tmp->pid, SIGKILL);
//             waitpid(tmp->pid, NULL, 0);  // UNCOMMENT THIS - it's crucial!
//             tmp->pid = -1;  // Mark as terminated
//         }
//         tmp = tmp->next;
//     }
// }
// void    kill_and_wait_all(t_cmd *cmd)
// {
// 	int	i;
// 	t_cmd *tmp = cmd;

//     while (tmp->pid > 0)
// 	{
// 		dprintf(2, "%d\n", tmp->pid);
//         kill(tmp->pid, SIGKILL);
//         // waitpid(tmp->pid, NULL, 0);
// 		tmp = tmp->next;
// 	}
// }
static int fork_exec(t_cmd *cmd, t_cmd *prev, t_env *env_list, char **env, t_cmd *head)
{
	int i;

	i = 0;
	pid_t pid = fork();
	if (pid < 0)
	{
		// kill_and_wait_all(head);
		perror("fork");
		// close_all_pipes(head);
		return(-1) ;
	}
	if (pid == 0)
	{
		sig_default_child();
		dup_pipe(cmd, prev);
		if (cmd->redirs && !isatty(cmd->redirs->fd[0]))
		{
			ft_redircte(cmd->redirs, env_list, cmd);
			// close_redirection(head->redirs);
		}
		close_child_pipes(head);
		if (!cmd->redirs || cmd->redirs->fd[0] != -1)
			execute_single_command(cmd, &env_list, env);
		exit(cmd->data.exit_status);
	}
	if (pid)
		cmd->pid = pid;
	if (prev && prev->pipe_out)
    {
		if(prev->fd_pipe[0] != -1)
        	close(prev->fd_pipe[0]);
		prev->fd_pipe[0] = -1;
        // close(prev->fd_pipe[1]);
		// prev->fd_pipe[1] = -1;
    }
	return (0);
}

static void wait_last_pid(pid_t last_pid, t_cmd *head)
{
	int status, pid;
	int last_status = 0;

	while ((pid = waitpid(-1, &status, 0)) > 0) // this wait;
	{
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGQUIT)
				ft_putstr_fd("Quit: 3\n", 1);
			else if (WTERMSIG(status) == SIGINT)
			{
				close_heredoc_fds(head->redirs);
				close_redirection(head->redirs);
				ft_putstr_fd("\n", 1);
			}
		}
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
			head->data.exit_status = get_or_set(SET, last_status);
		}
	}
	get_or_set(SET, head->data.exit_status);
}

void	ft_excute_mult_pipe(t_cmd *cmd, t_env *env, char **envp)
{
	t_cmd *head = cmd;
	t_cmd *prev = NULL;
	pid_t last_pid = 0;
	
	int i = 0;
	signal(SIGINT, SIG_IGN);
	pipe_all(cmd);
	while (cmd)
	{			
		if(fork_exec(cmd, prev, env, envp, head) == -1)
		{
			close_all_pipes(head);
			kill_and_wait_all(head);
			get_or_set(SET, 1);
			return ;
		}
		if (!cmd->next)
			last_pid = cmd->pid;
		if(cmd->pipe_out)
		{
			close(cmd->fd_pipe[1]);
			cmd->fd_pipe[1] = -1;
		}
		close_prev_pipe(prev);
		if(cmd->flag == 1)
			exit(130);
		prev = cmd;
		cmd = cmd->next;
	}
	close_prev_pipe(prev);
	close_all_pipes(head);
	wait_last_pid(last_pid, head);
}
















































// static int fork_exec(t_cmd *cmd, t_cmd *prev, t_env *env_list, char **env, t_cmd *head)
// {
//     pid_t pid = fork();
//     if (pid < 0) // Handle fork failure
//     {
//         perror("fork: Resource temporarily unavailable");
//         kill_and_wait_all(head);  // Terminate all running child processes
//         close_all_pipes(head);    // Close all open pipes
//         return -1;                // Indicate a failure
//     }
//     if (pid == 0) // Child process
//     {
//         sig_default_child();
//         dup_pipe(cmd, prev);      // Set up pipes for the child
//         close_all_pipes(head);    // Close all pipes in the child process
//         if (cmd->redirs && !isatty(cmd->redirs->fd[0]))
//         {
//             ft_redircte(cmd->redirs, env_list, cmd);
//             close_redirection(cmd->redirs);
//         }
//         if (!cmd->redirs || cmd->redirs->fd[0] != -1)
//             execute_single_command(cmd, &env_list, env);
//         exit(cmd->data.exit_status); // Exit child process
//     }
//     cmd->pid = pid;
//     return 0; // Indicate success
// }





















// #include "minishell.h"

// char **convert_env_list(t_env **list)
// {
// 	t_env * tmp;
// 	char **helper = NULL;
// 	char *str;
// 	char *temp;
// 	int  i = 0;
// 	tmp = *list;
// 	int len = ft_list_size(*list);

// 	helper = malloc(sizeof(char *) * (len + 1));
// 	while(tmp)
// 	{
// 		str = ft_strdup(tmp->key);
// 		temp = ft_strjoin(str, "=");
// 		free(str);
// 		str = ft_strjoin(temp, tmp->value);
// 		free(temp);
// 		helper[i] = ft_strdup(str);
// 		free(str);
// 		tmp = tmp->next;
// 		i++;
// 	}
// 	helper[i] = NULL;
// 	return (helper);
// }
// void close_all_pipes(t_cmd *cmd)
// {
// 	t_cmd *tmp = cmd;
	
// 	while (tmp)
// 	{
// 		if (tmp->pipe_out)
// 		{
// 			close(tmp->fd_pipe[0]);
// 			close(tmp->fd_pipe[1]);
// 		}
// 		tmp = tmp->next;
// 	}
// }

// // void check_close_red(t_cmd *cmd, t_cmd *prev, t_env *env)
// // {
// //         if (prev && prev->pipe_out)
// //         {
// //             close(prev->fd_pipe[0]);
// //             close(prev->fd_pipe[1]);
// //         }
// //         if (cmd->pipe_out)
// //         {
// //             close(cmd->fd_pipe[0]);
// //             close(cmd->fd_pipe[1]);
// //         }
// //         if (!isatty(cmd->redirs->fd)) //check if this function is working;
// //             ft_redircte(cmd->redirs, env, cmd);
// // }
// void ft_free_redrect(t_redir *redir)
// {
// 	t_redir *tmp = redir;
// 	if(!tmp)
// 		return ;
// 	while (tmp && tmp->fd)
// 	{
// 		close(tmp->fd);
// 		tmp = tmp->next;
// 	}

// }
// void handel_chil_pipe(t_cmd *cmd, t_cmd *prev, t_env *env_list, char *env[])
// {
// 	if(cmd->pid == 0)
// 	{
// 		signal(SIGINT, SIG_DFL);
// 		signal(SIGQUIT, SIG_DFL);
// 		if(prev && prev->pipe_out)
// 			dup2(prev->fd_pipe[0], 0);
// 		if(cmd->pipe_out)
// 			dup2(cmd->fd_pipe[1], 1);
// 		if(cmd->redirs != NULL)
// 			check_close_red(cmd, prev, env_list);
// 		close_all_pipes(cmd);
// 		if(cmd->redirs == NULL || cmd->redirs->fd != -1)
// 			execute_single_command(cmd, &env_list, env);
// 			printf("%d\n", cmd->data.exit_status);
// 		get_or_set(SET, cmd->data.exit_status);
// 		exit(0);
// 	}
// }





// void    pipe_all(t_cmd *head)
// {
// 	t_cmd *cmd = head;

// 	while (cmd)
// 	{
		
// 		if (cmd->pipe_out)
// 		{
// 			pipe(cmd->fd_pipe); // creates cmd->fd_pipe[0] (read) and [1] (write)
// 		}
// 		cmd = cmd->next;
// 	}
// }
// // void close_all_pipes(t_cmd *cmd)
// // {
// //     t_cmd *tmp = cmd;
	
// //     while (tmp)
// //     {
// //         if (tmp->pipe_out)
// //         {
// //             close(tmp->fd_pipe[0]);
// //             close(tmp->fd_pipe[1]);
// //         }
// //         tmp = tmp->next;
// //     }
// // }

// static void sig_default_child(void)
// {
// 	signal(SIGINT, SIG_DFL);
// 	signal(SIGQUIT, SIG_DFL);
// }

// static void dup_pipe(t_cmd *cmd, t_cmd *prev)
// {
// 	if (prev && prev->pipe_out)
// 	{
// 		dup2(prev->fd_pipe[0], 0);
// 		close(cmd->fd_pipe[1]);
// 		close(cmd->fd_pipe[0]);
// 	}
// 	if (cmd->pipe_out)
// 	{
// 		dup2(cmd->fd_pipe[1], 1);
// 		close(cmd->fd_pipe[0]);
// 		close(cmd->fd_pipe[1]);
// 	}
// }

// static void close_prev_pipe(t_cmd *prev, t_cmd *cmd)
// {
// 	if (prev && prev->pipe_out)
// 	{
// 		close(prev->fd_pipe[0]);
// 		close(prev->fd_pipe[1]);
// 		// close(cmd->fd_pipe[1]); //  check_it ;
// 	}
// 	// else if (cmd->pipe_out)
// 	// {
// 	// 	close(cmd->fd_pipe[1]);
// 	// 	close(cmd->fd_pipe[0]);
// 	//     puts("kkkkkkk0000");
// 	// }
// }

// void check_close_red(t_cmd *cmd, t_cmd *prev, t_env *env)
// {
// 		if (prev && prev->pipe_out)
// 		{
// 			close(prev->fd_pipe[0]);
// 			close(prev->fd_pipe[1]);
// 		}
// 		if (cmd->pipe_out)
// 		{
// 			close(cmd->fd_pipe[0]);
// 			close(cmd->fd_pipe[1]);
// 		}
// 		if (!isatty(cmd->redirs->fd)) //check if this function is working;
// 			ft_redircte(cmd->redirs, env, cmd);
// }
// static void fork_exec(t_cmd *cmd, t_cmd *prev, t_env *env_list, char **env)
// {
// 	pid_t pid;


// 	pid = fork();
// 	if (pid < 0)
// 	{
// 		perror("fork");
// 		close_all_pipes(cmd);
// 		return;
// 	}
// 	if (pid == 0)
// 	{
// 		sig_default_child();
// 		dup_pipe(cmd, prev);
// 		if (cmd->redirs)
// 			check_close_red(cmd, prev, env_list);
// 		if (!cmd->redirs || cmd->redirs->fd != -1)
// 			execute_single_command(cmd, &env_list, env);
// 		close_all_pipes(cmd);
// 		exit(cmd->data.exit_status);
// 	}
// 	if (cmd->redirs)
// 		ft_free_redrect(cmd->redirs);
// }

// static void wait_last_pid(int last_pid, t_cmd *head)
// {
// 	int status, pid, last_status = 0;

// 	while ((pid = wait(&status)) > 0)
// 	{
// 		puts("llll");
// 		if (WIFSIGNALED(status))
// 		{
// 			if (WTERMSIG(status) == SIGQUIT)
// 				ft_putstr_fd("Quit: 3\n", 1);
// 			else if (WTERMSIG(status) == SIGINT)
// 				ft_putstr_fd("\n", 1);
// 		}
// 		if (pid == last_pid)
// 		{
// 			if (WIFEXITED(status))
// 				last_status = WEXITSTATUS(status);
// 			else if (WIFSIGNALED(status))
// 				last_status = 128 + WTERMSIG(status);
// 			head->data.exit_status = get_or_set(SET, last_status);
// 		}
// 	}
// 	get_or_set(SET, head->data.exit_status);
// }

// void ft_excute_mult_pipe(t_cmd *cmd, t_env *env, char **envp)
// {
// 	t_cmd *head = cmd, *prev = NULL;
// 	pid_t last_pid = 0;

// 	signal(SIGINT, SIG_IGN);
// 	pipe_all(cmd);
// 	while (cmd)
// 	{
// 		fork_exec(cmd, prev, env, envp);
// 		if (!cmd->next)
// 			last_pid = get_last_pid();
// 		close_prev_pipe(prev, cmd);
// 		prev = cmd;
// 		cmd = cmd->next;
// 	}
// 	wait_last_pid(last_pid, cmd);
// 	close_all_pipes(head);
// }