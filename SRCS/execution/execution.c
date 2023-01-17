/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 00:41:59 by hakaddou          #+#    #+#             */
/*   Updated: 2023/01/18 00:42:28 by hakaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// parse_input expects clean input from l_cmd->arg[0]
// some args are being free like the echo args as
// they're being split, after merging, the freeing should be removed
// and replaced with a speacial freeing function

void	execute_ve_cmd(t_mini *mini)
{
	int	id;

	id = fork();
	if (id == 0)
		execve(VALG_PATH, mini->l_cmd->arg,
			convert_env(mini));
	else
		wait (NULL);
}

char	*find_str_env(char *arg, t_mini *mini, int flag)
{
	t_env	*temp;

	if (!mini->l_env || !arg)
		return (NULL);
	temp = mini->l_env;
	while (temp != NULL && ft_strncmp(arg, temp->key, ft_strlen(arg) + 1))
		temp = temp->next;
	if (!temp)
		return (NULL);
	if (flag == VALUE)
		return (temp->value);
	else if (flag == KEY)
		return (temp->key);
	return (NULL);
}

void	go_to_home(t_mini *mini)
{
	char	*path;
	int		i;

	i = 42;
	path = find_str_env("HOME", mini, VALUE);
	if (!path)
	{
		fd_printf(2, "minishell: cd: HOME not set\n");
		g_exit_code = CD_FAIL;
		return ;
	}
	i = chdir(path);
	if (i)
	{
		fd_printf(2, "minishell: cd: %s: %s\n", strerror(errno));
		return ;
	}
}

void	ft_cd(char **args, t_mini *mini)
{
	int	i;

	i = 42;
	if (!args[0])
	{
		go_to_home (mini);
		return ;
	}
	i = chdir(args[0]);
	if (i)
	{
		fd_printf(2, "minishell: cd: %s: %s\n", args[0], strerror(errno));
		return ;
	}
}

void	parse_input(t_mini *mini)
{
	if (ft_strncmp(mini->l_cmd->arg[0], "pwd", 4) == 0)
		print_pwd();
	else if (ft_strncmp(mini->l_cmd->arg[0], "cd", 3) == 0)
		ft_cd(&mini->l_cmd->arg[1], mini);
	else if (ft_strncmp(mini->l_cmd->arg[0], "exit", 5) == 0
		|| mini->l_cmd->arg[0][0] == 'q')
		ft_exit(mini->l_cmd->arg, mini);
	else if (ft_strncmp(mini->l_cmd->arg[0], "env", 4) == 0)
		print_env(mini);
	else if (ft_strncmp(mini->l_cmd->arg[0], "echo", 5) == 0)
		ft_echo(&mini->l_cmd->arg[1]);
	else if (ft_strncmp(mini->l_cmd->arg[0], "export", 7) == 0)
		ft_export(&mini->l_cmd->arg[1], mini);
	else if (ft_strncmp(mini->l_cmd->arg[0], "unset", 5) == 0)
		ft_unset(&mini->l_cmd->arg[1], mini);
	else if (ft_strncmp(mini->l_cmd->arg[0], "./", 2) == 0)
		execute_ve_cmd(mini);
	else if (!mini->l_cmd->arg[0][0])
		return ;
	else
	{
		g_exit_code = COMMAND_FAIL;
		printf(RED_FONT "minishell:  command not found "RESET_FONT "%s\n",
			mini->l_cmd->arg[0]);
	}
}
