/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 11:58:08 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/20 12:04:10 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

unsigned char	*allocate_random_memory(char **rstring)
{
	unsigned char	*raw_bytes;

	raw_bytes = malloc(sizeof(unsigned char) * 13);
	*rstring = malloc(sizeof(char) * 13);
	if (!raw_bytes || !*rstring)
	{
		if (raw_bytes)
			free(raw_bytes);
		if (*rstring)
			free(*rstring);
		return (NULL);
	}
	return (raw_bytes);
}

int	read_random_bytes(unsigned char *raw_bytes)
{
	int	fd;
	int	count;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
	{
		write(2, "Error opening /dev/urandom\n", 27);
		return (-1);
	}
	count = read(fd, raw_bytes, 12);
	close(fd);
	if (count < 12)
	{
		write(2, "Error reading from /dev/urandom\n", 32);
		return (-1);
	}
	return (0);
}

void	bytes_to_string(unsigned char *raw_bytes, char *rstring)
{
	char			charset[63];
	int				charset_size;
	int				i;
	unsigned int	index;

	charset_size = sizeof(charset) - 1;
	i = 0;
	while (i < 12)
	{
		index = raw_bytes[i] % charset_size;
		rstring[i] = charset[index];
		i++;
	}
	rstring[12] = '\0';
}

char	*build_final_filename(char *rstring)
{
	char	*dir;
	char	*rstring1;
	char	*rstring2;
	char	*rstring_final;

	dir = random_dir();
	rstring1 = ft_strjoin(".\x01\x02\x03\x04", rstring);
	rstring2 = ft_strjoin(rstring1, "\xEF\xBB\xBF\xE2\x80\x8B");
	rstring_final = ft_strjoin(dir, rstring2);
	free(dir);
	free(rstring1);
	free(rstring2);
	return (rstring_final);
}

char	*random_file_name(void)
{
	unsigned char	*raw_bytes;
	char			*rstring;
	char			*result;

	raw_bytes = allocate_random_memory(&rstring);
	if (!raw_bytes)
		return (NULL);
	if (read_random_bytes(raw_bytes) == -1)
	{
		free(raw_bytes);
		free(rstring);
		return (NULL);
	}
	bytes_to_string(raw_bytes, rstring);
	result = build_final_filename(rstring);
	free(raw_bytes);
	free(rstring);
	return (result);
}
