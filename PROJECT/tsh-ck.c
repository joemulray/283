/*
* tsh - A tiny shell program with job control
*
* <Joseph Mulray jam699>
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h> 
#include <unistd.h> 

/* Misc manifest constants */
#define MAXLINE 1024 /* max line size */
#define MAXARGS 128 /* max args on a command line */
#define MAXJOBS 16 /* max jobs at any point in time */
#define MAXJID 1<<16 /* max job ID */

/* Job states */
#define UNDEF 0 /* undefined */
#define FG 1 /* running in foreground */
#define BG 2 /* running in background */
#define ST 3 /* stopped */

/*
* Jobs states: FG (foreground), BG (background), ST (stopped)
* Job state transitions and enabling actions:
*     FG -> ST  : ctrl-z
*     ST -> FG  : fg command
*     ST -> BG  : bg command
*     BG -> FG  : fg command
* At most 1 job can be in the FG state.
*/

/* Global variables */
extern char **environ; /* defined in libc */
char prompt[] = "tsh> "; /* command line prompt (DO NOT CHANGE) */
int verbose = 0; /* if true, print additional output */
int nextjid = 1; /* next job ID to allocate */
char sbuf[MAXLINE]; /* for composing sprintf messages */
int size = 0;

struct job_t
 { /* The job struct */
  pid_t pid; /* job PID */
  int jid; /* job ID [1, 2, ...] */
  int state; /* UNDEF, BG, FG, or ST */
  char cmdline[MAXLINE]; /* command line */
 }
;
struct job_t jobs[MAXJOBS]; /* The job list */
/* End global variables */

/* Function prototypes */

/* Here are the functions that you will implement */
void eval(char *cmdline); //have to add more on
int builtin_cmd(char **argv); //Done
void do_bgfg(char **argv);  //Done
void waitfg(pid_t pid); //Done

void sigchld_handler(int sig);  //Done
void sigtstp_handler(int sig); //Done
void sigint_handler(int sig); //Done

/* Here are helper routines that we've provided for you */
int parseline(const char *cmdline, char **argv);
void sigquit_handler(int sig);

void clearjob(struct job_t *job);
void initjobs(struct job_t *jobs);
int maxjid(struct job_t *jobs);
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline);
int deletejob(struct job_t *jobs, pid_t pid);
pid_t fgpid(struct job_t *jobs);
struct job_t *getjobpid(struct job_t *jobs, pid_t pid);
struct job_t *getjobjid(struct job_t *jobs, int jid);
int pid2jid(pid_t pid);
void listjobs(struct job_t *jobs);

void usage(void);
void unix_error(char *msg);
void app_error(char *msg);
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);

/*
* main - The shell's main routine
*/
int main(int argc, char **argv)
 {
  char c;
  char cmdline[MAXLINE];
  int emit_prompt = 1;
  /* emit prompt (default) */

  /* Redirect stderr to stdout (so that driver will get all output
  * on the pipe connected to stdout) */
  dup2(1, 2);

  /* Parse the command line */
  while ((c = getopt(argc, argv, "hvp")) != EOF)
   {
    switch (c)
     {
      case 'h': /* print help message */
      usage();
      break;
      case 'v': /* emit additional diagnostic info */
      verbose = 1;
      break;
      case 'p': /* don't print a prompt */
      emit_prompt = 0;
      /* handy for automatic testing */
      break;
      default:
      usage();
     }
   }

  /* Install the signal handlers */

  /* These are the ones you will need to implement */
  Signal(SIGINT, sigint_handler); /* ctrl-c */
  Signal(SIGTSTP, sigtstp_handler); /* ctrl-z */
  Signal(SIGCHLD, sigchld_handler); /* Terminated or stopped child */

  /* This one provides a clean way to kill the shell */
  Signal(SIGQUIT, sigquit_handler);

  /* Initialize the job list */
  initjobs(jobs);

  /* Execute the shell's read/eval loop */
  while (1)
   {

    /* Read command line */
    if (emit_prompt)
     {
      printf("%s", prompt);
      fflush(stdout);
     }
    if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
    app_error("fgets error");
    if (feof(stdin))
     {
      /* End of file (ctrl-d) */
      fflush(stdout);
      exit(0);
     }

    /* Evaluate the command line */
    eval(cmdline);
    fflush(stdout);
    fflush(stdout);
   }

  exit(0);
  /* control never reaches here */
 }

