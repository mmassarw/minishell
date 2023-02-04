/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenizer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 16:26:09 by mmassarw          #+#    #+#             */
/*   Updated: 2023/02/04 18:04:55 by mmassarw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

enum e_tokentype	ft_label_token(char c)
{
	if (c == ' ' || c == '\t')
		return (SPACES);
	if (c == '\'')
		return (SINGLE);
	if (c == '\"')
		return (DOUBLE);
	if (c == '$')
		return (VARIABLE);
	if (c == '<' || c == '>')
		return (REDIRECTION);
	if (c == '|')
		return (PIPE);
	return (WORD);
}

void	ft_eovchr(size_t *i, const char *str, enum e_tokentype *type)
{
	if (!ft_isalpha(*str) && *str != '\'' && *str != '\"' \
	&& *str != '_' && *str != '?')
		*type = WORD;
	if (*str != '?')
		while (str[*i - 1] && (ft_isalnum(str[*i - 1]) || str[*i - 1] == '_'))
			*i = *i + 1;
	else
		*i = 2;
}

void	ft_next_token(t_mini *mini, t_token *new, char *line)
{
	enum e_tokentype	type;
	size_t				i;

	type = ft_label_token(*line);
	i = 1;
	if (type == VARIABLE)
		ft_eovchr(&i, line + 1, &type);
	if (type == WORD || type == SPACES)
		while (line[i] && ft_label_token(line[i]) == type)
			i++;
	if (type == REDIRECTION && *line == line[i])
		i++;
	if (type == SINGLE || type == DOUBLE)
		i = (unsigned long) ft_strchr(line + 1, *line) \
		- (unsigned long) line + 1;
	new->type = type;
	new->content = ft_substr(line, 0, i);
	if (!new->content)
		ft_exit_shell(mini, 137, "Page allocation failure", 2);
}

void	ft_tokenlist(t_mini *mini)
{
	t_token	*new;
	t_token	*prev;
	size_t	offset;

	offset = 0;
	prev = NULL;
	while (*(mini->rl + offset))
	{
		new = (t_token *) ft_calloc(1, sizeof(t_token));
		if (!new)
			ft_exit_shell(mini, 137, "Page allocation failure", 2);
		ft_next_token(mini, new, (mini->rl + offset));
		offset += ft_strlen(new->content);
		new->prev = prev;
		if (prev)
			prev->next = new;
		if (!mini->l_token)
			mini->l_token = new;
		prev = new;
	}
}

size_t	ft_evalvar(t_env *env)
{
	char	**split;
	size_t	word_count;

	word_count = 0;
	if (!env || !env->initialised)
		return (word_count);
	if (env && env->initialised)
	{
		split = ft_split(env->value, ' ');
		while (split[word_count])
			word_count++;
		ft_free_split(split);
	}
	return (word_count);
}

bool	ft_evalfile(t_token *current, t_mini *mini, bool heredoc_flag)
{
	bool	valid;
	size_t	count;
	t_env	*env;

	valid = false;
	while (current && current->type != SPACES && current->type != PIPE \
	&& current->type != REDIRECTION)
	{
		if (heredoc_flag)
			current->type = WORD;
		if (current->type == VARIABLE)
		{
			env = env_already_exist(current->content, mini);
			count = ft_evalvar(env);
			if (count > 1)
				return (ft_syntaxerr("ambiguous redirect", 1, false));
			else
				valid |= count;
		}
		else
			valid = true;
		current = current->next;
	}
	return (ft_syntaxerr("ambiguous redirect", 1, valid));
}

bool	ft_evalrdr(t_token *head, t_mini *mini)
{
	t_token	*current;
	bool	heredoc_flag;

	heredoc_flag = 0;
	if (!ft_strncmp(head->content, "<<", 3))
		heredoc_flag = true;
	current = head->next;
	if (!current)
		return (ft_syntaxerr("syntax error near unexpected token", 258, 0));
	if (current->type == SPACES)
		current = current->next;
	if (!current || current->type == PIPE || current->type == REDIRECTION)
		return (ft_syntaxerr("syntax error near unexpected token", 258, 0));
	if (!ft_evalfile(current, mini, heredoc_flag))
		return (false);
	return (true);
}

bool	ft_syntaxerr(char *errmsg, int num, bool ret)
{
	if (ret == false)
	{
		fd_printf(2, "minishell: %s\n", errmsg);
		g_exit_code = num;
		return (ret);
	}
	else
		return (true);
}

bool	ft_evalops(t_token *head, t_mini *mini)
{
	t_token	*current;

	current = head;
	if (current->type == PIPE)
	{
		if (!current->prev)
			return (ft_syntaxerr("syntax error near unexpected token", 1, 0));
		current = head->next;
		if (!current)
			return (ft_syntaxerr("syntax error near unexpected token", 1, 0));
		if (current->type == SPACES)
			current = current->next;
		if (!current || current->type == PIPE)
			return (ft_syntaxerr("syntax error near unexpected token", 1, 0));
		return (true);
	}
	else
		return (ft_evalrdr(current, mini));
}

bool	ft_evaltokens(t_mini *mini)
{
	t_token	*current;

	current = mini->l_token;
	while(current)
	{
		if (current->type == PIPE || current->type == REDIRECTION)
			if (!ft_evalops(current, mini))
				return (false);
		if (current->type == SINGLE || current->type == DOUBLE)
			if (!ft_strchr(current->content + 1, *current->content))
				return (ft_syntaxerr("syntax error near unexpected token",\
				258, false));
		current = current->next;
	}
	return (true);
}

void	ft_init_splitvar(t_mini *mini, t_token **current, char **split)
{
	t_token	*tmp;
	size_t	i;

	i = 1;
	while(split[i])
	{
		tmp = (t_token *) ft_calloc(1, sizeof(t_token));
		if (!tmp)
			ft_exit_shell(mini, 137, "Page allocation failure", 2);
		tmp->type = SPACES;
		add_node_middle((*current), tmp);
		(*current) = (*current)->next;
		tmp = (t_token *) ft_calloc(1, sizeof(t_token));
		if (!tmp)
			ft_exit_shell(mini, 137, "Page allocation failure", 2);
		tmp->content = ft_strdup(split[i++]);
		if (!tmp->content)
		{
			tmp = (t_token *) ft_free(tmp);
			ft_exit_shell(mini, 137, "Page allocation failure", 2);
		}
		tmp->type = WORD;
		add_node_middle((*current), tmp);
		(*current) = (*current)->next;
	}
}

void	ft_splitvar(t_mini *mini, t_token **current)
{
	char	**split;

	split = ft_split((*current)->content, ' ');
	if (!split)
		ft_exit_shell(mini, 137, "Page allocation failure", 2);
	if (*split)
	{
		(*current)->content = (char *) ft_free((*current)->content);
		(*current)->content = ft_strdup(*split);
		ft_init_splitvar(mini, current, split);
	}
	else
		(*current)->content[0] = '\0';
	split = (char **) ft_free_split(split);
}

void	ft_expandvar(t_mini *mini)
{
	t_token	*current;
	t_env	*l_env;

	current = mini->l_token;
	while (current)
	{
		if (current->type == VARIABLE)
		{
			l_env = env_already_exist(current->content + 1, mini);
			current->content[0] = '\0';
			if ((l_env && l_env->initialised) || *(current->content + 1) == '?')
			{
				current->content = (char *) ft_free(current->content);
				if (l_env)
					current->content = ft_strdup(l_env->value);
				else
					current->content = ft_itoa(g_exit_code);
			}
			current->type = WORD;
			ft_splitvar(mini, &current);
		}
		current = current->next;
	}
}

char	*ft_addmidstr(char *start, t_env *match, size_t i, t_mini *mini)
{
	char	*result;

	if (match && match->initialised)
		result = ft_strjoin(match->value, start + i);
	else
		result = ft_strdup(ft_memmove(start, start + i,\
		ft_strlen(start + i) + 1));
	if (!result)
		ft_exit_shell(mini, 137, "Page allocation failure", 2);
	return (result);
}

char	*ft_str_expand(char *start, char *quote, t_mini *mini, size_t i)
{
	char				*temp;
	char				*hold;

	temp = NULL;
	hold = ft_substr(start + 1, 0, i - 1);
	temp = ft_addmidstr(start, env_already_exist(hold, mini), i, mini);
	hold = (char *) ft_free(hold);
	*start = '\0';
	start = ft_strjoin(quote, temp);
	temp = (char *) ft_free(temp);
	quote = (char *) ft_free(quote);
	quote = start;
	return (quote);
	
}

char	*ft_check_var(char *quote, t_mini *mini)
{
	char				*start;
	enum e_tokentype	type;
	size_t				i;

	start = ft_strchr(quote, '$');
	while (start)
	{
		i = 1;
		type = VARIABLE;
		ft_eovchr(&i, start + 1, &type);
		if (type == VARIABLE)
		{
			quote = ft_str_expand(start, quote, mini, i);
			start = ft_strchr(quote, '$');
		}
		else
			start = ft_strchr(start + 1, '$');
	}
	return (quote);
}

void	ft_collapsequotes(t_mini *mini)
{
	t_token	*current;

	current = mini->l_token;
	while (current)
	{
		if (current->type == SINGLE || current->type == DOUBLE)
		{
			ft_memmove(current->content, current->content + 1,\
			(ft_strlen(current->content) - 2));
			current->content[ft_strlen(current->content) - 2] = '\0';
			if (current->type == DOUBLE)
				current->content = ft_check_var(current->content, mini);
		}
		current = current->next;
	}
}

void	ft_tokenize(t_mini *mini)
{
	ft_tokenlist(mini);
	if (!ft_evaltokens(mini))
	{
		mini->l_token = (t_token *) ft_free(mini->l_token);
		return ;
	}
	ft_expandvar(mini);
	ft_collapsequotes(mini);
	// delete_empty_word_nodes(&(mini->l_token));
	join_same_type_nodes(mini->l_token);
	// print_linked_list_by_type(mini->l_token);
	delete_empty_nodes(&(mini->l_token));
}