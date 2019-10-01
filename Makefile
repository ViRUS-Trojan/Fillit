# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vdelsie <vdelsie@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/01 19:43:59 by vdelsie           #+#    #+#              #
#    Updated: 2019/10/01 19:44:19 by vdelsie          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = fillit
SRCS_DIR = ./
INCLUDE = ./
RM = /bin/rm -f

FILES = main fillit_read tetra_list_utils node_namelist_utils dancing_links_ops\
make_xcover_cols make_xcover_rows fillit_dlx_solve cleanup_and_print

CFILES = $(patsubst %, $(SRCS_DIR)%.c, $(FILES))
OFILES = $(patsubst %, %.o, $(FILES))

CFLAGS = -Wall -Wextra -Werror

.PHONY: all clean fclean re

all: $(NAME)

$(OFILES):
	gcc $(CFLAGS) -c -I$(INCLUDE) $(CFILES)

$(NAME): $(OFILES)
	gcc $(CFLAGS) -o $(NAME) $(OFILES)

clean:
	$(RM) $(OFILES)

fclean: clean
	$(RM) $(NAME)

re: fclean all