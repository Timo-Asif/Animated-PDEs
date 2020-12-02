//
//  Insert list of bugs fixed here: this won't be marked, but may help both of us.
/*
University ID: u1720488
Comments added by me within the code will be signed with "T."

Bugs Resolved/Changes Made:
1. Compilation: Included math.h library

2. Parameters: Fixed pointer behaviour with & operator, ensured all variables read in correctly.
3. Parameters: Added parenthesis to dx calculation to make division by (nx - 1) explicit

4. Grid Storage: Mallocs are now explicitly cast and sizing corrected to sizeof(double)
5. Grid Storage: Added memory allocation error check as advised by lectures

6. Initialisation: U initial condition fixed to exp(cos())
7. Initialisation: V now being properly initialised within the loop

Main Loop Errors:

8. Loop Over Points: Loop arguments/iterator now ordered correctly
9. Loop Over Points: Centred finite difference not stable, now using Upwind Scheme
10. Loop Over Points: Removed jp index, not required for Upwind Scheme
11. Loop Over Points: Fixed jm index to match Periodic Boundary Conditions using modular arithmetic, as per week 6 demonstrations
12. Loop Over Points: Fixed dUdx to now be correct for the Upwind Scheme
13. Loop Over Points: Returned U_next and V_next to correct positions inside the loop
14. Loop Over Points: Corrected sign on U_next as per Upwind scheme

15. Main Loop: Corrected mixing of pointers within the pointer swap of U_next to U and V_next to V
16. Main Loop: Removed second time increment
17. Main loop: Corrected freeing of malloc variables
18. Main Loop: Added return statement

Other Changes Made:

19. Parameters: Timestep dt now chosen via the CFL condition (Week 6 Lectures).
20. Print out Initial Condition to output, now output files start from time t = 0
END OF CHANGES

A Final Note:
I really enjoyed this project and ended up spending a lot of time learning how
to create animations within Python to display my solutions, it was really fun and satisfying!

I have emailed you some of my examples in a GIF format!

Thanks a lot for this project, I've discovered something very enjoyable out of it!
*/
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h> //T. 1. Included math.h library

// The following line does not contain a bug.
void read_input(double *C, double *gamma, double *L, int *nx, double *t_F, double *t_out);

