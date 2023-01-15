/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 16:49:59 by mmassarw          #+#    #+#             */
/*   Updated: 2023/01/15 18:57:55 by mmassarw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_env	*env_list;
	t_cmd	*cmd_list;

	(void) argc;
	argv++;
	env_list = ft_parse_env((const char **)envp);
	cmd_list = ft_parse_token(argv);
	ft_free_cmd(cmd_list);
	ft_free_env(env_list);
}
