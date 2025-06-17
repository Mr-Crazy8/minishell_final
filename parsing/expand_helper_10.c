/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helper_10.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 18:35:15 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/16 18:36:15 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	process_quotes_for_cmd(t_cmd *cmd_list, int remove_mode)
{
	t_cmd		*current;
	t_redir		*redir;
	char		*processed;
	int			i;

	current = cmd_list;
	while (current)
	{
		process_quotes_for_cmd_hp(current, &i, remove_mode);
		redir = current->redirs;
		while (redir)
		{
			if (redir->file)
			{
				processed = selective_remove_quotes(
						redir->file, remove_mode);
				if (processed)
					processed_redir(redir, processed);
			}
			redir = redir->next;
		}
		current = current->next;
	}
}
