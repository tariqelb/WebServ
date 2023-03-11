/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitSet.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 20:16:35 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/03/10 20:19:28 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

int	ft_charset(char c, char *charset)
{
	int	i;

	i = 0;
	while (charset[i] != '\0')
	{
		if (charset[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_fill_table(char *str, char *table, int i, int j)
{
	int	index;
	int	k;

	index = 0;
	k = i - j;
	table = NULL;
	table = (char *) malloc(sizeof(char) * (j + 1));
	if (table != NULL)
	{
		while (k < i)
		{
			table[index] = str[k];
			k++;
			index++;
		}
		table[index] = '\0';
	}
	return (table);
}

char	**ft_alocate(char *str, char *charset, char **table, int i)
{
	int	j;
	int	k;

	k = 0;
	while (str[i] != '\0')
	{
		if (ft_charset(str[i], charset) == 0)
		{
			j = 0;
			while (str[i + j] != '\0' && ft_charset(str[i + j], charset) == 0)
				j++;
			i = i + j;
			if (ft_charset(str[i], charset) == 1 || str[i] == '\0')
			{
				table[k] = ft_fill_table(str, table[k], i, j);
				k++;
			}
		}
		else
			i++;
	}
	return (table);
}

int	ft_count_words(char *str, char *charset)
{
	int	i;
	int	flag;
	int	words;

	words = 0;
	flag = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (ft_charset(str[i], charset) == 0)
			flag = 1;
		if (ft_charset(str[i], charset) == 1 && flag == 1)
		{
			flag = 0;
			words++;
		}
		i++;
	}
	if (str[i] == '\0' && flag == 1)
		words++;
	return (words);
}

char	**ft_split_str(char *str, char *charset)
{
	int		index;
	int		words;
	char	**table;

	if (str == NULL)
		return (NULL);
	table = NULL;
	index = 0;
	words = 0;
	words = ft_count_words(str, charset);
	table = (char **) malloc(sizeof(char *) * (words + 1));
	if (table != NULL)
	{
		table = ft_alocate(str, charset, table, index);
		table[words] = NULL;
	}
	return (table);
}
