# Animated Solutions to Coupled PDEs

During my formulation of a solution of a large Scientific Computing assignment in C, I realised that it would be very difficult to analyse my results without a nice way to visualise them. 

To tackle this, I first looked at MATLAB to try quickly plot my results using 3D scatter plots. This worked well for its purpose, but it felt a bit incomplete as the true behaviour of the solutions arise when viewed as a pair of functions changing in time. 

Since I had more experience creating visualisations within Python, I looked into creating some animated GIFs. I found that the `matplotlib.Animation` was a very good fit for my problem as I could control what goes into each frame of my animation due to how my output files are set up.

Below are some examples of GIF outputs that I have produced using the C code and Jupyter Notebook:
