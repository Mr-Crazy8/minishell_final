/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Heredoc_Expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 12:29:41 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/19 16:44:38 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*add(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	size_t	i;
	size_t	j;
	char	*s3;

	i = 0;
	j = 0;
	s1_len = strlen(s1);
	s2_len = strlen(s2);
	s3 = (char *)malloc((s1_len + s2_len + 1) * sizeof(char));
	if (s3 == NULL)
		return (NULL);
	while (i < s1_len)
	{
		s3[i] = s1[i];
		i++;
	}
	while (j < s2_len)
		s3[i++] = s2[j++];
	s3[i] = '\0';
	return (s3);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*s3;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (strdup(s2));
	if (!s2)
		return (strdup(s1));
	s3 = add(s1, s2);
	return (s3);
}


char	*random_dir(void)
{
	//put_in_struct++++++++++++++++++++++++++++++++++++++++++++
	char			**dir;
	int				fd;
	unsigned char	*raw_bytes;
	char			*result;
	int				dir_size; 
	static int		i;
	unsigned int	index;
	int				j;
	//put_in_struct++++++++++++++++++++++++++++++++++++++++++++
	dir_size = 14;
	i = 0;
	j = 0;
	raw_bytes = malloc(sizeof(unsigned char) * 5);
	if (raw_bytes == NULL) 
		return NULL;
	//func_00++++++++++++++++++++++++++++++++++++++++++++
	dir = malloc(sizeof(char *) * (dir_size + 1));
	if (dir == NULL)
		return (free(raw_bytes), NULL);
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
	//func_00++++++++++++++++++++++++++++++++++++++++++++
	//func_01++++++++++++++++++++++++++++++++++++++++++++
	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
	{
		write(2, "Error opening /dev/urandom\n", 27);
		while (j < dir_size) 
		{
			if (dir[j] != NULL)
				free(dir[j]);
			j++;
		}
		free(dir);
		free(raw_bytes);
		return (NULL);
	}
	//func_01++++++++++++++++++++++++++++++++++++++++++++
	//func_02++++++++++++++++++++++++++++++++++++++++++++
	if (read(fd, raw_bytes, 5) != 5) 
	{
		close(fd);
		j = 0;
		while (j < dir_size) 
		{
			if (dir[j] != NULL) 
				free(dir[j]);
			j++;
		}
		free(dir);
		free(raw_bytes);
		return NULL;
	}
	//func_02++++++++++++++++++++++++++++++++++++++++++++
	//func_03++++++++++++++++++++++++++++++++++++++++++++
	index = raw_bytes[i] % dir_size;
	i = (i + 1) % 5;
	result = strdup(dir[index]);
	close(fd);
	j = 0;
	while (j < dir_size) 
	{
		if (dir[j] != NULL)
			free(dir[j]);
		j++;
	}
	free(dir);
	free(raw_bytes);
	return result;
	//func_03++++++++++++++++++++++++++++++++++++++++++++
}
char	*random_file_name(void)
{
	int				fd;
	unsigned char	*raw_bytes;
	char			*rstring;
	int				count;
	int				i;
	
	raw_bytes = malloc(sizeof(unsigned char) * 13);
	rstring = malloc(sizeof(char) * 13);
	if (!raw_bytes || !rstring) 
	{
		if (raw_bytes)
			free(raw_bytes);
		if (rstring)
			free(rstring);
		return NULL;
	}
	
	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1) {
		write(2, "Error opening /dev/urandom\n", 27);
		free(raw_bytes);
		free(rstring);
		return NULL;
	}
	count = read(fd, raw_bytes, 12);
	close(fd); 
	
	if (count < 12) 
	{
		write(2, "Error reading from /dev/urandom\n", 32);
		free(raw_bytes);
		free(rstring);
		return NULL;
	}
	
	char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	int charset_size = sizeof(charset) - 1; 
	
	i = 0;
	while (i < 12)
	{
		unsigned int index = raw_bytes[i] % charset_size;
		rstring[i] = charset[index];
		i++;
	}
	rstring[12] = '\0';
	char *dir = random_dir();
	char *rstring1 = ft_strjoin(".\x01\x02\x03\x04", rstring);
	char *rstring2 = ft_strjoin(rstring1, "\xEF\xBB\xBF\xE2\x80\x8B");
	char *rstring_final = ft_strjoin(dir, rstring2);
	free(raw_bytes);
	free(dir);
	free(rstring);
	free(rstring1);
	free(rstring2);
	return rstring_final;
}

