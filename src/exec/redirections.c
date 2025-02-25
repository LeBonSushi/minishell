/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 02:40:39 by macorso           #+#    #+#             */
/*   Updated: 2025/02/24 22:06:43 by jd-halle         ###   ########.fr       */
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
/*char **env_to_array(t_env *env)
{
	int size = 0;
	t_env *tmp = env;
	while (tmp)
	{
		size++;
		tmp = tmp->next;
	}

	char **env_array = malloc(sizeof(char *) * (size + 1));
	if (!env_array)
		return NULL;

	tmp = env;
	int i = 0;
	while (tmp)
	{
		env_array[i] = 
		tmp = tmp->next;
		i++;
	}
	env_array[i] = NULL;  // Terminer le tableau par un NULL, comme un environnement classique
	return env_array;
}*/

char	*join_path_cmd(char *path, char *cmd)
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
	while (current && current->prev && current->prev->token_type != PIPE)
	{
		args_count++;
		current = current->prev;
	}
	args = (char **)malloc(sizeof(char *) * (args_count + 1));
	while (current && current->token_type != PIPE && current->token_type != REDIR_IN)
	{
		args[i] = ft_strdup(current->value);
		printf("args[%d] = %s\n", i, args[i]);
		current = current->next;
		i++;
	}
	args[i] = 0;
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
		printf("Redirection d'entrée effectuée dans le processus fils\n");
		close(fd);
		exit(EXIT_SUCCESS);
	}
	else
		wait(NULL);
	
}

void	ft_redirection (t_token *token, t_env *env, char **envp)
{
	int i = 1;
	t_token *cur = token;
	
	while (cur)
	{
		if (cur->token_type == REDIR_IN && cur->prev->token_type == WORD && cur->next && cur->next->token_type == WORD) // TYPE_REDIR_IN  <      //CUR == SYMBOLE
		{
			//printf("current que je donne a ft_direction: %s\n", cur->next->value);
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
		cur = cur->next;
		i++;
	}
	/*if (token->token_type == REDIR_IN) // TYPE_REDIR_IN
	{
		printf("Redirection d'entrée détectée\n");
		printf("ici %d\n", token->token_type);
	}*/
}