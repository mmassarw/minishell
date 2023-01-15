
t_rdr	*ft_add_rdr(char **token, int *i)
{
	t_rdr	*rdr_new;


	rdr_new = (t_rdr *) ft_calloc(1, sizeof(t_rdr));
	if (!rdr_new)
		exit(1);	// change exit
	rdr_new->e_rdr = ft_check_rdr(token[*i++]);
	rdr_new->file = ft_strdup(token[*i]);
	if (!rdr_new->file)
		exit(1);	// change exit
	rdr_new->next = NULL;
	return(rdr_new);
}

int	ft_init_arg_n_rdr(t_cmd *cmd, char **token)
{
	t_rdr	*rdr_new;
	t_rdr	*rdr_head;
	t_rdr	*rdr_tail;
	int		i[2];

	i[0] = 0;
	i[1] = 0;
	rdr_head = NULL;
	cmd->arg = (char **) ft_calloc(ft_count_till_pipe(token), 8);
	if (!cmd->arg)
		exit(1);	// change exit
	while (token[i[0]] && ft_strncmp(token[i[0]], "|", 2))
	{
		if (ft_check_rdr(token[i[0]]))
		{
			rdr_new = ft_add_rdr(token, i);
			if (rdr_head == NULL)
				rdr_head = rdr_new;
			else
				rdr_tail->next = rdr_new;
			rdr_tail = rdr_new;
			cmd->rdr = rdr_head;
		}
		else
		{
			cmd->arg[i[1]] = ft_strdup(token[i[0]]);
			if (!cmd->arg[i[1]])
				exit(1);	// change exit
			i[1]++;
		}
		i[0]++;
	}
	return (i[0]);
}