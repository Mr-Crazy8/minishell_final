/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper_4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 15:25:27 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/16 20:09:44 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h" 

t_redir	*creat_redir_list_helper0(char *str1, char *str2)
{
	char	*str;

	if (!str2 || creat_redir_list_helper(str2) != 1)
		return (NULL);
	str = ft_strdup(str2);
	if (!str)
		return (NULL);
	if (strcmp(str1, ">>") == 0)
		return (creat_redir_node(2, str));
	else if (strcmp(str1, "<<") == 0)
		return (creat_redir_node(3, str));
	else if (strcmp(str1, ">") == 0)
		return (creat_redir_node(1, str));
	else if (strcmp(str1, "<") == 0)
		return (creat_redir_node(0, str));
	free(str);
	return (NULL);
}

t_redir	*handle_empty_redirection(char *token)
{
	t_redir	*tmp_node;

	if (strcmp(token, ">") == 0)
		tmp_node = creat_redir_node(1, ft_strdup(""));
	else if (strcmp(token, ">>") == 0)
		tmp_node = creat_redir_node(2, ft_strdup(""));
	else if (strcmp(token, "<") == 0)
		tmp_node = creat_redir_node(0, ft_strdup(""));
	else if (strcmp(token, "<<") == 0)
		tmp_node = creat_redir_node(3, ft_strdup(""));
	else
		tmp_node = NULL;
	return (tmp_node);
}

int	process_single_token(char **split_str, int i, t_redir **tmp_list)
{
	t_redir	*tmp_node;

	if ((strcmp(split_str[i], ">") == 0 || strcmp(split_str[i], ">>") == 0
			|| strcmp(split_str[i], "<") == 0
			|| strcmp(split_str[i], "<<") == 0)
		&& (!split_str[i + 1] || split_str[i + 1][0] == '\0'))
	{
		tmp_node = handle_empty_redirection(split_str[i]);
		if (tmp_node)
			add_redir_back(tmp_list, tmp_node);
		return (i + 1);
	}
	else
	{
		tmp_node = creat_redir_list_helper0(split_str[i], split_str[i + 1]);
		if (tmp_node)
		{
			add_redir_back(tmp_list, tmp_node);
			return (i + 2);
		}
		return (i + 1);
	}
}

t_redir	*process_redirection_tokens(char **split_str)
{
	t_redir	*tmp_list;
	int		i;

	i = 0;
	tmp_list = NULL;
	while (split_str[i])
	{
		i = process_single_token(split_str, i, &tmp_list);
	}
	return (tmp_list);
}

t_redir	*creat_redir_list(char *str)
{
	t_redir	*result;
	char	**split_str;
	char	*redir;

	redir = redir_extracter(str);
	split_str = ft_split_q(redir, ' ');
	if (!split_str)
		return (NULL);
	free(redir);
	result = process_redirection_tokens(split_str);
	free_split_str(split_str);
	return (result);
}