/*
* eval - Evaluate the command line that the user has just typed in
*
* If the user has requested a built-in command (quit, jobs, bg or fg)
* then execute it immediately. Otherwise, fork a child process and
* run the job in the context of the child. If the job is running in
* the foreground, wait for it to terminate and then return.  Note:
* each child process must have a unique process group ID so that our
* background children don't receive SIGINT (SIGTSTP) from the kernel
* when we type ctrl-c (ctrl-z) at the keyboard.
*/
void eval(char *cmdline)
 {

  char *argv[MAXARGS];  
  int process;         
  pid_t pid;          
  int isRedirect = 0;
  int fd1 = 0, pos = 0;
  int fd2 = 0, pos2 = 0;
  int isPipe = 0, pipePos =0;


  //user does not enter anything just return
  if(strcmp("\n", cmdline) == 0)
     return;

  //get responce of parsing command line
  process = parseline(cmdline, argv);
  if (!builtin_cmd(argv))   
  {
  //check for redirection
  for(int start = 0; start < size; start++){

    //cycle through arguments check for both handling mutiple
    //redirections at once
    if(argv[start]!= NULL){
    if(!strcmp(argv[start], ">")){
      isRedirect = 1;
      pos = start;
      }   
      //if value is redirect in 
      else if(!strcmp(argv[start], "<")){
      isRedirect = 2;
      pos2 = start;
      }
      //also check for pipes during this process.
      else if(!strcmp(argv[start], "|")){
        isPipe = 1;
        pipePos = start;

        }
      }
    }

    //intitilize a signal set 
    sigset_t sigset;    
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);
    sigaddset(&sigset, SIGTSTP);
    sigaddset(&sigset, SIGCHLD);
    sigprocmask(SIG_BLOCK, &sigset, NULL); 
    
    pid = fork();
    if(pid == 0) 
    { 
      //stop signals when running child processes
      sigprocmask(SIG_BLOCK, &sigset, NULL);
      //put child process in group whose id is equal to childs id
      setpgid(0, 0);
      if(isRedirect == 1){ 

      //if there was a redirect found, open a file decscriptor 
      fd1 = open(argv[pos + 1], O_WRONLY | O_CREAT | O_TRUNC);
       if(fd1 < 0 ){
        printf("Error ridirecting file\n");
        return;
       }
            //point std out to file
            dup2(fd1, STDOUT_FILENO);
            close(fd1);
            //replace rest of arguments so program can executre
            argv[pos] = NULL;
            argv[pos+1] = NULL;
    }
    //check if there was redirect stdin 
    else if(isRedirect == 2){
      //if redirect is to stdin
      fd2 = open(argv[pos2 + 1], O_RDONLY);
      if(fd2 < 0 ){
        printf("Error ridirecting file\n");
        return;
       }
       //point file descriptor 2 to stdin
       dup2(fd2, STDIN_FILENO);
       argv[pos2] = NULL;
       argv[pos2+1] = NULL;
    }

    else if(isPipe){
      //Pipe part should redirect one to stdin and other to stdout
        char *cmd1 = argv[pipePos - 1];
        char *cmd2 = argv[pipePos + 1];

        //just conitnue running program
        printf("cmd1: %s\n", cmd1 );
        printf("cmd2: %s\n", cmd2);
        //had both commands outlined couldnt figure way to work out
    }

    //execute command check if command is valid or not
  

  if(execve(argv[0], argv, environ) <0)
      {
        printf("%s: Command not found.\n", argv[0]);
        exit(0);
      } 
    }

  if(!process)   
    { 
      //foreground process returned,
      //addjob(jobs, pid, FG, cmdline); 
      //unblock signals
      sigprocmask(SIG_UNBLOCK, &sigset, NULL);  
      waitfg(pid); 
    }
    else  
    {
      //store as a background process, add job to jobs
      addjob(jobs, pid, BG, cmdline);
      printf("Job: [%%%d] [%d]\n", getjobpid(jobs, pid)->jid, pid);
    }
  }

  return;

 }


