# Notes

## Command Line for Unix and Windows

|            Action             |  Unix  | Windows |              Notes               |
| :---------------------------: | :----: | :-----: | :------------------------------: |
|    Print working directory    |  pwd   |   pwd   |                                  |
|       Change directory        |   cd   |   cd    |                                  |
|        Make directory         | mkdir  |         |                                  |
|         List contents         |   ls   |   dir   |    -a to include hidden files    |
|       Remove directory        | rmdir  |         | Only works if directory is empty |
| Remove directory and contents | rm -rf |         |                                  |
|          Create file          | touch  |         |                                  |
|          Remove file          |   rm   |         |                                  |

## Docker

|               Action                |                  Command                   |                                                                             Notes                                                                              |
| :---------------------------------: | :----------------------------------------: | :------------------------------------------------------------------------------------------------------------------------------------------------------------: |
|             List images             |             `docker image ls`              |                                                                                                                                                                |
|       List running containers       |                `docker ps`                 |                                                                                                                                                                |
|         List all containers         |               `docker ps -a`               |                                                                                                                                                                |
|      Remove all docker things       |           `docker system prune`            |                                                                                                                                                                |
|        Remove unused images         |            `docker image prune`            |                                                                                                                                                                |
|    Remove non running containers    |          `docker container prune`          |                                                                                                                                                                |
|      Get output from container      |               `docker logs`                |                                                                                                                                                                |
|      Start a stopped container      |       `docker start <container_id>`        |                                                                                                                                                                |
|     Stop container with SIGTERM     |        `docker stop <container_id>`        |                                                                                                                                                                |
|     Stop container with SIGKILL     |        `docker kill <container_id>`        |                                                                                                                                                                |
| Run extra commands inside container | `docker exec -it <container_id> <command>` | `-i` makes stuff you type get set to the STDIN and back from the STDOUT and STDERR processes. `-t` makes it get show up on your screen in nicely formatted way |
|      Copy file into container       |   `docker cp <path> <container_id:path>`   |                                                                                                                                                                |

## tmux

Remember shift key for lots of the characters in these commands

|            Action            |                      Command                       |               Notes               |
| :--------------------------: | :------------------------------------------------: | :-------------------------------: |
|           Install            |              `sudo apt install tmux`               |                                   |
| Create new pane to the right |                     `Ctrl+b %`                     |                                   |
|            Start             |                       `tmux`                       |                                   |
|    Create new pane below     |                     `Ctrl+b "`                     |                                   |
|    Navigate between panes    |    `Ctrl+b →` `Ctrl+b ←` `Ctrl+b ↑` `Ctrl+b ↓`     |                                   |
|      Close current pane      |                       `exit`                       |                                   |
|      Create new window       |                     `Ctrl+b c`                     |                                   |
|  Switch to window by number  |              `Ctrl+b [WINDOW NUMBER]`              |                                   |
|    Rename current window     |        `Ctrl+b ,` + `[NEW NAME]` + `Enter`         |                                   |
|         Close window         |      `exit` when there is only one pane left       |                                   |
|        Detach session        |                     `Ctrl+b d`                     |                                   |
|   List background sessions   |                     `tmux ls`                      |                                   |
|      Attach to session       |          `tmux attach -t [SESSION NAME]`           |                                   |
|        Rename session        | `tmux rename-session -t [CURRENT NAME] [NEW NAME]` |   name is 0, 1, etc by defualt.   |
|      Create new session      |            `tmux new -s [SESSION NAME]`            | `tmux` names it after next number |
|        Delete session        |       `tmux kill-session -t [SESSION NAME]`        |                                   |
|  Enter scroll mode in pane   |                     `Ctrl+b [`                     |         `q` to quit mode          |

## Unix

Various Linux and/or MacOS things that I find myself forgetting how to do a lot

### Commands

|                Action                 |              Command              |                                    Notes                                    |
| :-----------------------------------: | :-------------------------------: | :-------------------------------------------------------------------------: |
| Add a directory to start of your PATH |  `export PATH=<DIRECTORY>:$PATH`  |                                                                             |
|    Get the size of of a directory     |       `du -sh <DIRECTORY>`        | [See more](https://linuxize.com/post/how-get-size-of-file-directory-linux/) |
|  Change the ownership of a directory  | `sudo chown -R <username> <path>` |                                                                             |

## Excel

|      Action      |     Mac     |   Windows   |                                                   Notes                                                    |
| :--------------: | :---------: | :---------: | :--------------------------------------------------------------------------------------------------------: |
| Go to first cell | `Ctrl+home` | `Ctrl+home` | On a MacOS laptop this means `Ctrl+fn+left arrow`. On a desktop keyboard there are home up and down arrows |

## Visual Studio Code

|    Shortcut     |                   Description                    |
| :-------------: | :----------------------------------------------: |
| shift+alt+arrow |       Duplicate the line the cursor is on        |
|  cmd+alt+arrow  | New cursor on line below or above current cursor |
|   cmd+shift+L   |  Cursors on every occurance of highlighted text  |
|       cmd+g     | Move cursor to next occurance of highlighted text|
|       cmd+d     | Highlight next occurance of highlighted text as well|

## Python

### F Strings

| Input                 | Output                              |
| :-------------------- | :---------------------------------- |
| `f"{'text':10}"`      | "text\\\\\\\\\\" (\ = whitespace)   |
| `f"{"test":#>10}"`    | "######test"                        |
| `f"{"test":#<10}"`    | "test######"                        |
| `f"{"test":#^10}"`    | "###test###"                        |
| `f"{12345:0>10}"`     | "0000012345"                        |
| `f"{-12345:0=10}"`    | "-000012345"                        |
| `f"{12345:010}"`      | "0000012345"                        |
| `f"{-12345:010}"`     | "-000012345"                        |
| `f"{math.pi:.2f}"`    | "3.14"                              |
| `f"{1000000:,.2f}"`   | "1,000,000.00"                      |
| `f"{1000000:\_.2f}"`  | "1_000_000.00"                      |
| `f"{12345:+}"`        | "+12345"                            |
| `f"{-12345:+}"`       | "-12345"                            |
| `f"{-12345:+10}"`     | "\\\\\\\\\\-12345" (\ = whitespace) |
| `f"{-12345:+010}"`    | "-000012345"                        |
| `f"{10:b}"`           | "1010"                              |
| `f"{10:o}"`           | "12"                                |
| `f"{200:x}"`          | "c8"                                |
| `f"{200:X}"`          | "C8"                                |
| `f"{345600000000:e}"` | "3.456000e+11"                      |
| `f"{65:c}"`           | "A" (ASCII value)                   |
| `f"{10:#b}"`          | "0b1010"                            |
| `f"{10:#o}"`          | "0o12"                              |
| `f"{10:#x}"`          | "0xa"                               |
| `f"{0.25:0%}"`        | "25.000000%"                        |
| `f"{0.25:.0%}"`       | "25%"                               |
