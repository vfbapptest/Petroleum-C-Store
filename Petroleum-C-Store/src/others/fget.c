/**                             fget.c                  **/

/** This is intended to be an easy way to copy files from a remote
    anonymous FTP archive site to your local system.  The format is

        fget host:filename {localfilename}

    "host" can be any Internet hostname, and "filename" must be a
    complete file specifier, including all path information as
    needed. If no "localfilename" is specified, then the file
    is saved under the same name in your own directory. If,
    instead, you use '-' as a filename, it sends the file to
    stdout instead.

    If no path information is specified, then the program invokes an
    "ls" on the remote machine instead: a second argument specifies
    the directory to examine, so "fget netcom.com /etc" will
    result in a listing of the "/etc" directory.

    This program is included in "Teach Yourself UNIX In 24 Hours", a
    new edition of the classic introduction to UNIX from SAMS.net.

    (C) Copyright 1998, Dave Taylor.  All Rights Reserved.
***/

#include "fget.h"

main(argc, argv)
int argc;
char **argv;
{
        FILE *fd;
        char buffer[SLEN], username[NLEN], hostname[NLEN];
        char remotehost[SLEN], remotefile[SLEN], localfname[SLEN];

        if (argc < 2) usage();  /* too few args: usage and quit */

        splitword(argv[1], remotehost, remotefile);     /* split host/file */

        if (argc == 2) strcpy(localfname, basename_of(remotefile));
        else           strcpy(localfname, argv[2]);

        initialize(username, hostname); /* get username and local host */

        if ((fd = fopen(TEMPFILE, "w")) == NULL) {
          fprintf(stderr,
             "Couldn't open tempfile '%s': move into your home directory?\n",
             TEMPFILE);
          exit(1);
        }

        /** now build the information to hand to ftp in the temp file **/

        fprintf(fd, "ascii\nuser %s %s@%s\n", ANONFTP, username, hostname);

        if (strlen(remotefile) == 0) {
          if (strlen(localfname) > 0)           /* directory specified? */
            fprintf(fd, "cd %s\n", localfname); /*     add 'cd' command */
          fprintf(fd, "dir\n");            
        }
        else    /* get a file from the remote site */
          fprintf(fd, "get %s %s\n", remotefile, localfname);

        fprintf(fd, "bye\n");

        fclose(fd);

        /* the input file is built, now to hand it to 'ftp' */

        sprintf(buffer, "ftp -n %s < %s; rm %s", remotehost,TEMPFILE,TEMPFILE);

        exit(system(buffer));
}


/**                             utils.c                 **/

/** Utilities used in the FGET program. See "fget.c" for more info.

    (C) Copyright 1998, Dave Taylor, All Rights Reserved.
***/

#include "fget.h"

initialize(username, hostname)
char *username, *hostname;
{
        /** figure out username and hostname for this system **/

        if (strcpy(username, getenv("USER")) == NULL)
          strcpy(username, getenv("LOGNAME"));

        gethostname(hostname, NLEN);
}

splitword(buffer, word1, word2)
char *buffer, *word1, *word2;
{
        /** Given a buffer that's in the form of host.name:/file/path, break
            it into two words around the colon, with the first word stored as
            word1, and the second as word2. Initialize word2 to NULL if no
            argument is present.  **/

        register int i, j = 0;

        for (i=0; buffer[i] != ':' && buffer[i] != 0; i++)   /* copy word1 */
          word1[i] = buffer[i];
        word1[i] = 0;

        word2[0] = 0;                   /* initialize word2 to NULL */

        if (buffer[i] == 0) return;     /* no colon or nothing following it */

        i++;                            /* skip the colon */

        while (buffer[i] != 0)          /* copy word2 */
          word2[j++] = buffer[i++];
        word2[j] = 0;
}

char *basename_of(filename)
char *filename;
{
        /** returns just the information to the right of all the directory
            slashes in the given filename, or the filename if no slashes! **/

        register int i;

        for (i=strlen(filename)-1; i>0 && filename[i] != '/' &&
             filename[i] != ':'; i--)
           /** zoom backwards through the filename **/ ;

        if (i <= 0)     /* no directories specified, return NULL */
          return( (char *) filename );

        return( (char *) filename + i + 1);
}

usage()
{
        fprintf(stderr, "\nUsage: fget host:remotefile {local}\n\n");
        fprintf(stderr, "If you omit the ':remotefile' portion, then fget will");
	fprintf(stderr, " produce a listing of the\n");
        fprintf(stderr, "files on the remote system. To specify a particular");
	fprintf(stderr, " directory on the\n");
        fprintf(stderr, "remote system, replace 'local' with that ");
	fprintf(stderr, "directory name. For example,\n");
        fprintf(stderr,
"the command 'fget ftp.intuitive.com /pub' will list the contents of the /pub\n");
        fprintf(stderr,
"directory on that machine. Copy a file from the remote system to the local\n");
	fprintf(stderr, 
"system with 'fget ftp.intuitive.com:README' or rename it as you go by using\n");
	fprintf(stderr,
"'fget ftp.intuitive.com:README new.readme'. To display a file directly on\n");
	fprintf(stderr,
"the screen, use '-' as the value for local.\n\n");
        exit(1);
}