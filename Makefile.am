AUTOMAKE_OPTIONS = foreign
bin_PROGRAMS = sqgo
sqgo_SOURCES = aes_code.c authentication.c conn.c display.c get.c knownhosts.c loop.c quiry.c sqgo.c ssh.c verify.c aes_code.h define.h display.h get.h quiry.h sqgo.h ssh.h verify.h direct_login.c direct_login.h
EXTRA_DIST=$(top_srcdir)/scripts
LIBS += `mysql_config --libs` -lssl -lreadline -lncurses -lpcre -lssh
