/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 17:35:13 by ayoakouh          #+#    #+#             */
/*   Updated: 2025/06/17 15:10:11 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void split_stoper(t_cmd *cmd)
{
    t_cmd *tmp;
    char *new_str;
    int exp = 0;
    
    if (!cmd)
        return;
        
    tmp = cmd;
    while(tmp)
    {
        int i = 0;
        exp = 0; 
        
        while (tmp->args && tmp->args[i])
        {
            if (i == 0 && tmp->args[i] && strcmp(tmp->args[i], "export") == 0) 
                exp = 1;
            if (tmp->args_befor_quotes_remover && tmp->args_befor_quotes_remover[i])
            {
                new_str = split_helper(tmp->args[i], tmp->args_befor_quotes_remover[i], exp);
                if (new_str != NULL)
                {
                    free(tmp->args[i]);
                    tmp->args[i] = new_str;
                }
            }
            i++;
        }
        tmp = tmp->next;
    }
}


char *chenger_back(char *str)
{
    int i = 0;

    while (str && str[i])
    {
        if (str[i] == 3)
            str[i] = '\'';
        else if (str[i] == 4)
            str[i] = '\"';
        i++;
    }
    return str;
}

void change_back_cmd(t_cmd *cmd)
{
    t_cmd *tmp;
    int i = 0;
    tmp = cmd;

    while (tmp)
    {
        i = 0;
        while (tmp->args[i])
        {
            tmp->args[i] = chenger_back(tmp->args[i]);
            i++;
        }
        tmp->cmd = chenger_back(tmp->cmd);
        i = 0;
        t_redir *tp = tmp->redirs;
        while (tp)
        {
            tp->file = chenger_back(tp->file);
            tp = tp->next;
        }
        tmp = tmp->next;
    }
}
char *change_space(char *str)
{
    int i = 0;
    int quote_state = 0;
    
    while(str && str[i])
    {
        if (str[i] == '\'')
        {
            if (quote_state == 0)
                quote_state = 1;
            else if (quote_state == 1)
                quote_state = 0;
        }
        else if (str[i] == '"')
        {
            if (quote_state == 0)
                quote_state = 2;
            else if (quote_state == 2)
                quote_state = 0;
        }
        if (str[i] >= 9 && str[i] <=13 && quote_state == 0)
            str[i] = ' ';
        i++;
    }
    return str;
}
void free_extract_result(char **split)
{
    if (!split)
        return;
    
    if (split[0])
        free(split[0]);
    if (split[1])
        free(split[1]);
    free(split);
}

char **extract(char *str)
{ 
    int i = 0;
    char *key;
    char *value;
    char **split = malloc(sizeof(char *) * 3);
    
    if (!split)
        return NULL;
    while (str && str[i] && str[i] != '=')
        i++;
    key = ft_substr(str, 0, i);
    if (!key) 
    {
        free(split);
        return NULL;
    }
    value = ft_substr(str, i+1, ft_strlen(str) - (i+1));
    if (!value) 
    {
        free(key);
        free(split);
        return NULL;
    }
    split[0] = key;
    split[1] = value;
    split[2] = NULL;
    
    return split;
}
char *plus_checker(char *str)
{
    int i = 0;
    
    if (!str)
        return str;
        
    while (str[i])
        i++;
        
    if (i > 0 && str[i - 1] == '+')
        str[i - 1] = '\0';
    return str;
}


char *split_helper(char *str, char *befor, int exp)
{
    char **split;
    char *join1 = NULL;
    char *join3 = NULL;
    char *join4 = NULL;
    char *join2 = NULL;
    char *new_key = NULL;
    
    if (exp != 1)
        return NULL;
        
    if (strchr(str, '=') != NULL)
    {
        split = extract(str);
        if (split != NULL)
        {
            new_key = ft_strdup(split[0]);
            if (!new_key)
            {
                free_extract_result(split);
                return NULL;
            }
            
            new_key = plus_checker(new_key);
            if ((((strchr(split[0], '\'') == NULL && strchr(split[0], '\"') == NULL) && 
                 strchr(split[0], '$') == NULL)) && is_valid_key(new_key) == 0)
            {
                if (split && split[1] && strchr(split[1], '$') != NULL)
                {
                    join1 = ft_strjoin("\"", split[1]);
                    if (!join1) 
                    {
                        free(new_key);
                        free_extract_result(split);
                        return NULL;
                    }
                    join2 = ft_strjoin(join1, "\"");
                    if (!join2) 
                    {
                        free(join1);
                        free(new_key);
                        free_extract_result(split);
                        return NULL;
                    }
                    join3 = ft_strjoin("=", join2);
                    if (!join3) 
                    {
                        free(join1);
                        free(join2);
                        free(new_key);
                        free_extract_result(split);
                        return NULL;
                    }
                    join4 = ft_strjoin(split[0], join3);
                    if (!join4)
                    {
                        free(join1);
                        free(join2);
                        free(join3);
                        free(new_key);
                        free_extract_result(split);
                        return NULL;
                    }
                    free(join1);
                    free(join2);
                    free(join3);
                }
                else
                {
                    join3 = ft_strjoin(split[0], "=");
                    if (!join3) 
                    {
                        free(new_key);
                        free_extract_result(split);
                        return NULL;
                    }
                    join4 = ft_strjoin(join3, split[1]);
                    if (!join4)
                    {
                        free(join3);
                        free(new_key);
                        free_extract_result(split);
                        return NULL;
                    }
                    free(join3);
                }
            }
            else
            {
                join4 = ft_strdup(str);
                if (!join4)
                {
                    free(new_key);
                    free_extract_result(split);
                    return NULL;
                }
            }
            free(new_key); 
            free_extract_result(split);
        }
        else
        {
            join4 = ft_strdup(str);
            if (!join4)
                return NULL;
        }
    }
    else
    {
        join4 = ft_strdup(str);
        if (!join4)
            return NULL;
    }
    return join4;    
}

void add_one_shlvl(t_env *env)
{
	t_env *tmp = env;
	int shl_vl = 0;
	int found = 0;

	if (!env) 
		return;
	while (tmp) 
	{
		if (tmp->key && strcmp(tmp->key, "SHLVL") == 0)
		{
			found = 1;
			if (tmp->value && tmp->value[0] != '\0')
			{
				shl_vl = atoi(tmp->value);
				free(tmp->value);
				tmp->value = NULL;
			}
			
			shl_vl++; 
			tmp->value = ft_itoa(shl_vl);
			if (!tmp->value) 
				tmp->value = strdup("1");
			break;
		}
		tmp = tmp->next;
	}
	if (!found && env)
	{
		t_env *new_node = malloc(sizeof(t_env));
		if (!new_node)
			return;
			
		new_node->key = strdup("SHLVL");
		new_node->value = strdup("1");
		new_node->is_not_active = 0;
		new_node->next = NULL;
		tmp = env;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
	}
}

int global_sig = 0;
int is_builtin(char **args)
{
	if(!args || !*args)
		return (1);
	if (strcmp(args[0], "cd") == 0)
		return (0);
	else if (strcmp(args[0], "echo") == 0)
		return (0);
	else if (strcmp(args[0], "unset") == 0)
		return (0);
	else if (strcmp(args[0], "export") == 0)
		return (0);
	else if (strcmp(args[0], "pwd") == 0)
		return (0);
	else if (strcmp(args[0], "env") == 0)
		return (0);
	else if (strcmp(args[0], "exit") == 0)
		return (0);
	else
		return (1);
}

void excute_builting(t_cmd **command, t_env **env_list, char *env[])
{
	t_cmd	*cmd;
	int		status;

	cmd = *command;
	status = 0;

	if (strncmp("export", cmd->args[0], 6) == 0 && strlen(cmd->args[0]) == 6)
	{
		status = ft_export(cmd->args, env_list);
	}
	else if (strncmp("env", cmd->args[0], 3) == 0 && strlen(cmd->args[0]) == 3)
	{
		status = ft_env(*command, *env_list);
	}
	else if (strncmp("exit", cmd->args[0], 4) == 0 && strlen(cmd->args[0]) == 4)
	{
		status = ft_exit(cmd->args, cmd->data);
	}
	else if (strncmp("unset", cmd->args[0], 5) == 0 && strlen(cmd->args[0]) == 5)
	{
		status = ft_unset(env_list, cmd->args + 1);
	}
	else if (strncmp("echo", cmd->args[0], 4) == 0 && strlen(cmd->args[0]) == 4)
		status = echo(cmd->args);
	else if (strncmp("pwd", cmd->args[0], 3) == 0 && strlen(cmd->args[0]) == 3)
	{
		status = pwd(cmd);
	}
	else if (strncmp("cd", cmd->args[0], 2) == 0 && strlen(cmd->args[0]) == 2)
		status = ft_cd(cmd->args, env_list, cmd);
	// puts("kkkkk");
	// t_env *tmp = env_list;
	// while(tmp)
	// {
	//     printf("%s,,, %s\n", tmp->key, tmp->value);
	//     tmp = tmp ->next;
	// }
	cmd->data.exit_status = get_or_set(SET, status);
}
void restory_window(t_cmd *cmd)
{
	dup2(cmd->fd[0], 0);
	dup2(cmd->fd[1], 1);
	close(cmd->fd[0]);
	close(cmd->fd[1]);
}
void execute_single_command(t_cmd *cmd, t_env **list_env, char *env[])
{
		if(!is_builtin(cmd->args))
		{
			excute_builting(&cmd, list_env, env);
			get_or_set(SET, cmd->data.exit_status);
		}
		else
		{
			if(cmd->pipe_out)
				ft(cmd->args, list_env);
			else
				ft_excute_commands(cmd, list_env);
			get_or_set(SET, cmd->data.exit_status);	
		}
}
void handel_signal(int sig)
{
	if (sig == SIGINT)
	{
		global_sig = 2;
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else 
		global_sig = sig;
}

void excute_redrction(t_cmd **cmd, t_env *env_list)
{
		if((*cmd)->redirs)
		{
			if((*cmd)->redirs->fd[0] == -1)
			{
				(*cmd)->data.exit_status = get_or_set(SET, 1);
				restory_window((*cmd));
				return ;
			}
			ft_redircte((*cmd)->redirs, env_list, *cmd);
		}
}
void cleanup_all_heredocs(t_cmd *cmd)
{
	t_cmd *tmp = cmd;
	t_redir *tmp_redir;
	
	while (tmp)
	{
		tmp_redir = tmp->redirs;
		while (tmp_redir)
		{
			if (tmp_redir->type == 3)
			{
				close_heredoc_fds(tmp_redir);
			}
			tmp_redir = tmp_redir->next;
		}
		tmp = tmp->next;
	}
}
void check_line(t_cmd **command, t_env **env_list, char *env[])
{
	t_cmd	*cmd;
	cmd = *command;

	(*command)->fd[0] = dup(0);
	(*command)->fd[1] = dup(1);
    if ((*command)->redirs != NULL)
    {
        check_here_doc(*command, *env_list);
        if ((*command)->flag == 1)
        {
			cleanup_all_heredocs(cmd);
            restory_window(*command);
            return;
        }
    }
	if (cmd->pipe_out)
	{
		ft_excute_mult_pipe(cmd, *env_list, env);
		cleanup_all_heredocs(cmd);
		get_or_set(SET, cmd->data.exit_status);
		restory_window(cmd);
		return ;
	}
	else
	{
		excute_redrction(command, *env_list);
		execute_single_command(*command, env_list, env);	
	}
	restory_window(cmd);
}



void handl_sig_herdoc()
{
	ft_putstr_fd("\n", 1);
	exit(12);
}
void wait_parent_children(t_cmd *cmd, pid_t child_pid)
{
	int status;

	status = 0;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(child_pid, &status, 0);
	cmd->data.exit_status = WEXITSTATUS(status);
		if (WIFSIGNALED(status))
		{
			int sig = WTERMSIG(status);
			if (sig == SIGQUIT)
				ft_putstr_fd("Quit: 3\n", 1);
			else if (sig == SIGINT)
			{
				cmd->flag = 1;
				cmd->data.exit_status = get_or_set(SET, 1);
			}
			else
			{
				cmd->data.exit_status = get_or_set(SET, 0);
			}
		}
}
void hb(int sig)
{
	(void)sig;
	ft_putstr_fd("\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// void check_here_doc(t_cmd *cmd, t_env *env)
// {
// 	t_cmd *tmp;
// 	t_redir *tmp_redir;

// 	tmp_redir = NULL;
// 	tmp = cmd;
// 	int *fd;
// 	cmd->flag = 1;
// 	while (tmp)
// 	{
// 		tmp_redir = tmp->redirs;
// 		while (tmp_redir)
// 		{
// 			pid_t pid;
// 			pid = fork();
// 			if(pid == 0){
// 			if (tmp_redir->type == 3)
// 				{
// 					signal(SIGINT, hb);
// 					fd = heredoc(tmp_redir->file, env, 0, tmp_redir->orig_token);
// 					// printf("%d\n", fd[1]);
// 					if (fd != NULL)
//                 	{
//                     	tmp_redir->fd = fd[1];
//                     	close(fd[0]);
//                 	}
//                 	else
//                 	{
//                     	tmp_redir->fd = -1;
//                 	}
// 				}
// 			}
// 			if(pid > 0)
// 				wait_parent_children(cmd, pid);
// 			else 
// 				perror("fork");
// 			tmp_redir = tmp_redir->next;
// 		}
		
// 		tmp = tmp->next;
// 	}
// }
void check_sig(t_cmd *cmd)
{
	if(global_sig != 0)
	{
		if (global_sig == 2)
		{
			global_sig = 1;
		}
		else
		{
			global_sig = 0;
		}
		cmd->data.exit_status = get_or_set(SET, global_sig);
	}
}



//  handl_sigee(int sig)
//  {
// 	(void)sig;
// 	ft_putstr_fd("\n", 1);
//  }
int main(int argc, char *argv[], char *env[])
{
	struct termios infos;
	t_token *token_list;
	t_env *env_struct = NULL;
	char *input;
	t_cmd *cmd = NULL;
	char *preprocessed_input;
	int exit_status;

	token_list = NULL;
	(void)argc;
	(void)argv;
	env_struct = env_maker(env, &env_struct);
	if (!env_struct)
	{
		env_struct = env_null();
	}
	if(!isatty(1) || !isatty(0))
	{
		exit(1);
	}
	add_one_shlvl(env_struct);
	tcgetattr(1, &infos);
	while (1)
	{
		desable_echo_term();
		signal(SIGINT, handel_signal);
		signal(SIGQUIT, SIG_IGN);
		input = readline("minishell $> ");
		if (!input)
		{
			printf("exit\n");
			if(!cmd || !cmd->data.exit_status)
				exit(0);
			exit(cmd->data.exit_status);
		}
		// if (global_sig == 2 && input[0] == '\0')
		// {
		// 	global_sig = 0;
		// 	free(input);
		// 	continue;
		// 	puts("lll");
		// }
		add_history(input);
		if (check_quotes(input))
		{
			exit_status = get_or_set(SET, 258);
			free(input);
			continue;
		}
		preprocessed_input = preprocess_command(input); 
		free(input);
		if (!preprocessed_input)
		{
			free(input);
			continue;
		}
		char *new_input = change_space(preprocessed_input); 
		token_list = tokin_list_maker(new_input);
		free(preprocessed_input);
		if (token_list && !error_pipi(token_list) && !check_syntax_errors(token_list))
		{
			cmd = parser(token_list);
			split_stoper(cmd);
			free_token_list(token_list);
			check_sig(cmd);
			expand_handle(cmd, env_struct, get_or_set(GET, 0));
			ambiguous_finder(cmd);
			process_quotes_for_cmd(cmd, 1);
			change_back_cmd(cmd);
			file_opener(cmd, env_struct);
			print_ambiguous_redir_errors(cmd);
			print_cmd(cmd);
			check_line(&cmd, &env_struct, env);
			// free_cmd_list(cmd);
			global_sig = 0;
		}
		else if (error_pipi(token_list)  || check_syntax_errors(token_list))/// must stay
			exit_status = get_or_set(SET, 258); /// must stay
		else if (token_list)/// must stay
		{
			free_token_list(token_list);/// must stay
		}
		tcsetattr(1, TCSANOW, &infos);
		// free(input);
		// free_token_list(token_list);
	}
	free_env_struct(env_struct);
	return 0;
}