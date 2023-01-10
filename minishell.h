/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 01:09:00 by mmassarw          #+#    #+#             */
/*   Updated: 2023/01/11 00:09:53 by mmassarw         ###   ########.fr       */
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

#endif