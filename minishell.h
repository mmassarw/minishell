/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 01:09:00 by mmassarw          #+#    #+#             */
/*   Updated: 2023/01/15 18:57:51 by mmassarw         ###   ########.fr       */
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
enum e_rdr
{
	NONE = 0,
	INPUT = 1,
	OUTPUT = 2,
	HEREDOC = 3,
	APPEND = 4,
};

// redirections linked list
typedef struct s_rdr
{
	char			*file;
	int				e_rdr;
	struct s_rdr	*next;
}	t_rdr;

// command linked list
typedef struct s_cmd
{
	char			**arg;
	t_rdr			*rdr;
	struct s_cmd	*next;
} t_cmd;

// enviromental linked list
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

t_env	*ft_parse_env(const char **envp);
t_cmd	*ft_parse_token(char **token);
int		ft_check_rdr(char *string);
void	ft_free_cmd(t_cmd *s_cmd);
void	ft_free_env(t_env *env_list);
void	ft_populate_cmd(t_cmd *cmd, char **token, int *j, int *i);
int		ft_count_till_pipe(char **token);
void	ft_print_cmd(t_cmd *s_head);

#endif