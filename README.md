# CS575 Parallel Programming
### Rachel Xing

## Instruction
- To run the project on the **OSU Flip/Rabbit/HPC server**:
  - Open the terminal, log in to the specific server using your ENGR account, and navigate to the folder
    containing the downloaded files (you might upload the downloaded files from the local first via `scp`).
  - For Projects #1-6: Run the command `bash ProjectX.bash` on the Flip/Rabbit server
  - For Projects #5-7: Run the command `sbatch submit.bash` on the HPC server <br/><br/>
- To run Projects #1-4 on the **local MacOS system**:
  - Install the OpenMP library (if not already installed): `brew install libomp`
  - Set the OpenMP root path in your `~/.zshrc` file:
    - Create/Open the file by `vim ~/.zshrc`
    - Add `export OpenMP_ROOT=$(brew --prefix)/opt/libomp` at the end of the file
  - For every new terminal session, reload the shell configuration from `~/.zshrc` by `source ~/.zshrc`, and
    check if the variable `OpenMP_ROOT` is set correctly by `echo $OpenMP_ROOT`, which should output the path to `libomp`.
  - Finally, run the command `bash ProjectXMac.bash`

 ## Note
- Projects #5 and #6 are GPU programming projects that require a system equipped with a GPU to run.
- Project #7 is an MPI project that must be run on a CPU cluster.
