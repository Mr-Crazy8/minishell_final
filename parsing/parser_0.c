/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 15:39:26 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/16 16:25:08 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	skip_redirections(char *str)
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
		{
			i++;
			if (str[i] == '<' || str[i] == '>')
				i++;
			while (str[i] && str[i] == ' ')
				i++;
			quote_state = 0;
			while (str[i]
				&& ((quote_state != 0)
					|| (str[i] != ' ' && str[i] != '<' && str[i] != '>')))
			{
				skip_redirections_hp(str[i], &quote_state);
				i++;
			}
			if (quote_state != 0)
			{
				while (str[i] && str[i] != '<' && str[i] != '>')
					i++;
			}
		}
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

void	cmd_extracter_hp_2(char *str, int *i, char *result,
			int *result_len, int quote_state)
{
	if (str[(*i)] == ' ')
	{
		result[(*result_len)++] = ' ';
		(*i)++;
		if (quote_state == 0)
		{
			while (str[(*i)] && str[(*i)] == ' ')
				(*i)++;
		}
	}
	else
		result[(*result_len)++] = str[(*i)++];
}

char	*init_cmd_buffer(char *str, int *i, int *result_len, int *quote_state)
{
	char	*result;

	*i = 0;
	*quote_state = 0;
	*result_len = 0;
	if (!str)
		return (NULL);
	result = (char *)malloc(strlen(str) + 1);
	if (!result)
		return (NULL);
	result[0] = '\0';
	*i = skip_redirections(str);
	return (result);
}

char	*cmd_extracter(char *str)
{
	char	*result;
	int		i;
	int		result_len;
	int		quote_state;
	int		had_quotes;

	had_quotes = 0;
	result = init_cmd_buffer(str, &i, &result_len, &quote_state);
	if (!result)
		return (NULL);
	while (str[i])
	{
		cmd_extracter_hp_0(str[i], &quote_state);
		if (quote_state != 0)
			had_quotes = 1;
		if (quote_state == 0 && (str[i] == '>' || str[i] == '<'))
			cmd_extracter_hp_1(str, &quote_state, &i, &result_len, result);
		else
			cmd_extracter_hp_2(str, &i, result, &result_len, quote_state);
	}
	return (cmd_extracter_hp_3(result, &result_len, had_quotes));
}
