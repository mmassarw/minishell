/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 01:09:00 by mmassarw          #+#    #+#             */
/*   Updated: 2023/01/20 23:52:18 by hakaddou         ###   ########.fr       */
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
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

//	colors
# define BLUE_FONT	"\033[1;36m"
# define RESET_FONT	"\033[0m"
# define RED_FONT	"\033[31m"

// eeror strings
# define ALPHA_EXIT "minishell: exit: %s: numeric argument required\n"
# define UNSET_NO_ARG "unset: not enough arguments\n"

// PATHS
# define NORM_PATH "/Users/hakaddou/Desktop/minishell/minishell"
# define VALG_PATH "/home/vscode/src/minishell"

// command flags
# define BUILTIN 68
# define PWD 923
# define VALUE 432
# define KEY 645

//builtin exit codes
# define SUCCESS 0
# define PWD_FAIL_CODE 2
# define ENV_FAIL_CODE 1
# define EXPORT_FAIL_CODE 1
# define EXPORT_FLAG 654
# define UNSET_FLAG 456
# define UNSET_FAIL_CODE 1
# define COMMAND_FAIL 127
# define EXIT_FAIL 1
# define EXIT_ALPHA_CODE 255
# define CD_FAIL 1

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
	int				fd;
	enum e_rdr		e_rdr;
	struct s_rdr	*next;
}	t_rdr;

// command linked list
typedef struct s_cmd
{
	char			**arg;
	t_rdr			*rdr;
	int				fd_pipe[2];
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
	t_cmd	*l_cmd;
	t_env	*l_env;
	char	*rl;
	char	**token;
	int		cmd_flag;
}	t_mini;

// temporary functions, tbc if they'd be used or not

int		random_between(int min, int max);

// builtins

void	print_pwd(void);
void	ft_echo(char **args);
void	ft_export(char **args, t_mini *mini);
void	ft_unset(char **args, t_mini *mini);
void	ft_exit(char **args, t_mini *mini);
int		builtin_check(t_mini *mini);

// export

void	ft_export(char **args, t_mini *mini);
void	parse_new_export(char *arg, t_mini *mini);
void	ft_modify_env(char *arg, t_mini *mini);
int		env_already_exist(char *arg, t_mini *mini);
int		check_export_args(char *arg);
int		check_valid_identifier(char *arg);
void	add_to_env(char *arg, t_mini *mini);
char	*set_env_value(char *arg, t_env *new);
char	*set_env_key(char *arg);
void	print_export(t_mini *mini);

// parsing

int		ft_check_rdr(char *string);
void	ft_populate_cmd(t_mini *mini, t_cmd *cmd, char **token, int *i);
int		ft_count_till_pipe(char **token);
void	ft_parse_env(t_mini *mini, const char **envp);
void	ft_parse_token(t_mini *mini, char **token);
void	set_env_underscore(char *cmd, t_mini *mini);

// frees

void	ft_free_cmd(t_cmd *s_cmd);
void	ft_free_env(t_env *env_list);
void	ft_free_cycle(t_mini *mini);
void	ft_free_all(t_mini *mini);

// exit shell

void	ft_exit_shell(t_mini *m, int er, char *p_er, int fd);

// utils

void	ft_print_cmd(t_cmd *s_head);
void	print_env(t_mini *mini);

void	parse_input(t_mini *mini);

// global exit code

int	g_exit_code;

// unset
void	ft_unset(char **args, t_mini *mini);
int		check_valid_identifier_export(char *arg);
int		check_unset_args(char *arg);
void	free_single_env(t_env *node);
void	delete_env_list(char *arg, t_mini *mini);

// exit
void	ft_exit(char **args, t_mini *mini);
int		arg_count(char **args);
int		check_exit_alpha(char **args);
void	exit_and_print(int code);
void	exit_success(char **args, t_mini *mini);

// chdir (cd)
void	ft_cd(char **args, t_mini *mini);
void	go_to_home(t_mini *mini);
char	*find_str_env(char *arg, t_mini *mini, int flag);

// env **char conversion from linked list
void	*perror_return(char *str, void *ret);
int		return_env_size(t_env *env);
char	*join_key_val(char *key, char *value);
char	*join_key_eq(char *key, t_env *env);
char	**convert_env(t_mini *mini);

// get path srcs
char	*split_and_join(char *av_cmd, char *splitted);
int		word_count(char const *s, char c);
char	*get_path(char *cmd, char *env);

#endif
