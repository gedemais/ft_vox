/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboye <maboye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/06 16:10:51 by maboye            #+#    #+#             */
/*   Updated: 2020/08/08 16:31:45 by maboye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>

bool			ft_isupper(int c)
{
	return (c >= 'A' && c <= 'Z');
}