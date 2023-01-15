/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 01:09:00 by mmassarw          #+#    #+#             */
/*   Updated: 2023/01/15 06:53:27 by hakaddou         ###   ########.fr       */
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
# define BLUE_FONT	"\033[1;36m"
# define RESET_FONT	"\033[0m"
# define RED_FONT	"\033[31m"

// command flags
# define BUILTIN 68
# define PWD 923

// global exit code
int	exit_code;

//builtin exit codes
# define SUCCESS 0
# define PWD_FAIL_CODE 2
# define ENV_FAIL_CODE 1
# define EXPORT_FAIL_CODE 1
# define EXPORT_FLAG 654
# define COMMAND_FAIL 127

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
	enum e_rdr		e_rdr;
	struct s_rdr	*next;
}	t_rdr;

// command linked list
typedef struct s_cmd
{
	char			**arg;
	t_rdr			*rdr;
	struct s_cmd	*next;
}	t_cmd;

// enviromental linked list
typedef struct s_env
{
	char			*key;
	char			*value;
	bool			initialised;
	struct s_env	*next;
}	t_env;

// struct with all the other structs in it
typedef struct s_mini
{
	char	*read_line;
	int		cmd_flag;
	t_env	*l_env;
	char	*env;
}	t_mini;

//temporary functions, tbc if they'd be used or not
int		random_between(int min, int max);

//env parsing
t_env	*ft_parse_env(const char **envp);
void	ft_free_env(t_env *env_list);

// builtins
void	print_env(t_mini *mini);
void	print_pwd(void);
void	ft_echo(char **args);
void	ft_export(char **args, t_mini *mini);

#endif