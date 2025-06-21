/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_one_shlvl.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 15:07:38 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/21 15:16:16 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	add_one_shlvl(t_env *env)
{
	t_env	*shlvl_node;

	if (!env)
		return ;
	shlvl_node = find_shlvl_node(env);
	if (shlvl_node)
		update_shlvl_value(shlvl_node);
	else
		add_shlvl_node(env);
}
