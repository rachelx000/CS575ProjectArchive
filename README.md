# CS575 Parallel Programming
### Author: Rachel Xing

## Instruction
- To run the project on the **OSU flip server**:
  - Open the terminal, log in to the flip server using your ENGR account, and navigate to the folder
    containing the downloaded files (you might upload the downloaded files from the local first via `scp`).
  - Run the command `bash ProjectX.bash`
<br>
- To run the project on the **local MacOS system**:
  - Install the OpenMP library (if not already installed): `brew install libomp`
  - Set the OpenMP root path in your `~/.zshrc` file:
    - Create/Open the file by `vim ~/.zshrc`
    - Add `export OpenMP_ROOT=$(brew --prefix)/opt/libomp` at the end of the file
  - For every new terminal session, reload the shell configuration from `~/.zshrc` by `source ~/.zshrc`, and
    check if the variable `OpenMP_ROOT` is set correctly by `echo $OpenMP_ROOT`, which should
    output the path to `libomp`.
  - Finally, run the command `bash ProjectXMac.bash`