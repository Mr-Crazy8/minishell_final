/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 19:41:27 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/16 19:43:03 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	process_redir_hp(char *str, int *start, int *i, int *after_operator)
{
	if (str[(*start)] == '>' && str[(*i)] == '>')
		(*i)++;
	else if (str[(*start)] == '<' && str[(*i)] == '<')
		(*i)++;
	(*after_operator) = (*i);
	while (str[(*i)] == ' ')
		(*i)++;
}

void	process_redir_helper(char str, int *quote_state)
{
	if (str == '\'')
	{
		if ((*quote_state) == 0)
			(*quote_state) = 1;
		else if ((*quote_state) == 1)
			(*quote_state) = 0;
	}
	else if (str == '\"')
	{
		if ((*quote_state) == 0)
			(*quote_state) = 2;
		else if ((*quote_state) == 2)
			(*quote_state) = 0;
	}
}
