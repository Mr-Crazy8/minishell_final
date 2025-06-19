/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utiles.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 10:29:47 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/19 11:47:16 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	get_or_set(int type, int status)
{
	static int	exit_status = 0;

	if (type == SET)
		exit_status = status;
	return (exit_status);
}

int	is_valid_key(char *key)
{
	int	i;

	if (!key || (!((key[0] >= 'a' && key[0] <= 'z')
				|| (key[0] >= 'A' && key[0] <= 'Z')
				|| key[0] == '_')))
		return (1);
	i = 1;
	while (key[i] && key[i] != '=')
	{
		if (!((key[i] >= 'a' && key[i] <= 'z')
				|| (key[i] >= 'A' && key[i] <= 'Z')
				|| (key[i] >= '0' && key[i] <= '9')
				|| key[i] == '_'))
			return (1);
		i++;
	}
	return (0);
}

void	print_ambiguous_redir_errors(t_cmd *cmd)
{
	t_cmd		*tmp;
	t_redir		*redir;
	char		*prefix;
	char		*suffix;

	prefix = "minishell: ";
	suffix = ": ambiguous redirect\n";
	tmp = cmd;
	while (tmp)
	{
		redir = tmp->redirs;
		while (redir)
		{
			if (redir->ambiguous)
			{
				write(2, prefix, ft_strlen(prefix));
				if (redir->orig_token)
					write(2, redir->orig_token,
						ft_strlen(redir->orig_token));
				write(2, suffix, ft_strlen(suffix));
			}
			redir = redir->next;
		}
		tmp = tmp->next;
	}
}

void	*ft_memcpy_q(void *dest, const void *src, size_t n)
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

void	count_words_q_hp(char s, int *quote_state)
{
	if (s == '\'')
	{
		if (*quote_state == 0)
			*quote_state = 1;
		else if (*quote_state == 1)
			*quote_state = 0;
	}
	else if (s == '\"')
	{
		if (*quote_state == 0)
			*quote_state = 2;
		else if (*quote_state == 2)
			*quote_state = 0;
	}
}