int main(void)
{
  // **********
  // Parameters
  // **********

  // Define pointers for all the variables to pass them to the input function.
  // I set them all to zero to prevent some stupid warning about uninitialised variables.
  // Number of grid points
  int pnx = 0;
  // Length of domain
  double pL = 0;
  // Equation coefficients
  double pC = 0, pgamma = 0;
  //  Length of time to run simulation.
  double pt_F = 0;
  // How frequently in time to output.
  double poutput_timestep = 0;
  // Read in from file;
  read_input(&pC, &pgamma, &pL, &pnx, &pt_F, &poutput_timestep);
  //T. 2. Fixed Pointer behaviour with & operators
  // Get data from pointers
  double C = pC;
  double gamma = pgamma;
  double L = pL;
  int nx = pnx;
  double t_F = pt_F;
  double output_timestep = poutput_timestep;

  //T. Test Print Statement to see if all variables were read correctly
  // printf("%f %f %f %d %f %f \n", C, gamma, L, nx, t_F, output_timestep);
  // All data read in correctly on testing.

  // Grid spacing
  double dx = L / (nx - 1);
  //T. 3. Added parenthesis to make division by (nx - 1) explicit

  // Small time step for stability.

  //T. 19. Fixed timestep to ensure stability via CFL Condition.
  //T. Want Cdt/dx < 1, choose Cdt/dx = 0.5
  double dt = 0.5 * dx / C;

  //T. Test Print: statement for checking dx and dt on given inputs
  // printf("%f %f \n", dx, dt);
  // On testing, calculated dx = 0.128205, dt = 0.064103

  // ************
  // Grid Storage
  // ************
  double *U, *U_next; //y at current and next timestep
  double *V, *V_next; //u, at current and next timestep.

  /* Allocate memory according to size of nx */
  //T. 4. Mallocs are now explicitly cast and sizing corrected to sizeof(double)
  U = (double *)malloc(nx * sizeof(double));
  V = (double *)malloc(nx * sizeof(double));
  U_next = (double *)malloc(nx * sizeof(double));
  V_next = (double *)malloc(nx * sizeof(double));

  //T. 5. I have added a memory allocation error check, as advised in lectures.
  if ((U == NULL) || (V == NULL) ||
      (U_next == NULL) || (V_next == NULL))
  {
    printf("T. Memory Allocation Failed, Exiting...\n");
    return (1);
  }

  int j;
  double x;

  // **************
  // initialisation
  // **************

  // Initialise U and V at time t = 0
  double pi = 4 * atan(1);
  for (j = 0; j < nx; j++)
  {
    x = j * dx;

    //Initialise U
    U[j] = exp(cos(2 * pi * x / L)); //T. 6. Initial condition fixed to exp(cos())

    //Initialise V
    V[j] = 0.0;
    //T. 7. V now being initialised properly within loop to match Boundary Conditions
  }

  double next_output_time = output_timestep;
  double ctime = 0.0;

  //T.20. Print out Initial Condition points to output, so that results start from t=0 rather than t=0.4
  for (j = 0; j < nx; j++)
  {
    x = j * dx;
    printf("%g %g %g %g \n", ctime, x, U[j], V[j]);
  }

  /*{loop over timesteps}*/
  while (ctime < t_F)
  {
    double dt0 = dt;
    int output = 0;
    // If we would overrun the next output step, reduce the timestep.
    if (ctime + dt0 > next_output_time)
    {
      dt0 = next_output_time - ctime;
      output = 1;
    }

    /*{ loop over points }*/
    //T. 8. Loop arguments fixed
    for (j = 0; j < nx; j++)
    {
      /*T. 9. !!! Centred Finite Difference is NOT stable. !!!
      As per specification stability requirements, 
      I have changed the scheme to an upwinding scheme.*/

      //T. 10. jp index not required for upwind
      // int jp = j + 1;

      //T. 11. Upwind Scheme indices, from week 6 advect_upwind.c demonstration
      int jm = (j + nx - 1) % nx;

      //T. 12. Now using Upwind scheme derivative
      /*{Upwind Scheme Calculation of derivative}*/
      double dUdx;
      dUdx = (U[j] - U[jm]) / (dx);

      //T. 13. U_next and V_next put back inside the loop
      /*{ Update pointwise }*/
      U_next[j] = U[j] - dt * C * dUdx;
      V_next[j] = V[j] - dt * gamma * (V[j] - U[j]);

      //T. 14. Changed Sign on U_next expression to match scheme
    }

    //T. 15. Fixed pointer swap, no longer mixing variables
    //T. Want to copy U_next into U and V_next into V
    /*{ Efficiently move values at next timestep to current timestep arrays by swapping pointers }*/
    double *tmp;
    tmp = V_next;
    V_next = V;
    V = tmp;

    tmp = U_next;
    U_next = U;
    U = tmp;

    // Update time.
    ctime += dt0;
    if (output)
    {
      for (j = 0; j < nx; j++)
      {
        x = j * dx;
        printf("%g %g %g %g \n", ctime, x, U[j], V[j]);
      }
      next_output_time += output_timestep;
    }
    //T. 16. Removed second time increment outside of loop
    // ctime += dt;
  }

  //T. 17. Corrected freeing of variables.
  free(U);
  free(U_next);
  free(V);
  free(V_next);

  //T. 18. Added return statement
  return 0;
}

// The lines below don't contain any bugs! Don't modify them.
void read_input(double *C, double *gamma, double *L, int *nx, double *t_F, double *t_out)
{
  FILE *infile;
  if (!(infile = fopen("input.txt", "r")))
  {
    printf("Error opening file\n");
    exit(1);
  }
  if (6 != fscanf(infile, "%lf %lf %lf %d %lf %lf", C, gamma, L, nx, t_F, t_out))
  {
    printf("Error reading parameters from file\n");
    exit(1);
  }
  fclose(infile);
}
