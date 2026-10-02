# Structure
<pre>
minishell/
├── Makefile
├── README.md
├── includes/
│   └── minishell.h            every include, the global, enums, structs, all prototypes
├── libft/                     your libft, with its own Makefile
└── sources/
    ├── main.c                 <b>[bahtiyor]</b> prompt loop
    ├── shell/                 <b>[baran]</b>
    │   ├── shell_init.c       shell_init <i>[ND]</i>
    │   ├── run_line.c         run_line, report_stage_error <i>[ND]</i>
    │   └── shell_exit.c       shell_exit <i>[ND]</i>
    ├── utils/                 <b>[baran]</b>
    │   ├── error.c            print_error <i>[D]</i>
    │   ├── free.c             free_strarr, free_tokens, free_redirs, free_cmds <i>[D]</i>
    │   └── str_utils.c        is_space, is_meta <i>[D]</i>
    ├── env/                   <b>[baran]</b>
    │   ├── env_init.c         env_init, env_new, env_add_back, env_free <i>[ND]</i>
    │   ├── env_access.c       env_get, env_set, env_unset <i>[ND]</i>
    │   └── env_array.c        env_to_array, env_count <i>[ND]</i>
    ├── lexer/                 <b>[baran + bahtiyor]</b>
    │   ├── lexer.c            lexer, read_operator, read_word <i>[D]</i>
    │   └── token.c            token_new, token_add_back, token_text <i>[D]</i>
    ├── parser/                <b>[baran + bahtiyor]</b>
    │   ├── parser.c           parser, parse_command <i>[ND]</i>
    │   ├── syntax.c           syntax_check, syntax_error <i>[ND]</i>
    │   └── cmd.c              cmd_new, cmd_add_back, argv_append, redir_append <i>[ND]</i>
    ├── heredoc/               <b>[baran]</b>
    │   ├── heredoc.c          heredoc_collect, heredoc_one, hd_make_path <i>[ND]</i>
    │   ├── heredoc_child.c    hd_child, hd_read_loop <i>[ND]</i>
    │   └── heredoc_cleanup.c  heredoc_cleanup <i>[ND]</i>
    ├── executor/              <b>[baran]</b>
    │   ├── execute.c          execute, run_builtin_parent <i>[ND]</i>
    │   ├── pipeline.c         run_pipeline, spawn_one <i>[D]</i>
    │   ├── child.c            child_process, exec_external <i>[D]</i>
    │   ├── wait.c             wait_children, report_signal, decode_status <i>[D]</i>
    │   ├── path.c             find_in_path, check_exec_path <i>[D]</i>
    │   └── redirect.c         apply_redirs, open_redir <i>[ND]</i>
    ├── expander/              <b>[bahtiyor]</b>
    ├── signals/               <b>[bahtiyor]</b>
    └── builtins/              <b>[bahtiyor]</b>
</pre>

Shell Execution Flow [REPL]:
	*-->Read user input-->Parse the input into commands-->Execute-->Wait for finish-->*

STEP 1: Readline
STEP 2: Parsing into Commands:
	2.1: Lexical Analysis/Tokenisation
			Breaking the input into WORDS and OPERATORS
				Example: echo "Hello World" | grep Hello > output.txt
				Would be tokenised as:
						- echo
						- "Hello World"
						- |
						- grep
						- Hello
						- >
						- output.txt
	2.2: Syntax Analysis/Parsing
			Organising tokens into a structured command format (AST or alternative command struct)
			Needs to be able to distinguish grammer structure
	
STEP 3: Execution of commands
	3.1:

