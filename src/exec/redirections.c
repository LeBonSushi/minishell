/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 02:40:39 by macorso           #+#    #+#             */
/*   Updated: 2025/03/06 02:53:41 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


// compteur de pipe ?

// void	close_all(int fd_in, int fd_out, int pipe_fd[2])
// {
// 	if (fd_in != -1)
// 	{	
// 		printf("close fd_in\n");
// 		close(fd_in);	
// 	}
// 	if (fd_out != -1)
// 	{
// 		printf("close fd_out\n");
// 		close(fd_out);
// 	}
// 	if (pipe_fd[0] != -1)
// 	{
// 		printf("close pipe_fd[0]\n");
// 		close(pipe_fd[0]);
// 	}
// 	if (pipe_fd[1] != -1)
// 	{
// 		printf("close pipe_fd[1]\n");
// 		close(pipe_fd[1]);
// 	}
// }


void	ft_free_tab(char **tab)
{
	int i;
	
	i = 0;
	if (!tab)
		return;
	while (tab[i])
	{
		free (tab[i]);
		tab[i] = NULL;
		i++;
	}
	free (tab);
	tab = NULL;
}

/*char	*join_path_cmd(char *path, char *cmd)
{
	char	*path_with_slash;
	char	*full_path;
	
	path_with_slash = ft_strjoin(path, "/", 0, 0);
	if (!path_with_slash)
		return (NULL);
	full_path = ft_strjoin(path_with_slash, cmd, 0, 0);
	free(path_with_slash);
	if (!full_path)
		return (NULL);
	return (full_path);
}

char *get_command_path(char *cmd, char **envp)
{
	char *path_var;
	char **paths;
	char *full_path;
	int i;
	
	i = 0;
	path_var = NULL;

	if (!ft_strncmp(cmd, "/", 1) || !ft_strncmp(cmd, "./", 2) || !ft_strncmp(cmd, "../", 3))
	{
		return (ft_strdup(cmd));
	}
	while (envp[i])
	{
		if (ft_strncmp ("PATH=", envp[i], 5) == 0)
		{
			path_var = envp[i] + 5;
			break;
		}
		i++;
	}
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);
	i= 0;
	while (paths[i])
	{
		full_path = join_path_cmd(paths[i], cmd);
		if (!full_path)
		{
			ft_free_tab(paths);
			return (NULL);
		}
		if (access(full_path, X_OK) == 0)
		{
			ft_free_tab(paths);
			return (full_path);
		}
		free (full_path);
		i++;
	}
	ft_free_tab(paths);
	return (NULL);
}

void	ft_redirection_in(t_token *token, t_env *env, char **envp)
{
	(void)env;                       //on a besoin de env pour les < ? des exports etc?
	t_token *current;
	char **args;
	int	args_count = 0;
	int i = 0;
	
	if (!token || !token->prev || !token->prev->prev)
	{
		(printf("Erreur: Liste trop courte pour gérer la redirection\n"));
		return;
	}
	current = token->prev->prev;
	if (current)
		args_count = 1;
	while (current && current->prev && current->prev->token_type != PIPE)
	{
		args_count++;
		current = current->prev;
	}
	if (args_count == 0)
	{
		printf("Erreur: Aucun argument pour la redirection\n");
		return;
	}
	args = (char **)malloc(sizeof(char *) * (args_count + 1));
	if (!args)
	{
    	printf("Erreur: Échec de l'allocation de args\n");
    	return;
	}
	while (current->prev && current->prev->token_type != PIPE)
		current = current->prev;
	while (current && current->token_type != PIPE && current->token_type != REDIR_IN)
	{
		if (i >= args_count)
		{
			printf("Erreur: i (%d) dépasse args_count (%d)\n", i, args_count);
			break;
		}
		args[i] = ft_strdup(current->value);
		if (!args[i]) // Vérifier malloc
		{
			printf("Erreur: strdup a échoué pour args[%d]\n", i);
			break;
		}
		current = current->next;
		i++;
	}
	args[i] = NULL;
	if (!args[0])
	{
    	printf("Erreur: Commande introuvable\n");
		free(args);
    	return;
	}
	char *cmd_path = get_command_path(args[0], envp);
	
	int fd;
	fd = open(token->value, O_RDONLY);
	if (fd == -1)
		printf("ouverture impossible\n");
	pid_t pid = fork();
	if (pid == -1)
	{
		perror("Erreur lors de la création du processus");
		close(fd);
		return;
	}
	else if (pid == 0)   // Processus fils
	{
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			// Si la redirection échoue, affiche un message d'erreur
			perror("Erreur de redirection de l'entrée");
			close(fd);
			exit(EXIT_FAILURE);  // Quitte le processus fils
		}	
		execve(cmd_path, &args[0], envp);
		printf("GG\n");
		printf("Redirection d'entrée effectuée dans le processus fils\n");
		close(fd);
		exit(EXIT_SUCCESS);
	}
	else
		wait(NULL);
	
}

void	ft_redirection(t_token *token, t_env *env, char **envp)
{
	t_token *cur = token;
	
	while (cur)
	{
		if (cur->token_type == REDIR_IN)
		{
			if (!cur->prev || !cur->next)
			{
				printf("Erreur: Redirection mal formée\n");
            	return;
			}
			if (cur->prev->token_type == WORD && cur->next->token_type == WORD) // TYPE_REDIR_IN  <      //CUR == SYMBOLE
			{
				ft_redirection_in(cur->next, env, envp);
			}
			else if (cur->token_type == REDIR_OUT)
			{
			
			}
			else if (cur->token_type == REDIR_APPEND)
			{
			
			}
			else if (cur->token_type == REDIR_HEREDOC)
			{
			
			}
		//printf("le token numero %d est de type: %d et il vaut %s\n", i, cur->token_type, cur->value);
		}
		cur = cur->next;
	}
}*/

