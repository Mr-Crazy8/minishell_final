/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helper_0.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:21:05 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/19 11:28:56 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	selective_hp(char *str, int *i, int *quote_state)
{
	if ((str[(*i)] == '\'' && *quote_state == 2)
		|| (str[(*i)] == '\"' && *quote_state == 1))
	{
		return (1);
	}
	return (0);
}

char	*select_rm_hp(t_quote_params	*params, char *new_str, int j)
{
	return (free(params), new_str[j] = '\0', new_str);
}

char	*selective_remove_quotes(char *str, int remove_mode)
{
	char			*new_str;
	t_quote_params	*params;

	new_str = allocate_and_init(str);
	if (!new_str)
		return (NULL);
	params = add_quote_params(new_str, remove_mode);
	if (!params)
		return (free(new_str), NULL);
	while (str[params->i])
	{
		params->in_opposite_quote = 0;
		if (selective_hp(str, &params->i, &params->quote_state) == 1)
		{
			new_str[params->j++] = str[params->i++];
			params->in_opposite_quote = 1;
		}
		else if (!params->in_opposite_quote
			&& !process_quote_char(str[params->i], params))
			params->new_str[(params->j)++] = str[params->i++];
		else
			params->i++;
	}
	return (select_rm_hp(params, new_str, params->j));
}

void	processed_cmd(t_cmd *current, char	*processed)
{
	free(current->cmd);
	current->cmd = processed;
}

void	processed_redir(t_redir *redir, char *processed)
{
	free(redir->file);
	redir->file = processed;
}