/*
* parseline - Parse the command line and build the argv array.
*
* Characters enclosed in single quotes are treated as a single
* argument.  Return true if the user has requested a BG job, false if
* the user has requested a FG job.
*/
int parseline(const char *cmdline, char **argv)
 {
  static char array[MAXLINE]; /* holds local copy of command line */
  char *buf = array; /* ptr that traverses command line */
  char *delim; /* points to first space delimiter */
  int argc; /* number of args */
  int bg; /* background job? */

  strcpy(buf, cmdline);
  buf[strlen(buf)-1] = ' '; /* replace trailing '\n' with space */
  while (*buf && (*buf == ' ')) /* ignore leading spaces */
  buf++;

  /* Build the argv list */
  argc = 0;
  if (*buf == '\'')
   {
    buf++;
    delim = strchr(buf, '\'');
   }
  else
   {
    delim = strchr(buf, ' ');
   }

  while (delim)
   {
    argv[argc++] = buf;
    *delim = '\0';
    buf = delim + 1;
    while (*buf && (*buf == ' ')) /* ignore spaces */
    buf++;

    if (*buf == '\'')
     {
      buf++;
      delim = strchr(buf, '\'');
     }
    else
     {
      delim = strchr(buf, ' ');
     }
   }
  argv[argc] = NULL;
  size = argc;


  if (argc == 0) /* ignore blank line */
  return 1;

  /* should the job run in the background? */
  if ((bg = (*argv[argc-1] == '&')) != 0)
   {
    argv[--argc] = NULL;
   }
  return bg;
 }

/*
* builtin_cmd - If the user has typed a built-in command then execute
*    it immediately.
*/
int builtin_cmd(char **argv)
 {

  /*return 1 if builtin command, otherwise return 0;
  unless quit which will exit*/
  char *command = argv[0];

  //compare each build in command to argument 
  //return 0 if not build in command
  if(!strcmp(command,"quit"))
      exit(0);

  else if(!strcmp(command,"jobs")){
      listjobs(jobs);
      return 1;
    }
  // if command is background
  else if(!strcmp(command,"bg")){
      do_bgfg(argv);
      return 1;
    }

  // if command is background
  else if (!strcmp(command,"fg")){
      do_bgfg(argv);
      return 1; 
    }

  return 0;
  /* not a builtin command */
 }

/*
* do_bgfg - Execute the builtin bg and fg commands
*/
void do_bgfg(char **argv)
 {
  //check for bg or fg and excute based on argument passed in
  //also have to check for validity of arguments
  char * command = argv[0];
  char * id = argv[1];
  struct job_t * job;
  int process;

  if(id == NULL){
   printf("No pid or jid was Given\n");
   printf("Usage: %s %%[jid]\n", command);
   printf("Usage: %s [pid]\n", command);
   return;
  }

  //compare value of argument to either foreground or background
  if(!strcmp(command, "fg")){
    process = FG;
  }
  else{
      process = BG;
   }

  //determine if pid or jid is given
  if(id[0] == '%'){
    
    //check if jid is valid see if any non numbers
    for(int start=1; start < strlen(id); start++){
      if(isalpha(id[start])){
        printf("Invalid jid Given: %s\n", id);
        printf("Usage: %s %%[jid]\n", command);
        return;
      }
    }

    //no characters, now remove % character from jid
    memmove(id, id+1, strlen(id));
    job = getjobjid(jobs, atoi(id));

    if(job != NULL){

        //if state is switched to foreground wait for job
        if(process == FG){
          job->state = FG;
          kill(-(job->pid), SIGCONT);
          waitfg(job->pid);
        }
        //if state is switched to background
        else if(process == BG){
          job->state = BG;
          kill(-(job->pid), SIGCONT);
          waitfg(job->pid);
        }

        printf("Job: [%d]\nPid: [%d]\nCommand:%s", job->jid, job->pid, job->cmdline);
    }

    //no process was found return.
    printf("No Process Found.\n");
    return;
  }

  else{

    //pid was given test for non numbers
    for(int start=0; start < strlen(id); start++){
      if(isalpha(id[start])){
        printf("Invalid pid Given: %s\n", id);
        printf("Usage: %s [pid]\n", command);
        return;
      }
    }

    //passes test get job from pid
    job = getjobpid(jobs, atoi(id));
    if(job != NULL){
      //change state of job to process , FG BG


        //if state is switched to foreground wait for job
        if(process == FG){
          job->state = FG;
          kill(-(job->pid), SIGCONT);
          waitfg(job->pid);
        }
        //check for both bg and fg
        else if(process == BG){
          job->state = BG;
          kill(-(job->pid), SIGCONT);
          waitfg(job->pid);
        }

        printf("Job: [%d]\nPid: [%d]\nCommand:%s", job->jid, job->pid, job->cmdline);
    }

    //no job was found
    printf("No Process Found.\n");
    return;
  }


  return;
 }