int	env_get_size(t_env *env)
{
	t_env	*curr;
	int		i;

	curr = env;
	i = 0;
	while (curr)
	{
		i++;
		curr = curr->next;
	}
	return (i);
}

int	cmd_get_size(t_cmd *cmd)
{
	t_cmd	*curr;
	int		i;

	curr = cmd;
	i = 0;
	while (curr)
	{
		i++;
		curr = curr->next;
	}
	return (i);
}

char **env_to_tab(t_env *env)
{
	char	**env_tab;
	t_env	*curr;
	int		i;
	
	curr = env;
	env_tab = malloc(sizeof(char *) * (env_get_size(env) + 1));
	if (!env_tab)
		return (NULL);
	i = 0;
	while (curr)
	{
		env_tab[i] = ft_strjoin(curr->key, "=", 0, 0);
		env_tab[i] = ft_strjoin(env_tab[i], curr->value, 1, 0);
		i++;
		curr = curr->next;
	}
	env_tab[i] = NULL;
	return (env_tab);
}

char **cmd_to_tab(t_cmd *cmd)
{
	char	**cmd_tab;
	t_cmd	*curr;
	int		i;

	curr = cmd;
	cmd_tab = malloc(sizeof (char *) * (cmd_get_size(cmd) + 1));
	i = 0;
	if (!cmd_tab)
		return (NULL);
	while (curr)
	{
		cmd_tab[i] = ft_strdup(curr->tokens->value);
		i++;
		curr = curr->next;
	}
	cmd_tab[i] = NULL;
	return (cmd_tab);
}

// static void	remove_redir_tokens(t_token **head, t_token *redir, t_token *file)
// {
// 	t_token	*curr;
// 	t_token	*prev;

// 	curr = *head;
// 	prev = NULL;
// 	while (curr)
// 	{
// 		if (curr == redir)
// 		{
// 			if (prev)
// 				prev->next = file->next;
// 			else
// 				*head = file->next;
// 			free(redir->value);
// 			free(redir);
// 			free(file->value);
// 			free(file);
// 			return ;
// 		}
// 		prev = curr;
// 		curr = curr->next;
// 	}
// }

// static void	manage_single_redir(t_cmd *cmd, t_token *curr)
// {
// 	if (curr->token_type == REDIR_IN)
// 	{
// 		if (cmd->infile_fd > 0)
// 			close(cmd->infile_fd);
// 		cmd->infile_fd = open(curr->next->value, O_RDONLY);
// 		remove_redir_tokens(&cmd->tokens, curr, curr->next);
// 	}
// 	else if (curr->token_type == REDIR_OUT)
// 	{
// 		if (cmd->outfile_fd > 1)
// 			close(cmd->outfile_fd);
// 		cmd->outfile_fd = open(curr->next->value,
// 				O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 		remove_redir_tokens(&cmd->tokens, curr, curr->next);
// 	}
// 	else if (curr->token_type == REDIR_APPEND)
// 	{
// 		if (cmd->outfile_fd > 1)
// 			close(cmd->outfile_fd);
// 		cmd->outfile_fd = open(curr->next->value,
// 				O_WRONLY | O_CREAT | O_APPEND, 0644);
// 		remove_redir_tokens(&cmd->tokens, curr, curr->next);
// 	}
// }

// static void	manage_cmd_redir(t_cmd *cmd)
// {
// 	t_token	*curr;
// 	t_token	*next;

// 	curr = cmd->tokens;
// 	while (curr && curr->next)
// 	{
// 		next = curr->next;
// 		if (curr->token_type == REDIR_IN || curr->token_type == REDIR_OUT
// 			|| curr->token_type == REDIR_APPEND)
// 		{
// 			manage_single_redir(cmd, curr);
// 			curr = cmd->tokens;
// 			continue ;
// 		}
// 		curr = curr->next;
// 	}
// 	if (cmd->infile_fd == -1 || cmd->outfile_fd == -1)
// 		cmd->skip_cmd = true;
// }


bool ft_compare(char *limiter, char *line)
{
    size_t limiter_len = ft_strlen(limiter);
    size_t line_len = ft_strlen(line);
    
    if (line_len < limiter_len + 1)  // +1 for newline
        return (0);
    if (ft_strncmp(line, limiter, limiter_len))
        return (0);
    if (line[limiter_len] != '\n')
        return (0);
    return (1);
}

