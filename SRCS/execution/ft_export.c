/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 19:58:43 by hakaddou          #+#    #+#             */
/*   Updated: 2023/01/15 06:55:33 by hakaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	print_export(t_mini *mini)
{
	t_env	*env;

	env = mini->l_env;
	if (!env)
	{
		fd_printf(2, "env: permission denied\n");
		exit_code = ENV_FAIL_CODE;
	}
	while (env)
	{
		if (env->value)
			fd_printf(1, "declare -x %s=\"%s\"\n", env->key, env->value);
		else
			fd_printf(1, "declare -x %s=\"\"\n", env->key);
		env = env->next;
	}
	exit_code = SUCCESS;
}

int	check_valid_identifier(char *arg)
{
	int		i;
	size_t	d;

	d = 0;
	i = 0;
	if ((arg[0] && ft_isdigit(arg[0])) || arg[0] == '=')
		return (1);
	while (arg[i] != '\0' && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			d++;
		i++;
	}
	if (d)
		return (1);
	else
		return (0);
}

int	check_export_args(char *arg)
{
	if (!arg[0] || check_valid_identifier(arg))
	{
		exit_code = EXPORT_FLAG;
		return (1);
	}
	return (0);
}

int	env_already_exist(char *arg, t_mini *mini)
{
	t_env	*temp;
	int		i;

	i = 0;
	while (arg[i] != '\0' && arg[i] != '=')
		i++;
	temp = mini->l_env;
	while (temp)
	{
		if (ft_strncmp(arg, temp->key, i) == 0)
			return (1);
		temp = temp->next;
	}
	return (0);
}

char	*set_env_key(char *arg)
{
	int		i;
	char	*new_key;

	i = 0;
	while (arg[i] != '\0' && arg[i] != '=')
		i++;
	new_key = ft_calloc(1, i + 1);
	i = 0;
	while (arg[i] != '\0' && arg[i] != '=')
	{
		new_key[i] = arg[i];
		i++;
	}
	return (new_key);
}

char	*set_env_value(char *arg, t_env *new)
{
	char	*value;

	value = ft_strchr(arg, '=');
	if (value == NULL)
	{
		new->initialised = false;
		return (ft_strdup(""));
	}
	else
	{
		new->initialised = true;
		value++;
		return (ft_strdup(value));
	}
}

void	add_to_env(char *arg, t_mini *mini)
{
	t_env	*temp;
	t_env	*new;

	temp = mini->l_env;
	new = ft_calloc(1, sizeof(t_env));
	new->next = NULL;
	new->value = set_env_value(arg, new);
	new->key = set_env_key(arg);
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
}

void	ft_modify_env(char *arg, t_mini *mini)
{
	t_env	*temp;
	char	*new_key;
	int		i;

	i = 0;
	new_key = ft_strchr(arg, '=');
	if (new_key == NULL)
		return ;
	while (arg[i] != '\0' && arg[i] != '=')
		i++;
	temp = mini->l_env;
	while (temp)
	{
		if (ft_strncmp(arg, temp->key, i) == 0)
			break ;
		temp = temp->next;
	}
	free (temp->value);
	new_key++;
	temp->initialised = true;
	temp->value = ft_strdup(new_key);
}

void	parse_new_export(char *arg, t_mini *mini)
{
	if (!env_already_exist(arg, mini))
	{
		printf("%s added\n", arg);
		add_to_env(arg, mini);
	}
	else
	{
		ft_modify_env(arg, mini);
		printf("%s modified\n", arg);
	}
}

void	ft_export(char **args, t_mini *mini)
{
	int		i;

	i = 0;
	if (!args[0])
	{
		print_export(mini);
		return ;
	}
	while (args[i] != NULL)
	{
		if (check_export_args(args[i]))
			fd_printf(2, "minishell: export: `%s': not a valid identifier\n",
				args[i]);
		else
			parse_new_export(args[i], mini);
		i++;
	}
	if (exit_code == EXPORT_FLAG)
		exit_code = EXPORT_FAIL_CODE;
}
