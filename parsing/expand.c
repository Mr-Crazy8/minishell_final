/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:20:59 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/16 11:44:46 by anel-men         ###   ########.fr       */
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

void	process_string(char *str, t_exp_helper *expand, t_env *env,
		t_add_int *two_number)
{
	char	*new_expanded;

	if (!expand_fill_str(expand, str))
	{
		free(expand->expanded);
		expand->expanded = NULL;
		return ;
	}
	while (expand->original[expand->i])
	{
		if (!expand_handle_helper0(expand) && !expand_handle_helper1(expand,
				two_number->exit_status, env, two_number->pipe_out))
		{
			if (!ensure_buffer_space(expand, 1))
			{
				free(expand->expanded);
				expand->expanded = NULL;
				break ;
			}
			expand->expanded[expand->j++] = expand->original[expand->i++];
		}
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

t_add_int	*add_two_int(int exit_status, int pipe_out)
{
	t_add_int	*two_number;

	two_number = malloc(sizeof(t_add_int));
	if (!two_number)
		return (NULL);
	two_number->exit_status = exit_status;
	two_number->pipe_out = pipe_out;
	return (two_number);
}

void	expand_handle(t_cmd *cmd_list, t_env *env, int exit_status)
{
	t_cmd			*current;
	t_exp_helper	*expand;
	t_redir			*redir;
	int				i;
	int				j;

	expand = alloc_expand();
	current = cmd_list;
	while (current)
	{
		i = 0;
		while (current->args && current->args[i])
		{
			process_string(current->args[i], expand, env,
				add_two_int(exit_status, cmd_list->pipe_out));
			if (expand->expanded && expand->expanded[0] == '\0'
				&& strchr(current->args[i], '$'))
			{
				free(current->args[i]);
				free(expand->expanded);
				expand->expanded = NULL;
				if (current->args_befor_quotes_remover
					&& current->args_befor_quotes_remover[i])
				{
					free(current->args_befor_quotes_remover[i]);
					j = i;
					while (current->args_befor_quotes_remover[j + 1])
					{
						current->args_befor_quotes_remover[j]
							= current->args_befor_quotes_remover[j + 1];
						j++;
					}
					current->args_befor_quotes_remover[j] = NULL;
				}
				j = i;
				while (current->args[j + 1])
				{
					current->args[j] = current->args[j + 1];
					j++;
				}
				current->args[j] = NULL;
			}
			else
			{
				free(current->args[i]);
				current->args[i] = expand->expanded;
				expand->expanded = NULL;
				i++;
			}
		}
		if (current->args && current->args[0])
		{
			if (current->cmd)
				free(current->cmd);
			current->cmd = ft_strdup(current->args[0]);
			if (!current->cmd)
			{
				fprintf(stderr, "minishell: memory allocation failed\n");
				exit(1);
			}
		}
		redir = current->redirs;
		while (redir)
		{
			if (redir->file)
			{
				if (redir->type != 3)
				{
					process_string(redir->file, expand, env,
						add_two_int(exit_status, cmd_list->pipe_out));
					free(redir->file);
					redir->file = expand->expanded;
					expand->expanded = NULL;
				}
			}
			redir = redir->next;
		}
		current = current->next;
	}
	apply_word_splitting(cmd_list, expand);
}
