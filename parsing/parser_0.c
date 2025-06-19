/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 15:39:26 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/19 11:59:59 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	process_redirection_token(char *str, int *i, int *quote_state)
{
	(*i)++;
	if (str[*i] == '<' || str[*i] == '>')
		(*i)++;
	while (str[*i] && str[*i] == ' ')
		(*i)++;
	*quote_state = 0;
	while (str[*i]
		&& ((*quote_state != 0)
			|| (str[*i] != ' ' && str[*i] != '<' && str[*i] != '>')))
	{
		skip_redirections_hp(str[*i], quote_state);
		(*i)++;
	}
	if (*quote_state != 0)
	{
		while (str[*i] && str[*i] != '<' && str[*i] != '>')
			(*i)++;
	}
	return (1);
}

int	skip_redirections(char *str)
{
	int	i;
	int	quote_state;

	quote_state = 0;
	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		skip_redirections_hp(str[i], &quote_state);
		if ((str[i] == '<' || str[i] == '>') && quote_state == 0)
			process_redirection_token(str, &i, &quote_state);
		else if (str[i] == ' ' && quote_state == 0)
			i++;
		else
			break ;
	}
	return (i);
}

void	skip_redirections_hp(char str, int *quote_state)
{
	if (str == '\'')
	{
		if (*quote_state == 0)
			*quote_state = 1;
		else if (*quote_state == 1)
			*quote_state = 0;
	}
	else if (str == '\"')
	{
		if (*quote_state == 0)
			*quote_state = 2;
		else if (*quote_state == 2)
			*quote_state = 0;
	}
}

void	cmd_extracter_hp_0(char str, int *quote_state)
{
	if (str == '\'')
	{
		if (*quote_state == 0)
			*quote_state = 1;
		else if (*quote_state == 1)
			*quote_state = 0;
	}
	else if (str == '\"')
	{
		if (*quote_state == 0)
			*quote_state = 2;
		else if (*quote_state == 2)
			*quote_state = 0;
	}
}

void	cmd_extracter_hp_2(char *str, t_extra_param	*extra_param)
{
	if (str[(*extra_param->i)] == ' ')
	{
		extra_param->result[(*extra_param->result_len)++] = ' ';
		(*extra_param->i)++;
		if (*extra_param->quote_state == 0)
		{
			while (str[(*extra_param->i)] && str[(*extra_param->i)] == ' ')
				(*extra_param->i)++;
		}
	}
	else
		extra_param->result[(*extra_param->result_len)++]
			= str[(*extra_param->i)++];
}
