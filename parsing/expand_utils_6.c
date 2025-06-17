/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_6.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 11:51:19 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/16 15:18:11 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	ensure_buffer_space_hp(t_exp_helper *expand)
{
	size_t	original_len;

	if (!expand->buffer_size)
	{
		original_len = ft_strlen(expand->original);
		if (original_len > SIZE_MAX / 2 - 1024)
		{
			write(2, "minishell: \n", 13);
			write(2, "memory allocation failed: input too large\n", 43);
			return (0);
		}
		expand->buffer_size = original_len * 2 + 1024;
	}
	return (1);
}

int	ensure_buffer_space(t_exp_helper *expand, size_t additional_needed)
{
	char	*new_buffer;
	size_t	new_size;

	if (!ensure_buffer_space_hp(expand))
		return (0);
	if (expand->j + additional_needed >= expand->buffer_size)
	{
		new_size = expand->buffer_size * 2;
		if (new_size < expand->j + additional_needed + 1)
			new_size = expand->j + additional_needed + 1024;
		new_buffer = ft_calloc(1, new_size);
		if (!new_buffer)
		{
			write(2, "minishell: memory allocation failed\n", 37);
			exit(1);
		}
		if (expand->expanded)
		{
			memcpy(new_buffer, expand->expanded, expand->j);
			free(expand->expanded);
		}
		expand->expanded = new_buffer;
		expand->buffer_size = new_size;
	}
	return (1);
}

void	shift_arrays_left(t_cmd *current, int *i, int *j)
{
	if (current->args_befor_quotes_remover
		&& current->args_befor_quotes_remover[(*i)])
	{
		free(current->args_befor_quotes_remover[(*i)]);
		(*j) = (*i);
		while (current->args_befor_quotes_remover[(*j) + 1])
		{
			current->args_befor_quotes_remover[(*j)]
				= current->args_befor_quotes_remover[(*j) + 1];
			(*j)++;
		}
		current->args_befor_quotes_remover[(*j)] = NULL;
	}
	(*j) = (*i);
	while (current->args[(*j) + 1])
	{
		current->args[(*j)] = current->args[(*j) + 1];
		(*j)++;
	}
	current->args[(*j)] = NULL;
}

int	process_quote_char(char c, int *quote_state,
	char *new_str, int *j, int remove_mode)
{
	if (c == '\'')
	{
		if (*quote_state == 0)
			*quote_state = 1;
		else if (*quote_state == 1)
			*quote_state = 0;
		if (remove_mode == 0)
			new_str[(*j)++] = c;
		return (1);
	}
	else if (c == '\"')
	{
		if (*quote_state == 0)
			*quote_state = 2;
		else if (*quote_state == 2)
			*quote_state = 0;
		if (remove_mode == 0)
			new_str[(*j)++] = c;
		return (1);
	}
	return (0);
}

char	*allocate_and_init(char *str, int *i, int *j, int *quote_state)
{
	char	*new_str;

	*i = 0;
	*j = 0;
	*quote_state = 0;
	if (!str)
		return (NULL);
	new_str = malloc(strlen(str) + 1);
	if (!new_str)
		return (NULL);
	return (new_str);
}
