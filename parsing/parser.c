/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 15:47:25 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/18 14:16:13 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	cmd_extracter_hp_1(char *str, int *quote_state, int *i,
			int *result_len, char *result)
{
	char	quote;

	if (*quote_state == 0 && (str[(*i)] == '>' || str[(*i)] == '<'))
	{
		if (*result_len > 0 && result[(*result_len) - 1] != ' ')
			result[(*result_len)++] = ' ';
		(*i)++;
		if (str[(*i)] == '>' || str[(*i)] == '<')
			(*i)++;
		while (str[(*i)] && str[(*i)] == ' ')
			(*i)++;
		while (str[(*i)] && str[(*i)] != ' '
			&& str[(*i)] != '>' && str[(*i)] != '<')
		{
			if (str[(*i)] == '\'' || str[(*i)] == '\"')
			{
				quote = str[(*i)++];
				while (str[(*i)] && str[(*i)] != quote)
					(*i)++;
				if (str[(*i)])
					(*i)++;
			}
			else
				(*i)++;
		}
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

// char	*cmd_extracter(char *str)
// {
// 	char	*result;
// 	int		i;
// 	int		result_len;
// 	int		quote_state;
// 	int		had_quotes;

// 	had_quotes = 0;
// 	result = init_cmd_buffer(str, &i, &result_len, &quote_state);
// 	if (!result)
// 		return (NULL);
// 	while (str[i])
// 	{
// 		cmd_extracter_hp_0(str[i], &quote_state);
// 		if (quote_state != 0)
// 			had_quotes = 1;
// 		if (quote_state == 0 && (str[i] == '>' || str[i] == '<'))
// 			cmd_extracter_hp_1(str, &quote_state, &i, &result_len, result);
// 		else
// 			cmd_extracter_hp_2(str, &i, result, &result_len, quote_state);
// 	}
// 	return (cmd_extracter_hp_3(result, &result_len, had_quotes));
// }

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
