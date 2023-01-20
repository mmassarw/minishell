/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 00:41:59 by hakaddou          #+#    #+#             */
/*   Updated: 2023/01/20 17:48:27 by hakaddou         ###   ########.fr       */
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

int	builtin_check(t_mini *mini)
{
	if (!mini->l_cmd->arg[0][0])
	{
		fd_printf(2, "minishell: : command not found\n");
		g_exit_code = COMMAND_FAIL;
		return (0);
	}
	else if (!ft_strncmp(mini->l_cmd->arg[0], "pwd", 4))
		print_pwd();
	else if (!ft_strncmp(mini->l_cmd->arg[0], "cd", 3))
		ft_cd(&mini->l_cmd->arg[1], mini);
	else if (!ft_strncmp(mini->l_cmd->arg[0], "exit", 5)
		|| mini->l_cmd->arg[0][0] == 'q')
		ft_exit(mini->l_cmd->arg, mini);
	else if (!ft_strncmp(mini->l_cmd->arg[0], "env", 4))
		print_env(mini);
	else if (!ft_strncmp(mini->l_cmd->arg[0], "echo", 5))
		ft_echo(&mini->l_cmd->arg[1]);
	else if (!ft_strncmp(mini->l_cmd->arg[0], "export", 7))
		ft_export(&mini->l_cmd->arg[1], mini);
	else if (!ft_strncmp(mini->l_cmd->arg[0], "unset", 5))
		ft_unset(&mini->l_cmd->arg[1], mini);
	else
		return (1);
	return (0);
}

// pipex functions
char	*split_and_join(char *av_cmd, char *splitted)
{
	char	*str;
	char	*path;

	str = ft_strjoin("/", av_cmd);
	path = ft_strjoin(splitted, str);
	free(str);
	return (path);
}

int	word_count(char const *s, char c)
{
	int	i;
	int	cnt;
	int	done;

	i = 0;
	cnt = 0;
	done = 1;
	while (s[i] != '\0')
	{
		if (s[i] == c && !done)
			done = 1;
		else if (s[i] != c && done)
		{
			++cnt;
			done = 0;
		}
		++i;
	}
	return (cnt);
}

char	*get_path(char *cmd, char *env)
{
	int		i;
	char	*str;
	char	**splitted;

	splitted = ft_split(env, ':');
	i = -1;
	while (++i < word_count(env, ':'))
	{
		str = split_and_join(cmd, splitted[i]);
		if (access(str, X_OK) == 0)
			break ;
		free(splitted[i]);
		splitted[i] = NULL;
		free(str);
		str = NULL;
	}
	while (splitted[i] != NULL)
	{
		free(splitted[i]);
		splitted[i++] = NULL;
	}
	free(splitted);
		splitted = NULL;
	return (str);
}

// pipex functions up

void set_env_underscore(char *cmd, t_mini *mini)
{
	t_env	*tmp;

	tmp = mini->l_env;
	while (tmp)
	{
		if (ft_strncmp("_=", tmp->key, 2) && ft_strlen(tmp->key) == 1)
			break ;
		tmp = tmp->next;
	}
	if (tmp == NULL)
		return ;
	free (tmp->value);
	tmp->value = ft_strdup(cmd);
}

void	parse_input(t_mini *mini)
{
	char	**envc;
	int		id;
	if (builtin_check(mini) == 0)
		return ;
	if (!mini->l_cmd->arg[0][0])
	return ;
	else if (!ft_strncmp(mini->l_cmd->arg[0], "./", 2)
			&& access(mini->l_cmd->arg[0], X_OK) == SUCCESS)
	{
		if (mini->l_cmd->arg[0][2] == '\0')
		{
			fd_printf(2, "bash: ./: is a directory\n");
			return ;
		}
		set_env_underscore(mini->l_cmd->arg[0] + 2, mini);
		id = fork();
		if (id == 0)
		{
			envc = convert_env(mini);
			if (execve(mini->l_cmd->arg[0], mini->l_cmd->arg, envc) == -1)
			{
				fd_printf(2, "minishell: %s\n", strerror(errno));
				ft_free_split(envc);
				g_exit_code = errno;
				exit (errno);
			}
		}
		else
			wait(NULL);
	}
	else
	{
		if (find_str_env("PATH", mini, KEY) == NULL)
		{
			g_exit_code = COMMAND_FAIL;
			fd_printf(2, RED_FONT "minishell:  command not found "RESET_FONT "%s\n",
				mini->l_cmd->arg[0]);
			return ;
		}
		char *cmd_path =  get_path(mini->l_cmd->arg[0], find_str_env("PATH", mini, VALUE));
		if (cmd_path)
		{
			set_env_underscore(mini->l_cmd->arg[0], mini);
			id = fork();
			if (id == 0)
			{
				envc = convert_env(mini);
				if (execve(cmd_path, mini->l_cmd->arg, envc) == -1)
				{
					fd_printf(2, "minishell: %s\n", strerror(errno));
					ft_free_split(envc);
					g_exit_code = errno;
					exit (errno);
				}
			}
			else
				wait (NULL);
			free (cmd_path);
		}
		else
		{
			g_exit_code = COMMAND_FAIL;
			fd_printf(2, RED_FONT "minishell:  command not found "RESET_FONT "%s\n",
				mini->l_cmd->arg[0]);
		}
			
	}
}
