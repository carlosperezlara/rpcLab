const int MAX=1000;

void primeit(
	     double x[MAX], double y[MAX],
	     double xp[MAX], double yp[MAX],
	     int n, bool norm=false) {
  for(int i=1; i!=n-1; ++i) {
    double den = x[i+1] - x[i-1];
    double num = y[i+1] - y[i-1];
    xp[i-1] = x[i];
    yp[i-1] = num/den;
    if(norm) yp[i-1] /= y[i];
    //std::cout << x[i] << " " << y[i] << "|" << xp[i-1] << " " << yp[i-1] << std::endl;
  }
  return;
}

void primeit(
	     TSpline *sp, double x[MAX], double y[MAX],
	     double xp[MAX], double yp[MAX],
	     int n, bool norm=false) {
  for(int i=1; i!=n-1; ++i) {
    double pre = 0.5*( x[i] + x[i-1] );
    double pos = 0.5*( x[i+1] + x[i] );
    double den = pos-pre;
    double num = sp->Eval(pos) - sp->Eval(pre);
    xp[i-1] = x[i];
    yp[i-1] = num/den;
    if(norm) yp[i-1] /= y[i];
    //std::cout << x[i] << " " << y[i] << "|" << xp[i-1] << " " << yp[i-1] << std::endl;
  }
  return;
}

double findLowEdge( double xval, double x[MAX], int size, bool reversed=false ) {
  int ret = -999;
  if(!reversed) {
    for(int i=0; i!=size; ++i)
      if(xval>x[i]) {
	ret = i;
      }
  } else {
    for(int i=size-1; i>=0; --i)
      if(xval<x[i]) {
	ret = i;
      }
  }
  return ret;
}

double findHighEdge( double xval, double x[MAX], int size ) {
  return findLowEdge(xval,x,size,true);
}

double threshold(
		 int xstart, int xstop,
		 double y[MAX], double thr,
		 bool rising, bool reversed) {
  int ret = -999;
  if(!reversed) {
    for(int i=xstart; i<=xstop; ++i)
      if(rising&&y[i]>thr) {
	ret = i;
	break;
      } else if((!rising)&&(y[i]<thr)) {
	ret = i;
	break;
      }
  } else {
    for(int i=xstop; i>=xstart; --i)
      if(rising&&y[i]>thr) {
	ret = i;
	break;
      } else if((!rising)&&(y[i]<thr)) {
	ret = i;
	break;
      }
  }
  return ret;
}

double rising(
	      int xstart, int xstop,
	      double y[MAX], double thr, bool reversed=false ) {
  return threshold( xstart, xstop, y, thr, true, reversed);
}

int falling(
	       int xstart, int xstop,
	       double y[MAX], double thr, bool reversed=false ) {
  return threshold( xstart, xstop, y, thr, false, reversed);
}

int peak(
	 int xstart, int xstop,
	 double y[MAX], double &max) {
  int ret = -999;
  max = -999;
  for(int i=xstart; i<xstop; ++i)
    if(y[i]>max) {
      max = y[i];
      ret = i;
    }
  return ret;
}

int floor(
	 int xstart, int xstop,
	 double y[MAX], double &min) {
  double ret = 999;
  min = 999;
  for(int i=xstart; i<xstop; ++i)
    if(y[i]<min) {
      min = y[i];
      ret = i;
    }
  return ret;
}

int locatePointBelowZero(
			 int xstart, int xstop,
			 double y[MAX], int times) {
  double ret = xstart;
  int found = 0;
  for(int i=xstart; i<xstop; ++i) {
    if(y[i]<0) {
      found++;
      ret = i;
    }
    if(found==times) break;
  }
  if(found==times) return ret;
  return 999;
}

int compress(int n, double x[MAX], double y[MAX], double xx[MAX], double yy[MAX], double thr=0.1) {
  int nn = 0;
  for(int i=0; i<n; ++i) {
    xx[nn] = x[i];
    yy[nn] = y[i];
    if(i<n-1)
      if(x[i+1]-x[i]<thr) { // less than 100 mV -> merge
	xx[nn] = 0.5*(x[i] + x[i+1]);
	yy[nn] = 0.5*(y[i] + y[i+1]);
	i++;
      }
    nn++;
  }
  return nn;
}


