/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utiles_3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 14:33:32 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/21 16:16:17 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	split_stoper_hp(t_cmd *tmp, int *i, int *exp)
{
	char	*new_str;

	if ((*i) == 0 && tmp->args[(*i)] && strcmp(tmp->args[(*i)], "export") == 0)
		*exp = 1;
	if (tmp->args_befor_quotes_remover && tmp->args_befor_quotes_remover[(*i)])
	{
		new_str = split_helper(tmp->args[(*i)],
				tmp->args_befor_quotes_remover[(*i)], *exp);
		if (new_str != NULL)
		{
			free(tmp->args[(*i)]);
			tmp->args[(*i)] = new_str;
		}
	}
}

void	split_stoper(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		exp;
	int		i;

	if (!cmd)
		return ;
	tmp = cmd;
	while (tmp)
	{
		i = 0;
		exp = 0;
		while (tmp->args && tmp->args[i])
		{
			split_stoper_hp(tmp, &i, &exp);
			i++;
		}
		tmp = tmp->next;
	}
}

char	*chenger_back(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == 3)
			str[i] = '\'';
		else if (str[i] == 4)
			str[i] = '\"';
		i++;
	}
	return (str);
}

void	change_back_cmd(t_cmd *cmd)
{
	t_cmd	*tmp;
	t_redir	*tp;
	int		i;

	tmp = cmd;
	while (tmp)
	{
		i = 0;
		while (tmp->args[i])
		{
			tmp->args[i] = chenger_back(tmp->args[i]);
			i++;
		}
		tmp->cmd = chenger_back(tmp->cmd);
		i = 0;
		tp = tmp->redirs;
		while (tp)
		{
			tp->file = chenger_back(tp->file);
			tp = tp->next;
		}
		tmp = tmp->next;
	}
}

void	free_extract_result(char **split)
{
	if (!split)
		return ;
	if (split[0])
		free(split[0]);
	if (split[1])
		free(split[1]);
	free(split);
}
