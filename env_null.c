/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_null.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:58:55 by ayoakouh          #+#    #+#             */
/*   Updated: 2025/06/20 16:11:56 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*creat_new_env(char *key, char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = key;
	new_node->value = value;
	if (ft_strcmp(key, "PATH") == 0)
		new_node->for_path = 1;
	new_node->next = NULL;
	return (new_node);
}

t_env	*env_null(void)
{
	t_env	*tmp;
	t_env	*list;

	list = NULL;
	tmp = creat_new_env(
			ft_strdup("PATH"),
			ft_strdup("/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:."));
	if (tmp == NULL)
		{
			return NULL;
		}
	tmp->for_path = 1;
	ft_lstadd_back(&list, tmp);
	ft_lstadd_back(&list, creat_new_env(ft_strdup("OLDPWD"), NULL));
	ft_lstadd_back(&list, creat_new_env(ft_strdup("PWD"), getcwd(NULL, 0)));
	ft_lstadd_back(&list, creat_new_env(ft_strdup("SHLVL"), ft_strdup("1")));
	return (list);
}

void	value_empty(t_env **env_list, char *key)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = strdup(key);
	new_node->value = NULL;
	new_node->is_not_active = 1;
	new_node->next = NULL;
	ft_lstadd_back(env_list, new_node);
}
	// tmp = *env_list;
	// while(tmp)
	// {
	// 	if (strcmp(key, tmp->key) == 0)
	// 	{
	// 		free(tmp->value);
	// 		tmp->value = strdup(value);
	// 		tmp->is_not_active = 1;
	// 		return;
	// 	}
	// 	tmp = tmp->next;
	// }

// void value_empty(t_env **env_list, char *key, char *value)
// {
// 	// t_env *tmp;
// 	t_env *new_node;

// 	// tmp = *env_list;
// 	// while(tmp)
// 	// {
// 	// 	if (strcmp(key, tmp->key) == 0)
// 	// 	{
// 	// 		free(tmp->value);
// 	// 		tmp->value = strdup(value);
// 	// 		tmp->is_not_active = 1;
// 	// 		return;
// 	// 	}
// 	// 	tmp = tmp->next;
// 	// }
// 	new_node = malloc(sizeof(t_env));
// 	if(!new_node)
// 		return;
// 	new_node->key = strdup(key);
// 	new_node->value = strdup(value);
// 	new_node->is_not_active = 1;
// 	new_node->next = NULL;
// 	ft_lstadd_back(env_list, new_node);
// }