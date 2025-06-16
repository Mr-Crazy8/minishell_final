/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayoakouh <ayoakouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:33:17 by ayoakouh          #+#    #+#             */
/*   Updated: 2025/06/10 13:10:28 by ayoakouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	remove_it(t_env *env_list, char *variable)
{
	t_env	*tmp;
	t_env	*to_dele;

	tmp = env_list;
	while (tmp->next)
	{
		if (ft_strcmp(tmp->next->key, variable) == 0)
		{
			to_dele = tmp->next;
			tmp->next = tmp->next->next;
			free(to_dele->key);
			free(to_dele->value);
			free(to_dele);
			return (1);
		}
		tmp = tmp->next;
	}
}

int	remove_env(t_env **env_list, char *variable)
{
	t_env	*tmp;
	t_env	*to_delete;

	if (!env_list || !*env_list)
		return (0);
	if (ft_strcmp((*env_list)->key, variable) == 0)
	{
		to_delete = *env_list;
		*env_list = (*env_list)->next;
		free(to_delete->key);
		free(to_delete->value);
		free(to_delete);
		return (1);
	}
	if (remove_it(*env_list, variable) == 1)
		return (1);
	return (0);
}

int	ft_unset(t_env **env_list, char **variables)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	if (!variables)
		return (0);
	while (variables[i])
	{
		if (is_valid_key(variables[i]))
		{
			printf("unset: `%s': not a valid identifier\n", variables[i]);
			status = 1;
			i++;
			continue ;
		}
		remove_env(env_list, variables[i]);
		i++;
	}
	return (status);
}
	// tmp = *env_list;
	// while (tmp->next)
	// {
	// 	if (ft_strcmp(tmp->next->key, variable) == 0)
	// 	{
	// 		to_delete = tmp->next;
	// 		tmp->next = tmp->next->next;
	// 		del(to_delete->key);
	// 		del(to_delete->value);
	// 		free(to_delete);
	// 		return (1);
	// 	}
	// 	tmp = tmp->next;
	// }