int	do_heredoc(t_cmd *cmd, t_token *curr_token, bool is_last)
{
	char	*line;
	int		heredoc_fd;
	char	*limiter;

	if (cmd->infile_fd > 0)
		close(cmd->infile_fd);
	heredoc_fd = open(HEREDOC_TMP, O_RDWR | O_TRUNC | O_CREAT, 0664);
	limiter = ft_strdup(curr_token->next->value);
	while (1)
	{
		ft_fprintf(0, "ministress heredoc> ");
		line = get_next_line(0);
		if (!line || ft_compare(limiter, line))
		{
			printf("ca rentre (titre)\n");
			if (line)
				free(line);
			free(limiter);
			break ;
		}
		ft_fprintf(heredoc_fd, "%s", line);
		free(line);
	}
	close(heredoc_fd);
	if (is_last)
		cmd->infile_fd = open(HEREDOC_TMP, O_RDONLY);
	else
		unlink(HEREDOC_TMP);
	return (0);
}

bool	is_last(t_token *token)
{
	t_token	*curr;

	curr = token->next;
	while (curr)
	{
		if (curr->token_type == token->token_type)
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
	if (access(file->value, F_OK) == -1)
	{
		curr_cmd->skip_cmd = true;
		return (perror(error), 1);
	}
	curr_cmd->infile_fd = open(file->value, O_RDONLY);
	return (0);
}

int	redir_append(t_cmd *curr_cmd, t_token *curr_token)
{
	t_token	*file;
	char	*error;

	if (curr_cmd->outfile_fd > 0)
		close(curr_cmd->outfile_fd);
	file = curr_token->next;
	error = ft_strjoin("ministress: ", file->value, 0, 0);
	if (access(file->value, F_OK) == -1 && errno == EACCES)
	{
		perror(error);
		return (1);
	}
	curr_cmd->outfile_fd = open(file->value, O_RDWR | O_APPEND | O_CREAT, 0644);
	return (0);
}

int	redir_out(t_cmd *curr_cmd, t_token *curr_token)
{
	t_token	*file;
	char	*error;

	if (curr_cmd->outfile_fd > 0)
		close(curr_cmd->outfile_fd);
	file = curr_token->next;
	error = ft_strjoin("ministress: ", file->value, 0, 0);
	if (access(file->value, F_OK) == -1 && errno == EACCES)
	{
		perror(error);
		return (1);
	}
	curr_cmd->outfile_fd = open(file->value, O_RDWR | O_TRUNC | O_CREAT, 0644);
	return (0);
}

void delete_redirection_tokens(t_token **tokens_ptr)
{
    t_token *curr;
    t_token *next;
    
    if (!tokens_ptr || !*tokens_ptr)
        return;
        
    curr = *tokens_ptr;
    while (curr)
    {
        if (is_redir(curr) && curr->next)
        {
            // Save the token that comes after the redirection pair
            next = curr->next->next;
            
            // Delete the redirection token and its argument
            delete_token(tokens_ptr, curr->next);
            delete_token(tokens_ptr, curr);
            
            // Move to the token after the pair
            curr = next;
        }
        else
        {
            // Not a redirection - just move to next token
            curr = curr->next;
        }
    }
}

int	manage_cmd_redir(t_cmd *curr_cmd)
{
	t_token	*curr_token;
	t_token	*next;
	int		status;

	curr_token = curr_cmd->tokens;
	status = 0;
	while (curr_token && curr_token->next)
	{
		next = curr_token->next->next;
		if (is_redir(curr_token))
		{
			if (curr_token->token_type == REDIR_HEREDOC)
			{
				status = do_heredoc(curr_cmd, curr_token, is_last(curr_token));
				// curr_cmd->here_doc = true;
			}
			else if (curr_token->token_type == REDIR_IN && curr_cmd->skip_cmd == false)
				status = redir_in(curr_cmd, curr_token);
			else if (curr_token->token_type == REDIR_APPEND && curr_cmd->skip_cmd == false)
				status = redir_append(curr_cmd, curr_token);
			else if (curr_token->token_type == REDIR_OUT && curr_cmd->skip_cmd == false)
				status = redir_out(curr_cmd, curr_token);
			curr_token = next;
		}
		else
			curr_token = curr_token->next;
	}
	return (status);
}

int	manage_redirections(t_cmd *cmds)
{
	t_cmd	*curr;
	int		status;

	curr = cmds;
	status = 0;
	while (curr)
	{
		status = manage_cmd_redir(curr);
		curr = curr->next;
	}
	return (status);
}


// void ft_redirection(t_all *all, t_token *token)
// {
//     if (!token)
//         return;
    
//     t_token *current = token;
//     while (current)
//     {
//         if ((is_redir(current) || current->token_type == PIPE) && current->next)
//         {
//             manage_redirections(all, &current);
//         }
//         else
//             current = current->next;
//     }
//     //print_list(all->redir_in_names);
//     //print_list(all->redir_out_names);
// }