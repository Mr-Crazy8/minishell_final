/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 19:41:27 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/19 11:39:14 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	process_redir_hp(char *str, int *start, int *i, int *after_operator)
{
	if (str[(*start)] == '>' && str[(*i)] == '>')
		(*i)++;
	else if (str[(*start)] == '<' && str[(*i)] == '<')
		(*i)++;
	(*after_operator) = (*i);
	while (str[(*i)] == ' ')
		(*i)++;
}

void	process_redir_helper(char str, int *quote_state)
{
	if (str == '\'')
	{
		if ((*quote_state) == 0)
			(*quote_state) = 1;
		else if ((*quote_state) == 1)
			(*quote_state) = 0;
	}
	else if (str == '\"')
	{
		if ((*quote_state) == 0)
			(*quote_state) = 2;
		else if ((*quote_state) == 2)
			(*quote_state) = 0;
	}
}

void	cmd_extra_helper(char *str, t_extra_param	*extra_param, char *quote)
{
	if (str[(*extra_param->i)] == '\''
		|| str[(*extra_param->i)] == '\"')
	{
		(*quote) = str[(*extra_param->i)++];
		while (str[(*extra_param->i)]
			&& str[(*extra_param->i)] != (*quote))
			(*extra_param->i)++;
		if (str[(*extra_param->i)])
			(*extra_param->i)++;
	}
	else
		(*extra_param->i)++;
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
	char			*result;
	int				i;
	int				result_len;
	int				quote_state;
	t_extra_param	*extra_param;

	result = init_cmd_buffer(str, &i, &result_len, &quote_state);
	if (!result)
		return (NULL);
	extra_param = init_extra_param(result, &i, &result_len, &quote_state);
	if (!extra_param)
		return (free(result), NULL);
	while (str[i])
	{
		cmd_extracter_hp_0(str[i], &quote_state);
		if (quote_state != 0)
			extra_param->had_quotes = 1;
		if (quote_state == 0 && (str[i] == '>' || str[i] == '<'))
			cmd_extracter_hp_1(str, extra_param);
		else
			cmd_extracter_hp_2(str, extra_param);
	}
	result = cmd_extracter_hp_3(result, extra_param->result_len,
			extra_param->had_quotes);
	return (free(extra_param), result);
}
