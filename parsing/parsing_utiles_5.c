/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utiles_5.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 14:33:26 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/21 16:32:50 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	cleanup_joins(char *join1, char *join2, char *join3)
{
	free(join3);
	free(join1);
	free(join2);
}

void	clean_hp(char *join1, char *join2, char **split, char *new_key)
{
	free(join1);
	free(join2);
	cleanup_split_key(split, new_key);
}

char	*create_quoted_value(char *value, char **split, char *new_key)
{
	char	*join1;
	char	*join2;
	char	*join3;
	char	*result;

	join1 = ft_strjoin("\"", value);
	if (!join1)
		return (cleanup_split_key(split, new_key), NULL);
	join2 = ft_strjoin(join1, "\"");
	if (!join2)
		return (free(join1), cleanup_split_key(split, new_key), NULL);
	join3 = ft_strjoin("=", join2);
	if (!join3)
		return (clean_hp(join1, join1, split, new_key), NULL);
	result = ft_strjoin(split[0], join3);
	if (!result)
	{
		cleanup_joins(join1, join2, join3);
		cleanup_split_key(split, new_key);
		return (NULL);
	}
	cleanup_joins(join1, join2, join3);
	return (result);
}

char	*create_simple_assignment(char *key, char *value, char **split,
			char *new_key)
{
	char	*join3;
	char	*result;

	join3 = ft_strjoin(key, "=");
	if (!join3)
		return (cleanup_split_key(split, new_key), NULL);
	result = ft_strjoin(join3, value);
	if (!result)
		return (free(join3), cleanup_split_key(split, new_key), NULL);
	free(join3);
	return (result);
}

char	*process_valid_assignment(char **split, char *new_key)
{
	char	*result;

	if (split && split[1] && strchr(split[1], '$') != NULL
		&& strchr(split[1], '\'') == NULL)
		result = create_quoted_value(split[1], split, new_key);
	else
		result = create_simple_assignment(split[0], split[1], split, new_key);
	return (result);
}
