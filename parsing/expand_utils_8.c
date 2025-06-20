/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_8.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 15:08:09 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/20 20:51:49 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	expand_handle_helper1(t_exp_helper *expand, int exit_status,
			t_env *env, int pipe_out)
{
	char	*var;
	int		extracting;
	int		res_adding_var;

	var = NULL;
	if (expand->original[expand->i] == '$' && expand->quote_state != 1)
	{
		expand->i++;
		extracting = extracting_the_key_value(expand, exit_status,
				env, pipe_out);
		if (extracting == 0)
			return (0);
		if (expand->var_value)
		{
			res_adding_var = adding_var_value(expand);
			if (res_adding_var == 0)
				return (0);
		}
		else if (extracting == 1)
			return (1);
		return (1);
	}
	return (0);
}

static int	process_string_loop(t_exp_helper *expand, t_env *env,
		t_add_int *two_number)
{
	while (expand->original[expand->i])
	{
		if (!expand_handle_helper0(expand) && !expand_handle_helper1(expand,
				two_number->number_1, env, two_number->number_2))
		{
			if (!ensure_buffer_space(expand, 1))
			{
				free(expand->expanded);
				expand->expanded = NULL;
				return (0);
			}
			expand->expanded[expand->j++] = expand->original[expand->i++];
		}
	}
	return (1);
}

void	process_string(char *str, t_exp_helper *expand, t_env *env,
		t_add_int *two_number)
{
	char	*new_expanded;

	if (!expand_fill_str(expand, str))
	{
		free(expand->expanded);
		expand->expanded = NULL;
		free(two_number);
		return ;
	}
	if (!process_string_loop(expand, env, two_number))
	{
		free(two_number);
		return ;
	}
	if (expand->expanded)
		expand->expanded[expand->j] = '\0';
	new_expanded = ft_strtrim(change_space((expand->expanded)), " ");
	free(expand->expanded);
	expand->expanded = new_expanded;
}

t_exp_helper	*alloc_expand(void)
{
	t_exp_helper	*expand;

	expand = malloc(sizeof(t_exp_helper));
	if (!expand)
	{
		fprintf(stderr, "minishell: memory allocation failed\n");
		exit(1);
	}
	expand->buffer_size = 0;
	expand->expanded = NULL;
	expand->var_name = NULL;
	expand->var_value = NULL;
	expand->had_removed_var = 0;
	return (expand);
}

char *change_space(char *str)
{
    int i = 0;
    int quote_state = 0;
    
    while(str && str[i])
    {
        if (str[i] == '\'')
        {
            if (quote_state == 0)
                quote_state = 1;
            else if (quote_state == 1)
                quote_state = 0;
        }
        else if (str[i] == '"')
        {
            if (quote_state == 0)
                quote_state = 2;
            else if (quote_state == 2)
                quote_state = 0;
        }
        if (str[i] >= 9 && str[i] <=13 && quote_state == 0)
            str[i] = ' ';
        i++;
    }
    return str;
}
