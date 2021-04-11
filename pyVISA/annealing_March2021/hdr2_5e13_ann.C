#include "helpers.C"

TF1 *gainenf;

void LoadGain() {
  TFile *rfile = new TFile("/Users/cperez/MTDcharacterization/HDR2data/sipm_spec_input_HDR2-015-v2-1e13.root");
  rfile->ls();
  TF1 *gai = (TF1*) rfile->Get("fGain_vs_OV");
  cout << gai->GetFormula()->GetExpFormula() << endl;
  double p0G = gai->GetParameter(0);
  double p1G = gai->GetParameter(1);
  TF1 *enf = (TF1*) rfile->Get("fENF_vs_OV");
  double p0E = enf->GetParameter(0);
  double p1E = enf->GetParameter(1);
  gainenf = new TF1("gainenf",Form("(%f+x*%f)*(1+%f*x+%f*x*x)",p0G,p1G,p0E,p1E),0,10);
}

void styleGR(TGraph *gr, int marker, int col) {
  gr->SetMarkerStyle(marker);
  gr->SetMarkerColor(col);
  gr->SetLineColor(col);
}

int hdr2_5e13_ann() {
  gStyle->SetOptStat(0);
  LoadGain();
  loadTemperatureTables();
  cout << gainenf->GetFormula()->GetExpFormula() << endl;

  TString ref1 = "outputfiles/20200831_HDR2-2e14-2/p20C/dark/HDR2-2e14-2-20C-dark-1.csv";
  double bdown_ref1 = 41.0126;

  TString vbrFile[2] = {"20200826_HDR2-5e13/temperatureTrend.txt",
    "20200831_HDR2-5e13-2/temperatureTrend.txt"};

  const int tryouts = 4;
  const int steps = 5;
  TString sfile_1[tryouts], sfile_2[tryouts], sLegend[tryouts], sTemp_1[tryouts], sTemp_2[tryouts];
  double bdown_1[tryouts], bdown_2[tryouts], dTemp_1[tryouts], dTemp_2[tryouts];
  sfile_2[0] = "outputfiles/20200831_HDR2-5e13-2/p10C/dark/HDR2-5e13-2-10C-dark-1.csv"; sTemp_2[0]="20200831_HDR2-5e13-2/p10C/dark/HDR2-5e13-2-10C-dark-temps-1.csv";
  sfile_2[1] = "outputfiles/hdr2-5e13/hdr_2_dark_fase1.csv";        sTemp_2[1]="hdr2-5e13/hdr_2_tempdata_dark.csv";
  sfile_2[2] = "outputfiles/n20ish/5e13-2/data1_DARK.csv";          sTemp_2[2]="n20ish/5e13-2/temp1_DARK.csv";
  sfile_2[3] = "outputfiles/n20ish/5e13-2/data2_DARK.csv";          sTemp_2[3]="n20ish/5e13-2/temp2_DARK.csv";

  sfile_1[0] = "outputfiles/20200826_HDR2-5e13/p12C/dark/HDR2-5e13-10C-dark-1.csv"; sTemp_1[0]="20200826_HDR2-5e13/p12C/dark/HDR2-5e13-10C-dark-temps-1.csv";
  sfile_1[1] = "outputfiles/hdr2-5e13/hdr_1_dark_fase1.csv";        sTemp_1[1]="hdr2-5e13/hdr_1_tempdata_dark.csv";
  sfile_1[2] = "outputfiles/n20ish/5e13-1/data1_DARK.csv";          sTemp_1[2]="n20ish/5e13-1/temp1_DARK.csv";
  sfile_1[3] = "outputfiles/n20ish/5e13-1/data2_DARK.csv";          sTemp_1[3]="n20ish/5e13-1/temp2_DARK.csv";
  
  sLegend[0] = "Oct 14th 2020";
  sLegend[1] = "aft Phase 1";
  sLegend[2] = "aft Phase 1";
  sLegend[3] = "aft Phase 1";

  for(int i=0; i!=tryouts; ++i) {
    double tmp;
    readtemperature( sTemp_2[i], dTemp_2[i], tmp );
    bdown_2[i] = readtemperaturetrend(vbrFile[1], dTemp_2[i], 0);
    readtemperature( sTemp_1[i], dTemp_1[i], tmp );
    bdown_1[i] = readtemperaturetrend(vbrFile[0], dTemp_1[i], 0);
  }

  
  int color[tryouts];
  color[0] = kCyan-3;
  color[1] = kBlue-3;
  color[2] = kGreen-3;
  color[3] = kOrange-3;
  //color[4] = kRed-3;
  //color[5] = kMagenta-3;
  
  TF1 *fit, *fit_1[tryouts], *fit_2[tryouts];
  fit = new TF1( "fitREF", "expo", 0, 42 );
  for(int i=0; i!=tryouts; ++i) {
    fit_1[i] = new TF1( Form("fit1_%d",i), "expo", 0, 49 );
    fit_2[i] = new TF1( Form("fit2_%d",i), "expo", 0, 49 );
  }
  TGraph *grNEWfinGEN_1[tryouts][steps];
  TGraph *grNEWfinGEN_2[tryouts][steps];
  TGraph *grREFfin[steps];
  
  for(int istep=0; istep!=steps; ++istep) {
    grREFfin[istep] = new TGraph(ref1.Data(),"%lg, %lg");
    styleGR( grREFfin[istep], 20, kBlack );
    for(int itry=0; itry!=tryouts; ++itry) {
      if(itry==0) {
	grNEWfinGEN_2[itry][istep] = new TGraph(sfile_2[itry].Data(),"%lg, %lg");
	grNEWfinGEN_1[itry][istep] = new TGraph(sfile_1[itry].Data(),"%lg, %lg");
      } else {
	grNEWfinGEN_2[itry][istep] = new TGraph(sfile_2[itry].Data(),"%*s %*s %lg, %*s %*s %lg, %*s %*s");
	grNEWfinGEN_1[itry][istep] = new TGraph(sfile_1[itry].Data(),"%*s %*s %lg, %*s %*s %lg, %*s %*s");
      }
      styleGR( grNEWfinGEN_2[itry][istep], 20, color[itry] );
      styleGR( grNEWfinGEN_1[itry][istep], 21, color[itry] );
    }
  }


  //step 0: IV-curves
  int istep = 0;
  TCanvas *main0 = new TCanvas();
  TH2D *axisIV = new TH2D("axisIV",";bias  (V);current  (A)",100,0,43,1000,1e-7,1e-2);
  main0->SetLogy(1);
  grREFfin[istep]->Fit( fit, "R", "", 20, 35 );
  for(int itry=0; itry!=tryouts; ++itry) {
    grNEWfinGEN_2[itry][istep]->Fit( fit_2[itry], "R", "", 20, 35);
    grNEWfinGEN_1[itry][istep]->Fit( fit_1[itry], "R", "", 20, 35);
  }
  axisIV->Draw();
  //grREFfin[istep]->Draw("*SAME");
  for(int itry=0; itry!=tryouts; ++itry) {
    grNEWfinGEN_2[itry][istep]->Draw("PSAME");
    grNEWfinGEN_1[itry][istep]->Draw("PSAME");
  }
  TLegend *leg = new TLegend(0.1,0.51,0.54,0.9);
  //leg->AddEntry( grREFfin[istep], Form("[2] Oct 15th 2020 @ ~18C | Vbr %.3f",bdown_ref1) );
  for(int itry=0; itry!=tryouts; ++itry) {
    leg->AddEntry( grNEWfinGEN_1[itry][istep], Form("[1] %s |  %.1fC  | Vbr %.2f",sLegend[itry].Data(),dTemp_1[itry], bdown_1[itry]) );
    leg->AddEntry( grNEWfinGEN_2[itry][istep], Form("[2] %s |  %.1fC  | Vbr %.2f",sLegend[itry].Data(),dTemp_2[itry], bdown_2[itry]) );
  }
  leg->Draw();

  //step 2: SHIFT X
  istep = 2;
  for(int i=0; i!=grREFfin[istep]->GetN(); ++i) {
    grREFfin[istep]->SetPointX( i, grREFfin[istep]->GetPointX(i)-bdown_ref1 );
  }
  for(int itry=0; itry!=tryouts; ++itry) {
    for(int i=0; i!=grNEWfinGEN_2[itry][istep]->GetN(); ++i) {
      grNEWfinGEN_2[itry][istep]->SetPointX( i, grNEWfinGEN_2[itry][istep]->GetPointX(i)-bdown_2[itry] );
    }
    for(int i=0; i!=grNEWfinGEN_1[itry][istep]->GetN(); ++i) {
      grNEWfinGEN_1[itry][istep]->SetPointX( i, grNEWfinGEN_1[itry][istep]->GetPointX(i)-bdown_1[itry] );
    }
  }  
  
  TCanvas *main2 = new TCanvas();
  TH2D *axispreDCR = new TH2D("axispreDCR",";bias  -  vbr  (V);current  (A)",100,-18,2,1000,1e-7,1e-2);
  main2->SetLogy(1);
  /*
  grREFfin[istep]->Fit( fit, "R", "", -21, -6 );
  for(int itry=0; itry!=tryouts; ++itry) {
    grNEWfinGEN_2[itry][istep]->Fit( fit_2[itry], "R", "", -21, -6);//-6 );
    grNEWfinGEN_1[itry][istep]->Fit( fit_1[itry], "R", "", -21, -6);//-6 );
  }
  */
  axispreDCR->Draw();
  //grREFfin[istep]->Draw("*SAME");
  for(int itry=0; itry!=tryouts; ++itry) {
    grNEWfinGEN_2[itry][istep]->Draw("PSAME");
    grNEWfinGEN_1[itry][istep]->Draw("PSAME");
  }
  leg->Draw();

  // Step 3: SHIFT in XY
  istep=3;
  for(int i=0; i!=grREFfin[istep]->GetN(); ++i) {
    double val0 = grREFfin[istep]->GetPointX(i);
    double val1 = val0-bdown_ref1;
    grREFfin[istep]->SetPointX( i, val1 );
    grREFfin[istep]->SetPointY( i, grREFfin[istep]->GetPointY(i)-fit->Eval( val0 ) );
  }
  for(int itry=0; itry!=tryouts; ++itry) {
    for(int i=0; i!=grNEWfinGEN_2[itry][istep]->GetN(); ++i) {
      double val0 = grNEWfinGEN_2[itry][istep]->GetPointX(i);
      double val1 = val0-bdown_2[itry];
      grNEWfinGEN_2[itry][istep]->SetPointX( i, val1 );
      //cout << val0 << " " << val1 << " ";
      //cout << fit_2[itry]->Eval( val0 ) << endl;
      grNEWfinGEN_2[itry][istep]->SetPointY( i, grNEWfinGEN_2[itry][istep]->GetPointY(i)-fit_2[itry]->Eval( val0 ) );
    }
    for(int i=0; i!=grNEWfinGEN_1[itry][istep]->GetN(); ++i) {
      double val0 = grNEWfinGEN_1[itry][istep]->GetPointX(i);
      double val1 = val0-bdown_1[itry];
      grNEWfinGEN_1[itry][istep]->SetPointX( i, val1 );
      grNEWfinGEN_1[itry][istep]->SetPointY( i, grNEWfinGEN_1[itry][istep]->GetPointY(i)-fit_1[itry]->Eval( val0 ) );
    }
  }  
  

  TCanvas *main3 = new TCanvas();
  TH2D *axisDCR = new TH2D("axisDCR",";bias  -  vbr  (V);current  -  leak  (A)",100,-18,2,1000,1e-12,1e-2);
  main3->SetLogy(1);
  axisDCR->Draw();
  //grREFfin[istep]->Draw("*SAME");
  for(int itry=0; itry!=tryouts; ++itry) {
    grNEWfinGEN_2[itry][istep]->Draw("PSAME");
    grNEWfinGEN_1[itry][istep]->Draw("PSAME");
  }
  leg->Draw();



  //Step 4: SHIFT in XY and scaling in Y
  istep=4;
  for(int i=0; i!=grREFfin[istep]->GetN(); ++i) {
    double val0 = grREFfin[istep]->GetPointX(i);
    double val1 = val0-bdown_ref1;
    grREFfin[istep]->SetPointX( i, val1 );
    double factor = 1e-9/(1.6e-19)/gainenf->Eval( val1 );
    grREFfin[istep]->SetPointY( i, factor*(grREFfin[istep]->GetPointY(i)-fit->Eval( val0 )) );
  }
  for(int itry=0; itry!=tryouts; ++itry) {
    for(int i=0; i!=grNEWfinGEN_2[itry][istep]->GetN(); ++i) {
      double val0 = grNEWfinGEN_2[itry][istep]->GetPointX(i);
      double val1 = val0-bdown_2[itry];
      grNEWfinGEN_2[itry][istep]->SetPointX( i, val1 );
      double factor = 1e-9/(1.6e-19)/gainenf->Eval( val1 );
      grNEWfinGEN_2[itry][istep]->SetPointY( i, factor*(grNEWfinGEN_2[itry][istep]->GetPointY(i)-fit_2[itry]->Eval( val0 )) );
    }
    for(int i=0; i!=grNEWfinGEN_1[itry][istep]->GetN(); ++i) {
      double val0 = grNEWfinGEN_1[itry][istep]->GetPointX(i);
      double val1 = val0-bdown_1[itry];
      grNEWfinGEN_1[itry][istep]->SetPointX( i, val1 );
      double factor = 1e-9/(1.6e-19)/gainenf->Eval( val1 );
      grNEWfinGEN_1[itry][istep]->SetPointY( i, factor*(grNEWfinGEN_1[itry][istep]->GetPointY(i)-fit_1[itry]->Eval( val0 )) );
    }
  }  

  TCanvas *main4 = new TCanvas();
  TH2D *axisDCR2 = new TH2D("axisDCR2",";bias  -  vbr  (V);DCR  (GHz)",100,0,3,1000,1,1e3);
  main4->SetLogy(1);
  axisDCR2->Draw();

  //grREFfin[istep]->Draw("*SAME");
  for(int itry=0; itry!=tryouts; ++itry) {
    grNEWfinGEN_2[itry][istep]->Draw("PSAME");
    grNEWfinGEN_1[itry][istep]->Draw("PSAME");
  }
  TLegend *leg2 = new TLegend(0.5,0.1,0.9,0.5);
  //leg2->AddEntry( grREFfin[istep], Form("[2] Oct 15th 2020 @ ~18C | Vbr %.3f",bdown_ref1) );
  for(int itry=0; itry!=tryouts; ++itry) {
    leg2->AddEntry( grNEWfinGEN_1[itry][istep], Form("[1] %s |  %.1fC  | Vbr %.2f",sLegend[itry].Data(),dTemp_1[itry], bdown_1[itry]) );
    leg2->AddEntry( grNEWfinGEN_2[itry][istep], Form("[2] %s |  %.1fC  | Vbr %.2f",sLegend[itry].Data(),dTemp_2[itry], bdown_2[itry]) );
  }
  leg2->Draw();

  new TCanvas();
  TSpline3 *grOri = new TSpline3("spOri",grNEWfinGEN_1[0][4]);
  TSpline3 *grFin = new TSpline3("spFin",grNEWfinGEN_1[1][4]);
  TH2D *axissp = new TH2D("axissp","",100,0,3,100,0,300);
  axissp->Draw();
  grOri->Draw("same");
  grFin->Draw("same");

  TSpline3 *grOri2 = new TSpline3("spOri2",grNEWfinGEN_2[0][4]);
  TSpline3 *grFin2 = new TSpline3("spFin2",grNEWfinGEN_2[1][4]);

  double xxxsp[100], yyysp[100];
  double yyysp2[100];
  for(int i=0; i!=100; ++i) {
    xxxsp[i] = 0.0 + 0.4/100. * i;
    yyysp[i] = grFin->Eval( xxxsp[i] ) / grOri->Eval( xxxsp[i] );
    yyysp2[i] = grFin2->Eval( xxxsp[i] ) / grOri2->Eval( xxxsp[i] );
  }
  new TCanvas();
  TH2D *axisDiv = new TH2D("axisDiv","",100,0,3,100,0,1);
  axisDiv->Draw();
  TGraph *grDiv = new TGraph(100,xxxsp,yyysp);
  TGraph *grDiv2 = new TGraph(100,xxxsp,yyysp2);
  grDiv->Draw("*SAME");
  grDiv2->Draw("*SAME");
  return 0;
}
