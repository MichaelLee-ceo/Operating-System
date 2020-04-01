/*
    source: https://www.ibm.com/developerworks/aix/library/au-spunix_sharedmemory/
    compile: gcc -o shm shm.c -lrt
    exec: ./shm
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/wait.h>

void error_and_die(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  int r;

  const char *memname = "sample";
  const size_t region_size = sysconf(_SC_PAGE_SIZE);

  int fd = shm_open(memname, O_CREAT | O_TRUNC | O_RDWR, 0666);
  if (fd == -1)
    error_and_die("shm_open");

  r = ftruncate(fd, region_size);
  if (r != 0)
    error_and_die("ftruncate");

  void *ptr = mmap(0, region_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (ptr == MAP_FAILED)
    error_and_die("mmap");
  close(fd);

  pid_t pid = fork();

  if (pid == 0) {
    u_long *d = (u_long *) ptr;
    *d = 0xdbeebee;
    exit(0);
  }
  else {
    int status;
    waitpid(pid, &status, 0);
    printf("child wrote %#lx\n", *(u_long *) ptr);
  }

  r = munmap(ptr, region_size);
  if (r != 0)
    error_and_die("munmap");

  r = shm_unlink(memname);
  if (r != 0)
    error_and_die("shm_unlink");

  return 0;
}


<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head><meta http-equiv="Content-Type" content="text/html; charset=utf-8" /><title>

</title></head>
<body>
    <form method="post" action="./DownloadFile.aspx?Source=Course&amp;CourseType=2&amp;AttachmentID=353101&amp;AttachmentFileName=shm.c" id="form1">
<div class="aspNetHidden">
<input type="hidden" name="__VIEWSTATE" id="__VIEWSTATE" value="/wEPDwUKLTEzNDM3NzkxOWRkE99b+haQV3ZBm26TDQfyZY8BZWY=" />
</div>

<div class="aspNetHidden">

	<input type="hidden" name="__VIEWSTATEGENERATOR" id="__VIEWSTATEGENERATOR" value="F69017DB" />
</div>
    <div>
    
    </div>
    </form>
</body>
</html>
