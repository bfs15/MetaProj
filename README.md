## MetaProj {#mainpage}
General starting project

`git submodule add https://github.com/bfs15/Grimoire.git;`
`git submodule update --recursive --init;`
Will create a folder ./Grimoire, it will be its own git repository.
Second command will initialize it.

Update submodules with
`git submodule update --recursive`


If LIKWID is going to be used, be sure to add the correct PATHs
example:
`export PATH=/home/soft/likwid/bin:/home/soft/likwid/sbin:$PATH`
`export LD_LIBRARY_PATH=/home/soft/likwid/lib:$LD_LIBRARY_PATH`
