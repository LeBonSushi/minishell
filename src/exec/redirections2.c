/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:53:14 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/18 19:14:38 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	do_heredoc(t_all *all, t_cmd *cmd, t_token *curr_token, bool is_last)
// {
// 	char	*line;
// 	char	*limiter;
// 	pid_t	pid;
// 	int		status;
	
// 	status = 0;
// 	pid = fork();
// 	signal(SIGINT, SIG_IGN);
// 	signal(SIGQUIT, SIG_IGN);
// 	if (pid == 0)
// 	{
// 		signal(SIGINT, SIG_DFL);
// 		signal(SIGQUIT, SIG_IGN);
// 		if (cmd->infile_fd > 0)
// 			close(cmd->infile_fd);
// 		cmd->infile_fd = open("/tmp/.heredoc", O_RDWR | O_TRUNC | O_CREAT, 0664);
// 		limiter = ft_strdup(curr_token->next->value);
// 		while (1)
// 		{
// 			line = readline("ministress heredoc> ");
// 			if (!line || ft_compare(limiter, line))
// 			{
// 				if (line)
// 					free(line);
// 				free(limiter);
// 				break ;
// 			}
// 			ft_fprintf(cmd->infile_fd, "%s\n", line);
// 			free(line);
// 		}
// 		close(cmd->infile_fd);
// 	}
// 	waitpid(pid, &status, 0);

// 	if (WIFSIGNALED(status))
// 	{
// 		if (WTERMSIG(status) == SIGINT)
// 		{
// 			write(1, "\n", 1);
// 			rl_on_new_line();
// 			rl_replace_line("", 0);
// 			rl_redisplay();
// 			skip_all_cmds(all->cmds);
// 			return (130);
// 		}
// 	}
// 	if (is_last)
// 		cmd->infile_fd = open("/tmp/.heredoc", O_RDONLY);
// 	unlink("/tmp/.heredoc");
// 	return (status);
// }

#include <termios.h>  // Ajoutez cet en-tête pour les fonctions de terminal

int	do_heredoc(t_all *all, t_cmd *cmd, t_token *curr_token, bool is_last)
{
	char			*line;
	pid_t			pid;
	int				status;
	struct termios	orig_termios;
	struct termios	new_termios;
	
	tcgetattr(STDIN_FILENO, &orig_termios);
	
	status = 0;
	pid = fork();
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
		if (cmd->infile_fd > 0)
			close(cmd->infile_fd);
		cmd->infile_fd = open("/tmp/.heredoc", O_RDWR | O_TRUNC | O_CREAT, 0664);
		if (cmd->infile_fd < 0)
			exit(1);
		all->limiter = ft_strdup(curr_token->next->value);
		if (!all->limiter)
			exit(1);
		tcgetattr(STDIN_FILENO, &new_termios);
		new_termios.c_lflag &= ~(ICANON);
		tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
		
		while (1)
		{
			line = readline("heredoc> ");
			if (!line || ft_compare(all->limiter, line))
			{
				if (line)
					free(line);
				free(all->limiter);
				break ;
			}
			ft_fprintf(cmd->infile_fd, "%s\n", line);
			free(line);
		}
		close(cmd->infile_fd);
		tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
		exit(0);
	}
	waitpid(pid, &status, 0);
	
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	
	tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);

	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			write(1, "\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			// rl_redisplay();
			skip_all_cmds(all->cmds);
			return (130);
		}
	}
	if (is_last)
		cmd->infile_fd = open("/tmp/.heredoc", O_RDONLY);
	unlink("/tmp/.heredoc");
	return (status);
}

bool	is_last(t_token *token)
{
	t_token	*curr;

	curr = token;
	while (curr->next)
	{
		if (curr->next->token_type == token->token_type)
			return (false);
		curr = curr->next;
	}
	return (true);
}

int	redir_in(t_cmd *curr_cmd, t_token *curr_token)
{
	t_token	*file;
	char	*error;

	if (curr_cmd->infile_fd > 0)
		close(curr_cmd->infile_fd);
	file = curr_token->next;
	error = ft_strjoin("ministress: ", file->value, 0, 0);
	if (access(file->value, F_OK | R_OK))
	{
		curr_cmd->skip_cmd = true;
		return (perror(error), free(error), 1);
	}
	free(error);
	curr_cmd->infile_fd = open(file->value, O_RDONLY);
	return (0);
}
