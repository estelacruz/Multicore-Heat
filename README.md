# Multicore-Heat-Analysis
A parallel application that determines the heat distribution in a space using synchronous iteration on a multicore.

We have 2-dimensional square space and simple boundary conditions (edge points have fixed temperatures). The objective is to find the temperature distribution within. The temperature of the interior depends upon the temperatures around it. We find the
temperature distribution by dividing the area into a fine mesh of points, hi,j. The temperature at an inside point is calculated as the average of the temperatures of the four neighboring points, as shown in the following figure.

<img width="551" alt="Screen Shot 2023-05-01 at 12 43 10 AM" src="https://user-images.githubusercontent.com/79770461/235407752-83ac8e0b-f92a-4445-8c12-13f2b4abe3ea.png">

# Assumptions
----------
We have (n x n) points (including edge points), (x,y) means row x and column y.

The initial situation is as follows:

• The edge points: (0,0)→(0,n-1), (0,0)→(n-1,0), (0,n1-)→(n-1, n-1), and (n-1,0)→(n-
1,n-1) are initialized to 100.  

• All other points are initialized to zero.

• The boundary points do not change across iterations


There will be a fixed number of iterations ITR. In each iteration, each point in the grid will be updated. 
The points at iteration i must use the neighbors’ values of iteration i, not the updated version. 

# Compilation

You run the code with ./heatdist x y z k 
- where x is the dimension of the 2D array
- y is the number of iterations 
- z can be 0 (runs and measures the time of the sequential version) or 1 (run the OpenMP version when you finish it and measure the timing)
- k is the number of threads for the OpenMP version. 
- Note that even if you try with z = 0 (i.e. sequential version) you have to type a number for k. Otherwise, the program will exit.













