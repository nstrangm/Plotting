# The Drawn plotting framework

This compact .h file can simply be included into any c++ code, where a root histogram or graph should be plotted.

## What's included?

All of the important root includes like TH1.h, TF1.h, TGraph.h and many more are included, so your code doesn't have to. The macro can plot one and two dimensional plots as well as two pads to show a ratio and some minor painting options.

## How do I plot stuff?

Step 1: Initialize the class you want to use. This could be Plotting1D, Plotting2D, PlottingRatio or PlottingPaint: Plotting1D PExample
Step 2: Add all the histograms and graphs you want to include in the plot and select the plotting options: PExample.NewHist(ptr_to_hist,"Legend entry",20,2,kBlack,"p")
Step 3: Choose other options like margins and axis ranges
Step 4: Export the plot: PExample->Plot("Example.pdf")

###### Simple example for a given pointer to a histogram h
Plotting1D PExample;
PExample->NewHist(h);
PExample->Plot("Example");
// Yes. It's that easy.
