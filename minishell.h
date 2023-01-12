/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 01:09:00 by mmassarw          #+#    #+#             */
/*   Updated: 2023/01/13 01:50:38 by hakaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"

# include <stdbool.h>
# include <curses.h>
# include <term.h>
# include <termios.h>
# include <sys/ioctl.h>
# include <string.h>
# include <dirent.h>
# include <sys/stat.h>
# include <signal.h>
# include <fcntl.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

//	colors
# define CYAN_BLUE_FONT	"\033[1;36m"
# define RESET_FONT	"\033[0m"
# define RED_FONT	"\033[31m"

// command flags
# define BUILTIN 68
# define PWD 923

// redirections enumiration
typedef enum e_rdr
{
	NONE = 0,
	INPUT = 1,
	OUTPUT = 2,
	HEREDOC = 3,
	APPEND = 4,
}	t_rdr;

//	cmd linked list
typedef struct s_cmd
{
	char			**cmd;
	t_rdr			rdr;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}	t_cmd;

// pipeline linked list
typedef struct s_pipeline
{
	t_cmd				*arg;
	struct s_pipline	*next_pipe;
}	t_pipeline;

// enviromental linked list
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

// struct with all the other structs in it
typedef struct s_mini
{
	char	*read_line;
	int		cmd_flag;
	char	cwd[2056];
}	t_mini;

//temporary functions, tbc if they'd be used or not
int	random_between(int min, int max);

#endif