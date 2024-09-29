### 1. **Pipes**

Pipes are one of the simplest forms of **Inter-Process Communication (IPC)** in Unix/Linux. They enable data to flow in one direction, typically between related processes (like a parent and child). Pipes work like a file: one process writes data to the pipe, and the other reads from it.

#### Types of Pipes:

1. **Anonymous Pipes**:
   - Used for communication between parent and child processes.
   - Data flows in a **unidirectional** manner.
   - Created using the `pipe()` system call.
   - Can only be used by processes that share a common ancestor (usually parent-child relationship).

2. **Named Pipes (FIFOs)**:
   - Allows unrelated processes to communicate.
   - Data still flows in a **unidirectional** manner.
   - Created using the `mkfifo()` system call.
   - Persist as a special file and can be accessed via the filesystem, allowing unrelated processes to communicate through it.

#### How Pipes Work

A pipe provides a communication channel between two processes where one can write data, and the other can read that data in a first-in-first-out (FIFO) order.

- **Anonymous Pipe**:
  - **Parent Process** creates a pipe using the `pipe()` system call.
  - Forks a **Child Process**.
  - Parent writes data into the pipe, and the child reads from the pipe (or vice versa).

- **Named Pipe (FIFO)**:
  - Unlike anonymous pipes, named pipes allow communication between unrelated processes.
  - Named pipes are created with a name and can persist in the file system.
  
#### System Calls for Pipes

1. **Anonymous Pipe**:
   - `pipe()` creates a pair of file descriptors for reading and writing.
     ```c
     int fd[2];
     pipe(fd);
     // fd[0]: reading end
     // fd[1]: writing end
     ```
   
   - **Parent process** writes to `fd[1]` and **child process** reads from `fd[0]`.

   Example:
   ```c
   #include <stdio.h>
   #include <unistd.h>

   int main() {
       int fd[2];
       pipe(fd);
       pid_t pid = fork();

       if (pid == 0) {  // Child process
           close(fd[1]);  // Close unused write end
           char buffer[100];
           read(fd[0], buffer, sizeof(buffer));  // Read from pipe
           printf("Child read: %s\n", buffer);
           close(fd[0]);
       } else {  // Parent process
           close(fd[0]);  // Close unused read end
           write(fd[1], "Hello, world!", 13);  // Write to pipe
           close(fd[1]);
       }
       return 0;
   }
   ```

2. **Named Pipe (FIFO)**:
   - `mkfifo()` creates a named pipe in the filesystem.
     ```c
     mkfifo("/tmp/my_fifo", 0666);
     ```

   Example:
   ```c
   // Writer process
   #include <stdio.h>
   #include <fcntl.h>
   #include <sys/stat.h>

   int main() {
       mkfifo("/tmp/my_fifo", 0666);
       int fd = open("/tmp/my_fifo", O_WRONLY);
       write(fd, "Hello from writer", 17);
       close(fd);
       return 0;
   }

   // Reader process
   #include <stdio.h>
   #include <fcntl.h>
   #include <sys/stat.h>

   int main() {
       char buffer[100];
       int fd = open("/tmp/my_fifo", O_RDONLY);
       read(fd, buffer, sizeof(buffer));
       printf("Reader got: %s\n", buffer);
       close(fd);
       return 0;
   }
   ```

#### Use Cases

- **Anonymous Pipes** are useful for:
  - **Parent-child** process communication where the processes are related.
  - Sending **small amounts of data** between processes.

- **Named Pipes (FIFOs)** are useful for:
  - **Unrelated processes** to communicate, as the named pipe persists in the filesystem.
  - Processes needing **persistent IPC** between restarts or across different parts of an application.

### Limitations of Pipes
- Pipes are **unidirectional**; data flows only one way. For two-way communication, two pipes must be used.
- **Blocking**: If the reading process doesn't read the data, the writing process can be blocked (and vice versa).
- For **large data**, performance can become an issue, as pipes are designed for stream-based communication.

In summary, pipes are an essential tool for simple IPC, especially in cases where one process needs to pass data to another process in a straightforward manner. They are particularly common in shell scripting and parent-child communication patterns in C programs.
