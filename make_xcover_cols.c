/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_xcover_cols.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdelsie <vdelsie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/01 19:43:48 by vdelsie           #+#    #+#             */
/*   Updated: 2019/10/01 19:44:21 by vdelsie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "fillit.h"
#define ENCODE(x) (((x) < 10) ? (x) + '0' : (x) + 'a' - 10)

static void	add_tetnames(char **tmp, t_tetlst *pieces)
{
	t_tetlst	*curr_piece;

	curr_piece = pieces;
	while (curr_piece)
	{
		*(*tmp)++ = curr_piece->name[0];
		*(*tmp)++ = ' ';
		curr_piece = curr_piece->next;
	}
	*(*tmp)++ = '|';
	*(*tmp)++ = ' ';
}

/*
** create_col_names - функция для создания именных столбцов (вторичные ограничения)
** для каждого из ограничений в нашей проблеме точного покрытия. На случай, если
** Заполните наши ограничения позиции в квадратной доске (вторичной) и
** наличие данного произведения (первичное).
**
** У нас уже есть имена для всех частей, введенных в fillit, поэтому нам просто нужно
** добавить их, а затем генерировать координаты для данного размера квадрата.
**
** Мы будем использовать систему представления координат Кнута как x1y1 x2y2 x3y3, так что для
** Сетка 3х3 мы бы сгенерировали: '00 01 02 10 11 12 20 21 22 '.
**
** Для еще больших сеток у нас будет a-z, представляющий 11 - 36, поэтому координаты
** «a1» будет (11, 2), а координата «zz» будет (36, 36).
**
** Квадратная длина n будет означать n ^ 2 пары символов и (n ^ 2) - 1 пробел
** нам также нужно выделить терминатор строки, чтобы наша конечная длина строки
** будет 2 * (n ^ 2) + (n ^ 2) - 1 + 1 символов.
*/

char		*make_col_names(int slen, t_tetlst *pieces, ssize_t tlst_len)
{
	int		sq_col_size;
	int		row;
	int		col;
	char	*col_string;
	char	*tmp;

	sq_col_size = (2 * slen * slen + slen * slen);
	col_string = (char *)malloc(sq_col_size + (2 * (tlst_len + 1)));
	tmp = col_string;
	add_tetnames(&tmp, pieces);
	row = -1;
	while (++row < slen)
	{
		col = -1;
		while (++col < slen)
		{
			*tmp++ = ENCODE(row);
			*tmp++ = ENCODE(col);
			if (row == slen - 1 && col == slen - 1)
				*tmp = '\0';
			else
				*tmp++ = ' ';
		}
	}
	return (col_string);
}

static void	col_init(t_col *curr_col, t_col *prev_col, char *name, int primary)
{
	mod_strcpy(curr_col->name, name);
	curr_col->head.up = &(curr_col->head);
	curr_col->head.down = &(curr_col->head);
	curr_col->len = 0;
	if (primary)
	{
		curr_col->prev = prev_col;
		prev_col->next = curr_col;
	}
	else
	{
		curr_col->prev = curr_col;
		curr_col->next = curr_col;
	}
}

/*
** link_columns - функция для связывания имен столбцов, которые мы создали
** такие, что они образуют двусвязный (тороидальный) список.
**
** Примечание: только первичные столбцы дважды связаны, в то время как вторичные столбцы
** связаны только с собой.
**
** col_names принимает форму «Имя1 Имя2 Имя3 | 00 01 02 03 ...» где
** «Имя перед« | » обозначить основные столбцы и имена после, назначить
** вторичные столбцы.
*/

void		link_columns(t_xcover *xcov)
{
	t_col	*curr_col;
	char	*col_names;
	int		primary;

	primary = 1;
	col_names = xcov->col_names;
	curr_col = (xcov->col_arr + 1);
	while (*col_names)
	{
		while (*col_names == ' ')
			col_names++;
		if (*col_names == '|')
		{
			primary = 0;
			(curr_col - 1)->next = xcov->root;
			(xcov->root)->prev = (curr_col - 1);
			col_names++;
			continue;
		}
		col_init(curr_col, (curr_col - 1), col_names, primary);
		while (*col_names && *col_names != ' ')
			col_names++;
		curr_col++;
	}
}
