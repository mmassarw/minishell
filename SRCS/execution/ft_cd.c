/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 19:07:10 by hakaddou          #+#    #+#             */
/*   Updated: 2023/01/21 08:31:28 by hakaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
		g_exit_code = CD_FAIL;
		return ;
	}
}
