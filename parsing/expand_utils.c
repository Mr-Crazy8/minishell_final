/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 11:16:14 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/18 14:30:21 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	rebuild_cmd_args(char **new_args, t_cmd *current, char **split,
					t_add_int	*two_numbers)
{
	int	j;
	int	arg_count;

	j = 1;
	arg_count = ft_lint(current->args);
	new_args[two_numbers->number_1] = strdup(split[0]);
	while (j < two_numbers->number_2)
	{
		new_args[two_numbers->number_1 + j] = strdup(split[j]);
		j++;
	}
	j = two_numbers->number_1 + 1;
	while (j < arg_count)
	{
		new_args[j + two_numbers->number_2 - 1] = current->args[j];
		j++;
	}
	new_args[arg_count + two_numbers->number_2 - 1] = NULL;
	free(current->args);
	current->args = new_args;
	return (two_numbers->number_1 + two_numbers->number_2 - 1);
}

void	split_the_rest_helper(t_cmd *current, int *i)
{
	char	**split;
	int		word_count;
	char	**new_args;

	split = split_if_needed(current->args[(*i)]);
	if (split && split[1])
	{
		word_count = ft_lint(split);
		new_args = malloc(sizeof(char *)
				* (ft_lint(current->args) + word_count));
		if (new_args)
		{
			prepare_new_args(new_args, current, (*i));
			(*i) = rebuild_cmd_args(new_args, current, split,
					add_two_int((*i), word_count));
		}
	}
	if (split)
		free_string_array(split);
}

int	split_the_rest_hp(t_cmd *current, int *i)
{
	char	*equals;

	if (!current->args)
		return (0);
	if (!current->cmd || strcmp(current->cmd, "export") != 0)
	{
		(*i) = 0;
		while (current->args && current->args[(*i)])
		{
			equals = strchr(current->args[(*i)], '=');
			split_the_rest_helper(current, i);
			(*i)++;
		}
		return (1);
	}
	return (0);
}

void	split_the_rest(t_cmd *current)
{
	int	i;

	if (split_the_rest_hp(current, &i))
		return ;
	i = 1;
	while (current->args && current->args[i])
	{
		split_the_rest_helper(current, &i);
		i++;
	}
}

void	apply_word_splitting(t_cmd *cmd_list, t_exp_helper *expand)
{
	t_cmd	*current;

	current = cmd_list;
	while (current)
	{
		split_the_rest(current);
		current = current->next;
	}
	if (expand)
	{
		if (expand->expanded)
			free(expand->expanded);
		if (expand->var_name)
			free(expand->var_name);
		if (expand->var_value)
			free(expand->var_value);
		free(expand);
	}
}
