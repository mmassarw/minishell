/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 01:05:21 by hakaddou          #+#    #+#             */
/*   Updated: 2023/01/17 01:18:56 by hakaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// prints the error message to STD ERROR
// passed and returns NULL
void	*perror_return(char *str)
{
	fd_printf(2, "%s", str);
	return (NULL);
}

int	return_env_size(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		env = env->next;
		i++;
	}
	return (i);
}

char	*join_key_val(char *key, char *value)
{
	char	*str;

	str = ft_strjoin(key, "=");
	str = ft_strjoin(str, value);
	return (str);
}

char	*join_key_eq(char *key, t_env *env)
{
	char	*str;

	str = ft_strjoin(key, "=");
	free(env->value);
	env->value = NULL;
	return (str);
}

char	**convert_env(t_mini *mini)
{
	t_env	*env;
	char	**s_env;
	int		i;

	env = mini->l_env;
	if (!env)
		return (perror_return("conv_env_function fail\n"));
	i = return_env_size(env);
	s_env = ft_calloc(sizeof(char *), i + 1);
	i = 0;
	while (env != NULL)
	{
		if (env->initialised == true)
		{
			if (env->value == NULL)
				s_env[i] = join_key_eq(env->key, env);
			else
				s_env[i] = join_key_val(env->key, env->value);
			i++;
		}
		env = env->next;
	}
	s_env[i] = NULL;
	return (s_env);
}

void	print_env_char(char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
	{
		printf("%s\n", args[i]);
		i++;
	}
}

// parse_input expects clean input from read_line
// some args are being free like the echo args as
// they're being split, after merging, the freeing should be removed
// and replaced with a speacial freeing function
void	parse_input(t_mini *mini)
{
	char	**args;
	int		id;

	if (ft_strncmp(mini->read_line, "print", 6) == 0)
		print_env_char(convert_env(mini));
	else if (ft_strncmp(mini->read_line, "pwd", 4) == 0)
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
	else if (ft_strncmp(mini->read_line, "unset", 5) == 0)
	{
		args = ft_split(mini->read_line, ' ');
		ft_unset(&args[1], mini);
		ft_free_split(args);
	}
	else if (ft_strncmp(mini->read_line, "./", 2) == 0)
	{
		id = fork();
		if (id == 0)
			execve("/home/vscode/src/minishell",
				ft_split(mini->read_line, ' '), convert_env(mini));
		else
			wait (NULL);
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

	(void) ac;
	(void) av;
	mini.env = (char *)env;
	mini.l_env = ft_parse_env(env);
	take_input(&mini);
	ft_free_env(mini.l_env);
}
