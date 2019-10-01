/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit_read.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdelsie <vdelsie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/01 19:04:21 by vdelsie           #+#    #+#             */
/*   Updated: 2019/10/01 19:04:46 by vdelsie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "fillit.h"

#define IS_VALIDCHAR(x) ((x) == '.' || (x) == '#')
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

/*
**	count_neighbors()
**	проверить, если слева, вверх, вправо, вниз от строки, столбец содержит '#'
**
**	Поскольку count_neighbors () требует, чтобы была найдена '#', мы можем сделать это
**	тройной долг на 2) увеличивая нашу переменную piece-> block_count и
**	3) определение минимальных и максимальных координат строки и столбца для всех «#»
**
**	слева:  row, col - 1
**	вверх:	   row - 1, col
**	справа: row, col + 1
**	вниз:  row + 1, col
**
**	Вызывает себя рекурсивно, чтобы он мог проверить, если граничные условия
**	применяются перед попыткой индексировать значения платы.
**
**	Если количество блоков равно 4 или больше, то наша предполагаемая часть имеет 5
**	или больше блоков, поэтому мы не должны тратить время на подсчет соседей ...
*/

static void		count_neighbors(t_tetlst *piece, int row, int col, int level)
{
	if (level == 0)
	{
		if (piece->block_count < 4)
		{
			piece->row_coords[piece->block_count] = row;
			piece->col_coords[piece->block_count] = col;
			piece->min_row = MIN(piece->min_row, row);
			piece->min_col = MIN(piece->min_col, col);
			piece->max_row = MAX(piece->max_row, row);
			piece->max_col = MAX(piece->max_col, col);
			piece->block_count++;
			count_neighbors(piece, row, col - 1, 1);
			count_neighbors(piece, row - 1, col, 1);
			count_neighbors(piece, row, col + 1, 1);
			count_neighbors(piece, row + 1, col, 1);
		}
		else
			piece->is_valid = 0;
	}
	else if ((row >= 0 && row <= 3) && (col >= 0 && col <= 3))
	{
		if (piece->board[row][col] == '#')
			piece->neighbors++;
	}
}

/*
**	check_tetra()
**	Даже если тетра-доска прочитана успешно, не обязательно
**	указывают на то, что доска содержит действительный тетринимо
**
**	Чтобы проверить, так ли это, мы посчитаем количество «#» на доске
**	Кроме того, когда мы сталкиваемся с '#', мы проверяем, что это 4 соседей, чтобы увидеть
**	если они тоже "#".
**
**	Действительный кусок всегда будет иметь 4 блока и вернет количество соседей
**	из 6 (все части, кроме квадрата) или 8 (квадрат). Любой другой счет
**	означает, что произведение не является действительным и связанным тетринимо !!!
*/

static void		check_tetra(t_tetlst *piece)
{
	int row;
	int col;

	if (piece->is_valid)
	{
		row = -1;
		while (++row < 4)
		{
			col = -1;
			while (++col < 4)
			{
				if (piece->board[row][col] == '#')
					count_neighbors(piece, row, col, 0);
			}
		}
	}
	if (piece->block_count != 4 || piece->name[0] > 'Z')
		piece->is_valid = 0;
	if (!(piece->neighbors == 6 || piece->neighbors == 8))
		piece->is_valid = 0;
}

/*
**	read_tetra()
**	Функция принимает файловый дескриптор и структуру t_tetlst.
**	ssize_t ret и char * buf - хакерские хаки, чтобы попасть под 25 строку
**	 limit, поэтому представьте, что это эквивалентно определению ret и buf (см. read_file ()).
**
**	Функция прочитает 4 строки из данного файла и попытается
**	прочитайте их в переменную t_tetlst-> board.
**
**	Сбои форматирования приведут к тому, что флаг t_tetlst-> is_valid будет
**	установлено на 0.
**
**	В крайнем случае, когда fd описывает пустой файл, read_tetra ()
**	вернуть значение ret (которое будет 0), чтобы t_tetlst-> is_valid Флаг
**	может быть установлен в 0 в функции read_file ().
*/

static ssize_t	read_tetra(int fd, t_tetlst *piece, ssize_t ret, char *buf)
{
	size_t	col;
	size_t	row;

	row = 0;
	while (row < 4 && piece->is_valid && (ret = read(fd, buf, 5)))
	{
		if (buf[ret - 1] == '\n' && ret == 5)
		{
			buf[ret - 1] = '\0';
			col = 0;
			while (buf[col])
			{
				if (IS_VALIDCHAR(buf[col]))
					(piece->board)[row][col] = buf[col];
				else
					piece->is_valid = 0;
				col++;
			}
			(piece->board)[row][col] = '\0';
		}
		else
			piece->is_valid = 0;
		row++;
	}
	return (ret);
}

/*
**	read_file - функция для чтения нашего файла fillit. Мы читаем 4 строки одновременно
**	с функцией read_tetra, а затем приступаем к чтению 1 строки для проверки
**	что это \ n символ.что это \ n символ.
**
**	Функция принимает в filepath и тетринимо список ptr ptr (который хранит
**	информация о каждом чтении в тетринимо) и параметр «имя», который дает
**	каждая тетра часть 1 имя «имя».
*/

void			read_file(const char *fpath, t_tetlst **start, char name)
{
	int			fd;
	char		buf[5];
	ssize_t		ret;

	ret = 0;
	if ((fd = open(fpath, O_RDONLY)) != -1)
	{
		tet_lstadd(start, tet_lstnew(name++));
		if (read_tetra(fd, *start, ret, buf) == 0)
			(*start)->is_valid = 0;
		check_tetra(*start);
		while ((*start)->is_valid && (ret = read(fd, buf, 1)) == 1)
		{
			if (buf[0] == '\n')
			{
				tet_lstadd(start, tet_lstnew(name++));
				if (read_tetra(fd, *start, ret, buf) == 0)
					(*start)->is_valid = 0;
				check_tetra(*start);
			}
			else
				(*start)->is_valid = 0;
		}
		close(fd);
	}
}
