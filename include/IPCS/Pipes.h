#ifndef SSASM_5G_IPCS_PIPES_H
#define SSASM_5G_IPCS_PIPES_H

#define USER_PIPE             "/tmp/user-pipe.fifo"
#define USER_PIPE_PERMISSIONS 0644
#define BACK_PIPE             "/tmp/back-pipe.fifo"
#define BACK_PIPE_PERMISSIONS 0644

void createNamedPipe(const char *const name, const unsigned int permissions);

#endif // !SSASM_5G_IPCS_PIPES_H
