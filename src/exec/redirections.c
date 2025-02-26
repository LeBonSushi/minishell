/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 02:40:39 by macorso           #+#    #+#             */
/*   Updated: 2025/02/26 03:51:56 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


// compteur de pipe ?

void	close_all(int fd_in, int fd_out, int pipe_fd[2])
{
	if (fd_in != -1)
	{	
		ft_printf("close fd_in\n");
		close(fd_in);	
	}
	if (fd_out != -1)
	{
		ft_printf("close fd_out\n");
		close(fd_out);
	}
	if (pipe_fd[0] != -1)
	{
		ft_printf("close pipe_fd[0]\n");
		close(pipe_fd[0]);
	}
	if (pipe_fd[1] != -1)
	{
		ft_printf("close pipe_fd[1]\n");
		close(pipe_fd[1]);
	}
}


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

void	error(char *message, char **args, char *cmd, int exit_code)
{
	if (exit_code != 0)
	{
		if (message)
		{
			ft_putstr_fd("Error: ", 2);
			if (ft_strcmp(message, "Command not found: ") == 0 && cmd)
			{
				ft_putstr_fd("Command not found: ", 2);
				ft_putstr_fd(cmd, 2);
				ft_putstr_fd("\n", 2);
			}
			else if (ft_strcmp(message, "Redirecting input\n") == 0 || ft_strcmp(message, "Redirecting output\n") == 0)
				free(cmd);
			else
				ft_putstr_fd(message, 2);
		}
		if (args)
			ft_free_tab(args);
	}
	if (exit_code != 0)
		exit(exit_code);
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

char **env_to_char(t_env *env)
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
		env_tab[i] = malloc(sizeof(char) * (ft_strlen(env->key) + ft_strlen(env->value) + 2));
		env_tab[i] = ft_strjoin(curr->key, "=", 0, 0);
		env_tab[i] = ft_strjoin(env_tab[i], curr->value, 1, 0);
		i++;
		curr = curr->next;
	}
	env_tab[i] = NULL;
	return (env_tab);
}
void manage_redirections(t_all *all, t_token **token)
{
    if (!token || !*token || !(*token)->next)
        return;
    
    /* Store redirection information before deleting tokens */
    if ((*token)->token_type == REDIR_IN)
    {
        /* Add the filename (next token's value) to redir_in_names */
        ft_stradd_back(&all->redir_in_names, ft_strnew((*token)->next->value, REDIR_IN));
    }
    else if ((*token)->token_type == REDIR_OUT || (*token)->token_type == REDIR_APPEND || (*token)->token_type == PIPE)
    {
        /* Add the filename (next token's value) to redir_out_names */
        ft_stradd_back(&all->redir_out_names, ft_strnew((*token)->next->value, (*token)->token_type));
    }
    
    /* Save the node after the ones to be deleted (next->next) */
    t_token *next_next = (*token)->next->next;
    
    /* Delete the next token (the filename) */
    delete_token(token, (*token)->next);
    
    /* Delete the current token (the redirection operator) */
    delete_token(token, *token);
    
    /* Update token to point to what was previously next->next */
    *token = next_next;
}

void ft_redirection(t_all *all, t_token *token)
{
    if (!token)
        return;
    
    t_token *current = token;
    while (current)
    {
        if ((is_redir(current) || current->token_type == PIPE) && current->next)
        {
            manage_redirections(all, &current);
            // Don't advance current here - manage_redirections already updated it
        }
        else
            current = current->next;
    }
    
    print_list(all->redir_in_names);
    print_list(all->redir_out_names);
}