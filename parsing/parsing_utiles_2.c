/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utiles_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 10:52:43 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/21 14:56:31 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "parsing.h"



t_env *find_last_node(t_env *env)
{
    t_env *tmp = env;
    
    while (tmp->next)
        tmp = tmp->next;
    
    return (tmp);
}

void add_shlvl_node(t_env *env)
{
    t_env *new_node;
    t_env *last_node;
    
    new_node = create_shlvl_node();
    if (!new_node)
        return;
    
    last_node = find_last_node(env);
    last_node->next = new_node;
}

void add_one_shlvl(t_env *env)
{
    t_env *shlvl_node;
    
    if (!env)
        return;
    shlvl_node = find_shlvl_node(env);
    if (shlvl_node)
        update_shlvl_value(shlvl_node);
    else
        add_shlvl_node(env);
}