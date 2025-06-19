/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 15:38:23 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/19 11:47:01 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parsing.h"

size_t	count_words_q(char const *s, char c)
{
	size_t	i;
	size_t	n;
	int		quote_state;

	i = 0;
	n = 0;
	quote_state = 0;
	while (s[i])
	{
		while (s[i] == c && quote_state == 0)
			i++;
		if (s[i])
			n++;
		while (s[i])
		{
			count_words_q_hp(s[i], &quote_state);
			if (s[i] == c && quote_state == 0)
				break ;
			i++;
		}
	}
	return (n);
}

void	split_string_q_hp0(char s, int *quote_state)
{
	if (s == '\'')
	{
		if (*quote_state == 0)
			*quote_state = 1;
		else if (*quote_state == 1)
			*quote_state = 0;
	}
	else if (s == '\"')
	{
		if (*quote_state == 0)
			*quote_state = 2;
		else if (*quote_state == 2)
			*quote_state = 0;
	}
}

void	string_helper(char const *s, size_t *i, char c, int *quote_state)
{
	while (s[*i])
	{
		split_string_q_hp0(s[*i], quote_state);
		if (s[*i] == c && *quote_state == 0)
			break ;
		(*i)++;
	}
}

static char	**split_string_q(char const *s, char c, char **split, size_t n)
{
	size_t			i;
	size_t			j;
	size_t			start;
	int				quote_state;
	t_split_helper	*split_paramter;

	i = 0;
	j = 0;
	quote_state = 0;
	split_paramter = split_param(&i, &j, &start);
	if (!split_paramter)
		return (NULL);
	while (s[i] && j < n)
	{
		while (s[i] == c && quote_state == 0)
			i++;
		start = i;
		string_helper(s, &i, c, &quote_state);
		if (!split_string_q_hp1(split, split_paramter, s))
			return (free(split_paramter), free_split_q(split, j));
	}
	return (free(split_paramter), split[j] = NULL, split);
}

char	**ft_split_q(char const *s, char c)
{
	size_t	n;
	char	**split;

	if (s == NULL)
		return (NULL);
	n = count_words_q(s, c);
	split = (char **)malloc((n + 1) * sizeof(char *));
	if (split == NULL)
		return (NULL);
	return (split_string_q(s, c, split, n));
}
