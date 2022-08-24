/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3d_fops.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gedemais <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/08 21:51:11 by gedemais          #+#    #+#             */
/*   Updated: 2022/05/26 16:05:42 by gedemais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_vec.h"

t_vec3d		vec_fdiv(t_vec3d a, float n)
{
	t_vec3d		ret;

	ret.x = a.x / n;
	ret.y = a.y / n;
	ret.z = a.z / n;
	ret.w = a.w / n;
	return (ret);
}

t_vec3d		vec_fmult(t_vec3d a, float n)
{
	t_vec3d		ret;

	ret.x = a.x * n;
	ret.y = a.y * n;
	ret.z = a.z * n;
	ret.w = a.w * n;
	return (ret);
}

t_vec3d		vec_fsub(t_vec3d a, float n)
{
	t_vec3d		ret;

	ret.x = a.x - n;
	ret.y = a.y - n;
	ret.z = a.z - n;
	ret.w = a.w - n;
	return (ret);
}

t_vec3d		vec_cross(t_vec3d a, t_vec3d b)
{
	t_vec3d	ret;

	ret.x = (a.y * b.z) - (a.z * b.y);
	ret.y = (a.z * b.x) - (a.x * b.z);
	ret.z = (a.x * b.y) - (a.y * b.x);
	return (ret);
}
