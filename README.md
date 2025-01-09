# mmp
MMP - многопоточный клиент-сервер для любых рассчетов.

На примере ведется рассчет RANDOM 

<img src="https://github.com/oditynet/mmp/blob/main/pic1.jpg" title="example" width="500" />


Add after connect at server^
```
#include <fcntl.h>
#include <signal.h>

signal(SIGIO, hsig);
fcntl(client_fd, F_SETFL, 
fcntl(client_fd, F_GETFL) | O_ASYNC);
// fcntl(server_fd, F_SETOWN, getpid());
```
