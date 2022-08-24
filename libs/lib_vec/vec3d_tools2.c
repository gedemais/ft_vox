/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3d_tools2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboye <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/30 01:53:49 by maboye            #+#    #+#             */
/*   Updated: 2022/05/26 16:09:48 by gedemais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_vec.h"

bool		vec_outrange(t_vec3d dim, t_vec3d pos)
{
	return (pos.x < 0 || pos.x > dim.x - 1
		|| pos.y < 0 || pos.y > dim.y - 1
		|| pos.z < 0 || pos.z > dim.z - 1);
}

void		vec3d_swap(t_vec3d *a, t_vec3d *b)
{
	t_vec3d		t;

	t = *a;
	*a = *b;
	*b = t;
}

t_vec3d		abs_vector(t_vec3d vec)
{
	return ((t_vec3d){fabs(vec.x), fabs(vec.y), fabs(vec.z), fabs(vec.w)});
}