int bvfinder(TString file="20200804_20.8C/HDR2-_Ch4_iLED-1-20200804-1128.csv", //20.8C looks fine (no lights)
	     double merge=0.0005, double thrPrime=0.5, double rangeend=-1, bool gausInstead = false ) {
  gStyle->SetOptStat(0);
  int n=0;
  double volt[MAX]; //original
  double curr[MAX]; //original
  double volt0[MAX]; //compressed
  double curr0[MAX]; //compressed
  double volt1[MAX]; // primed normalized
  double curr1[MAX]; // primed normalized
  double volt2[MAX]; // doublePrimed
  double curr2[MAX]; // doublePrimed
  double sqrtc[MAX];
  
  ///////
  // my clumsy way of extracting data from a csv file
  TString outfilestring = Form("outputfiles/%s",file.Data());
  std::ifstream fin( outfilestring.Data() );
  std::string line;
  int indexAt15 = 0;
  for(;;n++) {
    if(!std::getline(fin,line)) break;
    if(n>MAX) break;
    TString rline = line;
    TObjArray *lst = rline.Tokenize(",");
    volt[n] =  ((TObjString*) lst->At(0))->GetString().Atof();
    curr[n] =  ((TObjString*) lst->At(1))->GetString().Atof()*1e6; // move to uA
    if(volt[n]<15) indexAt15 = n;
  }
  std::cout << "I read " << n << " pairs. My MAX stack was " << MAX << std::endl;
  // compressing
  int nn = 0;
  double xtemp1[MAX], xtemp2[MAX];
  double ytemp1[MAX], ytemp2[MAX];
  nn = compress(n,volt,curr,xtemp1,ytemp1,merge);
  std::cout << "**** nn=" << nn << std::endl;
  nn = compress(nn,xtemp1,ytemp1,xtemp2,ytemp2,merge);
  std::cout << "**** nn=" << nn << std::endl;
  nn = compress(nn,xtemp2,ytemp2,xtemp1,ytemp1,merge);
  std::cout << "**** nn=" << nn << std::endl;
  nn = compress(nn,xtemp1,ytemp1,xtemp2,ytemp2,merge);
  std::cout << "**** nn=" << nn << std::endl;
  nn = compress(nn,xtemp2,ytemp2,xtemp1,ytemp1,merge);
  std::cout << "**** nn=" << nn << std::endl;
  nn = compress(nn,xtemp1,ytemp1,xtemp2,ytemp2,merge);
  std::cout << "**** nn=" << nn << std::endl;
  nn = compress(nn,xtemp2,ytemp2,volt0,curr0,merge);
  std::cout << "**** nn=" << nn << std::endl;
  // transforming
  for(int i=0; i!=n; ++i) {
    curr[i] *= 1e3; // move to nA
  }
  for(int i=0; i!=nn; ++i) {
    sqrtc[i] =  TMath::Sqrt(curr0[i]); // sqrt of current
    curr0[i] *= 1e3; // move to nA
  }
  ///////
  
  //primeit(volt, curr, volt1,curr1,n,true);
  //primeit(volt1,curr1,volt2,curr2,n-2);
  
  // create graphical objects
  TGraph  *gr0  = new TGraph(n,volt,curr);
  TGraph  *gr00 = new TGraph(nn,volt0,curr0);
  TSpline *sp00 = new TSpline3("sp00",gr00);
  primeit(sp00,volt0,curr0,volt1,curr1,nn,true);
  TGraph  *gr1  = new TGraph(nn-2,volt1,curr1);
  TSpline *sp1  = new TSpline3("sp1",gr1);
  primeit(sp1,volt1,curr1,volt2,curr2,nn-2);
  TGraph  *gr2  = new TGraph(nn-4,volt2,curr2);
  TSpline *sp2  = new TSpline3("sp2",gr2);
  TGraph  *gr3  = new TGraph(nn,volt0,sqrtc);
  
  // finding first peak of primed
  int skipfirst = indexAt15;
  int xini, xend, xpea;
  /////////////////////
  if(rangeend<0) {
    std::cout << "*** Trying to get the range automatically ***" << std::endl;
    std::cout << "start analysis from index " << skipfirst << " with threshold " << thrPrime << std::endl;
    xini = rising(skipfirst,nn-1,curr1,thrPrime);
    if(xini==-999) {
      std::cout << " * ERROR no rising crossing found for " << thrPrime << std::endl;
      exit(0);
    }
    xend = falling(xini,nn-1,curr1,thrPrime);
    if(xend==-999) {
      std::cout << " *ERROR no falling crossing found for " << thrPrime << std::endl;
      exit(0);
    }
    std::cout << "Starting range from " << xini << ": " << volt1[xini] << " and " << xend << ": " << volt1[xend] << std::endl;
    double thr;
    xpea = peak(xini,xend,curr1,thr);
    std::cout << "first peak found at " << xpea << ": " << volt1[xpea] << "(" << thr << ")" << std::endl;
    xini = rising(skipfirst,xpea,curr1,0.46*thr);
    xend = falling(xpea,nn-1,curr1,0.68*thr);
    std::cout << "Found potential range from " << xini << ": " << volt1[xini] << " and " << xend << ": " << volt1[xend] << std::endl;
  } else {
    std::cout << "*** manual range mode ***" << std::endl;
    xini = findLowEdge(thrPrime,volt1,nn-2);
    xend = findHighEdge(rangeend,volt1,nn-2);
    double thr;
    xpea = peak(xini,xend,curr1,thr);
    std::cout << "Found potential range from " << xini << ": " << volt1[xini] << " and " << xend << ": " << volt1[xend] << std::endl;
    std::cout << "first peak found at " << xpea << ": " << thr << std::endl;
  }
  //make sure that the range is at least three points from maximum
  if(xpea-xini < 4 ) xini = xpea-4;
  if(xend-xpea < 5 ) xend = xpea+5;
  if(xend>nn-2) xend = nn-2;
  std::cout << "Correcting range now from " << xini << ": " << volt1[xini] << " and " << xend << ": " << volt1[xend] << std::endl;
  //exit(0);
  
  // so it turns out that the second primed point by point is jumpy, thus my proposal is the following:
  // First: estimate the inflection point via a Landau fit to the primed function in the nominal range
  // Second: fit to a straight line the second-primed using a range centered on the previous estimate
  // Third: since for the sqrt(I) method, the range is crucial. I will use the first estimate to open  range there too
  
  // here we go:
  // first: landau fit to the primed
  TF1 *fitLandau[100];
  //plenty of fits
  int nfit1 = 0;
  double sx = 0;
  double sxx = 0;
  for(int ii=xini; ii<xpea-2; ++ii) {
    for(int jj=xend; jj>xpea+2; --jj) {
      //cout << "Landau fit: " << ii << " " << jj << std::endl;
      if(gausInstead) {
	fitLandau[nfit1] = new TF1(Form("fitGaus%d",nfit1),"[0]*TMath::Gaus(x,[1],[2])", volt1[ii], volt1[jj]);
      } else {
	fitLandau[nfit1] = new TF1(Form("fitLandau%d",nfit1),"[0]*TMath::Landau(x,[1],[2])", volt1[ii], volt1[jj]);
      }
      fitLandau[nfit1]->SetParameter(1,volt1[xpea]); fitLandau[nfit1]->SetParLimits(1,volt1[ii],volt1[jj]);
      fitLandau[nfit1]->SetParameter(2,1);    fitLandau[nfit1]->SetParLimits(2,0.1,3);
      fitLandau[nfit1]->SetLineWidth(1);
      fitLandau[nfit1]->SetLineColor(kCyan-3);
      fitLandau[nfit1]->SetLineStyle(2);
      gr1->Fit( fitLandau[nfit1], "WWRQS", "", volt1[ii], volt1[jj] );
      sx += fitLandau[nfit1]->GetParameter(1);
      sxx += fitLandau[nfit1]->GetParameter(1)*fitLandau[nfit1]->GetParameter(1);
      nfit1++;
      if(nfit1>100) break;
    }
    if(nfit1>100) break;
  }
  double landauMean = sx/nfit1;
  double landauStd = TMath::Sqrt(sxx*nfit1-sx*sx)/nfit1;
  cout << "* Number of Landau fits " << nfit1 << std::endl;
  cout << "  MPV mean " << landauMean << " error " << landauStd << std::endl;
  //exit(0);
  
  // second: find peak position for second-primed and start range there for straight fit
  TF1 *fitStraight[100];
  double thr;
  int xpea2P = peak(xini,xend,curr2,thr);
  double est = landauMean;
  int nfit2=0;
  sx = sxx = 0;
  for(int ii=0; ii<100; ++ii) {
    int jj = locatePointBelowZero(xpea2P,xend,curr2,nfit2+1);
    if(jj==999) break;
    //std::cout << "2Primed " << xpea2P << ": " << thr;
    //std::cout << " " << jj << std::endl;
    fitStraight[nfit2] = new TF1( Form("fitStraight%d",nfit2), Form("[0]+[1]*(x-%f)",est), volt1[xpea2P], volt1[jj] );
    fitStraight[nfit2]->SetParameter(1,-10);
    fitStraight[nfit2]->SetParLimits(1,-1e6,0);
    fitStraight[nfit2]->SetLineWidth(1);
    fitStraight[nfit2]->SetLineColor(kOrange-3);
    fitStraight[nfit2]->SetLineStyle(2);
    gr2->Fit( fitStraight[nfit2], "WWRQS", "", volt1[xpea2P], volt1[jj] );
    double xxx = fitStraight[nfit2]->GetX(0,1,100);
    if(TMath::IsNaN(xxx)) continue;
    //std::cout << xxx << std::endl;
    sx += xxx;
    sxx += xxx*xxx;
    nfit2++;
  }
  double straightMean=sx/nfit2;
  double straightStd = TMath::Sqrt(sxx*nfit2-sx*sx)/nfit2;
  double estimator1=straightMean;
  std::cout << "* Number of Straight fits " << nfit2 << std::endl;
  std::cout << "  Inflection point estimated at " << straightMean << " error " << straightStd << std::endl;
  //exit(0);
  
  // third: estimating crossing point with zero from sqrt(I)
  // Note: this method is tricky as it depends strongly on the minimum Vbias used in the fit.
  // a totally arvitrary range is chosen here after having looked at all of the data
  // it seems to be that the amount of leakage current is non-negligible specially for the irradiated
  // SiPM but also is relatively strong in the nonirradiated ones.
  // The choice (whether good or not) is to use the Vbdwn found by the inflection point as the right limit
  // in the range and use as left limit the Voltage that correspond to 30% of the difference between the Sqrt{i}
  // at BreakdownInflection (Vbdwn1) and the Sqrt{I} of the Voltage that correspond to (Vbdwn1 - 1 Volt) assuming
  // that at the later Vbias we are below the real Vdown voltage.
  int xini3, xend3;
  xini3 = findLowEdge(landauMean-1.0,volt0,nn); // index at 1V below Vdwn1
  xend3 = findLowEdge(landauMean,volt0,nn);
  float upperY = sqrtc[xend3];
  float lowerY = sqrtc[xini3];
  std::cout << " Reference current is " << lowerY+0.3*(upperY-lowerY) << std::endl; 
  xini3 = findLowEdge( lowerY+0.3*(upperY-lowerY), sqrtc, nn ); // move to our choice
  xini3--;
  xend3++;
  //that works pretty well for unsubtracted distributions, for subtracted ones use this
  xini3 = findLowEdge(landauMean-0.2,volt0,nn); // 200mV
  xend3 = findLowEdge(landauMean+0.3,volt0,nn); // 300mV
  std::cout << " xini3 " << xini3 << ": " << volt0[xini3] << " xend3 " << xend3 << ": " << volt0[xend3] << std::endl;
  //exit(0);

  //
  TF1 *fitLinear[100];
  int nfit3 = 0;
  sx = sxx = 0;
  int xcent = 0.5*(xini3+xend3);
  for(int ii=xini3; ii<xend3-2; ++ii) {
    for(int jj=ii+1; jj<xend3; ++jj) {
      fitLinear[nfit3] = new TF1(Form("fitLinear%d",nfit3),"[0]+[1]*x", 30/*volt1[ii]*/, volt1[jj]);
      fitLinear[nfit3]->SetLineWidth(1);
      fitLinear[nfit3]->SetLineColor(kGreen-3);
      fitLinear[nfit3]->SetLineStyle(2);
      gr3->Fit( fitLinear[nfit3], "WWRQS", "", volt0[ii], volt0[jj] );
      double xxx = fitLinear[nfit3]->GetX(0,30,100);
      if(TMath::IsNaN(xxx)) continue;
      sx += xxx;
      sxx += xxx*xxx;
      nfit3++;
      if(nfit3>100) break;
    }
    if(nfit3>100) break;
  }
  double linearMean = sx/nfit3;
  double linearStd = TMath::Sqrt(sxx*nfit3-sx*sx)/nfit3;
  cout << "* Number of Linear fits " << nfit3 << std::endl;
  cout << "  Linear mean " << linearMean << " error " << linearStd << std::endl;
  double estimator2=linearMean;
  std::cout << "Crossing estimated at " << estimator2 << std::endl;


  /////////////
  // D O N E //
  /////////////

  
  ///////
  // display all
  double max0, max1, max2, max3;
  double min0, min1, min2, min3;
  peak( 0,nn,curr0,max0);
  floor(0,nn,curr0,min0);
  peak( xini,xend,curr1,max1);
  floor(xini,xend,curr1,min1);
  peak( xini,xend,curr2,max2);
  floor(xini,xend,curr2,min2);  
  peak( 0,xend3+3,sqrtc,max3);
  floor(2,xend3,curr0,min3);

  TLine *lin = new TLine();
  lin->SetLineColor(kRed-3);
  TLatex *tex = new TLatex();
  std::cout << "min " << min0 << " max0 " << max0 << std::endl;

  TCanvas *main = new TCanvas();
  main->SaveAs( Form("%s_Figs.pdf[",outfilestring.Data()), "pdf" );
  main->cd()->SetLogy(1);
  //TH2D *axis0 = new TH2D("axis0","IV-curve;Bias (V);I  (nA)",100,0, volt[n-1]+2,10000,min0,max0);
  TH2D *axis0 = new TH2D("axis0","IV-curve;Bias (V);I  (nA)",100,0, volt[n-1]+2,10000,0.1,max0);
  axis0->Draw("");
  gr0->Draw("*SAME");
  gr0->SetMarkerColor(kGreen-3);
  gr00->Draw("*SAME");
  sp00->Draw("SAME");
  gr0->GetYaxis()->SetTitle("I (nA)");
  gr0->GetXaxis()->SetTitle("Bias (V)");
  tex->SetTextSize(0.03);
  tex->DrawTextNDC(0.12,0.85,Form("%s",file.Data()));
  tex->SetTextSize(0.05);
  main->SaveAs( Form("%s_Figs.pdf",outfilestring.Data()), "pdf" );
  
  TCanvas *main1 = new TCanvas("methods","",1600,600);
  main1->Divide(3,1,0.001,0.001);
  main1->cd(1);
  gr1->Draw("A*");
  gr1->GetYaxis()->SetRangeUser(min1,max1);
  gr1->GetYaxis()->SetNdivisions(505);
  gr1->GetYaxis()->SetTitleOffset(1.2);  
  for(int ii=0; ii!=nfit1; ++ii)
    fitLandau[ii]->Draw("SAME");
  gr1->Draw("*SAME");
  sp1->Draw("SAME");
  lin->DrawLine(volt1[xini],-1,volt1[xini],+10);
  lin->DrawLine(volt1[xend],-1,volt1[xend],+10);
  gr1->GetXaxis()->SetRangeUser( volt1[xini]-2, volt1[xend]+2);
  tex->SetTextColor(kCyan-3);
  tex->DrawTextNDC(0.15,0.8,Form("%.2f (%.2f)",landauMean,landauStd));
  main1->cd(2);
  gr2->Draw("A*"); 
  gr1->GetYaxis()->SetRangeUser(min2,max2);
  gr2->GetYaxis()->SetNdivisions(505);
  gr2->GetYaxis()->SetTitleOffset(1.2);  
  for(int ii=0; ii!=nfit2; ++ii)
    fitStraight[ii]->Draw("SAME");
  gr2->Draw("*SAME");
  gr2->GetXaxis()->SetRangeUser( volt1[xini]-2, volt1[xend]+2);
  tex->SetTextColor(kOrange-3);
  tex->DrawTextNDC(0.15,0.8,Form("%.2f (%.2f)",straightMean,straightStd));
  //gr1->GetYaxis()->SetTitle("(dI / dV)  /  I");
  gr1->GetYaxis()->SetTitle("d/dv   Ln I");
  gr1->GetXaxis()->SetTitle("Bias (V)");
  //gr2->GetYaxis()->SetTitle("(d^{2} I / dV^{2})  /  I");
  gr2->GetYaxis()->SetTitle("d^{2}/dv^{2}   Ln I");
  gr2->GetXaxis()->SetTitle("Bias (V)");
  gr1->SetTitle("");
  gr2->SetTitle("");
  sp2->Draw("SAME");
  main1->cd(3);
  TH2D *axis3 = new TH2D("axis3",";Bias (V);#sqrt{ I (#muA) }", 100, volt1[xini]-3, volt1[xend3],10000,0,max3);
  axis3->Draw("");
  axis3->GetYaxis()->SetNdivisions(505);
  axis3->GetYaxis()->SetTitleOffset(1.2);  
  for(int ii=0; ii!=nfit3; ++ii)
    fitLinear[ii]->Draw("SAME");
  gr3->Draw("*SAME");    
  tex->SetTextColor(kGreen-3);
  tex->DrawTextNDC(0.15,0.8,Form("%.2f (%.2f)",linearMean,linearStd));
  main1->SaveAs( Form("%s_Figs.pdf",outfilestring.Data()), "pdf" );
  
  /*
  TCanvas *main3 = new TCanvas();
  //main3->SetLogy(1);
  gr0->Draw("A*");
  gr0->SetTitle("IV-curve");
  std::cout << "max0: " << max0 << std::endl;
  gr0->GetXaxis()->SetRangeUser( volt1[xini]-2, volt1[xend]+2);
  gr0->GetYaxis()->SetRangeUser( min3, max0 );
  lin->SetLineColor(kGreen-3);
  tex->SetTextColor(kGreen-3);
  lin->DrawLine(linearMean-linearStd,0,linearMean-linearStd,max0);
  lin->DrawLine(linearMean+linearStd,0,linearMean+linearStd,max0);
  tex->DrawLatex(linearMean,0.90*max0,Form("%.1f #pm %.1f",linearMean,linearStd));
  lin->SetLineColor(kCyan-3);
  tex->SetTextColor(kCyan-3);
  lin->DrawLine(landauMean-landauStd,0,landauMean-landauStd,max0);
  lin->DrawLine(landauMean+landauStd,0,landauMean+landauStd,max0);
  tex->DrawLatex(landauMean,0.95*max0,Form("%.1f #pm %.1f",landauMean,landauStd));
  lin->SetLineColor(kOrange-3);
  tex->SetTextColor(kOrange-3);
  lin->DrawLine(straightMean-straightStd,0,straightMean-straightStd,max0);
  lin->DrawLine(straightMean+straightStd,0,straightMean+straightStd,max0);
  tex->DrawLatex(straightMean,0.90*max0,Form("%.1f #pm %.1f",straightMean,straightStd));
  main3->SaveAs( Form("%s_Figs.pdf",outfilestring.Data()), "pdf" );
  */
  main1->SaveAs( Form("%s_Figs.pdf]",outfilestring.Data()), "pdf" );
  ///////

  ///// Saving fit data
  ofstream fout( Form("%s_Fits.txt",outfilestring.Data()) );
  fout << file.Data() << " ";
  fout << landauMean << " " << landauStd << " ";
  fout << straightMean << " " << straightStd << " ";
  fout << linearMean << " " << linearStd << std::endl;
  
  
  return 0;
}
