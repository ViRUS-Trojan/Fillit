/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dancing_links_ops.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdelsie <vdelsie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/01 19:04:03 by vdelsie           #+#    #+#             */
/*   Updated: 2019/10/01 19:04:09 by vdelsie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

/*
**	Реализация алгоритма Дональда Кнута  DLX
**	https://arxiv.org/pdf/cs/0011047.pdf
**
**	Сначала прочитайте функцию cover_col()!
*/

/*
**	Функция для «покрытия» всех узлов в той же строке, что и данный  (col_node)
**	1)Перейти к узлу (row_node) справа от (col_node)
**	2) назначить верхний узел (row_node)'s вниз как  (row_node)'s  вниз
**	3) назначить нижний узел (row_node)'s на верхний (row_node)'s верх
**
**	Представим, что цифры ниже перевернуты на 90 градусов
**	(т. е. вертикальная с UU вверху)
**
**	До:
**	+------+  ->  +----------+      +------+
**	|  uu  |  <-  | row_node |  ->  |  dd  |
**	+------+      +----------+  <-  +------+
**
**	После:	    /--------------\
**	+------+  --  +----------+  ->  +------+
**	|  uu  |  <-  | row_node |  ->  |  dd  |
**	+------+  <-  +----------+  --  +------+
**		    \______________/
**
**	4) После скрытия (row_node) в своем столбце переходите к следующему
**	вернитесь вправо и повторяйте 2) и 3), пока row_node == col_node снова.
*/

static void	cover_nodes(t_node *col_node)
{
	t_node	*row_node;
	t_node	*uu;
	t_node	*dd;

	row_node = col_node->right;
	while (row_node != col_node)
	{
		uu = row_node->up;
		dd = row_node->down;
		uu->down = dd;
		dd->up = uu;
		(row_node->col)->len--;
		row_node = row_node->right;
	}
}

/*
**  Функция для «покрытия» столбца (с) и связанных узлов
**	1) назначить следующий столбец (c) следующим (c) следующий
**	2) назначить предысторию следующего столбца (с) предваряющим (с)
**
**	До:
**	+------+  ->  +-------+      +--------+
**	| prev |  <-  |   c   |	 ->  |  next  |
**	+------+      +-------+  <-  +--------+
**
**	После:	    /-----------\
**	+------+  --  +-------+  ->  +--------+
**	| prev |  <-  |   c   |	 ->  |  next  |
**	+------+  <-  +-------+  --  +--------+
**		    \___________/
**
**	Примечание: (c) указатели 'next' и 'prev' по-прежнему сохраняются!
**
**	Затем мы переходим «вниз» к столбцу (с) и для каждого узла в столбце:
**	1) охватывает все другие узлы, которые находятся в той же строке, что и наш col_node
*/

void		cover_col(t_col *c)
{
	t_col	*l;
	t_col	*r;
	t_node	*col_node;

	l = c->prev;
	r = c->next;
	l->next = r;
	r->prev = l;
	col_node = c->head.down;
	while (col_node != &(c->head))
	{
		cover_nodes(col_node);
		col_node = col_node->down;
	}
}

/*
**	Функции для раскрытия «покрытых» столбцов и узлов
**	Мы буквально выполняем обратную операцию прикрытия, чтобы раскрыть
**	 наши покрытые узлы и столбцы
*/

static void	uncover_nodes(t_node *col_node)
{
	t_node	*row_node;
	t_node	*uu;
	t_node	*dd;

	row_node = col_node->left;
	while (row_node != col_node)
	{
		uu = row_node->up;
		dd = row_node->down;
		uu->down = row_node;
		dd->up = row_node;
		(row_node->col)->len++;
		row_node = row_node->left;
	}
}

void		uncover_col(t_col *c)
{
	t_col	*l;
	t_col	*r;
	t_node	*col_node;

	col_node = c->head.up;
	while (col_node != &(c->head))
	{
		uncover_nodes(col_node);
		col_node = col_node->up;
	}
	l = c->prev;
	r = c->next;
	l->next = c;
	r->prev = c;
}
