/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboye <maboye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/07 13:57:04 by maboye            #+#    #+#             */
/*   Updated: 2020/08/10 19:44:13 by maboye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char			*ft_itoa(int nb)
{
	char	ret[11];
	char	*r;
	int		neg;
	int		i;
	int		j;

	neg = nb < 0 ? -1 : 1;
	i = 0;
	while (neg * nb > 9 || neg * nb < 0)
	{
		ret[i++] = '0' + neg * (nb % 10);
		nb = nb / 10;
	}
	ret[i++] = '0' + neg * nb;
	if (neg < 0)
		ret[i++] = '-';
	if (!(r = (char *)ft_memalloc(sizeof(char) * i)))
		return (NULL);
	j = 0;
	while (i--)
		r[i] = ret[j++];
	return (r);
}
