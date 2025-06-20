/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:20:59 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/19 17:05:11 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	handle_empty_var(t_cmd *current, t_exp_helper *expand, int *i, int *j)
{
	if ((expand->expanded && expand->expanded[0] == '\0'
			&& strchr(current->args[(*i)], '$')))
	{
		free(current->args[(*i)]);
		free(expand->expanded);
		expand->expanded = NULL;
		shift_arrays_left(current, i, j);
		return (1);
	}
	return (0);
}

void	update_cmd(t_cmd *current)
{
	if (current->args && current->args[0])
	{
		if (current->cmd)
			free(current->cmd);
		current->cmd = ft_strdup(current->args[0]);
		if (!current->cmd)
		{
			write(2, "minishell: memory allocation failed\n", 37);
			exit(1);
		}
	}
}

void	expand_handle_redir(t_redir *redir, t_exp_helper *expand,
			t_env *env, t_add_int *two_number)
{
	t_add_int	*nbr;

	while (redir)
	{
		if (redir->file)
		{
			if (redir->type != 3)
			{
				nbr = add_two_int(two_number->number_1, two_number->number_2);
				if (!nbr)
					return ;
				process_string(redir->file, expand, env, nbr);
				free(redir->file);
				redir->file = expand->expanded;
				expand->expanded = NULL;
			}
		}
		redir = redir->next;
	}
}

void	expander_0(t_cmd *current, t_exp_helper *expand, int *i, int *j)
{
	if (handle_empty_var(current, expand, i, j) == 0)
	{
		free(current->args[(*i)]);
		current->args[(*i)] = expand->expanded;
		expand->expanded = NULL;
		(*i)++;
	}
}

void	expand_handle(t_cmd *cmd_list, t_env *env, int exit_status)
{
	t_cmd			*current;
	t_exp_helper	*expand;
	t_add_int		*number;
	int				i;
	int				j;

	expand = alloc_expand();
	current = cmd_list;
	number = add_two_int(exit_status, cmd_list->pipe_out);
	while (current)
	{
		i = 0;
		while (current->args && current->args[i])
		{
			process_string(current->args[i], expand, env, number);
			expander_0(current, expand, &i, &j);
		}
		update_cmd(current);
		expand_handle_redir(current->redirs, expand, env, number);
		current = current->next;
	}
	free(number);
	apply_word_splitting(cmd_list, expand);
}
