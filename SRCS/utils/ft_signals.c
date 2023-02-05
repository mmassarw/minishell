/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 21:33:20 by mmassarw          #+#    #+#             */
/*   Updated: 2023/02/05 23:26:11 by mmassarw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_interupt(int sig)
{
	if (sig == SIGINT)
	{
		g_exit_code = 1;
		rl_on_new_line();
		rl_redisplay();
		write(STDERR_FILENO, "  \n", 4);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	ft_intheredoc(int sig)
{
	if (sig == SIGINT)
	{
		g_exit_code = -420;
		rl_on_new_line();
		rl_replace_line("", 0);
		write(STDERR_FILENO, "  \n", 4);
		close(0);
	}
}
