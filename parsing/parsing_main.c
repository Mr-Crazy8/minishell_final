/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 16:06:35 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/18 17:46:31 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	preprocess_cmd_hp_0(t_pre_cmd *pre_cmd, int *in_quotes)
{
	if (pre_cmd->input[(*pre_cmd->i)] == '\'' && *in_quotes != 2)
	{
		if (*in_quotes == 1)
			*in_quotes = 0;
		else
			*in_quotes = 1;
		pre_cmd->result[(*pre_cmd->j)++] = pre_cmd->input[(*pre_cmd->i)++];
		return (1);
	}
	else if (pre_cmd->input[(*pre_cmd->i)] == '\"' && *in_quotes != 1)
	{
		if (*in_quotes == 2)
			*in_quotes = 0;
		else
			*in_quotes = 2;
		pre_cmd->result[(*pre_cmd->j)++] = pre_cmd->input[(*pre_cmd->i)++];
		return (1);
	}
	return (0);
}

void	preprocess_cmd_hp_1(t_pre_cmd *pre_cmd, int *in_quotes)
{
	if (pre_cmd->input[(*pre_cmd->i)] == '>'
		|| pre_cmd->input[(*pre_cmd->i)] == '<')
	{
		if ((*pre_cmd->j) > 0 && pre_cmd->result[(*pre_cmd->j) - 1] != ' '
			&& pre_cmd->result[(*pre_cmd->j) - 1] != '>'
			&& pre_cmd->result[(*pre_cmd->j) - 1] != '<')
			pre_cmd->result[(*pre_cmd->j)++] = ' ';
		pre_cmd->result[(*pre_cmd->j)++] = pre_cmd->input[(*pre_cmd->i)++];
		if (pre_cmd->input[(*pre_cmd->i)] == '>'
			|| pre_cmd->input[(*pre_cmd->i)] == '<')
			pre_cmd->result[(*pre_cmd->j)++] = pre_cmd->input[(*pre_cmd->i)++];
		if (pre_cmd->input[(*pre_cmd->i)]
			&& pre_cmd->input[(*pre_cmd->i)] != ' ')
			pre_cmd->result[(*pre_cmd->j)++] = ' ';
	}
	else
		pre_cmd->result[(*pre_cmd->j)++] = pre_cmd->input[(*pre_cmd->i)++];
}

int	preprocess_cmd_hp_2(t_pre_cmd *pre_cmd, int *in_quotes)
{
	if (*in_quotes)
	{
		pre_cmd->result[(*pre_cmd->j)++] = pre_cmd->input[(*pre_cmd->i)++];
		return (1);
	}
	return (0);
}

char	*preprocess_command(char *input)
{
	int			i;
	int			j;
	int			in_quotes;
	char		*result;
	t_pre_cmd	*pre_cmd;

	i = 0;
	j = 0;
	in_quotes = 0;
	if (!input)
		return (NULL);
	result = malloc(strlen(input) * 3 + 1);
	if (!result)
		return (NULL);
	pre_cmd = init_precmd(input, &i, result, &j);
	if (!pre_cmd)
		return (free(result), NULL);
	process_input_characters(pre_cmd, &in_quotes);
	pre_cmd->result[*pre_cmd->j] = '\0';
	result = pre_cmd->result;
	free(pre_cmd);
	return (result);
}

void	process_input_characters(t_pre_cmd *pre_cmd, int *in_quotes)
{
	while (pre_cmd->input[*pre_cmd->i])
	{
		if (preprocess_cmd_hp_0(pre_cmd, in_quotes))
			continue ;
		if (preprocess_cmd_hp_2(pre_cmd, in_quotes))
			continue ;
		preprocess_cmd_hp_1(pre_cmd, in_quotes);
	}
}
