#include "kcore.h"
#include "kread.h"

unsigned long symbol_init_pid_ns;
unsigned long pid_xarray;
struct offset_table offset_table = { 0 };
struct size_table size_table = { 0 };

/*
 *  Strip line-ending whitespace.
 */
char *strip_ending_whitespace(char *line)
{
        char *p;

	if (line == NULL || strlen(line) == 0)
                return(line);

        p = &LASTCHAR(line);

        while (*p == ' ' || *p == '\t') {
                *p = NULLCHAR;
                if (p == line)
                        break;
                p--;
        }

        return(line);
}

/*
 *  Strip line-ending linefeeds in a string.
 */
char *strip_linefeeds(char *line)
{
	char *p;

	if (line == NULL || strlen(line) == 0)
		return(line);

	p = &LASTCHAR(line);

	while (*p == '\n') {
		*p = NULLCHAR;
		if (--p < line)
			break; 
	}

	return(line);
}

/*
 *  Strip line-beginning whitespace.
 */
char *strip_beginning_whitespace(char *line)
{
	char buf[BUFSIZE];
        char *p;

	if (line == NULL || strlen(line) == 0)
                return(line);

	strcpy(buf, line);
	p = &buf[0];
	while (*p == ' ' || *p == '\t')
		p++;
	strcpy(line, p);

        return(line);
}

/*
 *  Strip line-ending whitespace and linefeeds.
 */
char *strip_line_end(char *line)
{
	strip_linefeeds(line);
	strip_ending_whitespace(line);
	return(line);
}

/*
 *  Strip line-beginning and line-ending whitespace and linefeeds.
 */
char *clean_line(char *line)
{
	strip_beginning_whitespace(line);
        strip_linefeeds(line);
        strip_ending_whitespace(line);
        return(line);
}

/*
 *  Parse a line into tokens, populate the passed-in argv[] array, and return
 *  the count of arguments found.  This function modifies the passed-string 
 *  by inserting a NULL character at the end of each token.  Expressions 
 *  encompassed by parentheses, and strings encompassed by apostrophes, are 
 *  collected into single tokens.
 */
int parse_line(char *str, char *argv[])
{
	int i, j, k;
    	int string;
	int expression;

	for (i = 0; i < MAXARGS; i++)
		argv[i] = NULL;

	clean_line(str);

        if (str == NULL || strlen(str) == 0)
                return(0);

        i = j = k = 0;
        string = FALSE;
	expression = 0;

	/*
	 * Special handling for when the first character is a '"'.
	 */
	if (str[0] == '"') {
next:
		do {
			i++;
		} while ((str[i] != NULLCHAR) && (str[i] != '"'));

		switch (str[i])
		{
		case NULLCHAR:
			argv[j] = &str[k];
			return j+1;
		case '"':
			argv[j++] = &str[k+1];
			str[i++] = NULLCHAR;
			if (str[i] == '"') {
				k = i;
				goto next;	
			}
			break;
		}
	} 
	else
		argv[j++] = str;

    	while (TRUE) {
		if (j == MAXARGS)
			error_msg("too many arguments in string!\n");

        	while (str[i] != ' ' && str[i] != '\t' && str[i] != NULLCHAR) {
            		i++;
        	}

	        switch (str[i]) {
	        case ' ':
	        case '\t':
	            str[i++] = NULLCHAR;

	            while (str[i] == ' ' || str[i] == '\t') {
	                i++;
	            }
	
	            if (str[i] == '"') {    
	                str[i] = ' ';
	                string = TRUE;
	                i++;
	            }

		    /*
		     *  Make an expression encompassed by a set of parentheses 
		     *  a single argument.  Also account for embedded sets.
		     */
		    if (!string && str[i] == '(') {     
			argv[j++] = &str[i];
			expression = 1;
			while (expression > 0) {
				i++;
				switch (str[i])
				{
				case '(':
					expression++;
					break;
				case ')':
					expression--;
					break;
				case NULLCHAR:
				case '\n':
					expression = -1;
					break;
				default:
					break;
				}
			}
			if (expression == 0) {
				i++;
				continue;
			}
		    }

	            if (str[i] != NULLCHAR && str[i] != '\n') {
	                argv[j++] = &str[i];
	                if (string) {
	                        string = FALSE;
	                        while (str[i] != '"' && str[i] != NULLCHAR)
	                                i++;
	                        if (str[i] == '"')
	                                str[i] = ' ';
	                }
	                break;
	            }
	                        /* else fall through */
	        case '\n':
	            str[i] = NULLCHAR;
	                        /* keep falling... */
	        case NULLCHAR:
	            argv[j] = NULLCHAR;
	            return(j);
	        }
    	}  
}

