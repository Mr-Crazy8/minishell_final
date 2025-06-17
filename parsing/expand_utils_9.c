/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_9.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 15:16:17 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/16 15:18:42 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	**split_if_needed(char *str)
{
	int		i;
	int		count;
	char	**result;
	char	*trimmed_str;

	i = 0;
	if (!str || !*str)
		return (NULL);
	trimmed_str = ft_strtrim(str, " ");
	if (!trimmed_str)
		return (NULL);
	if (!strchr(trimmed_str, ' ') && !strchr(trimmed_str, '\t')
		&& !strchr(trimmed_str, '\n'))
		return (free(trimmed_str), NULL);
	result = ft_split_q(trimmed_str, ' ');
	free(trimmed_str);
	count = 0;
	if (result)
	{
		while (result[count])
			count++;
	}
	if (count <= 1)
		return (free_string_array(result), NULL);
	return (result);
}

int	ft_lint(char **str)
{
	int	i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

void	prepare_new_args(char **new_args, t_cmd *current, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		new_args[j] = current->args[j];
		j++;
	}
	free(current->args[i]);
}
