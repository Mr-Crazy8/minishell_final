/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helper_0.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:21:05 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/16 20:19:48 by anel-men         ###   ########.fr       */
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

char	*selective_remove_quotes(char *str, int remove_mode)
{
	char	*new_str;
	int		i;
	int		j;
	int		quote_state;
	int		in_opposite_quote;

	new_str = allocate_and_init(str, &i, &j, &quote_state);
	if (!new_str)
		return (NULL);
	while (str[i])
	{
		in_opposite_quote = 0;
		if (selective_hp(str, &i, &quote_state) == 1)
		{
			new_str[j++] = str[i++];
			in_opposite_quote = 1;
		}
		else if (!in_opposite_quote
			&& !process_quote_char(str[i], &quote_state,
				new_str, &j, remove_mode))
			new_str[j++] = str[i++];
		else
			i++;
	}
	return (new_str[j] = '\0', new_str);
}

void	processed_cmd(t_cmd *current, char	*processed)
{
	free(current->cmd);
	current->cmd = processed;
}

void	process_quotes_for_cmd_hp(t_cmd *current, int *i, int remove_mode)
{
	char	*processed;

	if (current->args)
	{
		*i = 0;
		while (current->args[*i])
		{
			processed = selective_remove_quotes(
					current->args[*i], remove_mode);
			if (processed)
			{
				free(current->args[*i]);
				current->args[*i] = processed;
			}
			(*i)++;
		}
	}
	if (current->cmd)
	{
		*i = 0;
		processed = selective_remove_quotes(current->cmd, remove_mode);
		if (processed)
			processed_cmd(current, processed);
	}
}

void	processed_redir(t_redir *redir, char *processed)
{
	free(redir->file);
	redir->file = processed;
}