/*
 *  Determine whether a string contains only hexadecimal characters.
 *  If count is non-zero, limit the search to count characters.
 */
int hexadecimal(char *s, int count)
{
    	char *p;
	int cnt, digits;

	if (!count) {
		strip_line_end(s);
		cnt = 0;
	} else
		cnt = count;

	for (p = &s[0], digits = 0; *p; p++) {
        	switch(*p) 
		{
	        case 'a':
	        case 'b':
	        case 'c':
	        case 'd':
	        case 'e':
	        case 'f':
	        case 'A':
	        case 'B':
	        case 'C':
	        case 'D':
	        case 'E':
	        case 'F':
	        case '1':
	        case '2':
	        case '3':
	        case '4':
	        case '5':
	        case '6':
	        case '7':
	        case '8':
	        case '9':
	        case '0':
			digits++;
	        case 'x':
	        case 'X':
	                break;

	        case ' ':
	                if (*(p+1) == NULLCHAR)
	                    break;
	                else
	                    return FALSE;
		default:
			return FALSE;
        	}

		if (count && (--cnt == 0))
			break;
    	}

    	return (digits ? TRUE : FALSE);
}

/*
 *  Convert a string to a hexadecimal long value.
 */
unsigned long htol(char *s, int flags, int *errptr)
{
    	long i, j; 
	unsigned long n;

    	if (s == NULL) { 
		if (!(flags & QUIET))
			error_msg("received NULL string\n");
		goto htol_error;
	}

    	if (STRNEQ(s, "0x") || STRNEQ(s, "0X"))
		s += 2;

    	if (strlen(s) > MAX_HEXADDR_STRLEN) { 
		if (!(flags & QUIET))
			error_msg("input string too large: \"%s\" (%d vs %d)\n", 
				s, strlen(s), MAX_HEXADDR_STRLEN);
		goto htol_error;
	}

    	for (n = i = 0; s[i] != 0; i++) {
	        switch (s[i]) 
	        {
	            case 'a':
	            case 'b':
	            case 'c':
	            case 'd':
	            case 'e':
	            case 'f':
	                j = (s[i] - 'a') + 10;
	                break;
	            case 'A':
	            case 'B':
	            case 'C':
	            case 'D':
	            case 'E':
	            case 'F':
	                j = (s[i] - 'A') + 10;
	                break;
	            case '1':
	            case '2':
	            case '3':
	            case '4':
	            case '5':
	            case '6':
	            case '7':
	            case '8':
	            case '9':
	            case '0':
	                j = s[i] - '0';
	                break;
		    case 'x':
		    case 'X':
		    case 'h':
			continue;
	            default:
			if (!(flags & QUIET))
				error_msg("invalid input: \"%s\"\n", s);
			goto htol_error;
	        }
	        n = (16 * n) + j;
    	}

    	return(n);

htol_error:
	return BADADDR;
}

/*
 *  Get a symbol value from /proc/kallsyms.
 */
unsigned long lookup_symbol_from_proc_kallsyms(char *symname)
{
        FILE *kp;
	char buf[BUFSIZE];
        char *kallsyms[MAXARGS];
	unsigned long kallsym;
	int found;

	if (!file_exists("/proc/kallsyms", NULL)) {
		error_msg("cannot determine value of %s: "
			"/proc/kallsyms does not exist\n\n", symname);
		return BADVAL;
	}

	if ((kp = fopen("/proc/kallsyms", "r")) == NULL) {
		error_msg("cannot determine value of %s: "
			"cannot open /proc/kallsyms\n\n", symname);
		return BADVAL;
	}

	found = FALSE;
	while (!found && fgets(buf, BUFSIZE, kp) &&
	    (parse_line(buf, kallsyms) == 3)) {
		if (hexadecimal(kallsyms[0], 0) && 
		    STREQ(kallsyms[2], symname)) {
			kallsym = htol(kallsyms[0], RETURN_ON_ERROR, NULL);
			found = TRUE;
			break;
		}
	}
	fclose(kp);

	return (found ? kallsym : BADVAL);
}

