#include "../minishell.h"
#include <stdio.h>

/*__attribute__((destructor))
static void destructor()
{
	system("leaks -q minishell");
}*/

int main()
{
	char	  *line;
	t_token	*tok;
  t_node  *node;

	rl_outstream = stderr;
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break;
		if (*line)
			add_history(line);
		if (line[0] == '/' || line[0] == '.')
			abusolute_path(line);
		else
		{
			tok = tokenizer(line);
			//tok には空白区切りで全てのものが格納されている
			expand(tok);
			node = parse(tok);
			t_node *test;
			test = node;
			//if (tok->kind == EOF)
			//	;
			// if (syntax_error)
			//	continue;
			/*
			t_token *t = tok;
			while(t->next != NULL)
			{
				printf("%s\n", t->word);
				t = t->next;
			}
			*/
			exec(node->command);
			if (tok != NULL)
				free_token(tok);
		}
		free(line);
	}
	exit(0);
}
