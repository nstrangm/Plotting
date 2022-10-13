//******************************************************************************
// Plotting Class for exporting histograms (1D,2D,Ratios) in the root framework
// Author: Nicolas Strangmann (nicolas.strangmann@cern.ch) Created: 10.10.2020
//******************************************************************************

#ifndef DRAWN
#define DRAWN

#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TLine.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TObjString.h"
#include "TLatex.h"
#include "TString.h"
#include "TStyle.h"
#include "TColor.h"
#include "TFile.h"
#include "TRandom.h"
#include "TTree.h"
#include "TEllipse.h"
#include "TCurlyLine.h"
#include "TMath.h"
#include <iostream>
#include <string>
#include <vector>

using std::cout;  //  Now the std:: in std::cout can be omitted
using std::cerr;  //  Preferably use cerr since cout is not always printed exactly where called
using std::endl;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++ Plotting ++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  The Plotting-class itself is never used but all the other classes inherit functions and attributes from it

class Plotting{
  public:

    Plotting(); // Empty constructor

    ~Plotting();  // Empty destructor

    //  Universal function for setting the legends relative position on the canvas
    void SetLegend(Double_t x1 = 0.15, Double_t x2 = 0.4, Double_t y1 = 0.7, Double_t y2 = 0.9);

    //  Setting a border to 42 triggers the AutoSetAxisRanges funtion
    void SetAxisRange(Double_t xlow=42, Double_t xup=42, Double_t ylow=42, Double_t yup=42, Double_t zlow = 0, Double_t zup = 2);

    //  Adds a string to vector Latex that will be drawn on canvas in function Plot(). Position in relative coordinates. Use ; to split string into seperate lines.
    void DrawLatex(const Double_t  PositX = 0.2, const Double_t  PositY = 0.2, TString text = "", const Double_t TextSize = 0.035, const Double_t dDist = 0.05, const Int_t font = 42, const Int_t color = kBlack );

    //  Adds a line to vector lines that will be drawn on canvas in function Plot(). The lines coordinates relate to the axis. When setting negative style a curly line will be drawn instead of the straight one.
    void NewLine(Double_t x1 = 0, Double_t y1 = 0, Double_t x2 = 1, Double_t y2 = 1, Int_t style = 1, Int_t color = kBlack, Int_t width = 1, TString label = "");

    //  Set the relative empty space between hist and the edges aswell as the canvas dimensions in pixel
    void SetMargins(Double_t low = 0.1, Double_t left = 0.1, Double_t up = 0.01, Double_t right = 0.01, Int_t cw = 1200, Int_t ch = 1000);

  protected:

    TCanvas *Canvas = nullptr;  //  The canvas that all classes plot on

    TH2D* hDummy = nullptr; //  Empty histogram with the correct axis ranges and labels plotted first

    TLegend *leg = nullptr;

    //  Vectors of elements that are added by New... functions and are drawn in the Plot() functions
    std::vector<TH1F*> hists;
    std::vector<TGraph*> graphs;
    std::vector<TF1*> funcs;
    std::vector<TLine*> lines;
    std::vector<TCurlyLine*> clines;
    std::vector<TLatex*> Latex;

    std::vector<TString> DrawOption;  //  A histogram is plotted using the corresponding DrawOption ("p","h",..)
    std::vector<TString> LegendLabel; //  Strings corresponding to histograms are added to legend
    std::vector<TString> LegendLabelF;
    std::vector<TString> DrawOptionF;
    std::vector<TString> LegendLabelG;
    std::vector<TString> DrawOptionG;
    std::vector<TString>  LegendLabelL; //  Strings corresponding to lines are added to legend (if not empty)

    //  The following are standard settings that can be changes by calling Set.. functions before Plot()
    Double_t AxisRange[3][2] = {{42,42},{42,42},{0,2}}; // xlow,xuo,ylow,yup,zlow,zup (in PlottingRatio z=ratio)
    TString AxisLabel[3] = {"x","y","Ratio"};
    Double_t AxisLabelOffset[2] = {1.,1.};  // Third component not needed for ratio trivial, for 2D z has no Label currently
    Double_t LegendBorders[2][2] = {{0.15,0.4},{0.7,0.9}};  //  xlow,xup,ylow,yup in relative units (0-1)
    Double_t CanvasMargins[2][2] = {{0.1,0.01},{0.1,0.01}}; //  left,right,low,up in relative units
    Int_t CanvasDimensions[2] = {1200,1000};  // Dimension given in pixels

    //  If no style and or color are set these 10 standard styles and colors are used one after the other
    Int_t AutoStyle[10] = {20, 21, 34, 33, 27, 24, 28, 22, 23,29};
    Int_t AutoStyleLine[10] = {1, 7, 9, 2, 8, 1, 7, 9, 2, 8};
    Int_t AutoColor[10] = {kBlue+1, kRed+1, kGreen+2, kBlack, kOrange+2, kCyan+3, kTeal-7, kPink+2, kYellow+3, kSpring+4};
    Int_t counter = 0;  //  Count up after each added histogram and use e.g. AutoStyle[counter] -> Unique colors and styles

    //  Create the legend leg using LegendBorders that will be drawn in Plot()
    void InitializeLegend();

    //  Adjusts the x and y axis range depending on the histograms that will be drawn
    void AutoSetAxisRanges(Bool_t logy);

    //  When encountering NULL pointers or other errors, exit(0) with a short error Message
    void Abort(TString Message);

    //  Converts the given DrawOptions to good parametes for the legend reference symbols
    TString LegendDrawOption(TString DrawOpt);

};

Plotting::Plotting(){

}

Plotting::~Plotting(){

}

//  The following 3 functions simply copy the user given settings into attributes of the Plotting class
void Plotting::SetMargins(Double_t low, Double_t left, Double_t up, Double_t right, Int_t cw, Int_t ch){
  CanvasMargins[0][0] = left;
  CanvasMargins[0][1] = right;
  CanvasMargins[1][0] = low;
  CanvasMargins[1][1] = up;
  CanvasDimensions[0] = cw;
  CanvasDimensions[1] = ch;
} //  These parameters will be used when Plot() calls InitializeCanvas