/*
* waitfg - Block until process pid is no longer the foreground process
*/
void waitfg(pid_t pid)
 {

  //get pid from jobs, block until no longer in  foreground
  struct job_t *job;
  job = getjobpid(jobs, pid);

  //while job is a foreground process and not null
  while((job != NULL) && (job->state == FG))
  {
    //sleep recheck status of job
    sleep(1);
    job = getjobpid(jobs, pid);
  }
  return;
 }

/*****************
* Signal handlers
*****************/

/*
* sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
*     a child job terminates (becomes a zombie), or stops because it
*     received a SIGSTOP or SIGTSTP signal. The handler reaps all
*     available zombie children, but doesn't wait for any other
*     currently running children to terminate.
*/
void sigchld_handler(int sig)
 {
    //Catches SIGCHILD signals.
    int *stat_val = NULL;
    pid_t pid;

    //wait for any child process
    while((pid = waitpid(-1, stat_val, WUNTRACED | WNOHANG)) > 0){
      struct job_t * job;
      job = getjobpid(jobs, pid);

      //value if the child process terminated normally with exit
      if(WIFEXITED(stat_val))
          deletejob(jobs, pid);

      // value if the child process is stopped
      else if(WIFSTOPPED(stat_val)){
          job->state = ST;
          printf("Stopped Job: [%d]\n", pid);
      }
      //child process terminated because it received a signal that was not handled
      else if(WIFSIGNALED(stat_val)){
          printf("Job terminated: [%d]\n", pid);
          deletejob(jobs, pid);
       }

    }

  return;
 }

/*
* sigint_handler - The kernel sends a SIGINT to the shell whenver the
*    user types ctrl-c at the keyboard.  Catch it and send it along
*    to the foreground job.
*/
void sigint_handler(int sig)
 {
  pid_t pidKill;
  pidKill = fgpid(jobs);

  if(pidKill > 0)
    //if process exists, kill process.
    kill(-pidKill, SIGINT);
  return;
 }

/*
* sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
*     the user types ctrl-z at the keyboard. Catch it and suspend the
*     foreground job by sending it a SIGTSTP.
*/
void sigtstp_handler(int sig)
 {

  pid_t pidKill;
  pidKill = fgpid(jobs);

  if(pidKill > 0)
    //if process exists, kill process.
      kill(-pidKill, SIGTSTP);

  return;
 }

/*********************
* End signal handlers
*********************/

/***********************************************
* Helper routines that manipulate the job list
**********************************************/

/* clearjob - Clear the entries in a job struct */
void clearjob(struct job_t *job)
 {
  job->pid = 0;
  job->jid = 0;
  job->state = UNDEF;
  job->cmdline[0] = '\0';
 }

/* initjobs - Initialize the job list */
void initjobs(struct job_t *jobs)
 {
  int i;

  for (i = 0; i < MAXJOBS; i++)
  clearjob(&jobs[i]);
 }

/* maxjid - Returns largest allocated job ID */
int maxjid(struct job_t *jobs)
 {
  int i, max=0;

  for (i = 0; i < MAXJOBS; i++)
  if (jobs[i].jid > max)
  max = jobs[i].jid;
  return max;
 }

