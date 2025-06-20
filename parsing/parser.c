/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 15:47:25 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/20 12:30:58 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	cmd_extracter_hp_1(char *str, t_extra_param	*extra_param)
{
	char	quote;

	if (*extra_param->quote_state == 0
		&& (str[(*extra_param->i)] == '>' || str[(*extra_param->i)] == '<'))
	{
		if (*extra_param->result_len > 0
			&& extra_param->result[(*extra_param->result_len) - 1] != ' ')
			extra_param->result[(*extra_param->result_len)++] = ' ';
		(*extra_param->i)++;
		if (str[(*extra_param->i)] == '>' || str[(*extra_param->i)] == '<')
			(*extra_param->i)++;
		while (str[(*extra_param->i)] && str[(*extra_param->i)] == ' ')
			(*extra_param->i)++;
		while (str[(*extra_param->i)] && str[(*extra_param->i)] != ' '
			&& str[(*extra_param->i)] != '>' && str[(*extra_param->i)] != '<')
			cmd_extra_helper(str, extra_param, &quote);
	}
}

char	*cmd_extracter_hp_3(char *result, int *result_len, int had_quotes)
{
	char	*trimmed;
	char	*final;
	int		len;

	result[(*result_len)] = '\0';
	if (had_quotes)
	{
		final = ft_strdup(result);
		if (!final)
			return (free(result), NULL);
		free(result);
		return (final);
	}
	trimmed = result;
	while (*trimmed == ' ')
		trimmed++;
	final = ft_strdup(trimmed);
	if (!final)
		return (free(result), NULL);
	len = ft_strlen(final);
	while (len > 0 && final[len - 1] == ' ')
		final[--len] = '\0';
	free(result);
	return (final);
}

t_redir	*creat_redir_node(int type, char *file)
{
	t_redir	*tmp;

	tmp = malloc(sizeof(t_redir));
	if (!tmp)
		return (NULL);
	tmp->file = file;
	tmp->orig_token = ft_strdup(file);
	tmp->ambiguous = 0;
	tmp->type = type;
	tmp->fd = NULL;
	tmp->next = NULL;
	return (tmp);
}

t_cmd	*creat_cmd_node(char *str, t_token *tp, int pipe_out)
{
	t_cmd	*tmp;
	char	*cmd_str;

	tmp = malloc(sizeof(t_cmd));
	if (!tmp)
		return (NULL);
	cmd_str = cmd_extracter(str);
	if (!cmd_str)
	{
		free(tmp);
		return (NULL);
	}
	tmp->args = ft_split_q(cmd_str, ' ');
	tmp->args_befor_quotes_remover = ft_split_q(cmd_str, ' ');
	free(cmd_str);
	if (tmp->args && tmp->args[0])
		tmp->cmd = ft_strdup(tmp->args[0]);
	else
		tmp->cmd = NULL;
	tmp->redirs = creat_redir_list(str);
	tmp->pipe_out = pipe_out;
	tmp->data.exit_status = 0;
	tmp->flag = 0;
	tmp->next = NULL;
	return (tmp);
}

t_cmd	*parser(t_token *token_list)
{
	t_cmd	*cmd_node;
	t_cmd	*cmd_list;
	t_token	*tmp;

	cmd_list = NULL;
	tmp = token_list;
	while (tmp)
	{
		if (strcmp(tmp->tokin, "word_tokin") == 0)
		{
			if (tmp->next && strcmp(tmp->next->tokin, "pipe_token") == 0)
				cmd_node = creat_cmd_node(tmp->data, tmp, 1);
			else
				cmd_node = creat_cmd_node(tmp->data, tmp, 0);
			if (cmd_node)
				add_cmd_back(&cmd_list, cmd_node);
		}
		tmp = tmp->next;
	}
	return (cmd_list);
}
