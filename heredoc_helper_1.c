/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 11:55:34 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/20 12:06:39 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*heredoc_delemter(char *orig_token)
{
	char	*new_delemter;

	if (!orig_token)
		return (NULL);
	if (was_delimiter_quoted(orig_token) == 1)
	{
		new_delemter = selective_remove_quotes(orig_token, 1);
		return (new_delemter);
	}
	if (orig_token[0] == '$')
	{
		new_delemter = check_for_doller(orig_token);
		if (check_for_quotes(new_delemter) == 1)
		{
			new_delemter = selective_remove_quotes(orig_token, 1);
			return (new_delemter);
		}
		return (new_delemter);
	}
	return (ft_strdup(orig_token));
}

int	check_for_quotes(char *str)
{
	int	i;
	int	count_signal;
	int	count_duble;

	i = 0;
	count_signal = 0;
	count_duble = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			count_signal++;
		else if (str[i] == '\"')
			count_duble++;
		i++;
	}
	if (count_signal != 0 || count_duble != 0)
		return (1);
	return (0);
}

char	*check_for_doller(char *orig_token)
{
	char	*substr;
	char	*result;
	int		doller;

	doller = 0;
	while (orig_token[doller] != '\0' && orig_token[doller] == '$')
		doller++;
	if (orig_token[doller] != '\0'
		&& (orig_token[doller] == '\'' || orig_token[doller] == '\"'))
	{
		if (doller % 2 == 0)
			return (selective_remove_quotes(orig_token, 1));
		else
		{
			substr = ft_substr(orig_token, 1, ft_strlen(orig_token) - 1);
			result = selective_remove_quotes(substr, 1);
			free(substr);
			return (result);
		}
	}
	return (orig_token);
}

int	was_delimiter_quoted(char *orig_token)
{
	if (orig_token
		&& ((orig_token[0] == '\''
				&& orig_token[ft_strlen(orig_token)-1] == '\'')
			|| (orig_token[0] == '\"'
				&& orig_token[ft_strlen(orig_token)-1] == '\"')))
		return (1);
	return (0);
}

int	*heredoc_opener(void)
{
	char	*random_name;
	int		*fd_heredoc;

	random_name = random_file_name();
	fd_heredoc = malloc(2 * sizeof(int));
	if (!fd_heredoc)
		return (free(random_name), NULL);
	if (random_name)
	{
		fd_heredoc[0] = open(random_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		fd_heredoc[1] = open(random_name, O_CREAT | O_RDONLY, 0644);
		unlink(random_name);
		free(random_name);
	}
	return (fd_heredoc);
}
