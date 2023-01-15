/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 01:05:21 by hakaddou          #+#    #+#             */
/*   Updated: 2023/01/16 02:02:27 by mmassarw         ###   ########.fr       */
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

void	exit_success(t_mini *mini)
{
	ft_free_all(mini);
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
		exit_success(mini);
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
	free(mini->l_cmd->arg[0]);
	ft_free_split(args);
	ft_free_env(mini->l_env);
	exit_and_print(code);
}

// parse_input expects clean input from l_cmd->arg[0]
// some args are being free like the echo args as
// they're being split, after merging, the freeing should be removed
// and replaced with a speacial freeing function
void	parse_input(t_mini *mini)
{
	if (ft_strncmp(mini->l_cmd->arg[0], "pwd", 4) == 0)
		print_pwd();
	else if (ft_strncmp(mini->l_cmd->arg[0], "exit", 5) == 0
		|| mini->l_cmd->arg[0][0] == 'q')
	{
		ft_exit(mini->l_cmd->arg, mini);
	}
	else if (ft_strncmp(mini->l_cmd->arg[0], "env", 4) == 0)
		print_env(mini);
	else if (ft_strncmp(mini->l_cmd->arg[0], "echo", 5) == 0)
	{
		mini->l_cmd->arg = ft_split(mini->l_cmd->arg[0], ' ');
		ft_echo(&mini->l_cmd->arg[1]);
		ft_free_split(mini->l_cmd->arg);
	}
	else if (ft_strncmp(mini->l_cmd->arg[0], "export", 7) == 0)
	{
		mini->l_cmd->arg = ft_split(mini->l_cmd->arg[0], ' ');
		ft_export(&mini->l_cmd->arg[1], mini);
		ft_free_split(mini->l_cmd->arg);
	}
	else if (!mini->l_cmd->arg[0][0])
		return ;
	else
	{
		g_exit_code = COMMAND_FAIL;
		printf(RED_FONT "minishell:  command not found "RESET_FONT "%s\n",
			mini->l_cmd->arg[0]);
	}
}
