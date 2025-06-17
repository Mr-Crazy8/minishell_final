/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntx_check_helper_1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:47:56 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/16 20:11:36 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	redirection_helper(char *str, int *i, int *count)
{
	if (str[(*i)] == '>' && str[(*i) + 1] == '>')
	{
		(*count)++;
		(*i) += 2;
	}
	else if (str[(*i)] == '<' && str[(*i) + 1] == '<')
	{
		(*count)++;
		(*i) += 2;
	}
}

void	print_error(char *str, int *i)
{
	if (str[(*i)] == '\0')
		write(2, "minishell: syntax error near unexpected token `newline'\n",
			57);
	else if (str[(*i)] == '|')
		write(2, "minishell: syntax error near unexpected token `|'\n",
			51);
	else if (str[(*i)] == '>' && str[(*i) + 1] == '>')
		write(2, "minishell: syntax error near unexpected token `>>'\n",
			52);
	else if (str[(*i)] == '>')
		write(2, "minishell: syntax error near unexpected token `>'\n",
			51);
	else if (str[(*i)] == '<' && str[(*i) + 1] == '<')
		write(2, "minishell: syntax error near unexpected token `<<'\n",
			52);
	else if (str[(*i)] == '<')
		write(2, "minishell: syntax error near unexpected token `<'\n",
			51);
}

int	creat_redir_list_helper(char *str)
{
	if (str && strcmp(str, ">") != 0
		&& strcmp(str, ">>") != 0
		&& strcmp(str, "<") != 0
		&& strcmp(str, "<<") != 0)
		return (1);
	return (0);
}
