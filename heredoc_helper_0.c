/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Heredoc_Expansion_helper.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 11:51:14 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/20 11:53:29 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*add(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	size_t	i;
	size_t	j;
	char	*s3;

	i = 0;
	j = 0;
	s1_len = strlen(s1);
	s2_len = strlen(s2);
	s3 = (char *)malloc((s1_len + s2_len + 1) * sizeof(char));
	if (s3 == NULL)
		return (NULL);
	while (i < s1_len)
	{
		s3[i] = s1[i];
		i++;
	}
	while (j < s2_len)
		s3[i++] = s2[j++];
	s3[i] = '\0';
	return (s3);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*s3;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (strdup(s2));
	if (!s2)
		return (strdup(s1));
	s3 = add(s1, s2);
	return (s3);
}

void	doc_expand(char *str, t_exp_helper *expand,
			t_env *env, int exit_status)
{
	if (!expand_fill_str(expand, str))
		return ;
	while (expand->original[expand->i])
	{
		if (!expand_handle_helper0(expand)
			&& !expand_handle_helper1(expand, exit_status, env, 0))
			expand->expanded[expand->j++] = expand->original[expand->i++];
	}
	expand->expanded[expand->j] = '\0';
}

char	*process_heredoc_epxand(char *line, t_env *env,
			int exit_status, char *orig_delimiter)
{
	t_exp_helper	*expand;
	char			*result;

	if (was_delimiter_quoted(orig_delimiter)
		|| check_for_quotes(orig_delimiter))
		return (ft_strdup(line));
	expand = malloc(sizeof(t_exp_helper));
	if (!expand)
		return (NULL);
	doc_expand(line, expand, env, exit_status);
	result = expand->expanded;
	free(expand);
	return (result);
}

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
