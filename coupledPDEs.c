#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void read_input(double *C, double *gamma, double *L, int *nx, double *t_F, double *t_out);

int main(void)
{
  // **********
  // Parameters
  // **********

  // Define pointers for all the variables to pass them to the input function.
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
  
  // Get data from pointers
  double C = pC;
  double gamma = pgamma;
  double L = pL;
  int nx = pnx;
  double t_F = pt_F;
  double output_timestep = poutput_timestep;

  //Test Print Statement to see if all variables were read correctly
  // printf("%f %f %f %d %f %f \n", C, gamma, L, nx, t_F, output_timestep);
  // All data read in correctly on testing.

  // Grid spacing
  double dx = L / (nx - 1);

  // Small time step for stability.

  //Dynamically set timestep to ensure stability via CFL Condition.
  //Want Cdt/dx < 1, choose Cdt/dx = 0.5
  double dt = 0.5 * dx / C;

  //Test Print: statement for checking dx and dt on given inputs
  // printf("%f %f \n", dx, dt);
  // On testing, calculated dx = 0.128205, dt = 0.064103

  // ************
  // Grid Storage
  // ************
  double *U, *U_next; //y at current and next timestep
  double *V, *V_next; //u, at current and next timestep.

  /* Allocate memory according to size of nx */
  U = (double *)malloc(nx * sizeof(double));
  V = (double *)malloc(nx * sizeof(double));
  U_next = (double *)malloc(nx * sizeof(double));
  V_next = (double *)malloc(nx * sizeof(double));

  // Memory Allocation Check
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
    U[j] = exp(cos(2 * pi * x / L));

    //Initialise V
    V[j] = 0.0;
  }

  double next_output_time = output_timestep;
  double ctime = 0.0;

  //Print out Initial Condition points to output, so that results start from t=0 rather than t=0.4
  for (j = 0; j < nx; j++)
  {
    x = j * dx;
    printf("%g %g %g %g \n", ctime, x, U[j], V[j]);
  }

  /*{Loop over timesteps}*/
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

    /*{Loop over points}*/
    for (j = 0; j < nx; j++)
    {
      // Upwind Scheme indices
      int jm = (j + nx - 1) % nx;

      /*{Upwind Scheme Calculation of derivative}*/
      double dUdx;
      dUdx = (U[j] - U[jm]) / (dx);

      /*{ Update pointwise }*/
      U_next[j] = U[j] - dt * C * dUdx;
      V_next[j] = V[j] - dt * gamma * (V[j] - U[j]);
    }

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
  }

  // Free Variables
  free(U);
  free(U_next);
  free(V);
  free(V_next);
  return 0;
}

// Reading Inputs
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
