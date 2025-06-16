/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayoakouh <ayoakouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:48:53 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/15 18:16:23 by ayoakouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;
	size_t				i;

	if (dest == src)
		return (dest);
	d = (unsigned char *)dest;
	s = (const unsigned char *)src;
	i = 0;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}

t_env	*env_node_maker(char *env[])
{
	t_env	*env_node;

	env_node = malloc(sizeof(t_env));
	if (!env_node)
		return (NULL);
	if (!env || !env[0])
		env_node->key = NULL;
	else
		env_node->key = ft_strdup(env[0]);
	if (!env || !env[1])
		env_node->value = NULL;
	else
		env_node->value = ft_strdup(env[1]);
	env_node->is_not_active = 0;
	env_node->next = NULL;
	return (env_node);
}

t_env	*rmove_value(t_env *env_list)
{
	t_env	*tmp;

	tmp = env_list;
	if (!env_list)
		return (NULL);
	while (tmp)
	{
		if (ft_strcmp(tmp->key, "OLDPWD") == 0)
		{
			if (tmp->value)
			{
				free(tmp->value);
				tmp->value = NULL;
			}
			tmp->is_not_active = 1;
		}
		tmp = tmp->next;
	}
	return (env_list);
}

t_env	*env_maker(char *env[], t_env **env_struct)
{
	t_env	*env_list;
	int		i;
	int		j;
	char	**split;

	i = 0;
	j = 0;
	env_list = NULL;
	if (!env || !*env)
		return (NULL);
	while (env[i])
	{
		j = -1;
		split = ft_split(env[i], '=');
		if (split)
		{
			ft_lstadd_back_env(env_struct, env_node_maker(split));
			while (split[++j])
				free((split[j]));
			free(split);
		}
		i++;
	}
	env_list = rmove_value(*env_struct);
	return (env_list);
}

int	ft_env(t_cmd *cmd, t_env *env_list)
{
	t_env	*tmp;

	tmp = NULL;
	if (!env_list)
	{
		write(2, "minishell: ", 11);
		write(2, cmd->args[0], ft_strlen(cmd->args[0]));
		write(2, ": No such file or directory\n", 28);
		return (127);
	}
	tmp = env_list;
	while (tmp)
	{
		if (tmp->key && tmp->value && tmp->is_not_active != 1
			&& tmp->for_path != 1)
			printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}