void Plotting::SetAxisRange(Double_t xlow, Double_t xup, Double_t ylow, Double_t yup, Double_t zlow, Double_t zup){
  AxisRange[0][0] = xlow;
  AxisRange[0][1] = xup;
  AxisRange[1][0] = ylow;
  AxisRange[1][1] = yup;
  AxisRange[2][0] = zlow; //  Not used in Plotting 1D
  AxisRange[2][1] = zup;  // In PlottingRatio this gives the range of the ratio
} //  These parameters will be used when Plot() calls InitializeAxis

void Plotting::SetLegend(Double_t x1, Double_t x2, Double_t y1, Double_t y2){
  LegendBorders[0][0] = x1;
  LegendBorders[0][1] = x2;
  LegendBorders[1][0] = y1;
  LegendBorders[1][1] = y2;
} //  These parameters will be used when Plot() calls InitializeLegend

void Plotting::InitializeLegend(){
  leg = new TLegend(LegendBorders[0][0], LegendBorders[1][0], LegendBorders[0][1], LegendBorders[1][1]);
  leg->SetHeader(""); //  Remove title of legend
  leg->SetTextFont(42);
  leg->SetTextSize(0.035);
  leg->SetBorderSize(0);  //  Remove black rectangle around legend
  leg->SetFillStyle(1001);  // Solid white background to make legend readable. Set to 0 to make it hollow
}

void Plotting::AutoSetAxisRanges(Bool_t logy){

  if(hists.size() > 0){
    Double_t max = hists.at(0)->GetMaximum(); //  Estimated upper y border

    //  For logarithmic y axis the minimum has be be larger than 0
    Double_t min = logy ? hists.at(0)->GetMinimum(0.) : hists.at(0)->GetMinimum();

    //  Find the smalles and largest bin in all loaded histograms (larger than 0 for logy)
    for ( Int_t i = 0; i < (Int_t)hists.size(); i++) {
      max = hists.at(i)->GetMaximum() > max ? hists.at(i)->GetMaximum() : max;
      min = (hists.at(i)->GetMinimum() < min  && (!logy || hists.at(i)->GetMinimum() > 0)) ? hists.at(i)->GetMinimum() : min;
    }

    //  Leave space between highest/lowest bin and the axis borders so you can see every bin
    max = logy ? 2*max : max+(max-min)/10;  //  With log scales a factor 2 isn't too much
    min = logy ? 0.5*min : (max - 2*min > 0 ? (min > 0 ? 0 : 1.1*min) : min-(max-min)/8); //  max-2*min>0 ? min is small->go down to 0 : all bins rather full

    //  If the respective range was set to 42 use the just calculated estimates
    if (AxisRange[1][0] > 41.99 && AxisRange[1][0] < 42.01) AxisRange[1][0] = min;
    if (AxisRange[1][1] > 41.99 && AxisRange[1][1] < 42.01) AxisRange[1][1] = max;
    if (AxisRange[0][0] > 41.99 && AxisRange[0][0] < 42.01) AxisRange[0][0] = hists.at(0)->GetBinLowEdge(1);
    if (AxisRange[0][1] > 41.99 && AxisRange[0][1] < 42.01) {//  Get the x axis range assuming the hists have the same bins
      AxisRange[0][1] = hists.at(0)->GetBinLowEdge(hists.at(0)->GetNbinsX())+hists.at(0)->GetBinWidth(hists.at(0)->GetNbinsX());
    }
  }
  else if(graphs.size() > 0){
    //  When you only have graphs convert the first one into a hist and use its axis range. If you have multiple graphs the range has to be chosen manually
    TH1F* hAxisFromGraph = graphs.at(0)->GetHistogram();

    Double_t max = hAxisFromGraph->GetMaximum(); //  Estimated upper y border

    //  For logarithmic y axis the minimum has be be larger than 0
    Double_t min = logy ? hAxisFromGraph->GetMinimum(0.) : hAxisFromGraph->GetMinimum();

    //  Leave space between highest/lowest bin and the axis borders so you can see every bin
    max = logy ? 2*max : max+(max-min)/10;  //  With log scales a factor 2 isn't too much
    min = logy ? 0.5*min : (max - 2*min > 0 ? 0 : min-(max-min)/8); //  max-2*min>0 ? min is small->go down to 0 : all bins rather full

    //  If the respective range was set to 42 use the just calculated estimates
    if (AxisRange[1][0] > 41.99 && AxisRange[1][0] < 42.01) AxisRange[1][0] = min;
    if (AxisRange[1][1] > 41.99 && AxisRange[1][1] < 42.01) AxisRange[1][1] = max;
    if (AxisRange[0][0] > 41.99 && AxisRange[0][0] < 42.01) AxisRange[0][0] = hAxisFromGraph->GetBinLowEdge(1);
    if (AxisRange[0][1] > 41.99 && AxisRange[0][1] < 42.01) {//  Get the x axis range assuming the hists have the same bins
      AxisRange[0][1] = hAxisFromGraph->GetBinLowEdge(hAxisFromGraph->GetNbinsX())+hAxisFromGraph->GetBinWidth(hAxisFromGraph->GetNbinsX());
    }
  }
  else if(funcs.size() > 0){ cout << "Warning: Only functions have been inserted..." << endl; }
  else Abort("No histogram or graph given.");
}

void Plotting::DrawLatex(const Double_t  PositX, const Double_t  PositY, TString text, const Double_t TextSize, const Double_t dDist, const Int_t font, const Int_t color){
  std::vector<TString> LatStr;  //  Each element corresponds to a line of the printed latex string
  TObjArray *textStr = text.Tokenize(";");  //  The semicolon seperates the string into different lines
  for(Int_t i = 0; i<textStr->GetEntries() ; i++){
     TObjString* tempObj     = (TObjString*) textStr->At(i);
     LatStr.push_back( tempObj->GetString());
   }

  //  Loop thru the latex lines and set the formatting
  for( Int_t i = 0; i < (Int_t)LatStr.size(); ++i){
      Latex.push_back( new TLatex(PositX, PositY - i*dDist, LatStr[i]));
      Latex.at(Latex.size() - 1)->SetNDC();
      Latex.at(Latex.size() - 1)->SetTextFont(font);
      Latex.at(Latex.size() - 1)->SetTextColor(color);
      Latex.at(Latex.size() - 1)->SetTextSize(TextSize);
  }
}

