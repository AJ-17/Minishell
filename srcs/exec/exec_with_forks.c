/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_with_forks.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakubcz <ajakubcz@42Lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 09:14:56 by ajakubcz          #+#    #+#             */
/*   Updated: 2023/06/30 09:14:57 by ajakubcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	exec_cmd(t_parse *parse, int num_cmd, t_list **my_env, t_exec *data);
int	get_start_cmd(t_parse *parse, int num_cmd);
void	do_redirect_in(t_parse *parse, t_list **my_env, t_exec *data);
void	do_redirect_out(t_parse *parse, t_list **my_env, t_exec *data);
char **get_arg(t_parse *parse);
void fill_arg(t_parse *parse, char **arg);
void do_classique_in_pipe(int num_cmd, t_exec *data);
void do_classique_out_pipe(int num_cmd, t_exec *data);

void print_all(char **str)
{
	int i;

	i = 0;
	printf("PRINT ALL\n");
	while (str[i])
	{
		printf("arg : %s\n", str[i]);
		i++;
	}
	printf("END PRINT ALL\n");
}

void	wait_all(nbr_cmd)
{
	int i;

	i = 0;
	while (i < nbr_cmd)
	{
		waitpid(-1, NULL, 0);
		i++;
	}
}

void	exec_with_forks(t_parse *parse, t_list **my_env, t_exec *data)
{
	int	nbr_cmd;
	int num_cmd;
	int pid;

	nbr_cmd = how_many_cmds(parse) + 1;
	data->nbr_cmd = nbr_cmd;
	num_cmd = 0;
	while (num_cmd < nbr_cmd)
	{
		pid = fork();
		if (pid == 0)
		{
			exec_cmd(parse, num_cmd, my_env, data);
			exit(0);
		}
		num_cmd++;
	}
	wait_all(nbr_cmd);
}

void	exec_cmd(t_parse *parse, int num_cmd, t_list **my_env, t_exec *data)
{
	char **arg;
	char *path;
	int start_cmd;

	printf("new_cmd\n");
	start_cmd = get_start_cmd(parse, num_cmd);
	if (redirect_in(&parse[start_cmd]))
		do_redirect_in(&parse[start_cmd], my_env, data);
	else 
		do_classique_in_pipe(num_cmd, data);
	if (redirect_out(&parse[start_cmd]))
		do_redirect_out(&parse[start_cmd], my_env, data);
	else
		do_classique_out_pipe(num_cmd, data);
	arg = get_arg(&parse[start_cmd]);
	//print_all(arg);
	// if (!arg)
	// 	exit
	// if (is_builtin(arg[0]))
	// 	do_builtin();
	path = get_path(arg[0], my_env);
	printf("path : %s\n", path);
	// if (!path)
	// 	exit
	execve(path, arg, data->env);
	// exit car execve a crash si ici
}

int	get_start_cmd(t_parse *parse, int num_cmd)
{
	int start;
	int n_cmd;

	start = 0;
	n_cmd = 0;
	while (n_cmd < num_cmd && parse[start].str)
	{
		if (parse[start].type == PIPEE)
			n_cmd++;
		start++;
	}
	return (start);
}

int	redirect_in(t_parse *parse)
{
	int i;

	i = 0;
	while (parse[i].str && parse[i].type != PIPEE)
	{
		if (parse[i].type == HEREDOC || parse[i].type == REDIRECT_IN)
			return (1);
		i++;
	}
	return (0);
}

void	do_redirect_in(t_parse *parse, t_list **my_env, t_exec *data)
{
	char *infile;
	int fd;

	infile = NULL;
	if (check_all_redirect_in(parse, my_env, &infile) == 0) 
	{
		printf("NOT GOOD REDIRECT IN\n");
		//exit(1); //faudra free et exit proprement avec le bon message d'erreur
	}
	else
	{
		fd = open(infile, O_RDONLY);
		printf("GOOD REDIRECT IN : %s\n", infile);
		dup2(fd, STDIN_FILENO);
	}
}

int	check_all_redirect_in(t_parse *parse, t_list **my_env, char **infile)
{
	int i;

	i = 0;
	while (parse[i].str && parse[i].type != PIPEE)
	{
		if (parse[i].type == REDIRECT_IN && !check_redirect(parse[i + 1], my_env, infile, 1))
			return (0);
		i++;
	}
	return (1);
}

void do_classique_in_pipe(int num_cmd, t_exec *data)
{
	if (num_cmd  > 0)
	{
		dup2(data->prec_fd, STDIN_FILENO);
	}
	close(data->prec_fd);
}

void do_classique_out_pipe(int num_cmd, t_exec *data)
{
	int pipes[2];

	pipe(pipes);
	if (num_cmd < data->nbr_cmd - 1)
	{
		dup2(pipes[1], STDOUT_FILENO);
		data->prec_fd = pipes[0];
	}
	close(pipes[1]);
}

int	redirect_out(t_parse *parse)
{
	int i;

	i = 0;
	while (parse[i].str && parse[i].type != PIPEE)
	{
		if (parse[i].type == REDIRECT_OUT || parse[i].type == APPEND)
		{
			printf("REDIRECT OUT %s\n", parse[i].str);
			return (1);
		}
		i++;
	}
	return (0);
}

void	do_redirect_out(t_parse *parse, t_list **my_env, t_exec *data)
{
	char *outfile;
	int	fd;

	outfile = NULL;
	if (check_all_redirect_out(parse, my_env, &outfile) == 0) 
	{
		printf("NOT GOOD REDIRECT OUT\n");
		//exit(1); //faudra free et exit proprement avec le bon message d'erreur
	}
	else
	{
		printf("GOOD REDIRECT OUT : %s\n", outfile);
		fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		dup2(fd, STDOUT_FILENO);
	}
}

int	check_all_redirect_out(t_parse *parse, t_list **my_env, char **outfile)
{
	int i;

	i = 0;
	while (parse[i].str && parse[i].type != PIPEE)
	{
		if ((parse[i].type == REDIRECT_OUT || parse[i].type == APPEND) && !check_redirect(parse[i + 1], my_env, outfile, 0))
			return (0);
		i++;
	}
	return (1);
}

char **get_arg(t_parse *parse)
{
	char	**arg;
	int		size;

	size = get_nbr_arg(parse);
	printf("size of arg %d\n", size);
	arg = malloc(sizeof(char *) * (size + 1));
	fill_arg(parse, arg);
	return (arg);
}

int get_nbr_arg(t_parse *parse)
{
	int i;
	int nbr;

	nbr = 0;
	i = 0;
	while (parse[i].str && parse[i].type != PIPEE)
	{
		if (parse[i].type == COMMAND || parse[i].type == CMD_ARG)
			nbr++;
		i++;
	}
	return (nbr);
}

void fill_arg(t_parse *parse, char **arg)
{
	int i;
	int num_arg;

	i = 0;
	num_arg = 0;
	while (parse[i].str && parse[i].type != PIPEE)
	{
		if (parse[i].type == COMMAND)
		{
			arg[0] = parse[i].str;
			num_arg++;
		}
		i++;
	}
	i = 0;
	while (parse[i].str && parse[i].type != PIPEE)
	{
		if (parse[i].type == CMD_ARG)
		{
			arg[num_arg] = parse[i].str;
			num_arg++;
		}
		i++;
	}
	arg[num_arg] = NULL;
}
