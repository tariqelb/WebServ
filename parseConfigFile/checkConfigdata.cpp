/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkConfigdata.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 21:03:29 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/03/29 01:06:18 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int	checkConfigData(struct webserv web)
{
	int i;
	int size;
		displayServerFile(web.config);
	/*i = 0;
	size = web.config.size();
	while (i < size)
	{
		std::cout << "*********************************" << std::endl;
		i++;
	}*/
	return (0);
}

