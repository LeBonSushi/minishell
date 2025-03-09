/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 05:18:28 by macorso           #+#    #+#             */
/*   Updated: 2025/03/09 03:32:44 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	tokens_size(t_token *tokens)
{
	t_token	*curr;
	int		i;

	curr = tokens;
	i = 0;
	while (curr)
	{
		i++;
		curr = curr->next;
	}
	return (i);
}

char	**tokens_to_tab(t_token *tokens)
{
	char	**tab;
	t_token	*curr;
	int		i;

	if (!tokens)
		return (NULL);
	curr = tokens;
	tab = malloc(sizeof(char *) * (tokens_size(tokens) + 1));
	if (!tab)
		return (NULL);
	i = 0;
	while (curr)
	{
		tab[i] = ft_strdup(curr->value);
		i++;
		curr = curr->next;
	}
	tab[i] = NULL;
	return (tab);
}

char	**get_paths(t_all *all, char **env)
{
	int		i;
	char	**paths;

	i = 0;
	paths = NULL;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			paths = ft_split(env[i] + 5, ':');
			break ;
		}
		i++;
	}
	if (!paths)
	{
		ft_fprintf(2, "$PATH not found in env.\n");
		all->status = 127;
		return (NULL);
	}
	return (paths);
}

char	*find_exec_path(char *cmd, char **paths)
{
	int		i;
	char	*exec_path;
	char	*tmp;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/", 0, 0);
		exec_path = ft_strjoin(tmp, cmd, 1, 0);
		if (!access(exec_path, X_OK))
			return (exec_path);
		free(exec_path);
		exec_path = NULL;
		i++;
	}
	return (ft_strdup(cmd));
}

bool	is_dir(t_all *all, char *cmd)
{
	int	fd;
	char	*error;

	fd = open(cmd, O_RDONLY);
	error = ft_strjoin("ministress: ", cmd, 0, 0);
	if (fd < 0 && errno == EISDIR)
	{
		perror(error);
		all->status = 1;
	}
	free(error);
	close(fd);
	return (false);
}

bool	is_file(char *cmd)
{
	int	fd;

	fd = open(cmd, O_RDONLY);
	if (fd >= 0)
	{
		close(fd);
		return (true);
	}
	return (false);
}

void	exec_error(t_all *all, char *cmd)
{
	// if (errno == EACCES && all->status == 0 && !is_file(cmd))
	// {
	// 	ft_fprintf(2, "%s: Permission denied\n", cmd);
	// 	all->status = 126;
	// 	return ;
	// }
	// else if ((!ft_strncmp(cmd, "./", 2) || !ft_strncmp(cmd, "/", 1)) && all->status == 0)
	// 	perror(cmd);
	// else
	// 	ft_fprintf(2, "%s: command not found\n", cmd);
	// all->status = 127;
    // if (errno == EACCES && all->status == 0 && !is_file(cmd) && ft_strcmp(cmd, ""))
    // {
    //     ft_fprintf(2, "%s: Permission denied\n", cmd);
    //     all->status = 126;
	// 	// exit(126);
	// 	return ;
    // }
    // else if ((!ft_strncmp(cmd, "./", 2) || !ft_strncmp(cmd, "/", 1)) && all->status == 0)
	// {
    //     perror(cmd);
	// 	if (errno != ENOENT)
	// 	{
	// 		all->status = 126;
	// 		// exit(126);
	// 		return ;
	// 	}
	// }
	// else
	// 	ft_fprintf(2, "%s: command not found\n", cmd);
	// all->status = 127;
	// // exit(127);
    if (errno == EISDIR)
    {
        ft_fprintf(2, "%s: Is a directory\n", cmd);
        all->status = 126;
        return;
    }
    else if (errno == EACCES && all->status == 0)
    {
        ft_fprintf(2, "%s: Permission denied\n", cmd);
        all->status = 126;
        return;
    }
    else if ((!ft_strncmp(cmd, "./", 2) || !ft_strncmp(cmd, "/", 1)) && all->status == 0)
        perror(cmd);
    else
        ft_fprintf(2, "%s: command not found\n", cmd);
    all->status = 127;
}

char	*get_cmd_path(t_all *all, char *cmd, char **env)
{
	char	**paths;
	char	*exe_path;

	if (!cmd)
		return (NULL);
	if (!access(cmd, X_OK) && !is_dir(all, cmd))
		return (ft_strdup(cmd));
	if (!*env)
	{
		perror(cmd);
		return (NULL);
	}
	paths = get_paths(all, env);
	exe_path = find_exec_path(cmd, paths);
	free_tab(paths);
	return (exe_path);
}

void	pipe_error(t_all *all, int *fd)
{
	close(fd[0]);
	close(fd[1]);
	perror("pipe");
	all->status = 1;
}

void	fork_error(t_all *all, int *fd)
{
	close(fd[0]);
	close(fd[1]);
	perror("fork");
	all->status = 1;
}

int	ft_exit2(t_all *all, int exit_code)
{
	all->status = exit_code;
	exit(exit_code);
}

void	child_process(t_all *all, t_cmd *cmd, int *fd, char **env)
{

	close(fd[0]);
	if  (cmd->infile_fd > 0)
		dup2(cmd->infile_fd, STDIN_FILENO);
	if  (cmd->outfile_fd > 0)
		dup2(cmd->outfile_fd, STDOUT_FILENO);
	else if (cmd->next)
		dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	close(cmd->infile_fd);
	close(cmd->outfile_fd);
	// if (is_builtin())
	// 	execute_builtins()
	// else if (execve(cmd->path, cmd->args, env) == -1)
	// exec_error(all, cmd->args[0]);
	if (execve(cmd->path, cmd->args, env) == -1)
		exec_error(all, cmd->args[0]);
	exit(all->status);
}

void	parent_process(t_cmd *curr, int *fd)
{
	close(fd[1]);
	if (curr->infile_fd > 0)
		close(curr->infile_fd);
	if (curr->next && curr->next->infile_fd < 0)
		curr->next->infile_fd = dup(fd[0]);
	close(fd[0]);
	// if (curr->here_doc == true)
	// 	unlink()
}

void	waitall(t_all *all, t_cmd *cmds)
{
	t_cmd	*cmd;
	int		tmp;

	cmd = cmds;
	// ft_fprintf(2, "itoa: %s\n", ft_itoa(all->status));
	while (cmd)
    {
        waitpid(cmd->pid, &tmp, 0);// Always update status to the last command's result
		// if (WIFEXITED(tmp))
    	// 	all->status = WEXITSTATUS(tmp);
        cmd = cmd->next;
    }
	if (WIFEXITED(tmp))
		if (WEXITSTATUS(tmp) > all->status)
			all->status = WEXITSTATUS(tmp);
}

void	do_pipes(t_all *all, t_cmd *curr, char **env, int fd[2])
{
	if (pipe(fd) == -1)
			pipe_error(all, fd);
	curr->pid = fork();
	if (curr->pid == -1)
		fork_error(all, fd);
	if (curr->pid == 0)
		child_process(all, curr, fd, env);
	else
		parent_process(curr, fd);
}

void	exec_cmds(t_all *all, t_cmd *cmds)
{
	t_cmd	*curr;
	int		fd[2];
	char	**paths;
	char	**env;

	if (!cmds)
		return ;
	curr = cmds;
	all->status = 0;
	while (curr)
	{
		env = env_to_tab(all->env);
		paths = get_paths(all, env);
		curr->args = tokens_to_tab(curr->tokens);
		if (!curr->args)
			curr->path = NULL;
		else
			curr->path = find_exec_path(curr->args[0], paths);
		if (!curr->path || !curr->args[0] || curr->skip_cmd)
		{
			curr = curr->next;
			if (curr && curr->infile_fd < 0)
				curr->infile_fd = open("/dev/null", O_RDONLY);
			continue;
		}
		// else if (curr == cmds && !curr->next && is_builtin(curr->tokens))
		// 	execute_builtins(curr->tokens, &all->env, all);
		// else
		do_pipes(all, curr, env, fd);
		curr = curr->next;
	}
	// debug_print_cmdlist(cmds);
	waitall(all, cmds);
}
