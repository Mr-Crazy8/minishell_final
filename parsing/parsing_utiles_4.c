/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utiles_4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 14:33:29 by anel-men          #+#    #+#             */
/*   Updated: 2025/06/21 14:42:08 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "parsing.h"



char **extract(char *str)
{ 
    int i;
    char *key;
    char *value;
    char **split;
    
    split = malloc(sizeof(char *) * 3);
    if (!split)
        return (NULL);
    i = 0;
    while (str && str[i] && str[i] != '=')
        i++;
    key = ft_substr(str, 0, i);
    if (!key) 
        return (free(split), NULL);
    value = ft_substr(str, i+1, ft_strlen(str) - (i+1));
    if (!value) 
        return (free(key), free(split), NULL);
    split[0] = key;
    split[1] = value;
    split[2] = NULL;
    return (split);
}

char *plus_checker(char *str)
{
    int i;
    
    i = 0;
    if (!str)
        return (str);
    while (str && str[i])
        i++;  
    if (i > 0 && str[i - 1] == '+')
        str[i - 1] = '\0';
    return (str);
}


void cle0(char **split, char *new_key)
{
    free(new_key);
    free_extract_result(split);
}


void cle1(char *join1, char *join2, char *join3)
{
    free(join3);
    free(join1);
    free(join2);
}


void cleanup_split_key(char **split, char *new_key)
{
    free(new_key);
    free_extract_result(split);
}