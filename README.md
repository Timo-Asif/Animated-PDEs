# Animated Solutions to Coupled PDEs

## Motivation

During my formulation of a solution of a large Scientific Computing assignment in C, I realised that it would be very difficult to analyse my results without a nice way to visualise them. 

To tackle this, I first looked at MATLAB to try quickly plot my results using 3D scatter plots. This worked well for its purpose, but it felt a bit incomplete as the true behaviour of the solutions arise when viewed as a pair of functions changing in time. 

Since I had more experience creating visualisations within Python, I looked into creating some animated GIFs. I found that the `matplotlib.Animation` was a very good fit for my problem as I could control what goes into each frame of my animation due to how my output files are set up.

Full problem formulation located on the `Animation.ipynb` Jupyter Notebook. Open on Google Colab to ensure full LaTeX rendering of formulas. I have included example input and output files to create a quick 20s animation.

## Examples

### Low Gamma Solution
![Alt Text](https://github.com/Timo-Asif/Animated-PDEs/blob/main/Examples/Low%20Gamma.gif?raw=true)

### High Gamma Solution
![Alt Text](https://github.com/Timo-Asif/Animated-PDEs/blob/main/Examples/High%20Gamma.gif?raw=true)

### Slow Transport Solution
![Alt Text](https://github.com/Timo-Asif/Animated-PDEs/blob/main/Examples/Slow%20Transport.gif?raw=true)

### Fast Transport Solution
![Alt Text](https://github.com/Timo-Asif/Animated-PDEs/blob/main/Examples/Fast%20Transport.gif?raw=true)

