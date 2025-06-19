/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struc_maker.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 17:44:26 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/19 11:41:23 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_pre_cmd	*init_precmd(char *input, int *i, char *result, int *j)
{
	t_pre_cmd	*pre_cmd;

	pre_cmd = malloc(sizeof(t_pre_cmd));
	if (!pre_cmd)
		return (NULL);
	pre_cmd->input = input;
	pre_cmd->result = result;
	pre_cmd->i = i;
	pre_cmd->j = j;
	return (pre_cmd);
}

t_add_int	*add_two_int(int number_1, int number_2)
{
	t_add_int	*two_number;

	two_number = malloc(sizeof(t_add_int));
	if (!two_number)
		return (NULL);
	two_number->number_1 = number_1;
	two_number->number_2 = number_2;
	return (two_number);
}

t_quote_params	*add_quote_params(char *new_str, int remove_mode)
{
	t_quote_params	*params;

	params = malloc(sizeof(t_quote_params));
	if (!params)
		return (NULL);
	params->quote_state = 0;
	params->new_str = new_str;
	params->j = 0;
	params->remove_mode = remove_mode;
	params->in_opposite_quote = 0;
	params->i = 0;
	return (params);
}

t_extra_param	*init_extra_param(char *result,
	int *i, int *result_len, int *quote_state)
{
	t_extra_param	*extra_param;

	extra_param = malloc(sizeof(t_extra_param));
	if (!extra_param)
		return (NULL);
	extra_param->result = result;
	extra_param->i = i;
	extra_param->result_len = result_len;
	extra_param->quote_state = quote_state;
	extra_param->had_quotes = 0;
	return (extra_param);
}

int	split_string_q_hp1(char **split,
		t_split_helper	*split_paramter, char const *s)
{
	size_t	len;

	len = (*split_paramter->i) - (*split_paramter->start);
	if (len > 0)
	{
		split[(*split_paramter->j)] = (char *)malloc(len + 1);
		if (!split[(*split_paramter->j)])
			return (free(split_paramter), 0);
		ft_memcpy_q(split[(*split_paramter->j)],
			&s[(*split_paramter->start)], len);
		split[(*split_paramter->j)][len] = '\0';
		(*split_paramter->j)++;
	}
	return (1);
}
