# Final Project 

## Description of the repository

Each folder contains a number of c files and one pbs file. The pbs file runs the c files in same same way as when the results in the report were created. 

To recreate a result from a figure in the report look for the folder that has the same name as this figure. The pbs file in this folder will then be named "resultsFigureX" where X is the number of the figure. 

## How to run the pbs files on the cluster 

- You need to change on line in the pbs file you want to run. This is where you choose your woring directory on the cluster. The line above the line you should change is marked with: # ---CHANGE THE LINE BELOW---
- On the cluster, go into the folderX corresponding to the results you want to recreate. 
- Write: qsub resultsFigureX.pbs 
- After the job is done you can find the results by looking at the file figureX.o