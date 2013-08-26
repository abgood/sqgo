#include "sqgo.h"


#ifdef USE_CHANNEL_SELECT

/* channel_select base main loop, with a standard select(2)
 */
void select_loop(ssh_session session,ssh_channel channel){
    fd_set fds;
    struct timeval timeout;
    char buffer[4096];
    ssh_buffer readbuf=ssh_buffer_new();
    ssh_channel channels[2];
    int lus;
    int eof=0;
    int maxfd;
    int ret;
    while(channel){
       /* when a signal is caught, ssh_select will return
         * with SSH_EINTR, which means it should be started
         * again. It lets you handle the signal the faster you
         * can, like in this window changed example. Of course, if
         * your signal handler doesn't call libssh at all, you're
         * free to handle signals directly in sighandler.
         */
        do{
            FD_ZERO(&fds);
            if(!eof)
                FD_SET(0,&fds);
            timeout.tv_sec=30;
            timeout.tv_usec=0;
            FD_SET(ssh_get_fd(session),&fds);
            maxfd=ssh_get_fd(session)+1;
            ret=select(maxfd,&fds,NULL,NULL,&timeout);
            if(ret==EINTR)
                continue;
            if(FD_ISSET(0,&fds)){
                lus=read(0,buffer,sizeof(buffer));
                if(lus)
                    ssh_channel_write(channel,buffer,lus);
                else {
                    eof=1;
                    ssh_channel_send_eof(channel);
                }
            }
            if(FD_ISSET(ssh_get_fd(session),&fds)){
                ssh_set_fd_toread(session);
            }
            channels[0]=channel; // set the first channel we want to read from
            channels[1]=NULL;
            ret=ssh_channel_select(channels,NULL,NULL,NULL); // no specific timeout - just poll
            if(signal_delayed)
                sizechanged();
        } while (ret==EINTR || ret==SSH_EINTR);

        // we already looked for input from stdin. Now, we are looking for input from the channel

        if(channel && ssh_channel_is_closed(channel)){
            ssh_channel_free(channel);
            channel=NULL;
            channels[0]=NULL;
        }
        if(channels[0]){
            while(channel && ssh_channel_is_open(channel) && ssh_channel_poll(channel,0)>0){
                lus=channel_read_buffer(channel,readbuf,0,0);
                if(lus==-1){
                    fprintf(stderr, "Error reading channel: %s\n",
                        ssh_get_error(session));
                    return;
                }
                if(lus==0){
                    ssh_channel_free(channel);
                    channel=channels[0]=NULL;
                } else
                    if (write(1,ssh_buffer_get_begin(readbuf),lus) < 0) {
                      fprintf(stderr, "Error writing to buffer\n");
                      return;
                    }
            }
            while(channel && ssh_channel_is_open(channel) && ssh_channel_poll(channel,1)>0){ /* stderr */
                lus=channel_read_buffer(channel,readbuf,0,1);
                if(lus==-1){
                    fprintf(stderr, "Error reading channel: %s\n",
                        ssh_get_error(session));
                    return;
                }
                if(lus==0){
                    ssh_channel_free(channel);
                    channel=channels[0]=NULL;
                } else
                    if (write(2,ssh_buffer_get_begin(readbuf),lus) < 0) {
                      fprintf(stderr, "Error writing to buffer\n");
                      return;
                    }
            }
        }
        if(channel && ssh_channel_is_closed(channel)){
            ssh_channel_free(channel);
            channel=NULL;
        }
    }
    ssh_buffer_free(readbuf);
}
#else /* CHANNEL_SELECT */

void select_loop(ssh_session session,ssh_channel channel){
	fd_set fds;
	struct timeval timeout;
	char buffer[4096];
	/* channels will be set to the channels to poll.
	 * outchannels will contain the result of the poll
	 */
	ssh_channel channels[2], outchannels[2];
	int lus;
	int eof=0;
	int maxfd;
	unsigned int r;
	int ret;
	while(channel){
		do{
			FD_ZERO(&fds);
			if(!eof)
				FD_SET(0,&fds);
			timeout.tv_sec=30;
			timeout.tv_usec=0;
			FD_SET(ssh_get_fd(session),&fds);
			maxfd=ssh_get_fd(session)+1;
			channels[0]=channel; // set the first channel we want to read from
			channels[1]=NULL;
			ret=ssh_select(channels,outchannels,maxfd,&fds,&timeout);
			if(signal_delayed)
				sizechanged();
			if(ret==EINTR)
				continue;
			if(FD_ISSET(0,&fds)){
				lus=read(0,buffer,sizeof(buffer));
				if(lus)
					ssh_channel_write(channel,buffer,lus);
				else {
					eof=1;
					ssh_channel_send_eof(channel);
				}
			}
			if(channel && ssh_channel_is_closed(channel)){
				ssh_channel_free(channel);
				channel=NULL;
				channels[0]=NULL;
			}
			if(outchannels[0]){
				while(channel && ssh_channel_is_open(channel) && (r = ssh_channel_poll(channel,0))!=0){
					lus=ssh_channel_read(channel,buffer,sizeof(buffer) > r ? r : sizeof(buffer),0);
					if(lus==-1){
						fprintf(stderr, "Error reading channel: %s\n",
								ssh_get_error(session));
						return;
					}
					if(lus==0){
						ssh_channel_free(channel);
						channel=channels[0]=NULL;
					} else
					  if (write(1,buffer,lus) < 0) {
					    fprintf(stderr, "Error writing to buffer\n");
					    return;
					  }
				}
				while(channel && ssh_channel_is_open(channel) && (r = ssh_channel_poll(channel,1))!=0){ /* stderr */
					lus=ssh_channel_read(channel,buffer,sizeof(buffer) > r ? r : sizeof(buffer),1);
					if(lus==-1){
						fprintf(stderr, "Error reading channel: %s\n",
								ssh_get_error(session));
						return;
					}
					if(lus==0){
						ssh_channel_free(channel);
						channel=channels[0]=NULL;
					} else
					  if (write(2,buffer,lus) < 0) {
					    fprintf(stderr, "Error writing to buffer\n");
					    return;
					  }
				}
			}
			if(channel && ssh_channel_is_closed(channel)){
				ssh_channel_free(channel);
				channel=NULL;
			}
		} while (ret==EINTR || ret==SSH_EINTR);

	}
}

#endif
