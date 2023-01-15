/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 01:05:21 by hakaddou          #+#    #+#             */
/*   Updated: 2023/01/15 19:48:27 by hakaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	arg_count(char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
		i++;
	return (i);
}

int	check_exit_alpha(char **args)
{
	int	i;
	int	d;

	i = -1;
	while (args[++i] != NULL)
	{
		d = -1;
		while (args[i][++d] != '\0')
		{
			if (true == ft_isalpha(args[i][d]))
			{
				fd_printf(2, ALPHA_EXIT, args[i]);
				return (1);
			}
		}
	}
	return (0);
}

void	exit_and_print(int code)
{
	printf("exit\n");
	exit (code);
}

void	exit_success(char **args, t_mini *mini)
{
	free(mini->read_line);
	ft_free_split(args);
	ft_free_env(mini->l_env);
	exit_and_print(0);
}

// safe exit exits with the code passed to it
// if the code is greater than 255 it truncates it
// VERY IMPORTANT needs to be modidfed and change the freeing
// after merging with parsing as  ft_exit current frees
// the readline which should not be the case
// as parsing parses the readline
void	ft_exit(char **args, t_mini *mini)
{
	int	code;

	if (!args[1])
		exit_success(args, mini);
	else if (2 < arg_count(args))
	{
		fd_printf(2, "minishell: exit: too many arguments\n");
		g_exit_code = EXIT_FAIL;
		ft_free_split(args);
		return ;
	}
	if (check_exit_alpha(&args[1]))
		code = EXIT_ALPHA_CODE;
	else
		code = ft_atoi(args[1]) % 256;
	free(mini->read_line);
	ft_free_split(args);
	ft_free_env(mini->l_env);
	exit_and_print(code);
}

// parse_input expects clean input from read_line
// some args are being free like the echo args as
// they're being split, after merging, the freeing should be removed
// and replaced with a speacial freeing function
void	parse_input(t_mini *mini)
{
	char	**args;

	if (ft_strncmp(mini->read_line, "pwd", 4) == 0)
		print_pwd();
	else if (ft_strncmp(mini->read_line, "exit", 4) == 0
		|| mini->read_line[0] == 'q')
	{
		args = ft_split(mini->read_line, ' ');
		ft_exit(args, mini);
	}
	else if (ft_strncmp(mini->read_line, "env", 4) == 0)
		print_env(mini);
	else if (ft_strncmp(mini->read_line, "echo", 4) == 0)
	{
		args = ft_split(mini->read_line, ' ');
		ft_echo(&args[1]);
		ft_free_split(args);
	}
	else if (ft_strncmp(mini->read_line, "export", 6) == 0)
	{
		args = ft_split(mini->read_line, ' ');
		ft_export(&args[1], mini);
		ft_free_split(args);
	}
	else if (!mini->read_line[0])
		return ;
	else
	{
		g_exit_code = COMMAND_FAIL;
		printf(RED_FONT "minishell:  command not found "RESET_FONT "%s\n",
			mini->read_line);
	}
}

void	take_input(t_mini *mini)
{
	mini->read_line = NULL;
	while (1)
	{
		mini->read_line = readline (BLUE_FONT"mminishell-3.2> " RESET_FONT);
		if (mini->read_line == NULL)
			exit(0);
		parse_input(mini);
		free(mini->read_line);
	}
}

int	main(int ac, char **av, const char **env)
{
	t_mini	mini;
	t_env	*l_env;

	g_exit_code = 0;
	(void) ac;
	(void) av;
	l_env = NULL;
	mini.env = (char *)env;
	l_env = ft_parse_env(env);
	mini.l_env = l_env;
	take_input(&mini);
	ft_free_env(mini.l_env);
}
