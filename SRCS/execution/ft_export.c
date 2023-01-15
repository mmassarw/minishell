/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 19:58:43 by hakaddou          #+#    #+#             */
/*   Updated: 2023/01/15 14:07:48 by hakaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// prints the env in special format with
//the uninitialised values
void	print_export(t_mini *mini)
{
	t_env	*env;

	env = mini->l_env;
	if (!env)
	{
		fd_printf(2, "env: permission denied\n");
		exit_code = ENV_FAIL_CODE;
		return ;
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

// check if export arguments are valid just like
// bash export takes them
// checks if there is no number or '=' then checks
// if the argument before the first = has any white spaces
// or invalid character other than '_'
int	check_valid_identifier(char *arg)
{
	int		i;
	size_t	d;

	d = 0;
	i = -1;
	if ((arg[0] && ft_isdigit(arg[0])) || arg[0] == '=')
		return (1);
	while (arg[++i] != '\0' && arg[i] != '=')
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			d++;
	if (d)
		return (1);
	else
		return (0);
}

// place holder for check_valid_identifer function
int	check_export_args(char *arg)
{
	if (!arg[0] || check_valid_identifier(arg))
	{
		exit_code = EXPORT_FLAG;
		return (1);
	}
	return (0);
}

// checks if the export key exists and if it does it returns 1
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
		if (0 == ft_strncmp(temp->key, arg, i))
			return (1);
		temp = temp->next;
	}
	return (0);
}

// it sets the key of the env to whatever is before the 
// first occurance of '='
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
	new_key[i] = '\0';
	return (new_key);
}

// stes the value of a key to either NULL if there
// is no initialisation with '=' or else to whatever
// after the first occurance of '='
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

// allocates memory for a new node, sets is's
// next to NULL since this should be the last node.
// It also sets the key and value of the
// environmental variables by calling set_value and set_key
// functions. It then itrates to the end of the linked list
// and sets the tail's next to the new node
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

// after validating that the env variable already exists,
// it looks for the node with the key and modifies it if
// there is '=' or else it just returns
void	ft_modify_env(char *arg, t_mini *mini)
{
	t_env	*temp;
	char	*new_key;
	int		i;

	new_key = ft_strchr(arg, '=');
	if (new_key == NULL)
		return ;
	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	temp = mini->l_env;
	while (temp)
	{
		if (ft_strncmp(temp->key, arg, i) == 0)
			break ;
		temp = temp->next;
	}
	free (temp->value);
	new_key++;
	temp->initialised = true;
	temp->value = ft_strdup(new_key);
}

// checks if the export argument already exists as key or
// it should be added
void	parse_new_export(char *arg, t_mini *mini)
{
	if (!env_already_exist(arg, mini))
	{
		fd_printf(1, "%s added\n", arg);
		add_to_env(arg, mini);
	}
	else
	{
		ft_modify_env(arg, mini);
		fd_printf(1, "%s modified\n", arg);
	}
}

// if there are no arguemnts passed then it only prints
// the env variables whether initialised or not
// and if there are arguemnts it loops to each one
// and checks for validity, if it is not valid it
// skips the argument and moves to the next one
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