void arch_kernel_init(void)
{
	arm64_kernel_init();
}

/*
 *  Return the task_context structure of the first task found with a pid,
 *  while linking all tasks that have that pid. 
 */
struct task_context *
pid_to_context(unsigned long pid)
{
        int i;
        struct task_context *tc, *firsttc, *lasttc;

        tc = FIRST_CONTEXT();
        firsttc = lasttc = NULL;

        for (i = 0; i < RUNNING_TASKS(); i++, tc++) {
                if (tc->pid == pid) {
			if (!firsttc)
                        	firsttc = tc;
                        if (lasttc)
                                lasttc->tc_next = tc;
                        tc->tc_next = NULL;
                        lasttc = tc;
		}
	}

        return firsttc;
}

/*
 * First, we need translate addr into paddr. 'memtype' has following value:
 * 	KVADDR UADDR
 */
int arm64_readmem(ulonglong addr, int memtype, void *buffer, long size,
	char *type, unsigned long error_handle)
{
	long cnt;
	char *bufptr = (char *)buffer;
	unsigned long paddr;

	/* translate addr into paddr */
	
	/* read data by paddr */
	read_proc_kcore(kcore_fd, bufptr, cnt,
		(memtype == PHYSADDR) || (memtype == XENMACHADDR) ? 0 : addr, paddr))
}

/* Initial symbols from kallsyms */
void symbols_init_from_kallsyms(void)
{
	symbol_init_pid_ns = lookup_symbol_from_proc_kallsyms("init_pid_ns");
	if (symbol_init_pid_ns == BADVAL) {
		printf("failed: initial init_pid_ns");
	}
	/* task */
	task_symbol_init();
}

/*
 * In kernel, all tasks maybe managed in xarray or radix tree, depend on:
 *
 * struct idr {
 *	struct radix_tree_root	idr_rt;
 *	unsigned int		idr_base;
 *	unsigned int		idr_next;
 * };
 *
 * The 'radix_tree_root' can been defined "xarray" or "radix", so we can judge
 * type of idr_rt to select initial function.
 */
void task_symbol_init(void)
{
	unsigned long node_p;
	/* default: xarray */
	pid_xarray = symbol_init_pid_ns +
		OFFSET(pid_namespace_idr) + OFFSET(idr_idr_rt);
	/* KVADDR -> paddr -> value */
	arm64_readmem(ptr + OFFSET(xarray_xa_head), KVADDR, &node_p,
		sizeof(void *), "xarray xa_head", FAULT_ON_ERROR);
}

/*
 * #define STRUCT_SIZE(X)      datatype_info((X), NULL, NULL)
 * #define UNION_SIZE(X)       datatype_info((X), NULL, NULL)
 * #define DATATYPE_SIZE(X)    datatype_info((X)->name, NULL, (X))
 * #define MEMBER_OFFSET(X,Y)  datatype_info((X), (Y), NULL)
 * #define MEMBER_SIZE(X,Y)    datatype_info((X), (Y), MEMBER_SIZE_REQUEST)
 * #define MEMBER_TYPE(X,Y)    datatype_info((X), (Y), MEMBER_TYPE_REQUEST)
 */
long datatype_info(char *name, char *member, struct datatype_member *dm)
{
	struct gnu_request request, *req = &request;
	char buf[BUFSIZE];

	strcpy(buf, name);
	memset(req, sizeof(*req));
	req->command = GNU_GET_DATATYPE;
	req->flags |= GNU_RETURN_ON_ERROR;
	req->name = buf;
	req->member = member;
	req->fp = NULL;

	gdb_command_funnel(req);
	return req->length;
	/* request data form gdb */
	// request_gdb(req);
}

/*
 * example:
 * 	gdb -batch -ex 'file vmlinux' -ex 'p sizeof(struct task_struct)'
 */
long request_gdb(struct gnu_request *req)
{
	char buf1[100];
	char buf2[100];
	int cmd = req->command;

	switch (cmd) {
	case GNU_GET_DATATYPE:
		sprintf(buf1, "file %s", current_vmlinux_path);
		sprintf(buf2, "p sizeof(struct %s)", req->name);
		char *argv[] = {"-batch", "-ex", buf1, "-ex", buf2, NULL};
    		char *envp[] = {0, NULL};
	break;
	default:
		printf("something error!");
	break;
	}

	execve("/usr/bin/gdb", argv, envp);
}
