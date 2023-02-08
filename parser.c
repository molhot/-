#include <stdlib.h>
#include "minishell.h"
#include <string.h>

/**************************************************
 * 
 *	SHELL GRAMMER
 * .  Simple Commands 
 * 		A simple command is a sequence of optional variable assignments followed by blank-separated words and redirections,
 * 		and terminated by a control operator(パイプとか?).
 * 
 * 
 * 
 *************************************************/

t_token	*new_token(char *word, t_token_kind kind);

bool	at_eof(t_token *tok)
{
	return (tok->kind == TK_EOF);
}

t_node	*new_node(t_node_kind kind)
{
	t_node	*node;

	node = calloc(1, sizeof(*node));
	if (node == NULL)
		fatal_error("calloc");
	node->kind = kind;
	node->command = calloc(1, sizeof(t_command));
	node->next = calloc(1, sizeof(node->next));

	node->command->redirect = NULL;
	return (node);
}

t_token	*tokdup(t_token *tok)
{
	char	*word;

	word = strdup(tok->word);
	if (word == NULL)
		fatal_error("strdup");
	return (new_token(word, tok->kind));
}

void	append_tok(t_token **tokens, t_token *tok)
{
	if (*tokens == NULL)
	{
		*tokens = tok;
		return ;
	}
	append_tok(&(*tokens)->next, tok);
}

bool	parse_redirect(t_redirect **redirect, t_token **tok);

void	parse_word(t_token **args, t_token *tok)
{
	append_tok(args, tok);
}

t_node	*parse(t_token *tok)
{
	t_node		*node;
	t_redirect	*redirection_node;

	node = new_node(ND_SIMPLE_CMD);
	redirection_node = node->command->redirect;
	printf("redirection pointer is > %p\n",redirection_node);
	while (tok && !at_eof(tok))
	{
		printf("%s\n", tok->word);
		if (tok->kind == TK_REDIRECT)
			printf("this is redirect\n");
		if (tok->kind == TK_WORD)
		{
			parse_word(&node->command->args, tokdup(tok));
			tok = tok->next;
		}
		else if (tok->kind == TK_REDIRECT)
		{
			parse_redirect(&redirection_node, &tok);
			printf("redirection pointer is > %p\n",redirection_node);
			printf("redirection content is > %s\n", redirection_node->file_path);
			redirection_node->next = (t_redirect *)malloc(sizeof(t_redirect) * 1);
			redirection_node = redirection_node->next;
			tok = tok->next->next;
		}
		else
			fatal_error("Implement parser");
	}
	node->next = NULL;
	redirection_node = NULL;
	printf("redirection pointer is > %p\n",redirection_node);
	return (node);
}

/*************** 追加したパース用の関数 ****************/

bool	parse_redirect(t_redirect **redirect, t_token **tok)
{
	*redirect = malloc(sizeof(t_redirect));
	// error
	if (strcmp((*tok)->word, "<") == 0)
		(*redirect)->type = IN;
	else
		(*redirect)->type = OUT;

	// tok の next が word だったら redirect の filepathにそれを設定する
	if ((*tok)->next->kind == TK_WORD)
		(*redirect)->file_path = strdup((*tok)->next->word);
	return true;
}


/*************************************
 * 
 * 
 * 
 *  		TEST PARSER
 * 
 * 
 * 
 * ***********************************/


/*#include <stdio.h>
#include <assert.h>

void	test_append_tok(void)
{
	char *line = "echo hoge";
	t_token *t1 = tokenizer(line);
	t_token *t2 = new_token("huga", TK_WORD);
	append_tok(&t1, t2);
	//t_token *itr;
	//itr = t1;

	assert(strcmp(t1->word, line) == 0);
	assert(strcmp(t1->next->next->word, "huga") == 0);
	assert(t1->next->kind == TK_EOF);
	assert(t1->next->next->kind == TK_WORD);

	t_node *n = new_node(ND_SIMPLE_CMD);
	append_tok(&n->command->args, tokdup(t1));
	printf("%s\n", n->command->args->word);
}

void	test_parse(void)
{
	char	*line = "echo hoge";
	t_token *t = tokenizer(line);
	printf("%s\n", t->word);
	t_node  *n = parse(t);
	assert(strcmp(n->command->args->word, "echo hoge"));
	//assert(strcmp(n->command->args->next->word, "hoge"));


	line = "< hoge";
	t = tokenizer(line);
	n = parse(t);

	parse_redirect(&n->command->redirect, &t);
	assert(strcmp(n->command->redirect->file_path, "hoge") == 0);
	assert(n->command->redirect->type == IN);

	line = "cat < hoge";
	t = tokenizer(line);
	n = parse(t);
	assert(strcmp(n->command->args->word, "cat") == 0);
	assert(n->command->redirect->type == IN);
	assert(strcmp(n->command->redirect->file_path, "hoge") == 0);

	// Pipeのパース未実装
	//line = "echo hoge | echo huga";
	//t = tokenizer(line);
	//n = parse(t);
}
int main(void)
{
	test_parse();
	//test_append_tok();
}
*/