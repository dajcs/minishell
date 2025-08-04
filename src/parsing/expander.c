/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 11:34:10 by anemet            #+#    #+#             */
/*   Updated: 2025/08/04 17:49:07 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/* get_env_value
	- Using getenv() to get value
	- char *value = getenv(var_name);
	- return (ft_strdup(value)); // Return a copy
	- Return empty string if not found, like bash
*/
char	*get_env_value(const char *var_name)
{
	char	*value;

	value = getenv(var_name);
	if (value == NULL)
		return (ft_strdup(""));
	return (ft_strdup(value));
}
/* expand_once()
	searches the token for $var and expands the first var
	return the orig token if no vars inside or the new expanded token otherwise

	int		alen;			// length of token after var
	int		vlen;			// length of var
	char	*dollar;		// pointer to $ char in a token
	char	*var;			// var string
	char	*var_val;		// var expanded to var_val string
	char	*before_var;	// token string before the $ sign
	char	*after_var;		// token string after the var
	char	*tmp_token;		// tmp string: before_val + var_val
	char	*new_token;		// new_token: before_val + var_val + after_var

	- Scan the string for '$'.
	- if no '$' then return the original token
	* When '$' found:
	- dollar++ to advance pointer after the '$' character
	- end_varchar() searches the end of var
							(delimited by space/$/meta/quote and sets vlen)
	- create var with ft_substr()
	- get var value using getenv() or the last_exit_status for $?.
	- Reconstruct the string: [part before $] + [value] + [part after var]
	- free the strings used to create the new_token
	- a new string is created, the old one must be freed by the caller
*/
char	*expand_once(char *token, int last_exit_status)
{
	t_expand	x;

	x.dollar = ft_strchr(token, '$');
	if (!x.dollar)
		return (token);
	x.dollar++;
	x.vlen = 0;
	end_varchar(x.dollar, &x.vlen);
	x.var = ft_substr(x.dollar, 0, x.vlen);
	if (x.var[0] == '?')
		x.var_val = ft_itoa(last_exit_status);
	else
		x.var_val = get_env_value(x.var);
	x.before_var = ft_substr(token, 0, x.dollar - token - 1);
	x.alen = ft_strlen(x.dollar + ft_strlen(x.var));
	x.after_var = ft_substr(token, x.dollar - token + ft_strlen(x.var), x.alen);
	x.tmp_token = ft_strjoin(x.before_var, x.var_val);
	x.new_token = ft_strjoin(x.tmp_token, x.after_var);
	free(x.var);
	free(x.var_val);
	free(x.before_var);
	free(x.after_var);
	free(x.tmp_token);
	return (x.new_token);
}

/*  process_single_token()
	remove quotes, expand vars and return a new string

 1. Check the first character of the token.
	- If it's a single quote ('), do NOT expand.
		return a new substr by removing the outer quotes
 2. Variable expansion ($):
	- create a work copy of the token
	- execute repeatedly expand_once() until
		no difference between work and next
		(while always freeing the old work string)
 3. Quote removal:
	- After all expansions are done, if the token was quoted, create a new
		substring strip that omits the first and last characters
		free the old work string and return the strip string
 4. If there was no quote return the expanded work string.
*/
static char	*process_single_token(char *token, int last_exit_status)
{
	char	*work;
	char	*next;
	char	*strip;

	if (token[0] == '\'')
		return (ft_substr(token, 1, ft_strlen(token) - 2));
	work = ft_strdup(token);
	if (!work)
		return (NULL);
	next = expand_once(work, last_exit_status);
	while (next != work)
	{
		free(work);
		work = next;
		next = expand_once(work, last_exit_status);
	}
	if (work[0] == '"')
	{
		strip = ft_substr(work, 1, ft_strlen(work) - 2);
		free(work);
		return (strip);
	}
	return ((work));
}

char	**expand_and_clean(char **tokens, int last_exit_status)
{
	char	**new_tokens;
	int		i;
	int		count;

	count = 0;
	while (tokens[count])
		count++;
	new_tokens = malloc(sizeof(char *) * (count + 1));
	if (!new_tokens)
		return (NULL);
	i = 0;
	while (tokens[i])
	{
		new_tokens[i] = process_single_token(tokens[i], last_exit_status);
		i++;
	}
	new_tokens[i] = NULL;
	return (new_tokens);
}