/* addjob - Add a job to the job list */
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline)
 {
  int i;

  if (pid < 1)
  return 0;

  for (i = 0; i < MAXJOBS; i++)
   {
    if (jobs[i].pid == 0)
     {
      jobs[i].pid = pid;
      jobs[i].state = state;
      jobs[i].jid = nextjid++;
      if (nextjid > MAXJOBS)
      nextjid = 1;
      strcpy(jobs[i].cmdline, cmdline);
      if(verbose)
       {
        printf("Added job [%d] %d %s\n",
         jobs[i].jid, jobs[i].pid, jobs[i].cmdline);
       }
      return 1;
     }
   }
  printf("Tried to create too many jobs\n");
  return 0;
 }

/* deletejob - Delete a job whose PID=pid from the job list */
int deletejob(struct job_t *jobs, pid_t pid)
 {
  int i;

  if (pid < 1)
  return 0;

  for (i = 0; i < MAXJOBS; i++)
   {
    if (jobs[i].pid == pid)
     {
      clearjob(&jobs[i]);
      nextjid = maxjid(jobs)+1;
      return 1;
     }
   }
  return 0;
 }

/* fgpid - Return PID of current foreground job, 0 if no such job */
pid_t fgpid(struct job_t *jobs)
 {
  int i;

  for (i = 0; i < MAXJOBS; i++)
  if (jobs[i].state == FG)
  return jobs[i].pid;
  return 0;
 }

/* getjobpid  - Find a job (by PID) on the job list */
struct job_t *getjobpid(struct job_t *jobs, pid_t pid)
 {
  int i;

  if (pid < 1)
  return NULL;
  for (i = 0; i < MAXJOBS; i++)
  if (jobs[i].pid == pid)
  return &jobs[i];
  return NULL;
 }

/* getjobjid  - Find a job (by JID) on the job list */
struct job_t *getjobjid(struct job_t *jobs, int jid)
 {
  int i;

  if (jid < 1)
  return NULL;
  for (i = 0; i < MAXJOBS; i++)
  if (jobs[i].jid == jid)
  return &jobs[i];
  return NULL;
 }

/* pid2jid - Map process ID to job ID */
int pid2jid(pid_t pid)
 {
  int i;

  if (pid < 1)
  return 0;
  for (i = 0; i < MAXJOBS; i++)
  if (jobs[i].pid == pid)
   {
    return jobs[i].jid;
   }
  return 0;
 }

/* listjobs - Print the job list */
void listjobs(struct job_t *jobs)
 {

  int i;

  for (i = 0; i < MAXJOBS; i++)
   {
    if (jobs[i].pid != 0)
     {
      printf("[%d] (%d) ", jobs[i].jid, jobs[i].pid);
      switch (jobs[i].state)
       {
        case BG:
        printf("Running ");
        break;
        case FG:
        printf("Foreground ");
        break;
        case ST:
        printf("Stopped ");
        break;
        default:
        printf("listjobs: Internal error: job[%d].state=%d ",
        i, jobs[i].state);
       }
      printf("%s", jobs[i].cmdline);
     }
   }
 }
/******************************
* end job list helper routines
******************************/


/***********************
* Other helper routines
***********************/

/*
* usage - print a help message
*/
void usage(void)
 {
  printf("Usage: shell [-hvp]\n");
  printf(" -h print this message\n");
  printf(" -v print additional diagnostic information\n");
  printf(" -p do not emit a command prompt\n");
  exit(1);
 }

/*
* unix_error - unix-style error routine
*/
void unix_error(char *msg)
 {
  fprintf(stdout, "%s: %s\n", msg, strerror(errno));
  exit(1);
 }

/*
* app_error - application-style error routine
*/
void app_error(char *msg)
 {
  fprintf(stdout, "%s\n", msg);
  exit(1);
 }

/*
* Signal - wrapper for the sigaction function
*/
handler_t *Signal(int signum, handler_t *handler)
 {
  struct sigaction action, old_action;

  action.sa_handler = handler;
  sigemptyset(&action.sa_mask);   /* block sigs of type being handled */
  action.sa_flags = SA_RESTART;   /* restart syscalls if possible */

  if (sigaction(signum, &action, &old_action) < 0)
  unix_error("Signal error");
  return (old_action.sa_handler);
 }

/*
* sigquit_handler - The driver program can gracefully terminate the
*    child shell by sending it a SIGQUIT signal.
*/
void sigquit_handler(int sig)
 {
  printf("Terminating after receipt of SIGQUIT signal\n");
  exit(1);
 }



