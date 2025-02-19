/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_and_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprojean <cprojean@42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 18:24:28 by ajakubcz          #+#    #+#             */
/*   Updated: 2023/07/27 14:16:28 by cprojean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	conver_parse_to_list(t_parse *parse, t_list **list);
void	print_list(t_list *list);
int		convert_entry_to_parse(t_entry *entry, t_parse **parse);

void	parse_and_exec(char *str, t_list **my_env, t_exec *data)
{
	t_entry	*entry;
	t_entry	*new_entry;
	t_parse	*parse;

	new_entry = NULL;
	parse = NULL;
	if (!check_syntax_quote(str))
		return (free(str));
	entry = malloc(sizeof(t_entry) * (ft_strlen(str) + 1));
	if (!entry)
	{
		ft_error(ERROR_42, NULL, 1);
		return (free(str));
	}
	if (init_parse(entry, parse, str) == -1)
		return (free(str));
	free(str);
	if (expand(entry, &new_entry, my_env) == -1)
		return (free(entry));
	free(entry);
	if (!convert_entry_to_parse(new_entry, &parse))
		return ;
	free(new_entry);
	if (execution(parse, my_env, data) == -1)
		free_all_parse(parse);
}

int	init_parse(t_entry *entry, t_parse *parse, char *str)
{
	init_entry(entry, str);
	if (!convert_entry_to_parse(entry, &parse))
		return (-1);
	if (check_parse(parse) == -1)
	{
		g_errno = 2;
		free(entry);
		free_all_parse(parse);
		return (-1);
	}
	free_all_parse(parse);
	return (0);
}

int	convert_entry_to_parse(t_entry *entry, t_parse **parse)
{
	*parse = malloc(sizeof(t_parse) * (size_of_parse(entry) + 1));
	if (!*parse)
	{
		ft_error(ERROR_42, NULL, 12);
		free(entry);
		return (0);
	}
	if (colapse_all(entry, *parse) == 0)
	{
		free(entry);
		free_all_parse(*parse);
		return (0);
	}
	return (1);
}

void	conver_parse_to_list(t_parse *parse, t_list **list)
{
	int	i;

	i = 0;
	while (parse[i].str)
	{
		ft_lstadd_back(list, ft_lstnew(parse[i].str, parse[i].type));
		i++;
	}
}

void	print_list(t_list *list)
{
	ft_printf("LIST :\n");
	while (list)
	{
		ft_printf("%s : %d\n", list->content, list->type);
		list = list->next;
	}
}
