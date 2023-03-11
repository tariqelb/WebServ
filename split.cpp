/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 17:43:21 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/03/10 19:27:47 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

static int	ft_count_word(const char *s, char c)
{
	int	words;
	int	index_i;

	if (s == NULL)
		return (0);
	words = 0;
	index_i = 0;
	while (s[index_i])
	{
		while (s[index_i] == c && s[index_i])
			index_i++;
		if (s[index_i] != c && s[index_i])
		{
			while (s[index_i] != c && s[index_i])
				index_i++;
			words++;
		}
	}
	if (words == 0)
		return (1);
	return (words);
}

static void	ft_free(char **buff, int k)
{
	int	i;

	i = 0;
	while (i < k)
	{
		free(buff[i]);
		i++;
	}
	free(buff);
}

static char	*ft_assign(char **buff, int k, const char *s, int j)
{
	int	i;

	i = 0;
	buff[k] = NULL;
	buff[k] = (char *) malloc(sizeof(char) * (j + 1));
	if (buff[k] == NULL)
	{
		ft_free(buff, k);
		return (NULL);
	}
	while (i < j)
	{
		buff[k][i] = (char )s[i];
		i++;
	}
	buff[k][j] = '\0';
	return (buff[k]);
}

static char	**ft_allocate(char **buff, const char *s, char c)
{
	int	index_i;
	int	index_j;
	int	index_k;

	index_i = 0;
	index_k = 0;
	while (s[index_i])
	{
		index_j = 0;
		while (s[index_i] == c && s[index_i])
			index_i++;
		while (s[index_i + index_j] != c && s[index_i + index_j])
			index_j++;
		if (index_j > 0)
		{
			buff[index_k] = ft_assign(buff, index_k, &s[index_i], index_j);
			index_k++;
		}
		index_i += index_j;
	}
	buff[index_k] = 0;
	return (buff);
}

char	**ft_split(char const *s, char c)
{
	char	**buff;
	int		words;

	buff = NULL;
	words = ft_count_word(s, c);
	if (s == NULL)
		return (NULL);
	else if (words)
	{
		buff = (char **) malloc(sizeof(char *) * (words + 1));
		if (buff == NULL)
			return (NULL);
		buff = ft_allocate(buff, s, c);
	}
	return (buff);
}