void Plotting::NewLine(Double_t x1, Double_t y1, Double_t x2, Double_t y2, Int_t style, Int_t color , Int_t width, TString label){

  //  Curly lines can be used to draw photons or similar
  if (style < 0) {
    TCurlyLine* line = new TCurlyLine(x1,y1,x2,y2);
    line->SetLineColor(color);
    line->SetLineWidth(width);
    line->SetWaveLength(-0.02*style); //  Standard wavelength is 0.02 -> Style -1
    clines.push_back(line);
  }
  else{
    TLine* line = new TLine(x1,y1,x2,y2);
    LegendLabelL.push_back(label);
    line->SetLineColor(color);
    line->SetLineStyle(style);
    line->SetLineWidth(width);
    lines.push_back(line);
  }

}

//  This function is called when a fatal error occured (e.g. draw a NULLptr)
void Plotting::Abort(TString Message){
  cerr << Message << " Aborting..." << endl;
  exit(0);
}

TString Plotting::LegendDrawOption(TString UserDrawOpt){
  TString LegendDrawOpt = "p";  //  If user gives no DrawOption use p as standard
  if ((Int_t)*(UserDrawOpt.Data())) LegendDrawOpt = UserDrawOpt;
  if ( UserDrawOpt.Contains("h") ) LegendDrawOpt = "l"; //  Histogram style hists should have a line in the legend
  if ( UserDrawOpt.Contains("E1") ) LegendDrawOpt = "pE1"; //  Histogram style hists should have a line in the legend
  if ( UserDrawOpt.Contains("E1") && UserDrawOpt.Contains("f") ) LegendDrawOpt = "fpE1"; //  Histogram style hists should have a line in the legend
  if ( UserDrawOpt.Contains("E1") && UserDrawOpt.Contains("z") ) LegendDrawOpt = "fpE1"; //  Histogram style hists should have a line in the legend
  return LegendDrawOpt;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++ Plotting 1D ++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class Plotting1D : public Plotting{
  public:

    Plotting1D(); // This empty constructor always has to be called when plotting 1D

    ~Plotting1D();  // Destructor

    //  After adding all histograms, functions and graph using the New.. functions create the actual plot
    void Plot(TString name = "dummy.pdf", Bool_t logx = false, Bool_t logy = false);

    //  Add a histogram to the hists vector and put all its settings into different vectors
    void NewHist(TH1F* h = nullptr, TString label = "", Int_t style = -1, Int_t size = 1, Int_t color = -1, TString opt = "p");

    //  This entire macro is written for TH1Fs because you cant see the difference and it takes half the memory.
    //  If given a TH1D just convert it to a TH1F and give it to the other NewHist function
    void NewHist(TH1D* h = nullptr, TString label = "", Int_t style = -1, Int_t size = 1, Int_t color = -1, TString opt = "p");

    //  Add a new function/graph to the funcs/graphs vector that will be drawn when calling Plot()
    void NewFunc(TF1* f = nullptr, TString label = "", Int_t style = -1, Int_t size = 1, Int_t color = -1, TString opt = "l");
    void NewGraph(TGraph* h = nullptr, TString label = "", Int_t style = -1, Int_t size = 1, Int_t color = -1, TString opt = "p");

    //  Store the user wishes for labels and offsets in the AxisLabel and AxisLabelOffset attributes. They will later be used in InitializeAxis.
    void SetAxisLabel(TString labelx = "", TString labely = "", Double_t offsetx = 1., Double_t offsety = 1.);

  private:

    //  Create the canvas using the standard dimensions and margins, if they were not set by SetMargins
    void InitializeCanvas(Bool_t logx, Bool_t logy);

    //  Create the hdummy that will be plotted first and give it the Set xis ranges and labels
    void InitializeAxis(Bool_t logy);

};

Plotting1D::Plotting1D(){

}

Plotting1D::~Plotting1D(){

}

void Plotting1D::Plot(TString name, Bool_t logx, Bool_t logy){

  if(hists.size() < 1 && graphs.size() < 1 && funcs.size() < 1) Abort("No hists added for plotting.");

  InitializeCanvas(logx, logy); //  Creating Canvas with margins
  InitializeAxis(logy); //  Create the hDummy and set its axis label + ranges
  hDummy->Draw(); //  Draw the just set axis (label) on the Canvas

  InitializeLegend(); //  Create leg and set its dimensions + format

  //  Loop thru all elements of all vectors and plot them on top of the empty hDummy and add them to leg
  //----------------------------------------------------------------------------
  for( Int_t i = 0; i < (Int_t)lines.size(); ++i) lines.at(i)->Draw("same");

  for( Int_t i = 0; i < (Int_t)clines.size(); ++i) clines.at(i)->Draw("same");

  for( Int_t i = 0; i < (Int_t)graphs.size(); ++i){
    graphs.at(i)->Draw(Form("same %s", ((TString) DrawOptionG.at(i)).Data()));
  }

  for( Int_t i = 0; i < (Int_t)hists.size(); ++i){
    hists.at(i)->Draw(Form("same %s", ((TString) DrawOption.at(i)).Data()));
    if ((Int_t)*(LegendLabel.at(i).Data())) leg->AddEntry(hists.at(i), LegendLabel.at(i).Data(), LegendDrawOption(DrawOption.at(i)));
  } //  Dont add anything to the legend if LegendLabel is empty

  for( Int_t i = 0; i < (Int_t)graphs.size(); ++i){
    if ((Int_t)*(LegendLabelG.at(i).Data())) leg->AddEntry(graphs.at(i), LegendLabelG.at(i).Data(), LegendDrawOption(DrawOptionG.at(i)).Data());
  }

  for( Int_t i = 0; i < (Int_t)funcs.size(); ++i){
    funcs.at(i)->Draw(Form("same %s", ((TString) DrawOptionF.at(i)).Data()));
    if ((Int_t)*(LegendLabelF.at(i).Data())) leg->AddEntry(funcs.at(i), LegendLabelF.at(i).Data(), (DrawOptionF.at(i).Contains("l") || DrawOptionF.at(i).Contains("hist") || DrawOptionF.at(i).Contains("C") ) ? "l" : "p");
  } //  Dont add anything to the legend if LegendLabelF is empty


  for( Int_t i = 0; i < (Int_t)lines.size(); ++i){
    if ((Int_t)*(LegendLabelL.at(i).Data())) leg->AddEntry(lines.at(i), LegendLabelL.at(i).Data(),"l");
  }

  for(  Int_t i = 0; i < (Int_t)Latex.size(); ++i) Latex.at(i)->Draw("same");
  //----------------------------------------------------------------------------
  //  Now that everything is drawn just add the legend and print it.

  leg->Draw("same");
  Canvas->SaveAs(name);
  delete hDummy;
  delete Canvas;
}

void Plotting1D::NewHist(TH1F* h, TString label, Int_t style, Int_t size, Int_t color, TString opt){

  if(!h) Abort("NewHist was given a Nullptr.");

  hists.push_back(h);
  LegendLabel.push_back(label);
  //  Lines and curves are only drawn as such when you add "hist" to the DrawOption
  DrawOption.push_back( (opt == "l" || opt == "c") ? opt+" hist" : opt );
  h->SetStats(0);

  h->SetMarkerStyle(( style == -1) ? AutoStyle[counter] : style);
  //  LineStyles > 10 make root crash. If its not drawn in hist style, the errors are the only lines and should be style 1
  h->SetLineStyle( (opt.Contains("h") && style < 10) ? style : 1 );
  h->SetMarkerColor(( color == -1) ? AutoColor[counter] : color);
  h->SetLineColor(( color == -1) ? AutoColor[counter] : color);
  h->SetMarkerSize(size);
  h->SetLineWidth(size);

  counter++;  //  Make sure the next histogram has different colors and styles
}

void Plotting1D::NewHist(TH1D* h, TString label, Int_t style, Int_t size, Int_t color, TString opt){
  TH1F* hd = (TH1F*)h;
  NewHist(hd, label, style, size, color, opt);
}

void Plotting1D::NewFunc(TF1* f, TString label, Int_t style, Int_t size, Int_t color, TString opt){

  if(!f) Abort("NewFunc was given a Nullptr.");

  funcs.push_back(f);
  LegendLabelF.push_back(label);
  DrawOptionF.push_back(opt);

  f->SetMarkerStyle(( style == -1) ? AutoStyle[counter] : style);
  f->SetLineStyle(( style == -1) ? AutoStyleLine[counter] : style);
  f->SetMarkerColor(( color == -1) ? AutoColor[counter] : color);
  f->SetLineColor(( color == -1) ? AutoColor[counter] : color);
  f->SetMarkerSize(size);
  f->SetLineWidth(size);

  counter++;
}

void Plotting1D::NewGraph(TGraph* g, TString label, Int_t style, Int_t size, Int_t color, TString opt){

  if(!g) Abort("NewGraph was given a Nullptr.");

  graphs.push_back(g);
  LegendLabelG.push_back(label);
  DrawOptionG.push_back(opt);

  g->SetMarkerStyle(( style == -1) ? AutoStyle[counter] : style);
  g->SetLineStyle( (opt.Contains("l") && style < 10) ? style : 1 );
  g->SetMarkerColor(( color == -1) ? AutoColor[counter] : color);
  g->SetLineColor(( color == -1) ? AutoColor[counter] : color);
  g->SetMarkerSize(size);
  g->SetLineWidth(size);

  counter++;
}

void Plotting1D::InitializeCanvas(Bool_t logx, Bool_t logy){

  if(Canvas) delete Canvas; //  This should never happen, but better safe than sorry.

  Canvas = new TCanvas("Canvas", "Canvas", CanvasDimensions[0], CanvasDimensions[1]);
  Canvas->SetLeftMargin(CanvasMargins[0][0]);
  Canvas->SetRightMargin(CanvasMargins[0][1]);
  Canvas->SetBottomMargin(CanvasMargins[1][0]);
  Canvas->SetTopMargin(CanvasMargins[1][1]);

  //  Set ticks at regular intervals on every edge of the histogram (also right and top)
  gPad->SetTickx();
  gPad->SetTicky();

  Canvas->cd();
  Canvas->SetLogx(logx);
  Canvas->SetLogy(logy);
}

void Plotting1D::SetAxisLabel(TString labelx, TString labely, Double_t offsetx , Double_t offsety){
  AxisLabel[0] = labelx;
  AxisLabel[1] = labely;
  AxisLabelOffset[0] = offsetx;
  AxisLabelOffset[1] = offsety;
}

void Plotting1D::InitializeAxis(Bool_t logy){

  if(hDummy) delete hDummy;

  AutoSetAxisRanges(logy);  //  If any AxisRanges are still set to 42 -> Autoset them

  hDummy = new TH2D("hDummy", "hDummy", 1000, AxisRange[0][0], AxisRange[0][1], 1000, AxisRange[1][0], AxisRange[1][1]);
  hDummy->SetTitle("");
  hDummy->SetStats(0);

  hDummy->GetXaxis()->SetTitle(AxisLabel[0]);
  hDummy->GetYaxis()->SetTitle(AxisLabel[1]);
  hDummy->GetYaxis()->SetTitleFont(62);
  hDummy->GetXaxis()->SetTitleFont(62);
  hDummy->GetXaxis()->SetTitleOffset(AxisLabelOffset[0]);
  hDummy->GetYaxis()->SetTitleOffset(AxisLabelOffset[1]);
  hDummy->GetYaxis()->SetMaxDigits(3);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++ Plotting 2D ++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class Plotting2D : public Plotting{
  public:

    Plotting2D(); // This empty constructor always has to be called when plotting 2D

    ~Plotting2D();  //  Destructor

    //  After adding the histograms using the NewHist function create the actual plot
    void Plot(TString name = "dummy.pdf", Bool_t logx = false, Bool_t logy = false, Bool_t logz = false, Int_t numcontours = 100);

    //  The standard palette is kBird, but there are also other nice 2D plotting styles (https://root.cern.ch/doc/master/classTColor.html)
    void NewHist(TH2F* h = nullptr, TString opt = "COLZ", Int_t palette = kBird);
    void NewHist(TH2D* h = nullptr, TString opt = "COLZ", Int_t palette = kBird);  //  Convert to TH2F and call that NewHist function

    //  Add a new function to the funcs vector that will be drawn when calling Plot()
    void NewFunc(TF1* f = nullptr, TString label = "", Int_t style = -1, Int_t size = 1, Int_t color = -1, TString opt = "l");

    void SetAxisLabel(TString labelx = "", TString labely = "", Double_t offsetx = 1., Double_t offsety = 1.);

  protected:

    TH2F* hist = NULL;

    void InitializeCanvas(Bool_t logx, Bool_t logy, Bool_t logz);

    void InitializeAxis();

};

Plotting2D::Plotting2D(){

}

Plotting2D::~Plotting2D(){

}

void Plotting2D::Plot(TString name, Bool_t logx, Bool_t logy, Bool_t logz, Int_t numcontours){

  if(!hist) Abort("No hist added for plotting.");

  InitializeCanvas(logx, logy, logz); //Creating Canvas with margins
  InitializeAxis();
  InitializeLegend();
  gStyle->SetNumberContours(numcontours);

  hist->Draw(Form("same,%s", ((TString) DrawOption.at(0)).Data()));

  for( Int_t i = 0; i < (Int_t)funcs.size(); ++i){
    funcs.at(i)->Draw(Form("same %s", ((TString) DrawOptionF.at(i)).Data()));
    if ((Int_t)*(LegendLabelF.at(i).Data())) leg->AddEntry(funcs.at(i), LegendLabelF.at(i).Data(), (DrawOptionF.at(i).Contains("l") || DrawOptionF.at(i).Contains("hist") || DrawOptionF.at(i).Contains("C") ) ? "l" : "p");
  } //  Dont add anything to the legend if LegendLabelF is empty

  for( Int_t i = 0; i < (Int_t)Latex.size(); ++i) Latex.at(i)->Draw("same");

  for( Int_t i = 0; i < (Int_t)lines.size(); ++i) lines.at(i)->Draw("same");

  leg->Draw("same");

  Canvas->SaveAs(name);
  delete Canvas;
}

void Plotting2D::NewHist(TH2F* h, TString opt, Int_t palette){
  if(!h) Abort("NewHist was given a Nullptr.");
  hist = h;
  gStyle->SetPalette(palette);
  DrawOption.push_back(( opt == "p") ? "p" : opt);
}

void Plotting2D::NewHist(TH2D* h, TString opt, Int_t palette){
  TH2F* hd = (TH2F*)h;
  NewHist(hd, opt,palette);
}

void Plotting2D::NewFunc(TF1* f, TString label, Int_t style, Int_t size, Int_t color, TString opt){

  if(!f) Abort("NewFunc was given a Nullptr.");

  funcs.push_back(f);
  LegendLabelF.push_back(label);
  DrawOptionF.push_back(opt);

  f->SetMarkerStyle(( style == -1) ? AutoStyle[counter] : style);
  f->SetLineStyle(( style == -1) ? AutoStyleLine[counter] : style);
  f->SetMarkerColor(( color == -1) ? AutoColor[counter] : color);
  f->SetLineColor(( color == -1) ? AutoColor[counter] : color);
  f->SetMarkerSize(size);
  f->SetLineWidth(size);

  counter++;
}

void Plotting2D::InitializeCanvas(Bool_t logx, Bool_t logy, Bool_t logz){

  if(Canvas) delete Canvas; //  This should never happen, but better safe than sorry.

  Canvas = new TCanvas("Canvas", "Canvas", CanvasDimensions[0], CanvasDimensions[1]);
  Canvas->SetLeftMargin(CanvasMargins[0][0]);
  Canvas->SetRightMargin(1.2*CanvasMargins[0][1]);  //  To leave room for the z axis
  Canvas->SetBottomMargin(CanvasMargins[1][0]);
  Canvas->SetTopMargin(CanvasMargins[1][1]);

  //  Set ticks at regular intervals on every edge of the histogram (also right and top)
  gPad->SetTickx();
  gPad->SetTicky();
  gStyle->SetOptStat(0);

  Canvas->cd();
  Canvas->SetLogx(logx);
  Canvas->SetLogy(logy);
  Canvas->SetLogz(logz);
}

void Plotting2D::SetAxisLabel(TString labelx, TString labely, Double_t offsetx , Double_t offsety){
  AxisLabel[0] = labelx;
  AxisLabel[1] = labely;
  AxisLabelOffset[0] = offsetx;
  AxisLabelOffset[1] = offsety;
}

void Plotting2D::InitializeAxis(){
  hist->GetXaxis()->SetRangeUser(AxisRange[0][0], AxisRange[0][1]);
  hist->GetYaxis()->SetRangeUser(AxisRange[1][0], AxisRange[1][1]);
  //  Only set z axis if it has been manually changed from 0,2 (standard)
  if( !(AxisRange[2][0] > -0.001 && AxisRange[2][0] < 0.001 && AxisRange[2][1] > 1.99 && AxisRange[2][1] < 2.001) ) hist->GetZaxis()->SetRangeUser(AxisRange[2][0], AxisRange[2][1]);
  hist->SetStats(0);

  hist->SetTitle("");
  hist->GetXaxis()->SetTitle(AxisLabel[0]);
  hist->GetYaxis()->SetTitle(AxisLabel[1]);
  hist->GetYaxis()->SetTitleFont(62);
  hist->GetXaxis()->SetTitleFont(62);
  hist->GetXaxis()->SetTitleOffset(AxisLabelOffset[0]);
  hist->GetYaxis()->SetTitleOffset(AxisLabelOffset[1]);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++ Plotting Ratio +++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class PlottingRatio : public Plotting{
  public:

    PlottingRatio();  // This empty constructor always has to be called when plotting Ratio

    ~PlottingRatio(); //  Destructor

    void Plot(TString name = "dummy.pdf", Bool_t logx = false, Bool_t logy = false, Bool_t logz = false);

    //  Add histograms to the upper pad
    void NewHist(TH1F* h = nullptr, TString label = "", Int_t style = -1, Int_t size = 1, Int_t color = -1, TString opt = "p");
    void NewHist(TH1D* h = nullptr, TString label = "", Int_t style = -1, Int_t size = 1, Int_t color = -1, TString opt = "p");

    //  Add histograms to the lower pad
    void NewRatio(TH1F* h = nullptr, TString label = "", Int_t style = -1, Int_t size = 1, Int_t color = -1, TString opt = "p");
    void NewRatio(TH1D* h = nullptr, TString label = "", Int_t style = -1, Int_t size = 1, Int_t color = -1, TString opt = "p");

    //  To remove the label conflict where y and ratio axis meet, add a white box there. This function can move that box (e.g. when margins are changed) or set to red to visualize the pad.
    void SetWhite(Double_t low, Double_t left, Double_t up, Double_t right, Bool_t red = false);

    //  The ratios have a seperate legend. It's position can be set analogously to SetLegend using this function
    void SetLegendR(Double_t x1 = 0.7, Double_t x2 = 0.95, Double_t y1 = 0.15, Double_t y2 = 0.25);

    //  The ratio label can be set indiviually, but its offset is set via the y axis label offset
    void SetAxisLabel(TString labelx = "", TString labely = "", TString labelz = "", Double_t offsetx = 1., Double_t offsety = 1.);

    //  Add functions to the tfuncs and bfuncs vector, which are added to the top and bottom pad when drawing in Plot()
    void NewTopFunc(TF1* h = nullptr, TString label = "", Int_t style = -1, Int_t size = 1, Int_t color = -1, TString opt = "l");
    void NewBotFunc(TF1* h = nullptr, TString label = "", Int_t style = -1, Int_t size = 1, Int_t color = -1, TString opt = "l");

  protected:

    //  Add the pads that are drawn on the canvas in Plot() as attributes
    TPad *HistoPad = nullptr; //  The upper pad containing all hists added by NewHist and TopFuncs
    TPad *RatioPad = nullptr; //  The lower pad containing all ratios added by NewRatio and BotFuncs
    TPad *WhitePad = nullptr; //  A white filled rectangle hiding the y axis label conflict at their border

    Double_t WhiteBorders[2][2] = {{0.04,0.095},{0.32,0.35}}; //  Defines where the white pad is drawn to hide the label conflict
    Bool_t wred = false;  //  Can be set to true to make WhitePad red (visible)

    TH2D* rDummy = nullptr; //Dummy for the ratio histogram analogously to hdummy
    TLegend *legR = nullptr;
    Double_t RatioLegendBorders[2][2] = {{0.7,0.95},{0.15,0.25}};

    //  Vectors containing the functions for the upper pad as well as the functions and ratios for the lower pad
    std::vector<TH1F*> ratios;
    std::vector<TF1*> tfuncs;
    std::vector<TF1*> bfuncs;
    std::vector<TString> LegendLabelR;  //  Ratio
    std::vector<TString> LegendLabelFt; //  FunctionTop
    std::vector<TString> LegendLabelFb; //  FunctionBot
    std::vector<TString> DrawOptionR;
    std::vector<TString> DrawOptionFt;
    std::vector<TString> DrawOptionFb;

    //  The bottom pad has its own counter because it can and often should have similar colors as the top.
    //  Starts at one, because there is often no ratio to standard, so now the top histos have the same colors and styles as their bot ratios.
    Int_t counterR = 1;

    //  Creates all three pads and the canvas that they are on. The canvas dimension attributes are NOT used, instead a standard size of 1000x1000 is used
    void InitializeCanvas(Bool_t logx, Bool_t logy, Bool_t logz);

    void InitializeAxis(Bool_t logy);

    void InitializeLegendR(); //  Creates the legR and sets its coordinates according to RatioLegendBorders

};

PlottingRatio::PlottingRatio(){

}

PlottingRatio::~PlottingRatio(){

}

void PlottingRatio::Plot(TString name, Bool_t logx, Bool_t logy, Bool_t logz){

  if(hists.size() < 1) Abort("No hists added for plotting.");
  if(ratios.size() < 1) Abort("No ratios added for plotting.");

  InitializeCanvas(logx, logy, logz); //Creating Canvas with margins
  InitializeAxis(logy);
  hDummy->Draw();

  InitializeLegend();
  InitializeLegendR();

  //  Print all hists and top funcs on the HistoPad
  //----------------------------------------------------------------------------
  for( Int_t i = 0; i < (Int_t)hists.size(); ++i){
    hists.at(i)->Draw(Form("same %s", ((TString) DrawOption.at(i)).Data()));
    if ((Int_t)*(LegendLabel.at(i).Data())) leg->AddEntry(hists.at(i), LegendLabel.at(i).Data(), LegendDrawOption(DrawOption.at(i)));
  }

  for( Int_t i = 0; i < (Int_t)tfuncs.size(); ++i){
    tfuncs.at(i)->Draw(Form("same %s", ((TString) DrawOptionFt.at(i)).Data()));
    if ((Int_t)*(LegendLabelFt.at(i).Data())) leg->AddEntry(tfuncs.at(i), LegendLabelFt.at(i).Data(), (DrawOptionFt.at(i).Contains("l") || DrawOptionFt.at(i).Contains("hist") || DrawOptionFt.at(i).Contains("C") ) ? "l" : "p");
  }
  //----------------------------------------------------------------------------
  //  The upper pad is now filled. Create and cd to the lower pad now

  Canvas->cd();
  RatioPad->Draw();
  RatioPad->cd();
  gPad->SetTickx();
  gPad->SetTicky();
  RatioPad->SetLogx(logx);
  RatioPad->SetLogy(logz);
  rDummy->Draw();

  //  Print all ratios, bot funcs and lines on the HistoPad
  //----------------------------------------------------------------------------
  for( Int_t i = 0; i < (Int_t)ratios.size(); ++i){
    ratios.at(i)->Draw(Form("same %s", ((TString) DrawOptionR.at(i)).Data()));
    if ((Int_t)*(LegendLabelR.at(i).Data())) legR->AddEntry(ratios.at(i), LegendLabelR.at(i).Data(), (DrawOptionR.at(i).Contains("l") || DrawOptionR.at(i).Contains("hist") ) ? "l" : "p");
  }

  for( Int_t i = 0; i < (Int_t)bfuncs.size(); ++i){
    bfuncs.at(i)->Draw(Form("same %s", ((TString) DrawOptionFb.at(i)).Data()));
    if ((Int_t)*(LegendLabelFb.at(i).Data())) legR->AddEntry(bfuncs.at(i), LegendLabelFb.at(i).Data(), (DrawOptionFb.at(i).Contains("l") || DrawOptionFb.at(i).Contains("hist") || DrawOptionFb.at(i).Contains("C") ) ? "l" : "p");
  }

  //  Lines are always drawn on the ratio pad, because they are almost exclusively needed there (e.g. line marking ratio 1)
  for( Int_t i = 0; i < (Int_t)lines.size(); ++i) lines.at(i)->Draw("same");
  //----------------------------------------------------------------------------
  //  Both pads are now filled. Create the white rectangle hiding the axis label conflict now

  Canvas->cd();
  if(wred) WhitePad->SetFillColor(kRed);
  WhitePad->Draw(); //  To eliminate the label conflict on y axis where plots meet

  Canvas->cd(); //  cd back to canvas in order to draw legend and Latex over entire canvas in relative coordinates
  Canvas->Update();

  leg->Draw("same");
  legR->Draw("same");

  for(  Int_t i = 0; i < (Int_t)Latex.size(); ++i) Latex.at(i)->Draw("same");

  Canvas->SaveAs(name);
  delete hDummy;
  delete rDummy;
  delete Canvas;
}

void PlottingRatio::NewHist(TH1F* h, TString label, Int_t style, Int_t size, Int_t color, TString opt){

  if(!h) Abort("NewHist was given a Nullptr.");

  hists.push_back(h);
  LegendLabel.push_back(label);
  //  Lines and curves are only drawn as such when you add "hist" to the DrawOption
  DrawOption.push_back( (opt == "l" || opt == "c") ? opt+" hist" : opt );
  h->SetStats(0);

  h->SetMarkerStyle(( style == -1) ? AutoStyle[counter] : style);
  //  LineStyles > 10 make root crash. If its not drawn in hist style, the errors are the only lines and should be style 1
  h->SetLineStyle( (opt.Contains("h") && style < 10) ? style : 1 );
  h->SetMarkerColor(( color == -1) ? AutoColor[counter] : color);
  h->SetLineColor(( color == -1) ? AutoColor[counter] : color);
  h->SetMarkerSize(size);
  h->SetLineWidth(size);

  if((style == -1) && (color == -1) ) counter++;  //  Only count up, when Auto has been used -> Don't skip all the good colors
}

void PlottingRatio::NewHist(TH1D* h, TString label, Int_t style, Int_t size, Int_t color, TString opt){
  TH1F* hd = (TH1F*)h;
  NewHist(hd, label, style, size, color, opt);
}

void PlottingRatio::NewRatio(TH1F* h, TString label, Int_t style, Int_t size, Int_t color, TString opt){

  if(!h) Abort("NewHist was given a Nullptr.");

  ratios.push_back(h);
  LegendLabelR.push_back(label);
  //  Lines and curves are only drawn as such when you add "hist" to the DrawOption
  DrawOptionR.push_back( (opt == "l" || opt == "c") ? opt+" hist" : opt );

  h->SetStats(0);
  h->SetMarkerStyle(( style == -1) ? AutoStyle[counterR] : style);
  //  LineStyles > 10 make root crash. If its not drawn in hist style, the errors are the only lines and should be style 1
  h->SetLineStyle( (opt.Contains("h") && style < 10) ? style : 1 );
  h->SetMarkerColor(( color == -1) ? AutoColor[counterR] : color);
  h->SetLineColor(( color == -1) ? AutoColor[counterR] : color);
  h->SetMarkerSize(size);
  h->SetLineWidth(size);

  if((style == -1) && (color == -1) ) counterR++;
}

void PlottingRatio::NewRatio(TH1D* h, TString label, Int_t style, Int_t size, Int_t color, TString opt){
  TH1F* hd = (TH1F*)h;
  NewRatio(hd, label, style, size, color, opt);
}


void PlottingRatio::NewTopFunc(TF1* f, TString label, Int_t style, Int_t size, Int_t color, TString opt){

  if(!f) Abort("NewTopFunc was given a Nullptr.");

  tfuncs.push_back(f);
  LegendLabelFt.push_back(label);
  DrawOptionFt.push_back(opt);

  f->SetMarkerStyle(( style == -1) ? AutoStyle[counter] : style);
  f->SetLineStyle(( style == -1) ? AutoStyleLine[counter] : style);
  f->SetMarkerColor(( color == -1) ? AutoColor[counter] : color);
  f->SetLineColor(( color == -1) ? AutoColor[counter] : color);
  f->SetMarkerSize(size);
  f->SetLineWidth(size);

  counter++;
}

void PlottingRatio::NewBotFunc(TF1* f, TString label, Int_t style, Int_t size, Int_t color, TString opt){

  if(!f) Abort("NewBotFunc was given a Nullptr.");

  bfuncs.push_back(f);
  LegendLabelFb.push_back(label);
  DrawOptionFb.push_back(opt);

  f->SetMarkerStyle(( style == -1) ? AutoStyle[counter] : style);
  f->SetLineStyle(( style == -1) ? AutoStyleLine[counter] : style);
  f->SetMarkerColor(( color == -1) ? AutoColor[counter] : color);
  f->SetLineColor(( color == -1) ? AutoColor[counter] : color);
  f->SetMarkerSize(size);
  f->SetLineWidth(size);

  counter++;
}

void PlottingRatio::SetAxisLabel(TString labelx, TString labely, TString labelz, Double_t offsetx , Double_t offsety){
  AxisLabel[0] = labelx;
  AxisLabel[1] = labely;
  AxisLabel[2] = labelz;
  AxisLabelOffset[0] = offsetx;
  AxisLabelOffset[1] = offsety;
}

void PlottingRatio::InitializeAxis(Bool_t logy){

  if(hDummy) delete hDummy;

  AutoSetAxisRanges(logy);

  hDummy = new TH2D("hDummy", "hDummy", 1000, AxisRange[0][0], AxisRange[0][1], 1000, AxisRange[1][0], AxisRange[1][1]);
  hDummy->SetTitle("");
  hDummy->SetStats(0);

  if(rDummy) delete rDummy; //  This should never happen, but better safe than sorry.

  Double_t max = ratios.at(0)->GetMaximum();
  Double_t min = 0; //  Ratios can often start at 0.
  //  Find the smalles and largest bin in all loaded ratios
  for ( Int_t i = 0; i < (Int_t)ratios.size(); i++) {
    max = ratios.at(i)->GetMaximum() > max ? ratios.at(i)->GetMaximum() : max;
    min = ratios.at(i)->GetMinimum() < min ? ratios.at(i)->GetMinimum() : min;
  }

  //  Leave room between the highest bin and the upper pad
  max = max+(max-min)/10;

  //  If the respective range was set to 42 use the just calculated estimates
  if (AxisRange[2][0] > 41.99 && AxisRange[2][0] < 42.01) AxisRange[2][0] = min;
  if (AxisRange[2][1] > 41.99 && AxisRange[2][1] < 42.01) AxisRange[2][1] = max;

  rDummy = new TH2D("rDummy", "rDummy", 1000, AxisRange[0][0], AxisRange[0][1], 1000, AxisRange[2][0], AxisRange[2][1]);
  rDummy->SetTitle("");
  rDummy->SetStats(0);

  Double_t labelandtitlesize = 0.04;  //  Labels and titles can use the same size
  hDummy->GetYaxis()->SetLabelSize(labelandtitlesize);
  hDummy->GetYaxis()->SetTitleSize(labelandtitlesize);
  hDummy->GetYaxis()->SetTitle(AxisLabel[1]);
  hDummy->GetXaxis()->SetTitle("");
  hDummy->GetYaxis()->SetTitleFont(62);
  hDummy->GetXaxis()->SetTitleFont(62);
  hDummy->GetXaxis()->SetTitleOffset(AxisLabelOffset[0]);
  hDummy->GetYaxis()->SetTitleOffset(AxisLabelOffset[1]);

  //  Since the ratio pad is only one third the size, its labels have to be scaled up to be the same size as the histo labels
  rDummy->GetXaxis()->SetLabelSize(labelandtitlesize*1.7);
  rDummy->GetYaxis()->SetLabelSize(labelandtitlesize*1.7);
  rDummy->GetXaxis()->SetTitleSize(labelandtitlesize*2);
  rDummy->GetYaxis()->SetTitleSize(labelandtitlesize*2);
  rDummy->GetYaxis()->SetNdivisions(8);
  rDummy->GetXaxis()->SetTitle(AxisLabel[0]);
  rDummy->GetYaxis()->SetTitle(AxisLabel[2]);
  rDummy->GetYaxis()->SetTitleFont(62);
  rDummy->GetXaxis()->SetTitleFont(62);
  rDummy->GetYaxis()->SetTitleOffset(AxisLabelOffset[1]/2.);
  rDummy->GetXaxis()->SetTitleOffset(AxisLabelOffset[0]);
}

void PlottingRatio::SetWhite(Double_t low, Double_t left, Double_t up, Double_t right, Bool_t red){
  WhiteBorders[0][0] = left;
  WhiteBorders[0][1] = right;
  WhiteBorders[1][0] = low;
  WhiteBorders[1][1] = up;
  wred = red;
}

void PlottingRatio::InitializeCanvas(Bool_t logx, Bool_t logy, Bool_t logz){

  if(Canvas) delete Canvas; //  This should never happen, but better safe than sorry.

  Canvas = new TCanvas("Canvas", "Canvas", 1000, 1000);

  HistoPad = new TPad("HistoPad", "HistoPad", 0.0, 1.0/3.0, 1, 1);
  RatioPad = new TPad("RatioPad", "RatioPad", 0.0, 0.0, 1, 1.0/3.0);
  WhitePad = new TPad("WhitePad", "WhitePad", WhiteBorders[0][0], WhiteBorders[1][0], WhiteBorders[0][1], WhiteBorders[1][1]);

  HistoPad->SetTopMargin(CanvasMargins[1][1]);
  HistoPad->SetRightMargin(CanvasMargins[0][1]);
  HistoPad->SetLeftMargin(CanvasMargins[0][0]);
  HistoPad->SetBottomMargin(0);
  RatioPad->SetTopMargin(0);
  RatioPad->SetRightMargin(CanvasMargins[0][1]);
  RatioPad->SetLeftMargin(CanvasMargins[0][0]);
  RatioPad->SetBottomMargin(CanvasMargins[1][0]*2);

  Canvas->cd();
  HistoPad->Draw();
  HistoPad->cd();

  gPad->SetTickx();
  gPad->SetTicky();

  HistoPad->SetLogy(logy);
  HistoPad->SetLogx(logx);
}

void PlottingRatio::SetLegendR(Double_t x1, Double_t x2, Double_t y1, Double_t y2){
  RatioLegendBorders[0][0] = x1;
  RatioLegendBorders[0][1] = x2;
  RatioLegendBorders[1][0] = y1;
  RatioLegendBorders[1][1] = y2;
}


void PlottingRatio::InitializeLegendR(){
  legR = new TLegend(RatioLegendBorders[0][0], RatioLegendBorders[1][0], RatioLegendBorders[0][1], RatioLegendBorders[1][1]);
  legR->SetHeader("");
  legR->SetTextFont(42);
  legR->SetTextSize(0.6*0.035);
  legR->SetBorderSize(0);
  legR->SetFillStyle(1001);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++ Plotting Paint +++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class PlottingPaint : public Plotting{

  public:

    //  Using this class one can draw ellipses, angles, lines and curly lines
    void Plot(TString name = "You_forgot_the_name_..._dummy.pdf");

    void SetCanvas(Double_t cw = 1200, Double_t ch = 1200);

    //  Draw an incomplete ellipse, emulating an angle for example to draw a particle decay angle
    void NewAngle(Double_t x = 0.5, Double_t y = 0.5, Double_t r1 = 0.3, Double_t r2 = 0.2,
                  Double_t phimin = 60, Double_t phimax = 120, Double_t theta = 0);

  protected:

    std::vector<TEllipse*> angles;

    void InitializeCanvas();

};

void PlottingPaint::Plot(TString name){

  InitializeCanvas(); //  Creating Canvas with margins

  for( Int_t i = 0; i < (Int_t)angles.size(); ++i) angles.at(i)->Draw("same");

  for( Int_t i = 0; i < (Int_t)lines.size(); ++i) lines.at(i)->Draw("same");

  for( Int_t i = 0; i < (Int_t)clines.size(); ++i) clines.at(i)->Draw("same");

  for( Int_t i = 0; i < (Int_t)Latex.size(); ++i) Latex.at(i)->Draw("same");

  Canvas->SaveAs(name);
  delete Canvas;
}

void PlottingPaint::NewAngle(Double_t x, Double_t y, Double_t r1, Double_t r2, Double_t phimin, Double_t phimax , Double_t theta){

  TEllipse* angle = new TEllipse(x,y,r1,r2,phimin,phimax,theta);
  angle->SetNoEdges();
  angles.push_back(angle);
}

void PlottingPaint::InitializeCanvas(){

  if(Canvas) delete Canvas; //  This should never happen, but better safe than sorry.

  Canvas = new TCanvas("Canvas", "Canvas", CanvasDimensions[0], CanvasDimensions[1]);
  Canvas->cd();
}

#endif
