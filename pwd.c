/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayoakouh <ayoakouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 12:34:07 by ayoakouh          #+#    #+#             */
/*   Updated: 2025/06/10 14:27:59 by ayoakouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_set(char *type, int flag)
{
	static char	*pwd;

	if (flag == 0 && type)
	{
		if (pwd)
			free(pwd);
		pwd = ft_strdup(type);
	}
	return (pwd);
}

int	pwd(t_cmd *cmd)
{
	char	*pwd;
	int		status;

	status = 0;
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		pwd = get_set(NULL, 1);
		if (!pwd)
			perror("pwd");
		else
			printf("%s\n", pwd);
		return (status);
	}
	else
	{
		cmd->data.new_pwd = get_set(pwd, 0);
		printf("%s\n", pwd);
	}
	free (pwd);
	return (status);
}

// int pwd(t_cmd *cmd)
// {
// 	char	*pwd;
// 	int		status;

// 	status = 0;
// 	pwd = getcwd(NULL, 0);
// 	if(!pwd)
// 	{
// 		pwd = cmd->data.new_pwd;
// 		printf("%s\n", pwd);
// 		return(status);
// 	}
// 	else
// 	{
// 		cmd->data.new_pwd = pwd;
// 		printf("%s\n", pwd);
// 	}
// 	free(pwd);
// 	return (status);
// }
