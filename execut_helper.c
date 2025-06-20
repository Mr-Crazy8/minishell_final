/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execut_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 20:53:44 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/20 20:56:20 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void execute_single_command(t_cmd *cmd, t_env **list_env, char *env[])
{
		if(!is_builtin(cmd->args))
		{
			excute_builting(&cmd, list_env, env);
			get_or_set(SET, cmd->data.exit_status);
		}
		else
		{
			if(cmd->pipe_out)
				ft(cmd->args, list_env);
			else
				ft_excute_commands(cmd, list_env);
			get_or_set(SET, cmd->data.exit_status);	
		}
}

void excute_builting(t_cmd **command, t_env **env_list, char *env[])
{
	t_cmd	*cmd;
	int		status;

	cmd = *command;
	status = 0;

	if (strncmp("export", cmd->args[0], 6) == 0 && strlen(cmd->args[0]) == 6)
		status = ft_export(cmd->args, env_list);
	else if (strncmp("env", cmd->args[0], 3) == 0 && strlen(cmd->args[0]) == 3)
		status = ft_env(*command, *env_list);
	else if (strncmp("exit", cmd->args[0], 4) == 0 && strlen(cmd->args[0]) == 4)
		status = ft_exit(cmd->args, cmd->data);
	else if (strncmp("unset", cmd->args[0], 5) == 0 && strlen(cmd->args[0]) == 5)
		status = ft_unset(env_list, cmd->args + 1);
	else if (strncmp("echo", cmd->args[0], 4) == 0 && strlen(cmd->args[0]) == 4)
		status = echo(cmd->args);
	else if (strncmp("pwd", cmd->args[0], 3) == 0 && strlen(cmd->args[0]) == 3)
		status = pwd(cmd);
	else if (strncmp("cd", cmd->args[0], 2) == 0 && strlen(cmd->args[0]) == 2)
		status = ft_cd(cmd->args, env_list, cmd);
	cmd->data.exit_status = get_or_set(SET, status);
}

int is_builtin(char **args)
{
	if(!args || !*args)
		return (1);
	if (strcmp(args[0], "cd") == 0)
		return (0);
	else if (strcmp(args[0], "echo") == 0)
		return (0);
	else if (strcmp(args[0], "unset") == 0)
		return (0);
	else if (strcmp(args[0], "export") == 0)
		return (0);
	else if (strcmp(args[0], "pwd") == 0)
		return (0);
	else if (strcmp(args[0], "env") == 0)
		return (0);
	else if (strcmp(args[0], "exit") == 0)
		return (0);
	else
		return (1);
}