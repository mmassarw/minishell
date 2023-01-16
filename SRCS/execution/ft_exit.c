/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 00:37:39 by hakaddou          #+#    #+#             */
/*   Updated: 2023/01/16 23:54:04 by hakaddou         ###   ########.fr       */
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
	printf("exit code is %d before exiting\n", g_exit_code);
	exit (code);
}

void	exit_success(char **args, t_mini *mini)
{
	free(mini->read_line);
	ft_free_split(args);
	ft_free_env(mini->l_env);
	g_exit_code = 0;
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
	g_exit_code = code;
	exit_and_print(code);
}
