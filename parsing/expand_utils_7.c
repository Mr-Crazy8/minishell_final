/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_7.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 10:55:05 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/16 10:55:35 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	var_name_extract(t_exp_helper *expand)
{
	if (ft_isdigiti(expand->original[expand->i]))
	{
		expand->i++;
		return (2);
	}
	else if ((expand->original[expand->i] == '"'
			|| expand->original[expand->i] == '\'')
		&& expand->quote_state == 0)
	{
		if (expand->i >= 2 && expand->original[expand->i - 2] == '$')
		{
			if (!ensure_buffer_space(expand, 1))
				return (0);
			expand->expanded[expand->j++] = '$';
			return (1);
		}
		return (1);
	}
	else
	{
		while (expand->original[expand->i]
			&& is_valid_var_char(expand->original[expand->i]))
			expand->i++;
		return (2);
	}
}

int	alloc_var_name(t_exp_helper *expand)
{
	size_t	var_len;

	var_len = expand->i - expand->start;
	if (var_len > SIZE_MAX - 1)
	{
		write(2, "minishell: ", 12);
		write(2, "memory allocation failed: variable name too long\n", 50);
		return (0);
	}
	if (var_len == 0)
	{
		if (!ensure_buffer_space(expand, 1))
			return (0);
		expand->expanded[expand->j++] = '$';
		return (1);
	}
	expand->var_name = malloc(var_len + 1);
	if (!expand->var_name)
	{
		fprintf(stderr, "minishell: memory allocation failed\n");
		exit(1);
	}
	memcpy(expand->var_name, expand->original + expand->start, var_len);
	expand->var_name[var_len] = '\0';
	return (2);
}

void	value_extracter(t_exp_helper *expand, t_env *env)
{
	char	*var;
	char	*lk_var;

	var = NULL;
	lk_var = NULL;
	if (is_valid_key(expand->var_name) != 1)
	{
		lk_var = lookup_variable(expand->var_name, env);
		if (lk_var != NULL)
		{
			var = chenger(ft_strdup(lk_var));
			free(lk_var);
		}
	}
	if (var != NULL)
		expand->var_value = var;
	free(expand->var_name);
	expand->var_name = NULL;
}

int	adding_var_value(t_exp_helper *expand)
{
	size_t	len;

	len = strlen(expand->var_value);
	if (len > SIZE_MAX - expand->j)
	{
		write(2, "minishell: memory allocation failed: buffer overflow\n", 54);
		free(expand->var_value);
		expand->var_value = NULL;
		return (0);
	}
	if (!ensure_buffer_space(expand, len))
	{
		free(expand->var_value);
		expand->var_value = NULL;
		return (0);
	}
	memcpy(expand->expanded + expand->j, expand->var_value, len);
	expand->j += len;
	free(expand->var_value);
	expand->var_value = NULL;
	return (1);
}

int	extracting_the_key_value(t_exp_helper *expand, int exit_status,
	t_env *env, int pipe_out)
{
	int	extract_result;
	int	alloc_result;

	if (helper3(expand, exit_status, pipe_out) == 0)
	{
		expand->start = expand->i;
		extract_result = var_name_extract(expand);
		if (extract_result == 1)
			return (1);
		else if (extract_result == 0)
			return (0);
		alloc_result = alloc_var_name(expand);
		if (alloc_result == 0)
			return (0);
		else if (alloc_result == 1)
			return (1);
		value_extracter(expand, env);
		return (2);
	}
	return (1);
}