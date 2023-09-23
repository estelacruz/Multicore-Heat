# Multicore-Heat-Project
-------------------------

# Motivation
-------------
A multi-core computer has a single physical processor that contains multiple processing cores. Thus, all cores have the same resources: the same memory, cache, and I/O channels. A multi-core computer can increase its processing power by adding more cores to the processor. Each core is a separate processing unit that can execute instructions independently, allowing the processor to handle multiple tasks simultaneously. 

Multi-core computers are more efficient in terms of:

1. Power Consumption: They require less power to perform the same amount of work (one core has to do all of the work, with multiple cores we can distribute the workload among cores: each core can work at a lower frequency and voltage, resulting in reduced power consumption)

2. Heat Generation: Each core operates at a lower frequency and voltage, reducing heat generation: and requires less cooling, resulting in lower energy consumption and quieter operation

3. Less Physical Space Requirements: Multiple cores can be integrated into a single processor chip (multiple processors require additional physical space to accommodate separate chips).  Multi cores can be easily integrated into smaller devices such as laptops and smartphones

4. Cost-Effective: They provide similar or better performance at a lower cost. They are easier and cheaper to manufacture, and require less power and cooling infrastructure, resulting in lower operating costs. 

5. Versatility: used in consumer electronics and high-end servers so they are more accessible and cost-effective for multiple things


# About 
-------------------
This program is a parallel application that determines the heat distribution in a space using synchronous iteration on a multi-core.


# Visualizing the Problem
-------------------------

We have 2-dimensional square space and simple boundary conditions (edge points have fixed temperatures for this project). The objective is to find the temperature distribution within. The temperature of the interior depends upon the temperatures around it. We find the temperature distribution by dividing the area into a fine mesh of points, hi,j. 

The temperature at an inside point is calculated as the average of the temperatures of the four neighboring points, as shown in the following figure:

<img width="423" alt="Screen Shot 2023-05-02 at 1 17 55 AM" src="https://user-images.githubusercontent.com/79770461/235584720-87ef9a5f-4b6e-4238-a562-ab1d495c868c.png">


# Assumptions | Notes
-------------------
We have (n x n) points (including edge points), (x,y) means row x and column y.

The initial situation is as follows:

• The edge points: (0,0)→(0,n-1), (0,0)→(n-1,0), (0,n1-)→(n-1, n-1), and (n-1,0)→(n-
1,n-1) are initialized to 100.  

• All other points are initialized to zero.

• The boundary points do not change across iterations


There will be a fixed number of iterations ITR. In each iteration, each point in the grid will be updated. 
The points at iteration i must use the neighbors’ values of iteration i, not the updated version. 

# Compilation
-------------
You run the code with ./heatdist x y z k 
- where x is the dimension of the 2D array
- y is the number of iterations 
- z can be 0 (runs and measures the time of the sequential version) or 1 (run the OpenMP version when you finish it and measure the timing)
- k is the number of threads for the OpenMP version. 
- Note that even if you try with z = 0 (i.e. sequential version) you have to type a number for k. Otherwise, the program will exit.



# Overall Project Takeaway

The parallel execution consistently takes less time than the sequential execution across all iterations, leading to a notable 1.5-2x speedup. This means that as the workload grows, our program becomes even more efficient. In practical terms, this translates to reduced processing times, enabling users to complete their tasks faster and more effectively. So, with this program, you can confidently handle larger workloads, knowing that it will run faster and deliver improved performance with increasing job size. Read more into this analysis in the MultiCoreHeat_Analysis.ipynb file.









