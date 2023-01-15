/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 14:58:28 by mmassarw          #+#    #+#             */
/*   Updated: 2023/01/15 07:44:39 by mmassarw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free_env(t_env *env_list)
{
	t_env	*env_head;

	env_head = env_list;
	while (env_head)
	{
		env_list = env_head;
		env_head = env_list->next;
		free(env_list->key);
		free(env_list->value);
		free(env_list);
	}
}

void free_structs(t_cmd *cmd) {
    t_cmd *current_cmd = cmd;
    t_rdr *current_rdr;
    while (current_cmd != NULL) {
        current_rdr = current_cmd->rdr;
        while (current_rdr != NULL) {
            free(current_rdr->file);
            t_rdr *next_rdr = current_rdr->next;
            free(current_rdr);
            current_rdr = next_rdr;
        }
        for (int i = 0; current_cmd->arg[i]; i++)
            free(current_cmd->arg[i]);
        free(current_cmd->arg);
        t_cmd *next_cmd = current_cmd->next;
        free(current_cmd);
        current_cmd = next_cmd;
    }
}

void print_linked_list(t_cmd *head)
{
	int	list = 0;
    t_cmd *current = head;
    while (current != NULL)
    {
		printf(">>>>>>>> %d <<<<<<<<<\n", list++);
        printf("Command: ");
        while (*current->arg)
            printf("%s ", *current->arg++);
        printf("\nRedirections: ");
        t_rdr *rdr = current->rdr;
        while (rdr != NULL)
        {
            printf("%i %s ", rdr->e_rdr, rdr->file);
            rdr = rdr->next;
        }
        printf("\n\n");
        current = current->next;
    }
}

int	main(int argc, char **argv, char **envp)
{
	// t_env	*env_list;
	t_cmd	*cmd_list;

	cmd_list = NULL;
	(void) argc;
	// (void) argv;
	(void) envp;
	// env_list = ft_parse_env((const char **)envp);
	// ft_free_env(env_list);
	// argv++;
	// for(int i = 0; argv[i]; i++)
	// 	printf("%s\n", argv[i]);
	argv++;
	cmd_list = ft_parse_token(argv);
	print_linked_list(cmd_list);
	// free_structs(cmd_list);
}
