/*
 * Frontend command-line utility for Linux NVRAM layer
 *
 * Copyright (C) 2015, Broadcom Corporation. All Rights Reserved.
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * $Id: main.c 496107 2014-08-11 09:29:41Z $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <typedefs.h>
#include <bcmnvram.h>
#include <ctype.h>

void
usage(void)
{
	fprintf(stderr,
		"usage: nvram [get name] [set name=value] [getflag flagname bit]"
		"[setflag flagname bit=value] [unset name] [show] [commit] ...\n");
	exit(0);
}

static int
isnumber(const char *src)
{
	char *iter = (char *)src;
	while (*iter) {
		if (! isdigit(*iter))
			return 0;
		iter++;
	}
	return 1;
}

#define NEXT_ARG(argc, argv) do { argv++; if (--argc <= 0) usage(); } while (0)
#define NEXT_IS_NUMBER(argc, argv) do { if ((argc - 1 <= 0) || !isnumber(argv[1])) usage(); } while (0)
#define NEXT_IS_VALID(argc) do { if ((argc - 1 <= 0)) usage(); }  while (0)

/* NVRAM utility */
int
main(int argc, char **argv)
{
	char *name, *value, *bit = NULL, buf[MAX_NVRAM_SPACE];
	int size;

	/* Skip program name */
	--argc;
	++argv;

	if (!*argv)
		usage();

	/* Process the arguments */
	for (; *argv; ++argv) {
		if (!strcmp(*argv, "get")) {
			if (*++argv) {
				if ((value = nvram_get(*argv)))
					puts(value);
			}
		} else if (!strcmp(*argv, "set")) {
			if (*++argv) {
				strncpy(value = buf, *argv, sizeof(buf));
				name = strsep(&value, "=");
				nvram_set(name, value);
			}
		} else if (!strcmp(*argv, "unset")) {
			if (*++argv)
				nvram_unset(*argv);
        } else if (!strcmp (*argv, "commit") || !strcmp (*argv, "save")) { /*Foxconn add, Jasmine Yang, 03/30/2006 */
			nvram_commit();
		} else if (!strcmp(*argv, "show") ||
		           !strcmp(*argv, "dump")) {
			nvram_getall(buf, sizeof(buf));
			for (name = buf; *name; name += strlen(name) + 1)
				puts(name);
			size = sizeof(struct nvram_header) + (int) name - (int) buf;
			if (**argv != 'd')
				fprintf(stderr, "size: %d bytes (%d left)\n",
				        size, MAX_NVRAM_SPACE - size);
		} else if (!strcmp(*argv, "getflag")) {
			NEXT_ARG(argc, argv);
			NEXT_IS_NUMBER(argc, argv);
			if ((value = nvram_get_bitflag(argv[0], atoi(argv[1]))))
				puts(value);
			++argv;
		} else if (!strcmp(*argv, "setflag")) {
			NEXT_ARG(argc, argv);
			NEXT_IS_VALID(argc);
			value = argv[1];
			bit = strsep(&value, "=");
			if (value && bit && isnumber(value) && isnumber(bit)) {
				nvram_set_bitflag(argv[0], atoi(bit), atoi(value));
				++argv;
			}
		}

#ifdef ACOS_MODULES_ENABLE
        else if (!strncmp (*argv, "loaddefault", 11))   /*Foxconn add, Jasmine Yang, 03/30/2006 */
        {
            /* Write only NVRAM header to flash */
            extern int nvram_loaddefault(void);
            nvram_loaddefault ();
        }
        else if (!strncmp (*argv, "version", 7))        /*Foxconn add, Jasmine Yang, 03/30/2006 */
        {
            memset (buf, '\0', sizeof (buf));
            if ((value = nvram_safe_get ("pmon_ver")))
            {
                strcpy (buf, "Boot Loader Version : CFE ");
                strcat (buf, value);
                strcat (buf, "\n");
            }
            if ((value = nvram_safe_get ("os_version")))
            {
                strcat (buf, "OS Version : Linux ");
                strcat (buf, value);
                strcat (buf, "\n");
            }
            puts (buf);
        }
#endif        
        else
			usage();
	}

	return 0;
}
