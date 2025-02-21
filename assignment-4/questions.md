1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  We use fork() to create a child process that will execute the command using execvp(). This provides several key benefits including isolation, preserving the shell, and process control. The child process running separately from the parent prevents the parent from being affected if anything goes wrong. Also, if we called execvp() directly it would replace the shell itself and prevent future command calls. Finally, the parent is able to manage the child process which is important for handling errors.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If fork() fails, it returns -1, indicating that no new process was created. This usually happens due to system resource limits. My implementation handles this by printing an error message telling the user that fork failed after checking if the pid is less than 0. It also returns a failure status

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  execvp() searches for the command in the directories listed in the PATH environment variable. It checks each directory in PATH sequentially until it finds an executable matching the command name. If it is not found the command fails and an error code is returned.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  The wait() system call ensures that the parent process waits for the child to finish executing before continuing. This prevents zombie processes and allows the shell to retrieve the child's exit status. If we didn't call wait() the child process would still run, but the parent would not track its completion or know when it finished. This means that the child process may linger in the system and sit as a zombie process until the parent exits. The shell may also try to start processing other commands before the previous command has been completed. 

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**: WEXITSTATUS(status) extracts the exit code of a terminated child process from the status returned by wait(). This exit code tells the shell whether the command succeeded (exit code 0) or failed (nonzero exit code). This is important so we know whether or not a child process has caused an error that should be reported. Some commands also depend on exit statuses, so being able to access the exit status returned by wait() is important. 

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  build_cmd_buff() ensures that quoted arguments (e.g., "hello world") are treated as a single argument instead of being split into "hello" and "world". It does this by recognizing quotes and treating everything inside them as a single argument. It also removes these quotes before passing arguments to execvp(). This is important since some commands take arguments with multiple words, as my example above.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  I made some changes with handling whitespaces in arguments. I had some trouble with quoted strings and nto splitting them incorrectly. Other than this I just had trouble debugging certain cases that were causing failures. 

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  Signals in Linux are used to notify processes of events like termination requests, interruptions, or alarms. Unlike other IPC mechanisms such as pipes, signals are asynchronous and can interrupt a process at any moment. They also require very minimal system resources. 



- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  The common signals I chose to look at are SIGKILL, SIGTERM, and SIGINT. SIGKILL is used to terminate a process forcefully. This cannot be caught or ignored. It may be used to stop programs which are unresponsive. SIGTERM, similar to SIGKILL, terminates a process. However, rather than forcefully ending a process it asks. This means that it can be caught and allow for cleanup. Finally, SIGINT interrupts a process. One case this happens is when the user presses Ctrl+C. 

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  SIGSTOP suspends a process, preventing it from executing until it receives the SIGCONT signal. Unlike SIGINT, SIGSTOP cannot be caught, ignored, or blocked because it is designed to ensure process suspension regardless of the process’s behavior.
