/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:53:14 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/09 17:30:26 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *get_heredoc_file()
{
	char *heredoc_file;
	char *tmp;
	int	i;

	i = 0;
	heredoc_file = ft_strdup(".heredoc");
	while (access(heredoc_file, F_OK) == 0)
	{
		tmp = heredoc_file;
		heredoc_file = ft_strjoin(".heredoc", ft_itoa(i++), 0, 1);
		free(tmp);
	}
	return (heredoc_file);
}

// int	do_heredoc(t_cmd *cmd, t_token *curr_token, bool is_last)
// {
// 	char	*line;
// 	int		heredoc_file;
// 	char	*limiter;
// 	bool	interupt;
// 	pid_t	pid;

// 	if (cmd->infile_fd > 0)
// 		close(cmd->infile_fd);
// 	heredoc_file = get_heredoc_file();
// 	interupt = 1;
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		cmd->infile_fd = open(heredoc_file, O_RDWR | O_APPEND | O_CREAT, 0664);
// 		limiter = ft_strdup(curr_token->next->value);
// 		while (1)
// 		{
// 			line = readline("ministress heredoc> ");
// 			if (!line || ft_compare(limiter, line))
// 			{
// 				if (line)
// 				{
// 					interupt = 0;
// 					free(line);
// 				}
// 				free(limiter);
// 				free(heredoc_file);
// 				close(cmd->infile_fd);
// 				if (interupt)
// 					exit(130);
// 				else
// 					exit(0);
// 			}
// 			ft_fprintf(cmd->infile_fd, "%s", line);
// 		}
// 	}
// 	else
// 	{
// 		wait(NULL);
// 	}
// 	if (is_last)
// 		cmd->infile_fd = open(heredoc_file, O_RDONLY);
// 	unlink(heredoc_file);
// 	free(heredoc_file);
// 	return (0);
// }


int do_heredoc(t_cmd *cmd, t_token *curr_token, bool is_last)
{
	char    *heredoc_file;
	int     heredoc_fd;
	pid_t   pid;
	int     status;
	struct termios term_old;

	// Obtenir un nom de fichier temporaire
	heredoc_file = get_heredoc_file();
	
	// Ouvrir le fichier temporaire
	heredoc_fd = open(heredoc_file, O_RDWR | O_TRUNC | O_CREAT, 0664);
	if (heredoc_fd < 0)
	{
		free(heredoc_file);
		return (1);
	}
	
	// Sauvegarder les attributs actuels du terminal
	tcgetattr(STDIN_FILENO, &term_old);
	
	// Fork pour gérer le heredoc
	pid = fork();
	
	if (pid == 0)  // Processus enfant
	{
		// Configurer les signaux
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
		
		char *line;
		char *limiter = ft_strdup(curr_token->next->value);
		
		// Boucle de lecture
		while (1)
		{
			line = readline("ministress heredoc> ");
			
			// Si EOF ou délimiteur
			if (!line || ft_compare(limiter, line))
			{
				if (line)
					free(line);
				free(limiter);
				close(heredoc_fd);
				exit(0);
			}
			
			// Écrire la ligne dans le fichier
			ft_fprintf(heredoc_fd, "%s\n", line);
			free(line);
		}
	}
	else if (pid < 0)  // Erreur de fork
	{
		close(heredoc_fd);
		unlink(heredoc_file);
		free(heredoc_file);
		return (1);
	}
	else  // Processus parent
	{
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
		signal(SIGINT, SIG_DFL);
		setup_signals();  // Restaurer vos gestionnaires personnalisés
		
		// Vérifier si interrompu par SIGINT
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			//write(STDOUT_FILENO, "\n", 1);
			tcsetattr(STDIN_FILENO, TCSANOW, &term_old);
			
			// Informer readline qu'il est sur une nouvelle ligne
			rl_on_new_line();
			
			// Nettoyer
			close(heredoc_fd);
			unlink(heredoc_file);
			free(heredoc_file);
			
			// Marquer pour ignorer
			cmd->skip_cmd = true;
			g_zebi.g_exit = 130;
			
			return (130);
		}
		
		// Configuration normale
		close(heredoc_fd);
		if (cmd->infile_fd > 0)
			close(cmd->infile_fd);
		
		cmd->here_doc = true;
		if (is_last)
			cmd->infile_fd = open(heredoc_file, O_RDONLY);
		
		unlink(heredoc_file);
		free(heredoc_file);
		return (0);
	}
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

int	redir_in(t_all *all, t_cmd *curr_cmd, t_token *curr_token)
{
	t_token	*file;
	char	*error;
	(void)all;
	if (curr_cmd->infile_fd > 0)
		close(curr_cmd->infile_fd);
	file = curr_token->next;
	error = ft_strjoin("ministress: ", file->value, 0, 0);
	if (access(file->value, F_OK | R_OK))
	{
		curr_cmd->skip_cmd = true;
		// skip_all_cmds(all->cmds);
		return (perror(error), free(error), 1);
	}
	curr_cmd->infile_fd = open(file->value, O_RDONLY);
	return (0);
}
