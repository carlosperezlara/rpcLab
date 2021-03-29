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

int fase2() {
  gStyle->SetOptStat(0);
  LoadGain();
  cout << gainenf->GetFormula()->GetExpFormula() << endl;

  /*
  TGraph *grREFDa = new TGraph("outputfiles/20200831_HDR2-2e14-2/p20C/dark/HDR2-2e14-2-20C-dark-1.csv","%lg, %lg");
  TGraph *grREFLi = new TGraph("outputfiles/20200831_HDR2-2e14-2/p20C/light/HDR2-2e14-2-20C-11.36mA-Orange-1.csv","%lg, %lg");
  grREFDa->SetMarkerStyle(24);
  grREFDa->SetMarkerColor(kBlue-3);
  grREFLi->SetMarkerStyle(24);
  grREFLi->SetMarkerColor(kRed-3);

  TGraph *grNEWDa = new TGraph("outputfiles/fase3/outputdata_DARK_1.csv","%*s %*s %lg, %*s %*s %lg, %*s %*s");
  TGraph *grNEWLi = new TGraph("outputfiles/fase3/outputdata_LED_1.csv","%*s %*s %lg, %*s %*s %lg, %*s %*s");
  grNEWDa->SetMarkerStyle(20);
  grNEWDa->SetMarkerColor(kBlue-3);
  grNEWLi->SetMarkerStyle(20);
  grNEWLi->SetMarkerColor(kRed-3);

  
  TCanvas *main = new TCanvas();
  TH2D *axis = new TH2D("axis",";bias  (V);current  (A)",100,0,43,1000,1e-8,1e-2);
  main->SetLogy(1);
  axis->Draw();
  grREFDa->Draw("PLSAME");
  grREFLi->Draw("PLSAME");
  grNEWDa->Draw("PLSAME");
  grNEWLi->Draw("PLSAME");
  */


  double bdown = 41.0126;
  const int tryouts = 6;
  const int steps = 5;
  TString sfile_1[tryouts], sfile_2[tryouts];
  double bdown_1[tryouts], bdown_2[tryouts];
  sfile_2[0] = "outputfiles/fase1/outputdata_DARK.csv";         bdown_2[0]=41.3856;
  sfile_2[1] = "outputfiles/fase1/outputdata_DARK_1_AFTER.csv"; bdown_2[1]=41.4691; // warning I saved them reversed
  sfile_2[2] = "outputfiles/fase2/outputdata_DARK_2.csv";       bdown_2[2]=41.4483;
  sfile_2[3] = "outputfiles/fase3/outputdata_DARK_2.csv";       bdown_2[3]=41.4648;
  sfile_2[4] = "outputfiles/fase4/outputdata_DARK_2.csv";       bdown_2[4]=41.4935;
  sfile_2[4] = "outputfiles/fase4/outputdata_DARK_2_234C.csv";  bdown_2[4]=41.4517;
  sfile_2[5] = "outputfiles/fase5/outputdata_2_DARK.csv";       bdown_2[5]=41.4446;

  sfile_1[0] = "outputfiles/fase1/outputdata_DARK_1.csv";       bdown_1[0]=41.2414;
  sfile_1[1] = "outputfiles/fase1/outputdata_DARK_2_AFTER.csv"; bdown_1[1]=41.3667; // warning I saved them reversed
  sfile_1[2] = "outputfiles/fase2/outputdata_DARK_1.csv";       bdown_1[2]=41.3623;
  sfile_1[3] = "outputfiles/fase3/outputdata_DARK_1.csv";       bdown_1[3]=41.3784;
  sfile_1[4] = "outputfiles/fase4/outputdata_DARK_1.csv";       bdown_1[4]=41.4007;
  sfile_1[4] = "outputfiles/fase4/outputdata_DARK_1_234C.csv";  bdown_1[4]=41.3605;
  sfile_1[5] = "outputfiles/fase5/outputdata_2_DARK.csv";       bdown_1[5]=41.3446;

  int color[tryouts];
  color[0] = kCyan-3;
  color[1] = kBlue-3;
  color[2] = kGreen-3;
  color[3] = kOrange-3;
  color[4] = kRed-3;
  color[5] = kBlue;
  
  TF1 *fit, *fit_1[tryouts], *fit_2[tryouts];
  fit = new TF1( "fitREF", "expo", 0, 42 );
  for(int i=0; i!=tryouts; ++i) {
    fit_1[i] = new TF1( Form("fit1_%d",i), "expo", 0, 42 );
    fit_2[i] = new TF1( Form("fit2_%d",i), "expo", 0, 42 );
  }
  TGraph *grNEWfinGEN_1[tryouts][steps];
  TGraph *grNEWfinGEN_2[tryouts][steps];
  TGraph *grREFfin[steps];
  
  for(int istep=0; istep!=steps; ++istep) {
    grREFfin[istep] = new TGraph("outputfiles/20200831_HDR2-2e14-2/p20C/dark/HDR2-2e14-2-20C-dark-1.csv","%lg, %lg");
    styleGR( grREFfin[istep], 20, kBlack );
    for(int itry=0; itry!=tryouts; ++itry) {
      grNEWfinGEN_2[itry][istep] = new TGraph(sfile_2[itry].Data(),"%*s %*s %lg, %*s %*s %lg, %*s %*s");
      grNEWfinGEN_1[itry][istep] = new TGraph(sfile_1[itry].Data(),"%*s %*s %lg, %*s %*s %lg, %*s %*s");
      styleGR( grNEWfinGEN_2[itry][istep], 20, color[itry] );
      styleGR( grNEWfinGEN_1[itry][istep], 21, color[itry] );
    }
  }


  //step 0: IV-curves
  int istep = 0;
  TCanvas *main0 = new TCanvas();
  TH2D *axisIV = new TH2D("axisIV",";bias  (V);current  (A)",100,0,42,1000,1e-7,1e-2);
  main0->SetLogy(1);
  grREFfin[istep]->Fit( fit, "R", "", 20, 35 );
  for(int itry=0; itry!=tryouts; ++itry) {
    grNEWfinGEN_2[itry][istep]->Fit( fit_2[itry], "R", "", 20, 35);
    grNEWfinGEN_1[itry][istep]->Fit( fit_1[itry], "R", "", 20, 35);
  }
  axisIV->Draw();
  grREFfin[istep]->Draw("*SAME");
  for(int itry=0; itry!=tryouts; ++itry) {
    grNEWfinGEN_2[itry][istep]->Draw("PSAME");
    grNEWfinGEN_1[itry][istep]->Draw("PSAME");
  }
  TLegend *leg = new TLegend(0.1,0.51,0.54,0.9);
  leg->AddEntry( grREFfin[istep], Form("[2] Oct 15th 2020 @ ~18C | Vbr %.3f",bdown) );
  for(int itry=0; itry!=tryouts; ++itry) {
    leg->AddEntry( grNEWfinGEN_1[itry][istep], Form("[1] phase %d  %s | Vbr %.3f",itry,itry==0?"RT [~23C]":"RT [unk]",bdown_1[itry]) );
    leg->AddEntry( grNEWfinGEN_2[itry][istep], Form("[2] phase %d  %s | Vbr %.3f",itry,itry==0?"RT [~24C]":"RT [unk]",bdown_2[itry]) );
  }
  leg->Draw();

  //step 2: SHIFT X
  istep = 2;
  for(int i=0; i!=grREFfin[istep]->GetN(); ++i) {
    grREFfin[istep]->SetPointX( i, grREFfin[istep]->GetPointX(i)-bdown );
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
  grREFfin[istep]->Draw("*SAME");
  for(int itry=0; itry!=tryouts; ++itry) {
    grNEWfinGEN_2[itry][istep]->Draw("PSAME");
    grNEWfinGEN_1[itry][istep]->Draw("PSAME");
  }
  leg->Draw();

  // Step 3: SHIFT in XY
  istep=3;
  for(int i=0; i!=grREFfin[istep]->GetN(); ++i) {
    double val0 = grREFfin[istep]->GetPointX(i);
    double val1 = val0-bdown;
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
  grREFfin[istep]->Draw("*SAME");
  for(int itry=0; itry!=tryouts; ++itry) {
    grNEWfinGEN_2[itry][istep]->Draw("PSAME");
    grNEWfinGEN_1[itry][istep]->Draw("PSAME");
  }
  leg->Draw();



  //Step 4: SHIFT in XY and scaling in Y
  istep=4;
  for(int i=0; i!=grREFfin[istep]->GetN(); ++i) {
    double val0 = grREFfin[istep]->GetPointX(i);
    double val1 = val0-bdown;
    grREFfin[istep]->SetPointX( i, val1 );
    double ov, factor;
    ov = grREFfin[istep]->GetPointX(i);
    factor = 1e-9/(1.6e-19)/gainenf->Eval( ov );
    grREFfin[istep]->SetPointY( i, factor*(grREFfin[istep]->GetPointY(i)-fit->Eval( val0 )) );
  }
  for(int itry=0; itry!=tryouts; ++itry) {
    for(int i=0; i!=grNEWfinGEN_2[itry][istep]->GetN(); ++i) {
      double val0 = grNEWfinGEN_2[itry][istep]->GetPointX(i);
      double val1 = val0-bdown_2[itry];
      grNEWfinGEN_2[itry][istep]->SetPointX( i, val1 );
      double ov, factor;
      ov = grNEWfinGEN_2[itry][istep]->GetPointX(i);
      factor = 1e-9/(1.6e-19)/gainenf->Eval( ov );
      grNEWfinGEN_2[itry][istep]->SetPointY( i, factor*(grNEWfinGEN_2[itry][istep]->GetPointY(i)-fit_2[itry]->Eval( val0 )) );
    }
    for(int i=0; i!=grNEWfinGEN_1[itry][istep]->GetN(); ++i) {
      double val0 = grNEWfinGEN_1[itry][istep]->GetPointX(i);
      double val1 = val0-bdown_1[itry];
      grNEWfinGEN_1[itry][istep]->SetPointX( i, val1 );
      double ov, factor;
      ov = grNEWfinGEN_1[itry][istep]->GetPointX(i);
      factor = 1e-9/(1.6e-19)/gainenf->Eval( ov );
      grNEWfinGEN_1[itry][istep]->SetPointY( i, factor*(grNEWfinGEN_1[itry][istep]->GetPointY(i)-fit_1[itry]->Eval( val0 )) );
    }
  }  

  TCanvas *main4 = new TCanvas();
  TH2D *axisDCR2 = new TH2D("axisDCR2",";bias  -  vbr  (V);DCR  (GHz)",100,0,0.8,1000,10,1e3);
  main4->SetLogy(1);
  axisDCR2->Draw();

  grREFfin[istep]->Draw("*SAME");
  for(int itry=0; itry!=tryouts; ++itry) {
    grNEWfinGEN_2[itry][istep]->Draw("PSAME");
    grNEWfinGEN_1[itry][istep]->Draw("PSAME");
  }
  grREFfin[istep]->Draw("*SAME");
  leg->Draw();

  
  return 0;
}
