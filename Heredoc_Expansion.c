/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Heredoc_Expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 12:29:41 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/20 12:03:51 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**init_dir(int dir_size)
{
	char			**dir;

	dir = malloc(sizeof(char *) * (dir_size + 1));
	if (dir == NULL)
		return (NULL);
	dir[0] = strdup("/mnt/homes/anel-men/");
	dir[1] = strdup("/mnt/homes/anel-men/Pictures/");
	dir[2] = strdup("/mnt/homes/anel-men/Library/Application Support/");
	dir[3] = strdup("/tmp/");
	dir[4] = strdup("/var/tmp/");
	dir[5] = strdup("/Users/Shared/");
	dir[6] = strdup("/mnt/homes/anel-men/Desktop/");
	dir[7] = strdup("/mnt/homes/anel-men/Documents/");
	dir[8] = strdup("/mnt/homes/anel-men/Downloads/");
	dir[9] = strdup("/mnt/homes/anel-men/Library/");
	dir[10] = strdup("/mnt/homes/anel-men/Library/Caches/");
	dir[11] = strdup("/mnt/homes/anel-men/Library/Preferences/");
	dir[12] = strdup("/mnt/homes/anel-men/Movies/");
	dir[13] = strdup("/mnt/homes/anel-men/Music/");
	dir[14] = NULL;
	return (dir);
}

char	*open_urandom_failed(int dir_size, char **dir, unsigned char *raw_bytes)
{
	write(2, "Error opening /dev/urandom\n", 27);
	free_dir(0, dir_size, dir);
	free(dir);
	free(raw_bytes);
	return (NULL);
}

char	*read_urandom_failed(int fd, int dir_size,
			char **dir, unsigned char *raw_bytes)
{
	close(fd);
	free_dir(0, dir_size, dir);
	free(dir);
	free(raw_bytes);
	return (NULL);
}

void	clean_up(int fd, int dir_size, char **dir, unsigned char	*raw_bytes)
{
	close(fd);
	free_dir(0, dir_size, dir);
	free(dir);
	free(raw_bytes);
}

char	*random_dir(void)
{
	char			**dir;
	int				fd;
	unsigned char	*raw_bytes;
	char			*result;
	static int		i = 0;

	raw_bytes = malloc(sizeof(unsigned char) * 5);
	if (raw_bytes == NULL)
		return (NULL);
	dir = init_dir(14);
	if (dir == NULL)
		return (free(raw_bytes), NULL);
	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
		return (open_urandom_failed(14, dir, raw_bytes));
	if (read(fd, raw_bytes, 5) != 5)
		return (read_urandom_failed(fd, 14, dir, raw_bytes));
	result = strdup(dir[raw_bytes[i] % 14]);
	i = (i + 1) % 5;
	clean_up(fd, 14, dir, raw_bytes);
	return (result);
}
