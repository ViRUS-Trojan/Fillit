/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit_dlx_solve.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdelsie <vdelsie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/01 19:04:15 by vdelsie           #+#    #+#             */
/*   Updated: 2019/10/01 19:04:17 by vdelsie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "fillit.h"

/*
**	prep_xcover - функция для подготовки нашей матрицы обложки eXact (наша двусвязная
**	тороидальный список). Необходимый шаг, прежде чем мы сможем выполнить танцевальные ссылки.
**
**	Becuase Fillit требует, чтобы все фигуры были на доске, мы знаем, что
**	количество выборов, которые будут сделаны, это точно количество тетринимо
**	куски.
*/

void	prep_xcover(t_xcover *xcov, t_tetlst **tetra_pieces, int bsize)
{
	size_t	tlst_len;
	size_t	col_arrlen;
	size_t	node_arrlen;

	tlst_len = tet_lstlen(*tetra_pieces);
	xcov->col_names = make_col_names(bsize, *tetra_pieces, tlst_len);
	col_arrlen = (bsize * bsize) + tlst_len + 1;
	xcov->col_arr = (t_col *)malloc(col_arrlen * sizeof(t_col));
	xcov->root = &(xcov->col_arr[0]);
	link_columns(xcov);
	make_node_namelst(&(xcov->nnlst), *tetra_pieces, bsize);
	node_arrlen = nn_lstlen(xcov->nnlst) * 5;
	xcov->node_arr = (t_node *)malloc(node_arrlen * sizeof(t_node));
	link_rows(xcov);
	xcov->choice = (t_node **)malloc(tlst_len * sizeof(*(xcov->choice)));
}

/*
**	t_col	*choose_best_col(t_col *root)
**	{
**		size_t	minlen;
**		t_col	*best_col;
**		t_col	*curr_col;
**
**		minlen = 99999;
**
**		curr_col = root->next;
**		while (curr_col != root)
**		{
**			if (curr_col->len < minlen)
**			{
**				best_col = curr_col;
**				minlen = curr_col->len;
**			}
**			curr_col = curr_col->next;
**		}
**		return(best_col);
**	}
**
**	Нужно нанести вред функции Choose_best_col, чтобы получить
**	соответствует ожидаемому результату fillit ...
*/

t_col	*choose_best_col(t_col *root)
{
	t_col	*curr_col;

	curr_col = root->next;
	return (curr_col);
}

void	cover_row_cols(t_node *curr_node)
{
	t_node	*pp;

	pp = curr_node->right;
	while (pp != curr_node)
	{
		cover_col(pp->col);
		pp = pp->right;
	}
}

void	uncover_row_cols(t_node *curr_node)
{
	t_node	*pp;

	pp = curr_node->left;
	while (pp != curr_node)
	{
		uncover_col(pp->col);
		pp = pp->left;
	}
}

/*
**	Реализация алгоритма Дональда Кнута DLX
**	См: https://arxiv.org/pdf/cs/0011047.pdf
**		 https://en.wikipedia.org/wiki/Dancing_Links
**		 https://en.wikipedia.org/wiki/Knuth%27s_Algorithm_X
*/

int		dlx_solve(t_xcover *xcov, int level)
{
	t_col	*best_col;
	t_node	*curr_node;

	best_col = choose_best_col(xcov->root);
	cover_col(best_col);
	curr_node = best_col->head.down;
	xcov->choice[level] = curr_node;
	while (curr_node != &(best_col->head))
	{
		cover_row_cols(curr_node);
		if ((xcov->root)->next == (xcov->root))
			return (1);
		if (dlx_solve(xcov, level + 1) == 1)
			return (1);
		uncover_row_cols(curr_node);
		curr_node = curr_node->down;
		xcov->choice[level] = curr_node;
	}
	uncover_col(best_col);
	return (0);
}
