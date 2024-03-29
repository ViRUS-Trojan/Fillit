/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdelsie <vdelsie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/01 19:04:35 by vdelsie           #+#    #+#             */
/*   Updated: 2019/10/01 19:04:43 by vdelsie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "fillit.h"

int			mod_strcmp(char *s1, char *s2)
{
	unsigned char *cs1;
	unsigned char *cs2;

	cs1 = (unsigned char *)s1;
	cs2 = (unsigned char *)s2;
	while (*cs1 == *cs2)
	{
		if (*cs1 == '\0')
			return (0);
		cs1++;
		cs2++;
	}
	return ((int)(*cs1 - *cs2));
}

void		mod_strcpy(char *dest, char *src)
{
	while (*src != ' ' && *src != '\0')
		*dest++ = *src++;
	*dest = '\0';
}

static int	ft_floorsqrt(int n)
{
	int start;
	int mid;
	int end;
	int	res;

	start = 1;
	end = n;
	if ((n == 0) || (n == 1))
		return (n);
	while (start <= end)
	{
		mid = (start + end) / 2;
		if (mid * mid == n)
			return (mid);
		if (mid * mid < n)
		{
			start = mid + 1;
			res = mid;
		}
		else
			end = mid - 1;
	}
	return (res);
}

/*
**	Основная функция для заполнения. Основная логика включает в себя подготовку
**	матрица покрытия eXact, представляющая собой sqrt ('# tetrinimo parts' * 4). это
**	дает нам более низкую оценку размера наименьшего квадрата, который может
**	подходят все части.
**
**	Затем мы пытаемся выяснить, можем ли мы найти точное покрытие для этого размера доски.
**	Если мы не можем, мы создаем новую большую матрицу обложки eXact и пробуем снова.
*/

int			main(int argc, char const *argv[])
{
	t_tetlst	*tetra_pieces;
	t_xcover	new_xcover;
	int			bsize;

	if (argc == 2)
	{
		read_file(argv[1], &tetra_pieces, 'A');
		if ((tetra_pieces && tetra_pieces->is_valid))
		{
			tet_lstrev(&tetra_pieces);
			bsize = ft_floorsqrt(tet_lstlen(tetra_pieces) * 4);
			prep_xcover(&new_xcover, &tetra_pieces, bsize);
			while (dlx_solve(&new_xcover, 0) != 1)
			{
				emancipate_xcover(&new_xcover);
				prep_xcover(&new_xcover, &tetra_pieces, ++bsize);
			}
			disp_res_n_clean(&new_xcover, &tetra_pieces, bsize);
		}
		else
			write(1, "error\n", 6);
	}
	else
		write(2, "usage: ./fillit [target_file]\n", 30);
	return (0);
}