Flowcharts:
1. Program Overview
```mermaid
flowchart TD
	A["readline: char *line"] --> B["lexer: t_token list"]
	B -->|"unclosed quote: status 2"| Z["free, back to prompt"]
	B --> C["parser: t_cmd list"]
	C -->|"syntax error: status 2"| Z
	C --> D["heredoc_collect: temp files"]
	D -->|"ctrl-C: status 130"| Z
	D --> E["expander: $VAR, $?, quotes removed"]
	E --> F["executor: fork, pipe, dup2, execve"]
	F --> G["sh->last_status = status"]
	G --> Z
```
2. Main Loop and run_line
```mermaid
flowchart TD
	S["shell_init"] --> P["set_signals_prompt"]
	P --> R["line = readline(prompt)"]
	R -->|"NULL (ctrl-D)"| X["print exit, shell_exit(sh, last_status)"]
	R --> G{"g_signal == SIGINT?"}
	G -->|yes| G2["last_status = 130, g_signal = 0"]
	G -->|no| E{"line empty?"}
	G2 --> E
	E -->|yes| F["free(line)"]
	E -->|no| H["add_history(line), run_line(sh, line)"]
	H --> F
	F --> P
```
3. LEXER and Tokenisation
```mermaid
flowchart TD
	A["i = start, quote = 0"] --> B{"end of string?"}
	B -->|yes| Q{"quote != 0?"}
	Q -->|yes| ERR["unclosed quote: return 2"]
	Q -->|no| DONE["word = substr(start, i - start)"]
	B -->|no| C{"quote == 0?"}
	C -->|yes| D{"is_meta(line[i])?"}
	D -->|yes| DONE
	D -->|no| E{"line[i] is #39; or #quot;?"}
	E -->|yes| F["quote = line[i]"]
	E -->|no| N["i++"]
	F --> N
	C -->|no| G{"line[i] == quote?"}
	G -->|yes| H["quote = 0"]
	G -->|no| N
	H --> N
	N --> B
```
4. Parser
```mermaid
flowchart TD
	A["syntax_check(tokens)"] -->|"!= 0"| X["return 2"]
	A -->|ok| B["cur = tokens"]
	B --> C{"cur != NULL?"}
	C -->|no| OK["return 0"]
	C -->|yes| D["cmd = cmd_new(), cmd_add_back"]
	D --> E["parse_command(cmd, &cur): consume until PIPE or end"]
	E --> F{"cur is PIPE?"}
	F -->|yes| G["cur = cur->next"]
	F -->|no| C
	G --> C
```
5. Executor
```mermaid
flowchart TD
	A["execute(sh)"] --> B{"only one cmd AND argv[0] is a builtin?"}
	B -->|yes| C["run_builtin_parent"]
	B -->|no| D["run_pipeline"]
	C --> C1["dup(0), dup(1): save"]
	C1 --> C2{"apply_redirs ok?"}
	C2 -->|yes| C3["status = run_builtin"]
	C2 -->|no| C4["status = 1"]
	C3 --> C5["dup2 back, close saved fds"]
	C4 --> C5
	D --> D1["set_signals_ignore"]
	D1 --> D2["for each cmd: pipe if not last, fork"]
	D2 --> D3["child_process (never returns)"]
	D2 --> D4["parent closes its copies"]
	D4 --> D5["wait_children: status of LAST cmd"]
```
6. The PIPELINE
```mermaid
sequenceDiagram
	participant P as parent
	participant A as child A
	participant B as child B
	participant C as child C
	P->>P: pipe(p1)
	P->>A: fork
	Note over A: dup2(p1[1], 1), close p1 both ends
	P->>P: close p1[1], prev_read = p1[0]
	P->>P: pipe(p2)
	P->>B: fork
	Note over B: dup2(p1[0], 0), dup2(p2[1], 1), close all
	P->>P: close p1[0], close p2[1], prev_read = p2[0]
	P->>C: fork (last: no pipe)
	Note over C: dup2(p2[0], 0), close it
	P->>P: close p2[0]
	P->>P: wait for A, B, C
```
7. exec_external and PATH lookup
```mermaid
flowchart TD
	A["argv[0]"] --> B{"empty string?"}
	B -->|yes| NF["command not found: 127"]
	B -->|no| C{"contains '/'?"}
	C -->|yes| K["path = argv[0]"]
	C -->|no| D{"PATH set?"}
	D -->|no| NS["No such file or directory: 127"]
	D -->|yes| E{"found in a PATH dir?"}
	E -->|no| NF
	E -->|yes| K
	K --> F{"stat ok?"}
	F -->|no| NS
	F -->|yes| G{"directory?"}
	G -->|yes| DIR["Is a directory: 126"]
	G -->|no| H{"access X_OK?"}
	H -->|no| PD["Permission denied: 126"]
	H -->|yes| X["execve"]
	X -->|"returns = failed"| EF["strerror(errno): 126"]
```
8. HEREDOC
```mermaid
flowchart TD
	A["heredoc_collect: next T_HEREDOC redir"] --> B["hd_path = /tmp/.minishell_hd_N"]
	B --> C["fork"]
	C --> CH["child: set_signals_heredoc, open hd_path"]
	CH --> L["line = readline('> ')"]
	L -->|NULL: ctrl-D| W["print warning, stop"]
	L -->|"== delimiter"| S["stop"]
	L -->|other| X["expand if delimiter unquoted, write line + newline"]
	X --> L
	W --> E["close, shell_exit(sh, 0)"]
	S --> E
	C --> PA["parent: set_signals_ignore, waitpid"]
	PA --> Q{"child killed by SIGINT?"}
	Q -->|yes| AB["write newline, return 130: line aborted"]
	Q -->|no| NX["next heredoc, or return 0"]
```
