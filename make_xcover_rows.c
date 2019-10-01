/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_xcover_rows.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdelsie <vdelsie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/01 19:43:54 by vdelsie           #+#    #+#             */
/*   Updated: 2019/10/01 19:44:20 by vdelsie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "fillit.h"

/*
** Перенести координаты фигуры в верхний левый угол
** Обновит строки max, max col, min row и min col, чтобы отразить новые
** координаты.
*/

static void	norm_piece(t_tetlst *piece)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		piece->row_coords[i] = piece->row_coords[i] - piece->min_row;
		piece->col_coords[i] = piece->col_coords[i] - piece->min_col;
		i++;
	}
	piece->max_row = piece->max_row - piece->min_row;
	piece->max_col = piece->max_col - piece->min_col;
	piece->min_row = 0;
	piece->min_col = 0;
}


/*
** Переведите 4 тетринимо координаты через все возможные места
** в квадратной доске с длиной стороны 'bsize'.
**
** Мы можем сэкономить время, понимая, что мы должны продолжать добавлять только строки и
** столбцы, только если они меньше, чем «длина стороны» - «максимальная длина»
*/

static void	gen_row_node_names(t_tetlst *piece, t_nnlst **nnlst, int bsize)
{
	int row;
	int	col;

	row = -1;
	while (++row < bsize - piece->max_row)
	{
		col = -1;
		while (++col < bsize - piece->max_col)
		{
			nn_lstadd(nnlst, nn_lstnew(piece, row, col));
		}
	}
}


/*
** make_node_namelst - функция для создания списка имен узлов строк, которые будут
** использоваться для связывания данного узла со столбцом. Каждый элемент в списке
** описывает 1) кусок (через имя) и 2) 4 координаты для каждого из четырех
** блоки, которые составляют кусок. Таким образом, каждый элемент в node_namelist
** описывает возможное местоположение для данного кусочка тетринимо.
*/

void		make_node_namelst(t_nnlst **nnlst, t_tetlst *pieces, int bsize)
{
	t_tetlst	*curr_p;

	curr_p = pieces;
	while (curr_p)
	{
		norm_piece(curr_p);
		gen_row_node_names(curr_p, nnlst, bsize);
		curr_p = curr_p->next;
	}
	nn_lstrev(nnlst);
}

static void	row_init(char *row, t_node *cnode, t_col *ccol, t_node **row_start)
{
	char	col_name[3];

	while (*row)
	{
		while (*row == ' ')
			row++;
		mod_strcpy(col_name, row);
		while (mod_strcmp(ccol->name, col_name))
			ccol++;
		while (*row && *row != ' ')
			row++;
		if (!(*row_start))
			*row_start = cnode;
		else
		{
			cnode->left = cnode - 1;
			(cnode - 1)->right = cnode;
		}
		cnode->col = ccol;
		cnode->up = ccol->head.up;
		ccol->head.up->down = cnode;
		ccol->head.up = cnode;
		(cnode++)->down = &(ccol->head);
		ccol->len++;
	}
}


/*
** link_rows - функция для связывания наших узлов строк (описывается узлом namelst)
** с колоннами. Узлы в данной строке будут возвращаться к своему узлу «начало строки».
**
** Мы используем созданный список имен узлов (nnlst), чтобы определить, какие узлы получают
** связано с какими столбцами / другими узлами.
*/

void		link_rows(t_xcover *xcov)
{
	t_node	*cnode;
	t_node	*row_start;
	t_nnlst	*curr_row;

	cnode = xcov->node_arr;
	curr_row = xcov->nnlst;
	while (curr_row)
	{
		row_start = NULL;
		row_init(curr_row->row, cnode, xcov->col_arr, &row_start);
		cnode += 5;
		row_start->left = cnode - 1;
		(cnode - 1)->right = row_start;
		curr_row = curr_row->next;
	}
}
