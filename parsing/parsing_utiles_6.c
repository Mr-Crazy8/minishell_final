/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utiles_6.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 14:38:22 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/21 16:42:38 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*handle_split_processing(char **split, char *str)
{
	char	*new_key;
	char	*result;

	new_key = ft_strdup(split[0]);
	if (!new_key)
	{
		free_extract_result(split);
		return (NULL);
	}
	new_key = plus_checker(new_key);
	if ((((strchr(split[0], '\'') == NULL && strchr(split[0], '\"') == NULL)
				&& strchr(split[0], '$') == NULL))
		&& is_valid_key(new_key) == 0)
		result = process_valid_assignment(split, new_key);
	else
	{
		result = ft_strdup(str);
		if (!result)
			return (cleanup_split_key(split, new_key), NULL);
	}
	cleanup_split_key(split, new_key);
	return (result);
}

char	*split_helper(char *str, char *befor, int exp)
{
	char	**split;
	char	*result;

	if (exp != 1)
		return (NULL);
	if (strchr(str, '=') == NULL)
	{
		result = ft_strdup(str);
		return (result);
	}
	split = extract(str);
	if (split == NULL)
	{
		result = ft_strdup(str);
		return (result);
	}
	result = handle_split_processing(split, str);
	return (result);
}

t_env	*find_shlvl_node(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (tmp->key && strcmp(tmp->key, "SHLVL") == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

void	update_shlvl_value(t_env *shlvl_node)
{
	int	shl_vl;

	shl_vl = 0;
	if (shlvl_node->value && shlvl_node->value[0] != '\0')
	{
		shl_vl = atoi(shlvl_node->value);
		free(shlvl_node->value);
		shlvl_node->value = NULL;
	}
	shl_vl++;
	shlvl_node->value = ft_itoa(shl_vl);
	if (!shlvl_node->value)
		shlvl_node->value = strdup("1");
}

t_env	*create_shlvl_node(void)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = strdup("SHLVL");
	new_node->value = strdup("1");
	new_node->is_not_active = 0;
	new_node->next = NULL;
	if (!new_node->key || !new_node->value)
	{
		free(new_node->key);
		free(new_node->value);
		free(new_node);
		return (NULL);
	}
	return (new_node);
}
