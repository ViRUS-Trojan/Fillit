/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_and_print.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdelsie <vdelsie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/01 19:03:47 by vdelsie           #+#    #+#             */
/*   Updated: 2019/10/01 19:04:10 by vdelsie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "fillit.h"

int		decode(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	else if (c >= 'A' && c <= 'Z')
		return (c + 10 - 'A');
	else if (c >= 'a' && c <= 'z')
		return (c + 10 - 'a');
	return (0);
}

void	emancipate_xcover(t_xcover *xcov)
{
	free(xcov->col_names);
	free(xcov->col_arr);
	xcov->root = NULL;
	nn_lstfree(&(xcov->nnlst));
	free(xcov->node_arr);
	free(xcov->choice);
}

void	fmt_result(char *result, int bsize)
{
	int	row;
	int	col;

	row = -1;
	while (++row < bsize)
	{
		col = -1;
		while (++col < bsize + 1)
		{
			if (col != bsize)
				*result++ = '.';
			else
				*result++ = '\n';
		}
	}
	*result = '\0';
}

void	update_result(char *result, t_node *choice, int bsize)
{
	char	*n;
	char	*c1;
	char	*c2;
	char	*c3;
	char	*c4;

	n = choice->col->name;
	c1 = choice->right->col->name;
	c2 = choice->right->right->col->name;
	c3 = choice->right->right->right->col->name;
	c4 = choice->right->right->right->right->col->name;
	result[decode(c1[0]) * (bsize + 1) + decode(c1[1])] = *n;
	result[decode(c2[0]) * (bsize + 1) + decode(c2[1])] = *n;
	result[decode(c3[0]) * (bsize + 1) + decode(c3[1])] = *n;
	result[decode(c4[0]) * (bsize + 1) + decode(c4[1])] = *n;
}

/*
**	disp_res_n_clean
**
**	1) Создайте строку соответствующего размера, чтобы соответствовать доске с длиной края
**	   'bsize'.
**	2) Заполните строку '.'s and '\n's
**	3) Считать переменную выбора  xcov->choice которая хранит узлы (начало строки)
**	   что алгоритм DLX выбрал при попытке найти точное покрытие. Мы
**	   распакуйте каждый выбор и обновите нашу строку результата соответствующим образом.
**	4)  Распечатать результат.
**	5) Очистите все распределенные нами структуры данных malloc.
*/

void	disp_res_n_clean(t_xcover *xcov, t_tetlst **tetra_pieces, int bsize)
{
	size_t	tlst_len;
	size_t	i;
	t_node	**choice_arr;
	char	*result;

	result = (char *)malloc((1 + bsize * (bsize + 1)) * sizeof(*result));
	fmt_result(result, bsize);
	tlst_len = tet_lstlen(*tetra_pieces);
	choice_arr = xcov->choice;
	i = 0;
	while (i < tlst_len)
		update_result(result, choice_arr[i++], bsize);
	write(1, result, (bsize * (bsize + 1)));
	emancipate_xcover(xcov);
	tet_lstfree(tetra_pieces);
	free(result);
}
