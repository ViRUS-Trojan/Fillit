/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdelsie <vdelsie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/01 19:04:27 by vdelsie           #+#    #+#             */
/*   Updated: 2019/10/01 19:04:45 by vdelsie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** 21 Fillit реализация с использованием алгоритма Дональда Кнута DLX
** https://arxiv.org/pdf/cs/0011047.pdf
*/

#ifndef FILLIT_H
# define FILLIT_H
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>

/*
** t_node - структура узла, как описано Кнутом. Узлы представляют собой возможные
** комбинации, которые выполняют подмножество ограничений (см. ниже).
*/
typedef struct		s_node
{
	struct s_node	*left;
	struct s_node	*right;
	struct s_node	*up;
	struct s_node	*down;
	struct s_col	*col;
}					t_node;

/*
** t_col - структура столбца, как описано Кнутом. Столбцы описывают
** ограничения в точной задаче покрытия.
*/
typedef struct		s_col
{
	t_node			head;
	size_t			len;
	char			name[8];
	struct s_col	*prev;
	struct s_col	*next;
}					t_col;

/*
** список имен узлов
** Список имен-> строка всегда будет иметь длину 13 + 1 символов
** название предмета (1 символ)
** имена координат (3 знака) * 4
** нулевой термин (1 символ)
*/
typedef struct		s_nnlst
{
	char			row[14];
	struct s_nnlst	*next;
}					t_nnlst;

/*
** Структура покрытия eXact содержит критические переменные для выполнения
** танцевальные ссылки. В качестве бонуса большинство объектов malloc'd в этом заполнении
** проект также содержится в этой структуре, делая
** реинициализация структуры оболочки eXact намного проще ...
*/
typedef struct		s_xcover
{
	char			*col_names;
	t_col			*col_arr;
	t_col			*root;
	t_nnlst			*nnlst;
	t_node			*node_arr;
	t_node			**choice;
}					t_xcover;

/*
** t_tetlst - каждый элемент в структуре списка tetrinimo описывает различные
** аспекты 1 тетринимо кусок, который был прочитан из файла.
*/
typedef struct		s_tetlst
{
	char			board[4][5];
	int				is_valid;
	struct s_tetlst	*next;
	int				block_count;
	int				neighbors;
	int				row_coords[4];
	int				col_coords[4];
	int				min_row;
	int				min_col;
	int				max_row;
	int				max_col;
	char			name[1];
}					t_tetlst;

/*
** main.c - включает в себя различные разные. полезные функции
*/
int					mod_strcmp(char *s1, char *s2);
void				mod_strcpy(char *dest, char *src);

/*
** tetra_list_utils.c - Tetrinimo связанный список утилит.
*/
t_tetlst			*tet_lstnew(char name);
void				tet_lstadd(t_tetlst **alst, t_tetlst *new);
size_t				tet_lstlen(t_tetlst *begin_list);
void				tet_lstrev(t_tetlst **begin_list);
void				tet_lstfree(t_tetlst **begin_list);

/*
** node_namelist_utils.c - имя узла утилиты связанного списка.
*/
t_nnlst				*nn_lstnew(t_tetlst *piece, int row, int col);
void				nn_lstadd(t_nnlst **alst, t_nnlst *new);
size_t				nn_lstlen(t_nnlst *begin_list);
void				nn_lstrev(t_nnlst **begin_list);
void				nn_lstfree(t_nnlst **begin_list);

/*
** fillit_read.c - Функции для чтения файлов fillit и загрузки их в
** тетринимо список конструкций. Также содержит функции для проверки того, что чтение
** в тетринимо доска содержит действующее тетринимо.
*/
void				read_file(const char *fpath, t_tetlst **start, char name);

/*
** create_xcover_cols.c - Функции для создания столбцов (a.k.a.
** ограничения) в нашей проблеме покрытия eXact.
*/
char				*make_col_names(int slen, t_tetlst *pieces,
									ssize_t tlst_len);
void				link_columns(t_xcover *xcov);

/*
** create_xcover_rows.c - Функции для создания узлов строки (a.k.a.
** возможные позиции куска) в нашей проблеме с крышкой eXact.
*/
void				make_node_namelst(t_nnlst **nnlst, t_tetlst *pieces,
										int bsize);
void				link_rows(t_xcover *xcov);

/*
** Dance_links_ops.c - Танцевальные ссылки операций
** ПРИМЕЧАНИЕ: операции с узлами ДОЛЖНЫ быть определены до того, как col накрывают и раскрывают операции.
*/
void				cover_col(t_col *c);
void				uncover_col(t_col *c);

/*
** fillit_dlx_solve.c - это часть, которая решает заливку ...
*/
void				prep_xcover(t_xcover *xcov, t_tetlst **tetra_pieces,
								int bsize);
int					dlx_solve(t_xcover *xcov, int level);
void				cover_row_cols(t_node *curr_node);
void				uncover_row_cols(t_node *curr_node);


/*
** cleanup_and_print.c - функции для печати окончательного результата, а также для
** освободить все структуры данных malloc'd.
*/
void				emancipate_xcover(t_xcover *xcov);
void				disp_res_n_clean(t_xcover *xcov, t_tetlst **tetra_pieces,
									int bsize);

#endif