int	*heredoc_opener(void)
{
	char	*random_name;
	int		*fd_heredoc;
	
	random_name = random_file_name();
	fd_heredoc = malloc(2 * sizeof(int));
	if (!fd_heredoc) 
		return (free(random_name), NULL);
	if (random_name) 
	{
		fd_heredoc[0] = open(random_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		fd_heredoc[1] = open(random_name, O_CREAT | O_RDONLY, 0644);
		unlink(random_name);
		free(random_name);  
	}
	return fd_heredoc;
}

int	was_delimiter_quoted(char *orig_token)
{
	if (orig_token && 
		((orig_token[0] == '\'' && orig_token[strlen(orig_token)-1] == '\'') ||
		 (orig_token[0] == '\"' && orig_token[strlen(orig_token)-1] == '\"')))
		return 1;
	return 0;
}

char	*check_for_doller(char *orig_token)
{
	char *substr;
	char *result;
	int doller;
	
	doller = 0;
	while (orig_token[doller] != '\0' && orig_token[doller] == '$')
		doller++;
	if (orig_token[doller] != '\0' && (orig_token[doller] == '\'' || orig_token[doller] == '\"'))
	{
		if (doller % 2 == 0)
		   return (selective_remove_quotes(orig_token, 1));
		else 
		{

			substr = ft_substr(orig_token, 1, ft_strlen(orig_token) - 1);
			result = selective_remove_quotes(substr, 1);
			free(substr);
			return (result);
		}
	}
	return (orig_token);
}

int	check_for_quotes(char *str)
{
	int i;
	int count_signal;
	int count_duble;

	i = 0;
	count_signal = 0;
	count_duble = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			count_signal++;
		else if (str[i] == '\"')
			count_duble++;
		i++;
	}
	if (count_signal != 0 || count_duble != 0)
		return (1);
	return (0);
}


char	*heredoc_delemter(char *orig_token) 
{
	char *new_delemter;

	if (!orig_token)
		return NULL;
	
	if (was_delimiter_quoted(orig_token) == 1) 
	{
		new_delemter = selective_remove_quotes(orig_token, 1);
		return new_delemter;
	}
	if (orig_token[0] == '$')
	{
		new_delemter = check_for_doller(orig_token);
		if (check_for_quotes(new_delemter) == 1)
		{
				new_delemter = selective_remove_quotes(orig_token, 1);
				return new_delemter;
		 }
		return new_delemter;
	}
	return ft_strdup(orig_token);
}


void	doc_expand(char *str, t_exp_helper *expand,
				   t_env *env, int exit_status)
{
	if (!expand_fill_str(expand, str))
		return;
	
	while (expand->original[expand->i]) {
		if (!expand_handle_helper0(expand)
			&& !expand_handle_helper1(expand, exit_status, env, 0))
			expand->expanded[expand->j++] = expand->original[expand->i++];
	}
	expand->expanded[expand->j] = '\0';
}

char	*process_heredoc_epxand(char *line, t_env *env, int exit_status, char *orig_delimiter)
{
	t_exp_helper *expand;
	char *result;
	if (was_delimiter_quoted(orig_delimiter) || check_for_quotes(orig_delimiter))
		return ft_strdup(line);
	expand = malloc(sizeof(t_exp_helper));
	if (!expand)
		return NULL;
	doc_expand(line, expand, env, exit_status);
	result = expand->expanded;
	free(expand);
	return result;
}

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
