# Virtual Prototyping CMake projects

Use  *createProject.sh  project*  to create a new project folder with all files from the blank project.
Use *cmakeNormal.sh* or *cmakeVoid.sh* to start the cmake procedure (or just use *cmake*).

**Don't forget** to add to *~/.bashrc* :
*export SYSTEMC=/directory/of/systemc* 
*export SYSTEMCAMS=/home/nkay/apps/systemc-ams*
*export LD_LIBRARY_PATH=$SYSTEMC/lib-linux64:$SYSTEMC/lib:$SYSTEMC/lib64:$SYSTEMCAMS/lib-linux64*
